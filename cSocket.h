//
// Created by charname on 7/9/19.
//

#ifndef CLIENT_C_CSOCKET_H
#define CLIENT_C_CSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

typedef unsigned int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define DEFAULTPORT  1024

SOCKET connector(char *host)

#endif //CLIENT_C_CSOCKET_H
