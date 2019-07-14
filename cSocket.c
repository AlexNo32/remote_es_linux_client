/*
 ============================================================================
 Name        : Simple Remote Execution System Linux client
 Author      : Alex
 Version     : 0.9v
 Copyright   : No copyright
 Description : 2803ICT assignment 1, Ansi-style, CLion + Ubuntu
 ============================================================================
 */

#include "cSocket.h"

void sockOpt(SOCKET conn);
int nonblock(SOCKET fd);

/** client connector */
SOCKET connector(char *host) {
    SOCKET listenFd;
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd == -1){
        perror("[ERROR] cannot create a client socket:");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] client socket created: %d. \n", listenFd);

    sockOpt(listenFd);
    printf("[INFO] client socket setting ... done. \n");

    SOCKADDR_IN addrClient;
    memset(&addrClient, 0, sizeof(addrClient));
    addrClient.sin_family = AF_INET;
    addrClient.sin_addr.s_addr = inet_addr(host);
    addrClient.sin_port = htons(DEFAULTPORT);

    if(connect(listenFd, (SOCKADDR*)&addrClient, sizeof(addrClient)) == -1){
        perror("[ERROR] cannot connect to server:");
        close(listenFd);
        exit(EXIT_FAILURE);
    }
    printf("[INFO] client is connected to server: %s. \n", host);

    if(nonblock(listenFd) == -1){
        perror("[ERROR] unable to set non-blocking mode:");
        close(listenFd);
        exit(EXIT_FAILURE);
    }

    return listenFd;
}

/* socket setting */
void sockOpt(SOCKET listenFd) {
    int sockOpt = 1;
    if(setsockopt(listenFd, SOL_SOCKET, SO_KEEPALIVE, &sockOpt, sizeof(sockOpt)) == -1)
        perror("[ERROR] socket set KEEPLIVE failed:");

    if(setsockopt(listenFd, IPPROTO_TCP, TCP_NODELAY, &sockOpt, sizeof(sockOpt)) == -1)
        perror("[ERROR] socket set NODELAY failed:");
}

/* non blocking setting */
int nonblock(SOCKET fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

