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

long long getSystemTime();
int getCode(char * ptype);
int sourceFile(char * str);
int fileExist(char * fileName, char * progName);
int loadFile(Buffer *buf, char *fileName, char *dirName);
int getFileSize(FILE * fp);

int make_request(SOCKET sock){
    char comm[STDBUF]; // user input buffer
    Request req;
    Buffer buf;
    int i;

    memset(&req, 0, sizeof(Request));
    buffer_init(&buf);

    /* 1, get user input */
    getCommand(comm);

    /* 2, fill request struct */
    check(builtRequest(comm, &req) > 0, "[ERROR] Command undefined.");

    /* 3, send data */
    check(sendRequest(&req, &buf) > 0, "[ERROR] Dir or file doesn't exist.");
    send_msg(sock, &buf);
    buffer_free(&buf);


    /* 4, load and send attachment files */
    for(i = 0; i < req.files; i++){
        buffer_init(&buf);
        loadFile(&buf, req.filev[i], req.dirname);
        send_msg(sock, &buf);
        buffer_free(&buf);
    }

    return 0;
    error:
    return -1;
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
    char *head = NULL;
    char *remain = NULL;

    head = strtok(comm, " ");
    if ((req->ptype = getCode(head)) == -1)
        return -1;

    req->timeStamp = getSystemTime();

    while(remain = strtok(NULL, " ")){

        if(strcmp(remain, LONG_MODE) == 0)
            req->lmode = 1;

        else if(strcmp(remain, FORCE_MODE) == 0)
            req->fmode = 1;

        else if(sourceFile(remain))
            req->filev[req->files++] = remain;

        else if(req->dirname == NULL)
            req->dirname = remain;

        else
            req->argv[req->args++] = remain;
    }
    return 1;
}

int sendRequest(Request *req, Buffer *buf){
    buffer_append_short(buf, req->ptype);
    buffer_append_timestamp(buf, req->timeStamp);

    buffer_append_short(buf, req->fmode);
    buffer_append_short(buf, req->lmode);
    buffer_append_short(buf, req->args);
    buffer_append_short(buf, req->files);

    if(req->dirname != NULL)
        buffer_append(buf, req->dirname, strlen(req->dirname));

    int i;
    for(i = 0; i < req->files; i ++){
        if(fileExist(req->filev[i], req->dirname) > 0){
            buffer_append(buf, req->filev[i], strlen(req->filev[i]));
        } else
            return -1;
    }

    for(i = 0; i < req->args; i++){
        buffer_append(buf, req->argv[i], strlen(req->argv[i]));
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
    char fPath[125];
    char *fTemp;
    snprintf(fPath, 125, "./%s/%s", dirName, fileName);

    fPtr = fopen(fPath, "rb");
    if (fPtr == NULL)
        return -1;

    fileLen = getFileSize(fPtr);
    fTemp = (char*) malloc (sizeof(char) * fileLen);

    if (fTemp == NULL)
        return -1;

    nCount = fread (fTemp, 1, fileLen,fPtr);
    if (nCount != fileLen)
        return -1;

    fclose(fPtr);
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