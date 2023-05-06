/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:18 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>

void errorHandling(const char* message);

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    int sockServ, sockClient;
    sockServ = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr, clientAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(bind(sockServ, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
    {
        errorHandling("bind() error!");
    }

    if(listen(sockServ, 5) == -1)
    {
        errorHandling("listen() error!");
    }

    socklen_t clientAddrLen = sizeof(clientAddr);
    sockClient = accept(sockServ, (sockaddr*)&clientAddr, &clientAddrLen);
    
    if(sockClient == -1)
    {
        errorHandling("accept() error!");
    }
    else
    {
        puts("New client connected...");
    }

    char buf[32];
    int readLen = 0;
    
    while(1)
    {
        readLen = recv(sockClient, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
        if(readLen>0)
        {
            break;
        }
    }
    
    buf[readLen] = 0;
    printf("Buffering %d bytes: %s \n", readLen, buf);

    readLen = recv(sockClient, buf, sizeof(buf)-1, 0);
    buf[readLen] = 0;
    printf("Read again: %s \n", buf);

    close(sockClient);
    close(sockServ);

    return 0;
}

void errorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
