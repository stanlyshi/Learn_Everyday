/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:19 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:06 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:47 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

void errorHandling(const char* message);

int main(int argc, char* argv[]){
    if(argc !=3){
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(-1 == connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr))){
        errorHandling("connect() error!");
    }
    else
        puts("Connect success!");
    
    char buf[32];
    while(1){
        if(NULL == fgets(buf, sizeof(buf), stdin)){
            errorHandling("gets() error!");
        }
        else{
            if((0 == strcmp(buf, "Q\n")) || (0 == strcmp(buf, "q\n"))){
                puts("Client disconnected...");
                break;
            }
            write(sock, buf, strlen(buf));
            int readLen = read(sock, buf, sizeof(buf)-1);
            buf[readLen] = 0;
            printf("Message from server: %s", buf);

            memset(buf, 0, sizeof(buf));
        }

    }

    close(sock);
    return 0;
}

void errorHandling(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}