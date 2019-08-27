/*
 ============================================================================
 Name        : Simple Remote Execution System Linux client
 Author      : Alex
 Version     : 0.9v
 Copyright   : No copyright
 Description : 2803ICT assignment 1, Ansi-style, CLion + Ubuntu
 ============================================================================
 */

#ifndef CLIENT_C_CSOCKET_H
#define CLIENT_C_CSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/timeb.h>
#include "stringbuffer.h"

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

typedef unsigned int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

#define check(A, M, ...) if(!(A)) { perror(M); errno = 0; goto error;}

#define DEFAULTPORT  1024
#define STDBUF 1024

//#define "put" 4, "get" 5, "run" 6, "list" 7, "sys" 8, "quit" 9
enum PTYPE {
    PUT = 4, GET, RUN, LIST, SYS, QUIT
};

SOCKET connector(char* host);
int make_request(SOCKET sock);
int recv_response(SOCKET sock);

int recv_Msg(SOCKET sockFd, Buffer* buf);
int send_Msg(SOCKET sockFd, Buffer* buf);


long long getSystemTime();
/* * Package struct
 *  1. timestamp ()
 *  2. package type (put, run, sys, etc..)
 *  3. is force mode? (0/1)
 *  4. is long list mode? (0/1)
 *  5. length of arg list ()
 *  6. arg list ()
 *  7. dir
 *  8. length of file list ()
 *  9. file list ()
 * */
typedef struct {
    unsigned long timeStamp;// length: 13
    unsigned short ptype; // [4 - 9]
    unsigned short fmode; // 1: On, 0: Off
    unsigned short lmode; // 1: On, 0: Off
    char* dirname; // folder name
    unsigned short args;
    unsigned short files;
    char* argv[10];
    char* filev[10];
} Request;

typedef struct {
    unsigned long timeStamp; // length :13
    unsigned short ptype; // [4 - 9]
    unsigned short success; // 1/0 cmd execute success or not
    char* response; // response message
} Response;

#define LONG_MODE "-l"
#define FORCE_MODE "-f"

#endif //CLIENT_C_CSOCKET_H
