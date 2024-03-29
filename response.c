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

int recvResponse(Response* resp, Buffer* buf);
int msgOutput(Response* resp);
long long timeCost(long long prev);
int getPname(short p, char* pname);

void responseInit(Response* resp);
void responseFree(Response* resp);

int recv_response(SOCKET sock) {
    int quit;
    Response resp;
    Buffer buf;
    responseInit(&resp);
    buffer_init(&buf);

    /* 1, receive data */
    recv_Msg(sock, &buf);

    /* 2, write data into response */
    recvResponse(&resp, &buf);

    /* 3, output message from server */
    quit = msgOutput(&resp);

    /* free memory */
    responseFree(&resp);

    return quit;
}

void responseInit(Response* resp) {
    memset(resp, 0, sizeof(Response));
    resp->response = (char *)malloc(sizeof(char) * 5120);
    memset(resp->response, 0, sizeof(char) * 5120);
}

void responseFree(Response* resp) {
   // free(resp->response);
}

/* 3, output message from server */
int msgOutput(Response* resp) {
    char* p, c;
    int cost, offset, lineCount, bufferCount;
    char printBuffer[1024];//

    p = (char*)malloc(sizeof(char) * 16);
    memset(p, 0, sizeof(char) * 16);

    if (resp->success) {
        offset = 0;
        lineCount = 0;
        bufferCount = 0;
        memset(printBuffer, 0, sizeof(char) * 1024);

        if (resp->ptype == QUIT) {
            printf("[CLIENT] Client closing...\n");
            exit(EXIT_SUCCESS);
        }

        getPname(resp->ptype, p);
        cost = timeCost(resp->timeStamp);

        printf("[Client] Command ' %s ' returned in %d milliseconds.\n", p, cost);
        free(p);

        while ((c = resp->response[offset++])) {

            if (bufferCount < sizeof(printBuffer))
                printBuffer[bufferCount++] = c;
            else {
                printf("%s", printBuffer);
                bufferCount = 0;
                memset(printBuffer, 0, sizeof(char) * 1024);
            }

            if (c == '\n')
                lineCount++;

            if (lineCount == 40) {
                printf("%s", printBuffer);
                printf("[INFO] Press any key to continue.");
                getchar();
                lineCount = 0;
                bufferCount = 0;
                memset(printBuffer, 0, sizeof(char) * 1024);
            }
        }
        printf("%s\n", printBuffer);
    }
    else {
        printf("[DEBUG] Command exception...\n");
    }

    return 1;
}

/* 2, write data into response */
int recvResponse(Response* resp, Buffer* buf) {
    char* tmp;
    int nCount = 0, replyLen = 0;
    tmp = (char *)malloc(sizeof(char) * 64);
    memset(tmp, 0, sizeof(char) * 64);

    /* read timestamp */
    if(sscanf(buf->data, "%13s", tmp) > 0){
        resp->timeStamp = atoll(tmp);
        memset(tmp, 0, sizeof(char) * 64);
        nCount += 13;
    }

    /* read ptype */
    if(sscanf(buf->data + nCount++, "%1s", tmp) > 0){
        resp->ptype = (short)atoi(tmp);
        memset(tmp, 0, sizeof(char) * 64);
    }

    /* read flag */
    if(sscanf(buf->data + nCount++, "%1s", tmp) > 0){
        resp->success = (short)atoi(tmp);
        memset(tmp, 0, sizeof(char) * 64);
    }

    if(sscanf(buf->data + nCount, "%[^&]", tmp) > 0){
        replyLen = atoi(tmp);
        nCount += strlen(tmp) + 1;
    }
    snprintf(resp->response, replyLen + 1, "%s", buf->data + nCount);

    free(tmp);
    return 0;
}

int getPname(short p, char* pname) {
    switch (p) {
        case PUT: strcpy(pname, "put");
            break;
        case GET: strcpy(pname, "get");
            break;
        case RUN: strcpy(pname, "run");
            break;
        case LIST: strcpy(pname, "list");
            break;
        case SYS: strcpy(pname, "sys");
            break;
        default:
            strcpy(pname, "Unknow");
    }

    return 0;
}

long long timeCost(long long prev) {
    return getSystemTime() - prev;
}

