#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "lib.h"


#define SERVER_PORT 12345
#define BUFF_SIZE 1024

int main() {
    // on linux to prevent crash on closing socket.
    signal(SIGPIPE, SIG_IGN);
    char buf[1024];
    // create a socket lisener.
    server_socket = -1;
    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Couldn't create a socket listener : %d",errno);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT); // short, network byte order

    // connect the server to a port which can read and write on.
    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed with error code : %d" , errno);
        return -1;
    }

    if(listen(server_socket, 10) == -1) {
        printf("listen() failed with error code : %d",errno);
        return -1;
    }

    //Accept and incoming connection
    printf("Waiting for connections\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_length = sizeof(client_addr);
    while(1){
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_length);
        if(client_socket == -1) {
            printf("listen failed with error code : %d",errno);
            close(server_socket);
            return -1;
        } else {
            printf("connection accepted\n");
        }
        while (1){
            bzero(buf, 1024);
            if (!recv(client_socket, buf, sizeof(buf), 0)){
                close(client_socket);
                break;
            }
            else {
                for (int i = 0; i < 1024; i++){
                    printf("%c", buf[i]);
                }
            }
        }
    }
    return 0;
}