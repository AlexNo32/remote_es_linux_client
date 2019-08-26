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

void getCommand(char *comm);
int  builtRequest(char *comm, Request *req);
int  sendRequest(Request *req, Buffer *buf);

void requestInit(Request *req);
void requestFree(Request *req);

int getCode(char * ptype);
int sourceFile(char * str);
int fileExist(char * fileName, char * progName);
int loadFile(Buffer *buf, char *fileName, char *dirName);
int getFileSize(FILE * fp);

/* make a request (client to server) */
int make_request(SOCKET sock){
    char comm[STDBUF]; // user input buffer
    int i;

    Request req;
    Buffer buf;
    requestInit(&req);
    buffer_init(&buf);

    /* 1, get user input */
    getCommand(comm);
    check(strlen(comm) > 0, "[ERROR] Empty command.");

    /* 2, fill request struct */
    check(builtRequest(comm, &req) > 0, "[ERROR] Command undefined.");

    /* 3, send data */
    check(sendRequest(&req, &buf) > 0, "[ERROR] Dir or file doesn't exist.");
    send_Msg(sock, &buf);

    buffer_free(&buf);

    /* 4, load and send attachment files */
    if(req.ptype == PUT)
        for(i = 0; i < req.files; i++){

            printf("[Client] Sending attachments: %s\n", req.filev[i]);
            sleep(1);

            buffer_init(&buf);
            loadFile(&buf, req.filev[i], req.dirname);
            send_Msg(sock, &buf);

            buffer_free(&buf);
        }

    /* free Request */
    requestFree(&req);
    return 1;

    error:
    requestFree(&req);
    return -1;
}

void requestInit(Request *req){
    int i;
    memset(req, 0, sizeof(Request));
    req->dirname = malloc(50);

    for(i = 0; i < 10; i++){
        req->argv[i] = malloc(50);
        req->filev[i] = malloc(50);
    }
}

void requestFree(Request *req){
    int i;
    free(req->dirname);

    for(i = 0; i < 10; i++){
        free(req->argv[i]);
        free(req->filev[i]);
    }
}

/* 1, get user input */
void getCommand(char *comm){
    char buf[STDBUF]; // user input buffer

    printf("\n$> [INPUT] Please input command: ");
    fgets(buf, STDBUF, stdin);

    int i = 0;
    while (buf[i] != '\n')
        *comm++ = buf[i++];

    *comm = '\0';
}

/* 2, fill request struct */
int builtRequest(char * comm, Request * req){
    char *head, *remain;
    int f = 1;

    head = (char *) malloc(sizeof(char) * 8);
    remain = (char *) malloc(sizeof(char) * 1024);
    memset(head, 0, sizeof(char) * 8);
    memset(remain, 0, sizeof(char) * 1024);

    head = strtok(comm, " ");
    if ((req->ptype = getCode(head)) == -1)
        return -1;

    while(remain = strtok(NULL, " ")){
        if(f && strncmp(remain, "-", 1) != 0){
            snprintf(req->dirname, strlen(remain) + 1, "%s", remain);
            f = 0;
            continue;
        }

        if(strcmp(remain, LONG_MODE) == 0)
            req->lmode = 1;
        else if(strcmp(remain, FORCE_MODE) == 0)
            req->fmode = 1;
        else if(sourceFile(remain))
            snprintf(req->filev[req->files++], strlen(remain) + 1, "%s", remain);
        else
            snprintf(req->argv[req->args++], strlen(remain) + 1, "%s", remain);
    }
    req->timeStamp = getSystemTime();

    return 1;
}

/* 3, write request to buffer */
int sendRequest(Request *req, Buffer *buf){
    buffer_append_timestamp(buf, req->timeStamp);
    buffer_append_short(buf, req->ptype);
    buffer_append_short(buf, req->fmode);
    buffer_append_short(buf, req->lmode);
    buffer_append_short(buf, req->args);
    buffer_append_short(buf, req->files);

    if(req->dirname != NULL){
        buffer_append(buf, req->dirname, strlen(req->dirname));
        buffer_append(buf, "&", 1);
    }

    int i;
    for(i = 0; i < req->files; i ++){
        if(req->ptype == GET || fileExist(req->filev[i], req->dirname) > 0){
            buffer_append(buf, req->filev[i], strlen(req->filev[i]));
            buffer_append(buf, "&", 1);
        } else
            return -1;
    }

    for(i = 0; i < req->args; i++){
        buffer_append(buf, req->argv[i], strlen(req->argv[i]));
        buffer_append(buf, "&", 1);
    }

    return 1;
}

long long getSystemTime() {
    struct timeb time;
    ftime(&time);
    return time.time * 1000 + time.millitm;
}

int getCode(char * ptype){
    if(strcmp(ptype, "put") == 0)       return PUT;

    else if(strcmp(ptype, "get")  == 0) return GET;

    else if(strcmp(ptype, "run")  == 0) return RUN;

    else if(strcmp(ptype, "list") == 0) return LIST;

    else if(strcmp(ptype, "sys")  == 0) return SYS;

    else if(strcmp(ptype, "quit") == 0) return QUIT;

    else return -1;
}

/* file filter, only accept .c and .h files */
int sourceFile(char * str){
    int l = strlen(str) - 2;
    if(strcmp((str + l), ".c") == 0 || strcmp((str + l), ".h") == 0)
        return 1;
    return 0;
}

int loadFile(Buffer *buf, char *fileName, char *dirName){
    FILE *fPtr;
    int fileLen, nCount;
    char *fPath, *fTemp;

    fPath = (char *)malloc(sizeof(char) * 128);
    memset(fPath, 0 ,sizeof(char) * 128);
    snprintf(fPath, 128, "./%s/%s", dirName, fileName);

    fPtr = fopen(fPath, "rb");
    if (fPtr == NULL)
        return -1;

    fileLen = getFileSize(fPtr);
    fTemp = (char*) malloc (sizeof(char) * fileLen);
    memset(fTemp, 0, sizeof(char) * fileLen);

    if (fTemp == NULL)
        return -1;

    nCount = fread (fTemp, 1, fileLen,fPtr);
    if (nCount != fileLen)
        return -1;

    buffer_append(buf, fTemp, fileLen);
    fclose(fPtr);
    free(fTemp);
    free(fPath);
    return 0;
}

/* return file length */
int getFileSize(FILE * fp) {
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

/* verify file 1. c or cpp source. 2 file exist. */
int fileExist(char * fileName, char * progName) {
    char path[125];
    snprintf(path, 125, "./%s/%s", progName, fileName);

    if (access(path, 0) == -1)
        return -1;
    return 1;
}