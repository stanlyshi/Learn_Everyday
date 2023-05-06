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
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>

void errorHandler(const char* message){
    perror(message);
    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    int sockServ = socket(PF_INET, SOCK_STREAM, 0);
    
    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    if(-1 == bind(sockServ, (sockaddr*)&servAddr, sizeof(servAddr)))
    {
        errorHandler("bind error!");
    }

    if(-1 == listen(sockServ, 5))
    {
        errorHandler("listen error!");
    }

    char buf[32];
    
    while(1)
    {
        int sockClient = accept(sockServ, 0, 0);
        if(sockClient)
        {
            puts("New client connected...");
        }
        else
        {
            errorHandler("accept error!");
        }

        FILE* writefp = fdopen(sockClient, "w");
        FILE* readfp  = fdopen(sockClient, "r");

        int receiveLen = 0;
        
        while(!feof(readfp) )
        {
            fgets(buf, sizeof(buf), readfp);
            fputs(buf, writefp);
            fflush(writefp);
        }

        fclose(writefp);
        fclose(readfp);
    }
    close(sockServ);

    return 0;
}