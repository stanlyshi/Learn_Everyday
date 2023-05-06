/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:20 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:07 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:47 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#define BUF_SIZE 30

void error_handling(const char* message);

int main(int argc,char* argv[])
{
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len=0;

    sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htons(atoi(argv[1]));

    if(bind(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
            error_handling("bind() error");

    while(1)
    {
        str_len=recvfrom(sock,message,BUF_SIZE-1,0,NULL,NULL);
        if(str_len<=0)
            break;
        message[str_len]=0;
        fputs(message,stdout);
    }
    close(sock);
    return 0;
}

void error_handling(const char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
