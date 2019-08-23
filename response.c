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

int recvResponse(Response *resp, Buffer *buf);
int msgOutput(Response *resp);
int timeCost(long long prev);
int getPtype(short p, char *pname);

void responseInit(Response *resp);
void responseFree(Response *resp);

int recv_response(SOCKET sock){
    int q;
    Response resp;
    Buffer buf;
    responseInit(&resp);
    buffer_init(&buf);

    /* 1, receive data */
    recv_Msg(sock, &buf);

    /* debug */
    printf("client receive %s\n", buf.data);

    /* 2, write data into response */
    recvResponse(&resp, &buf);

    /* 3, output message from server */
    q = msgOutput(&resp);

    /* free memory */
    responseFree(&resp);

    return q;
}

void responseInit(Response *resp){
    memset(resp, 0, sizeof(Request));
    resp->response = malloc(5120);
}

void responseFree(Response *resp){
    free(resp->response);
}

/* 3, output message from server */
int msgOutput(Response *resp){
    char *p, c;
    int cost, offset, lineCount, bufferCount;
    char printBuffer[1024];//

    if(resp->success){
        offset = 0;
        lineCount = 0;
        bufferCount = 0;
        memset(printBuffer, 0, sizeof(char) * 1024);

        getPtype(resp->ptype, p);
        cost = timeCost(resp->timeStamp);

        printf("[CLIENT] Command has %s returned in %d milliseconds\n", p, cost);

        while ((c = resp->response[offset++])) {

            if(bufferCount < sizeof(printBuffer))
                printBuffer[bufferCount++] = c;
            else{
                printf("%s", printBuffer);
                bufferCount = 0;
                memset(printBuffer, 0, sizeof(char) * 1024);
            }

            if(c == '\n')
                lineCount ++;

            if (lineCount == 40) {
                printf("%s\n", printBuffer);
                printf("[INFO] Press any key to continue.\n");
                getchar();
                lineCount = 0;
                bufferCount = 0;
                memset(printBuffer, 0, sizeof(char) * 1024);
                continue;
            }
        }
        printf("%s", printBuffer);
    }
    // clean before return

    return 0;
}

/* 2, write data into response */
int recvResponse(Response *resp, Buffer *buf){
    char *tmp;
    int nCount = 0;
    tmp = malloc(25);
    memset(tmp, 0 ,25);

    /* read timestamp */
    sscanf(buf->data, "%13s", tmp);
    resp->timeStamp = atoll(tmp);
    memset(tmp, 0 ,13);
    nCount += 13;

    /* read ptype */
    sscanf(buf->data + nCount++, "%1s", tmp);
    resp->ptype = (short)atoi(tmp);
    memset(tmp, 0 ,1);

    /* read flag */
    sscanf(buf->data + nCount++, "%1s", tmp);
    resp->success = (short)atoi(tmp);
    memset(tmp, 0 ,1);

    sscanf(buf->data + nCount, "%[^&]", resp->response);

    return 0;
}

int getPtype(short p, char *pname){
    switch(p){
        case PUT : pname = "put";
            break;
        case GET : pname = "get";
            break;
        case RUN : pname = "run";
            break;
        case LIST: pname = "list";
            break;
        case SYS : pname = "sys";
            break;
        case QUIT: pname = "quit";
            break;
        default:
            pname = "Unknown";

    }
}

int timeCost(long long prev){
    return getSystemTime() - prev;
}

