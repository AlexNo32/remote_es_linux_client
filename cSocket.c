//
// Created by charname on 7/9/19.
//

#include "cSocket.h"

void sockOpt(SOCKET conn);
int nonblock(SOCKET fd);

/** client connector */
SOCKET connector(char *host) {
    SOCKET listenFd;
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenFd == -1){
        printf("[ERROR] cannot create a client socket. \n");
        return -1;
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
        printf("[ERROR] cannot connect to server.\n");
        close(listenFd);
        return -1;
    }
    printf("[INFO] client connected to server: %s. \n", host);

    if(nonblock(listenFd) == -1){
        printf("[ERROR] unable to set nonblocking mode. \n");
        close(listenFd);
        return -1;
    }

    return listenFd;
}

/* socket setting */
void sockOpt(SOCKET listenFd) {
    int sockOpt = 1;
    if(setsockopt(listenFd, SOL_SOCKET, SO_KEEPALIVE, &sockOpt, sizeof(sockOpt)) == -1)
        printf("[ERROR] socket set KEEPLIVE failed. \n");

    if(setsockopt(listenFd, IPPROTO_TCP, TCP_NODELAY, &sockOpt, sizeof(sockOpt)) == -1)
        printf("[ERROR] socket set NODELAY failed. \n");
}

/* non blocking setting */
int nonblock(SOCKET fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

