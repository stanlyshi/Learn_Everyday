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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

void errorHandler(const char* message)
{
    perror(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc !=3)
    {
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    if(-1 == connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)))
    {
        errorHandler("connect error");
    }

    FILE* readfp = fdopen(sock, "r");
    FILE* writefp = fdopen(sock, "w");

    char buf[1024];
    int readLen = 0;
    
    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        fputs(buf, writefp);
        fflush(writefp);
        fgets(buf, sizeof(buf), readfp);
        printf("Message from server: %s", buf);
    }

    close(sock);
    return 0;
}