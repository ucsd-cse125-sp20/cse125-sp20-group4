/*
 * Copyright 2018, Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    int count = 10;
    if (argc == 2) {
        count = strtol(argv[1], NULL, 10);
    }
    
    struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(8080);
    connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    
    printf("Sending 'a' %d times\n", count);
    for (int i = 0; i < count; i++) {
        write(sockfd, "a", 1);
    }
    
    printf("Received:\n");
    for (int i = 0; i < count; i++) {
        char buf;
        read(sockfd, &buf, 1);
        printf("%c", buf);
    }
    printf("\n");
    
    close(sockfd);
}