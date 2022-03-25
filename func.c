#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "lib.h"

#define SERVER_PORT          12345
#define SERVER_IP_ADDRESS    "127.0.0.1"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

void printdir(){
    char ans[80];
    getcwd(ans,sizeof(ans));
    printf(CYN "eitansh:" MAG "%s$ " RESET,ans);
}


void socketConnect(){
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(client_socket == -1) {
            fprintf(stderr, "Couldn't create the socket : %s\n", strerror(errno));
            exit(EXIT_FAILURE); // failing exit status.
        }
        struct sockaddr_in server_address;
            memset(&server_address, 0, sizeof(server_address));
            server_address.sin_family = AF_INET;
            server_address.sin_port = htons(SERVER_PORT);
            //inet_pton(AF_INET, (const char*)SERVER_IP_ADDRESS, &server_address.sin_addr);

            // Connect to the server
            int connection = connect(client_socket, (struct sockaddr *) &server_address, sizeof(server_address));
            if(connection == -1) {
                fprintf(stderr, "connect() failed with error code:%s\n", strerror(errno));
                exit(EXIT_FAILURE); // failing exit status.
            }
            else {
                printf("client connected to server successfully.\n");
            }
}


void copyFiles(char *src, char *dst){
    char ch;
    FILE *fpSrc = fopen(src, "r");    //
    FILE *fpDst = fopen(dst, "a");
    while ((ch = fgetc(fpSrc)) != EOF){
        fputc(ch, fpDst);
    }
    printf("We copied your file!\n");
    fclose(fpSrc);
    fclose(fpDst);
}

