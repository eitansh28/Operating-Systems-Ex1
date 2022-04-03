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

int main(){
    printf("welcome to shell!\n");  //  Q1 + printdir
    int exit=0;
    char req[256];
    bzero(req,256);
    int check=1;
    while(exit==0){
        if(check==0){
            while (getchar()!='\n');  
        }
        printdir();  //Q1
        check=scanf("%[^\n]%*c",req);
        // if (getcwd(req, sizeof(req)) != NULL){  //Q2
        //     printf("cur dir is:%s\n",req);   //print the current directory
        //     continue;
        // }
        if(strcmp(req,"exit")==0){
            exit=1;
            return 0;
        }
        else if(strstr(req, "ECHO")){  //Q3
            printf("%s\n",req+5);
            bzero(req,256);
            continue;
        }
        else if(strcmp(req,"TCP PORT")==0){   //Q4
            socketConnect();  //func that open a client socket
            dup2(1,500);
            dup2(client_socket, 1);
            bzero(req,256);
            continue;
        }
        else if(strcmp(req,"LOCAL")==0){    //Q5
            //printf("EXIT");
            close(client_socket);
            wait(NULL);
            //close(server_socket);
            dup2(500,1);
            printf("we back to std output\n");

        }
        else if(strcmp(req,"DIR")==0){  //Q6
            DIR *dir;
            struct dirent *dent;
            getcwd(req, sizeof(req));  //get the current directory
            dir = opendir(req);
            while((dent=readdir(dir))!=NULL){
                printf("%s",dent->d_name);  //print all the file that in curr directory
                printf("\n");
            }closedir(dir);
            bzero(req,256);
            continue;
        }

        else if(strstr(req, "cd")){   //Q7
            chdir(req+3);      //chdir is a system call
            bzero(req,256);
            continue;
        }
        

        else if(strstr(req,"COPY")){  //Q10
            strtok(req," ");
            char* in=strtok(NULL," ");  
            char* out=strtok(NULL," ");
            FILE *src = fopen(in, "r");   //fopen, fread, fwrite are library functions
            FILE *dest = fopen(out, "w");
            char buffer[100];
            fread(buffer, 1, 1, src);
            while (feof(src) == 0) {
                fwrite(buffer, 1, 1, dest);
                fread(buffer, 1, 1, src);
            }
            fclose(src);
            fclose(dest);
            bzero(req,256);
            continue;
        }
        
        else if(strstr(req,"DELETE")){   //Q11
            strtok(req," ");
            unlink(strtok(NULL," "));
            printf("file %s has been deleted!\n",req+7);
            bzero(req,256);
            continue;
        }
        else {
            //system(req);      //'system' is a library function    //Q8
            char* exec[256];     //Q9
            int sonID=fork(); 
            // if(sonID<0){
            //     return 1;
            // }  
            if(sonID==0){
                char* split;
                split=strtok(req," ");
                int i=0;
                while(split !=NULL){
                    //printf("kkkkk\n");
                    exec[i++]=split;
                    split=strtok(NULL," ");
                    //i++;
                }
                exec[i]=NULL;
                if(exec[1]==NULL){
                    char execFolder[40]="/bin/";
                    strcat(execFolder,req);
                    execlp(execFolder,req,NULL);
                }else{
                    execvp(exec[0],exec);
                }  
            }
            else{
                wait(NULL);
                printf("your son finish his job\n");
            }
        }
        bzero(req,256);
    }
}
