/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:16 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:45 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<string.h>

#define BUF_SIZE 32

void errorHandling(const char* message);

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    char buf[BUF_SIZE];
    int sockServ = socket(PF_INET, SOCK_STREAM, 0);

    fd_set readSet, cpyReadSet;
    FD_ZERO(&readSet);
    FD_SET(sockServ, &readSet);

    int fdMax = sockServ;
    
    timeval timeout;

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(sockServ, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1){
        errorHandling("bind() error!");
    }

    if(listen(sockServ, 5) == -1){
        errorHandling("listen() error!");
    }

    while(1){
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        cpyReadSet = readSet;

        int fdNum = select(fdMax+1, &cpyReadSet, 0, 0, &timeout);
        if(fdNum == -1){
            break;
        }
        else if(fdNum == 0){
            continue;
        }
        else{
            for(int i=0;i<fdMax+1;i++){
                if(FD_ISSET(i, &cpyReadSet)){
                    if(i == sockServ){  //connection request
                        int sockClient = accept(sockServ, 0, 0);
                        if(sockClient == -1){
                            errorHandling("accept() error!");
                        }
                        else{
                            puts("New client connected...");

                            FD_SET(sockClient, &readSet);

                            if(fdMax < sockClient) fdMax = sockClient;


                        }
                    }
                    else{
                        int receivedBytes = read(i, buf, BUF_SIZE);
                        if(receivedBytes == 0){
                            FD_CLR(i, &readSet);
                            close(i);

                            puts("Client disconnected...");
                        }
                        else{
                            write(i, buf, receivedBytes);
                        }
                    } 
                }
            }
        }
    }

    close(sockServ);
    return 0;
}

void errorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}