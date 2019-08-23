/*
 ============================================================================
 Name        : Simple Remote Execution System Linux client
 Author      : Alex
 Version     : 0.9v
 Copyright   : No copyright
 Description : 2803ICT assignment 1, Ansi-style, CLion + Ubuntu
 ============================================================================
 */
#include "client.h"

int nonblock(SOCKET fd);
void closeConnection(SOCKET sock);

/* client connector */
SOCKET connector(char *host) {
    /* Initialize Winsock. */
#ifdef WIN64
    WSADATA wsaData;
	check(NO_ERROR == WSAStartup(MAKEWORD(2, 2), &wsaData), "[ERROR] WSAStartup failed. \n");
#endif
    SOCKET connector;
    int sockOpt = 1;

    /* Initial socket address */
    SOCKADDR_IN addrClient;
    memset(&addrClient, 0, sizeof(addrClient));
    addrClient.sin_family = AF_INET;
    addrClient.sin_addr.s_addr = inet_addr(host);
    addrClient.sin_port = htons(DEFAULTPORT);

    /* create socket */
    connector = socket(AF_INET, SOCK_STREAM, 0);
    check(connector >= 0, "[ERROR] Cannot create a socket... \n");
    printf("[INFO] Client socket created: %d... \n", connector);

    /* socket setting */
    setsockopt(connector, SOL_SOCKET, SO_KEEPALIVE, &sockOpt, sizeof(sockOpt));
    setsockopt(connector, IPPROTO_TCP, TCP_NODELAY, &sockOpt, sizeof(sockOpt));
    printf("[INFO] Socket setting ... done. \n");

    /* Connect to server */
    check(connect(connector, (SOCKADDR*)&addrClient, sizeof(addrClient)) == 0, "[ERROR] Cannot connect to server...\n");
    printf("[INFO] Client socket connect to server: %d... \n", connector);

    /* non-block setting */
    check(nonblock(connector) == 0, "[ERROR] Unable to set non-blocking mode. \n");
    printf("[INFO] Non-blocking setting is on: %d... \n", connector);

    return connector;

    error:
    closeConnection(connector);
    exit(EXIT_FAILURE);
}

/* non-block setting */
int nonblock(SOCKET fd) {
#ifdef WIN64
    unsigned long nNonBlocking = 1;
    return ioctlsocket(fd, FIONBIO, &nNonBlocking);// SOCKET_ERROR :-1
#else
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
}

/* clean up socket */
void closeConnection(SOCKET sock){
#ifdef WIN64
    closesocket(sock);
	WSACleanup(); //Clean up Winsock
#else
    close(sock);
#endif
}

/* receive message */
int recv_Msg(SOCKET sockFd, Buffer *buf){
    int nCount;
    char buff[STDBUF];

    while(1){
        nCount = recv(sockFd, buff, STDBUF, 0);
        buffer_append(buf, buff, nCount);
        if(nCount < STDBUF)
            break;
    }

    return 0;
}

/* send message */
int send_Msg(SOCKET sockFd, Buffer *buf){
    if(send(sockFd, buf->data, buf->size, 0) == -1){
        perror("[ERROR] send failed ");
        return -1;
    }
    return 0;
}