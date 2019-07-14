//
// Created by charname on 7/7/19.
//

#include "cSocket.h"

int process(SOCKET sock);
void help();

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    // command line argument for server address
    if(argc < 2){
        printf("[INFO] USAGE: $> client 'netclient host'.\n");
        return EXIT_FAILURE;
    }
    SOCKET clientFd = connector(argv[1]);
    if(clientFd == -1){
        printf("[ERROR] No service found...\n");
        return EXIT_FAILURE;
    }

    process(clientFd);

    return EXIT_SUCCESS;
}

/* communication with server */
int process(SOCKET sock){
    int loop = 1;
    // usage
    help();

    //  file descriptor
    fd_set read_flags, write_flags;
    struct timeval waitd;

    // polling wait I/O
    while (loop) {
        waitd.tv_sec = 10;//timeout 10 seconds
        FD_ZERO(&read_flags);
        FD_ZERO(&write_flags);
        FD_SET(sock, &read_flags);
        FD_SET(sock, &write_flags);

        switch(select(sock + 1, &read_flags, &write_flags, (fd_set*) 0, &waitd)){

            case -1: //select failed.
                printf("[ERROR] client polling select failed. \n");

            case 0: //keep polling
                break;

            default:
                // ready for reading
                if (FD_ISSET(sock, &read_flags)) {
                    FD_CLR(sock, &read_flags);
                    //responseDisplay(sock);
                }

                // ready for writing
                if (FD_ISSET(sock, &write_flags)) {
                    FD_CLR(sock, &write_flags);
                    //make a request
                    //if (requestAssemble(sock) == -1)
                    //   loop = 0;
                    //Sleep(500);
                }
        }

    }
    return EXIT_SUCCESS;
}


/* non blocking setting */
int nonblock(SOCKET fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* program usage*/
void help() {
    printf("\nCommand Help:\n");
    printf("A.	put progname sourcefile[s] [-f]: upload sourcefiles to progname dir, -f overwrite if exists.\n");
    printf("B.	get progname sourcefile : download sourcefile from progname dir to client screen.\n");
    printf("C.	run progname [args] [-f localfile]: compile (if req.) and run the exe with args and either print the return results to screen or given local file.\n");
    printf("D.	list [-l] [progname]: list the prognames on the server or files in the given progname directory to the screen,  -l = long list.\n");
    printf("E.	sys: return the name /version of the Operating System and CPU type.\n");
    printf("F.	quit: quit\n\n");
}