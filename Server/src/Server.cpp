// Server.cpp : Defines the entry point for the application.
//

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

int main(void)
{
    WSADATA wsaData;
    int status;

    SOCKET listenSock = INVALID_SOCKET;
    SOCKET clientSock = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    // Initialize Winsock
    status = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (status != 0) {
        printf("WSAStartup failed with error: %d\n", status);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    if (getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0) {
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

    // Accept a client socket
    clientSock = accept(listenSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(listenSock);

    // Receive until the peer shuts down the connection
    do {

        status = recv(clientSock, recvbuf, recvbuflen, 0);
        if (status > 0) {
            printf("Bytes received: %d\n", status);

            // Echo the buffer back to the sender
            iSendResult = send(clientSock, recvbuf, status, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(clientSock);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (status == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(clientSock);
            WSACleanup();
            return 1;
        }

    } while (status > 0);

    // shutdown the connection since we're done
    status = shutdown(clientSock, SD_SEND);
    if (status == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSock);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(clientSock);
    WSACleanup();

    return 0;
}