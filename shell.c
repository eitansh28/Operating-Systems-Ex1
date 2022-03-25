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
    printdir();  //Q1
    int exit=0;
    char req[256];
    bzero(req,256);
    int check=1;
    scanf("%[^\n]%*c",req);
    //printf("%s\n",req);
    while(exit==0){
        if(check==0){
            while (getchar()!='\n');  
        }
        // if (getcwd(req, sizeof(req)) != NULL){  //Q2
        //     printf("cur dir is:%s\n",req);
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
            
            close(client_socket);
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
            char src[40];
            char dst[40];
            bzero(src, 40);
            bzero(dst, 40);
            int i=0, j = 5;
            while (req[j] != ' '){
                src[i] = req[j];   //get the srcfilename from input
                i++;
                j++;
            }
            j++;
            i=0;
            while (req[j] != '\0'){   //get the dstfilename from input
                dst[i] = req[j];
                i++;
                j++;
            }
            copyFiles(src, dst);   //func that copy srcfile to dstfile
            bzero(req,256);
            continue;
        }

        else if(strstr(req,"DELETE")){   //Q11
            char filename[40];
            bzero(filename,40);
            int i=0, j=7;
            while(req[j]!=' '){
                filename[i]=req[j];   //get the filename from input
                j++;
                i++;
            }
            unlink(filename);   //'unlink' is a system call
            printf("file %s has been deleted!\n",filename);
            bzero(req,256);
            continue;
        }
        else{
            //system(req);    //'system' is a library function    //Q8
            int sonID=fork();    //Q9
            if(sonID==0){
                char** exec=NULL;
                char* split;
                //exec[0]=split;
                printf("%s fd\n",req);
                const char s[2] = " ";
                split=strtok(req,s);
                //printf("%s",req);
                //printf("%s",split);
                if(split==NULL){
                    printf("nvnvnvnv\n");
                }else{
                    printf("gfgf\n");
                    printf("%s",split);
                }
                int i=0;
                while(split){
                    printf("kkkkk");
                    
                    exec[i]=split;
                    split=strtok(NULL," ");
                    i++;
                }
                //char execFolder[40]="/bin/";
                //strcat(execFolder,req);
                //char* token = strtok(NULL, " ");
                execvp(exec[0],exec);
            }
            else{
                wait(NULL);
                printf("your son finish his job\n");
                bzero(req,256);
            }
        }
        printdir();
        bzero(req,256);
        check=scanf("%[^\n]%*c",req); 
    }
}
