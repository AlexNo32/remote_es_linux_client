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

int recv_response(SOCKET sock){
    Response resp;
    Buffer buf;
    int i;

    memset(&resp, 0, sizeof(Response));
    buffer_init(&buf);

    /* 1, receive data */
    recv_Msg(sock, &buf);

    /* debug */
    printf("client receive %s\n", buf.data);
}

