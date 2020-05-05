// Server.cpp : Defines the entry point for the application.
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <concurrent_queue.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdexcept>
#include <vector>

#include "client.h"
#include "logger.h"
#include "gamestate.h"
#include "statehandler.h"
#include "EventClasses/event.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/server.log"
#define LOGLEVEL spdlog::level::debug

#define MAX_CLIENTS 5
#define SERVER_TICK 50
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

struct GameThreadQueues { // TODO use real class
    concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue;
    concurrency::concurrent_queue<int>* signalQueue;
    Client** clients; //TODO do this in more concurrency safe fashion
};

// handles each client socket
// data passed as pointer to GameThreadQueues struct
DWORD WINAPI handleGame(void* data) {
    GameThreadQueues* queues = (GameThreadQueues*) data;

    // ************** SETUP GAME STATE ****************
    GameState gameState;
    gameState.initialize();
    GameStateHandler gameStateHandler;
    bool exit = false;
    while (!exit) {
        //int out;
        int signal;
        // ************** GAME LOGIC START **************
//        printf("%d Events in the event queue\n", queues->eventQueue->unsafe_size());
        // process all events
        gameStateHandler.getNextState(&gameState, queues->eventQueue);
        // TODO: check if we have hit the tick yet

        // TODO: send out new gameState
        for (int i = 0; i < MAX_CLIENTS; i++ ) {
            if(queues->clients[i] != NULL){
                queues->clients[i]->sendGameState(gameState);
			}
		}
        // *************** GAME LOGIC END ***************
        Sleep(SERVER_TICK);
        if (queues->signalQueue->try_pop(signal)) {
            // maybe case if more signals
            if (signal == 0) {
                exit = true;
            }
        }
    }
    return 0;
}

// handles each client socket
// data passed as pointer into array of pointers to Clients
DWORD WINAPI handleConn(void* data) {
    int status;
    char buf[DEFAULT_BUFLEN];
    Client* client = *(Client**) data;
    printf("Connected\n");
    while ((status = client->recv(buf, DEFAULT_BUFLEN)) > 0) {
        // TODO deserialize object from buffer and put on queue, no repsonse
        printf("Received %d bytes\n", status);
        //client->pushEvent(client->getId()); //TODO changed function signatures
        int sendStatus = client->send(buf, status);
        // Echo the buffer back to the sender
        if (sendStatus == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            return 1;
        }
        printf("Echoed %d bytes\n", sendStatus);
    }
    if (status == 0) {
        printf("Connection closed\n");
        return 0;
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
        return 1;
    } // todo destructor for client handler
}

int main_inner(void) {
    WSADATA wsaData;

    int status;

    // Initialize Winsock
    if ((status = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        printf("WSAStartup failed with error: %d\n", status);
        return 1;
    }

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // resolve server address and port
    if ((status = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result)) != 0) {
        printf("getaddrinfo failed with error: %d\n", status);
        WSACleanup();
        return 1;
    }

    SOCKET listenSock = INVALID_SOCKET;

    // create socket to listen for incoming connections
    listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // bind connection listener socket
    if (bind(listenSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // listen on socket
    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    spdlog::info("Listening on port {}.", DEFAULT_PORT);

    SOCKET clientSock = INVALID_SOCKET;
    Client* clients[MAX_CLIENTS] = { NULL };
    int count = 0; // TODO implement better client id assignment
    concurrency::concurrent_queue<std::shared_ptr<Event>> eventQueue = concurrency::concurrent_queue<std::shared_ptr<Event>>(); // TODO use shared data class instead of int
    concurrency::concurrent_queue<int> signalQueue = concurrency::concurrent_queue<int>();
    struct GameThreadQueues queues = {&eventQueue, &signalQueue, clients};

    // create thread to handle game state loop
    HANDLE gameThread = CreateThread(NULL, 0, handleGame, &queues, 0, NULL);

    // loop to accept connections
    while (((clientSock = accept(listenSock, NULL, NULL)) != INVALID_SOCKET) && count < MAX_CLIENTS) {
        // cleanup old client handlers
        for (int i = 0; i < count; i++) {
            DWORD exitCode;
            if (clients[i] != NULL) {
                // check if exited
                HANDLE thread = clients[i]->getHandle();
                if (GetExitCodeThread(thread, &exitCode)) {
                    if (exitCode != STILL_ACTIVE) {
                        printf("deleting client %d\n", i);
                        delete clients[i];
                        clients[i] = NULL;
                    }
                } else {
                    printf("get exit code failed with error: %d\n", GetLastError());
                }
            }
        }
        // instantiate new client handler
        HANDLE thread = CreateThread(NULL, 0, handleConn, clients + count, CREATE_SUSPENDED, NULL);
        if (thread == NULL) {
            printf("thread creation failed with error: %d\n", GetLastError());
        }
        else {
            clients[count] = new Client(count, clientSock, thread, &eventQueue);
        }
        if (ResumeThread(thread) == -1) {
            printf("thread resume failed with error: %d\n", GetLastError());
            delete clients[count];
            clients[count] = NULL;
        } else {
            count++;
        }
    }

    printf("accept failed with error: %d (or max clients exceeded)\n", WSAGetLastError());
    closesocket(listenSock);

    // wait on outstanding threads
    // first kill network connections
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != NULL) {
            HANDLE thread = clients[i]->getHandle();
            DWORD waitStatus = WaitForSingleObject(thread, INFINITE); // TODO actually kill instead of just waiting
            if (waitStatus != WAIT_OBJECT_0) {
                printf("bad\n"); // TODO look into whether this ever happens
            }
            delete clients[i];
            clients[i] = NULL;
        }
    }
    // kill game loop
    signalQueue.push(0);
    DWORD waitStatus = WaitForSingleObject(gameThread, INFINITE);
    if (waitStatus != WAIT_OBJECT_0) {
        printf("bad\n"); // TODO look into whether this ever happens
    }
    CloseHandle(gameThread);

    WSACleanup();
    return 1;
}

int main( void ) {

    initLogging( LOGFILE_NAME, LOGLEVEL );
    spdlog::info( "Server starting up." );
    try {
        int statusCode = main_inner();
        spdlog::info( "Server shutting down." );
        shutdownLogging();
        return statusCode;
    } catch ( std::exception & e ) {
        spdlog::critical( "Unhandled exception: {}", e.what() );
        shutdownLogging();
        throw e; // Record and rethrow
    }

}
