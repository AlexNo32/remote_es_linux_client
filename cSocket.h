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
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

typedef unsigned int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define DEFAULTPORT  1024
#define STDBUF 1024

#define PUT  "put"
#define GET  "get"
#define RUN  "run"
#define LIST "list"
#define SYS  "sys"
#define QUIT "quit"


SOCKET connector(char *host);
int poll(SOCKET sock);

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
struct request {
    unsigned long timeStamp;
    unsigned short ptype;
    unsigned short fmode; // 1: On, 0: Off
    unsigned short lmode; // 1: On, 0: Off
    char * dirname; // folder name
    unsigned short args;
    char * argv;
    unsigned short files;
    char * filev;
};



#endif //CLIENT_C_CSOCKET_H
