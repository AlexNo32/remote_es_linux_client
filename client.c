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

void process(SOCKET sock);
void usage();

/* main entry */
int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    if(argc < 2){
        printf("[INFO] USAGE: $> client 'netclient host'.\n");
        return EXIT_FAILURE;
    }

    SOCKET clientFd = connector(argv[1]);
    usage();
    process(clientFd);

    return EXIT_SUCCESS;
}

/* communication with server */
void process(SOCKET sock){
    /* file descriptor */
    fd_set read_flags, write_flags;

    struct timeval waitd;
    int loop = 1;

    /* wait for I/O */
    while (loop) {
        waitd.tv_sec = 10;//timeout 10 seconds
        FD_ZERO(&read_flags);
        FD_ZERO(&write_flags);
        FD_SET(sock, &read_flags);
        FD_SET(sock, &write_flags);

        switch(select(sock + 1, &read_flags, &write_flags, (fd_set*) 0, &waitd)){

            case -1: //select failed.
                printf("[ERROR] Client polling select function failed... \n");

            case 0: //keep polling
                break;

            default:
                /* ready for read */
                if (FD_ISSET(sock, &read_flags)) {
                    FD_CLR(sock, &read_flags);
                    /* receive response (from server) */
                    //responseDisplay(sock);
                }

                /* ready for write */
                if (FD_ISSET(sock, &write_flags)) {
                    FD_CLR(sock, &write_flags);
                    /* making a request (to server) */
                    if (make_request(sock) == 0)
                        loop = 0;
                    sleep(1);
                }
        }

    }
    return;
}

/* program usage*/
void usage() {
    printf("\nCommand Help:\n");
    printf("A.	put progname sourcefile[s] [-f]: upload sourcefiles to progname dir, -f overwrite if exists.\n");
    printf("B.	get progname sourcefile : download sourcefile from progname dir to client screen.\n");
    printf("C.	run progname [args] [-f localfile]: compile (if req.) and run the exe with args and either print the return results to screen or given local file.\n");
    printf("D.	list [-l] [progname]: list the prognames on the server or files in the given progname directory to the screen,  -l = long list.\n");
    printf("E.	sys: return the name /version of the Operating System and CPU type.\n");
    printf("F.	quit: quit\n\n");
}