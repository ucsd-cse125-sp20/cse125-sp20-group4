﻿// Server.cpp : Defines the entry point for the application.
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <processthreadsapi.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdexcept>

#include "logger.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/server.log"
#define LOGLEVEL spdlog::level::debug

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

// handles each client socket
DWORD WINAPI handleConn(void* data) {
    int status;
    char buf[DEFAULT_BUFLEN];
    SOCKET clientSock = *(SOCKET*) data;
    free(data);
    printf("Connected\n");
    while ((status = recv(clientSock, buf, DEFAULT_BUFLEN, 0)) > 0) {
        printf("Received %d bytes\n", status);
        int sendStatus = send(clientSock, buf, status, 0);
        // Echo the buffer back to the sender
        if (sendStatus == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(clientSock);
            WSACleanup();
            return 1;
        }
        printf("Echoed %d bytes\n", sendStatus);
    }
    if (status == 0) {
        printf("Connection closed\n");
        // shutdown the connection since we're done
        if (shutdown(clientSock, SD_SEND) == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(clientSock);
            return 1;
        }
        closesocket(clientSock);
        return 0;
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
        closesocket(clientSock);
        return 1;
    }
}

int main_inner( void )
{

    WSADATA wsaData;

    SOCKET listenSock = INVALID_SOCKET;
    SOCKET clientSock = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int status;

    // Initialize Winsock
    if ((status = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        printf("WSAStartup failed with error: %d\n", status);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    if ((status = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result)) != 0) {
        printf("getaddrinfo failed with error: %d\n", status);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    if (bind(listenSock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    printf("listening on %s\n", DEFAULT_PORT);

    while ((clientSock = accept(listenSock, NULL, NULL)) != INVALID_SOCKET) {
        SOCKET* threadSock = (SOCKET*) malloc(sizeof(SOCKET));
        // just pass client socket to new thread, will want to add more, like data xfer channel, etc
        *threadSock = clientSock;
        HANDLE thread = CreateThread(NULL, 0, handleConn, threadSock, 0, NULL);
        CloseHandle(thread); // TODO better thread handling, keep in array of clients, etc
    }

    printf("accept failed with error: %d\n", WSAGetLastError());
    closesocket(listenSock);
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
