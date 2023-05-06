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
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string.h>

#include "common.h"

int main(int argc,char* argv[])
{
    int sock;
    struct sockaddr_in addr;
    pid_t pid;

    if(argc!=3)
    {
        printf("Usage %s <address> <port>\n",argv[0]);
        exit(1);
    }

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(argv[1]);
    addr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
        error_handling("connect() error");

    pid=fork();
    if(pid==0)
    {
        write_handling(sock);
    }
    else
    {
        read_handling(sock);
    }
    close(sock);
    return 0;
}
