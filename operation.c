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

void cgets(char *comm);

int poll(SOCKET sock){
    char comm[STDBUF]; // user input buffer
    char *head = NULL;
    char *remain = NULL;

    cgets(comm);

    //TODO command struct
    head = strtok_r(comm, " ", &remain);

    if(strcmp(head, PUT) == 0){
        // TODO put func
        printf("put command.");
    }else if(strcmp(head, GET) == 0){
        // TODO get func
        printf("get command.");
    }else if(strcmp(head, RUN) == 0){
        // TODO run func
        printf("run command.");
    }else if(strcmp(head, LIST) == 0){
        // TODO list func
        printf("list command.");
    }else if(strcmp(head, SYS) == 0){
        // TODO sys func
        printf("sys command.");
    }else if(strcmp(head, QUIT) == 0){
        // TODO exist
    }else{
        printf("[WARNING] unknow command: %s. \n", head);
    }
}

void cgets(char *comm){
    char buf[STDBUF]; // user input buffer

    printf("\n$> [INPUT] Please input command: ");
    fgets(buf, STDBUF, stdin);

    int i = 0;
    while (buf[i] != '\n')
        *comm++ = buf[i++];

    *comm = '\0';
}

//void struct command

