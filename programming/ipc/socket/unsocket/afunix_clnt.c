/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:22 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:08 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_un addr;
    int str_len;
    char buf[BUF_SIZE];
    char* path="sock";
    sock=socket(AF_UNIX,SOCK_STREAM,0);

    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path,path,strlen(path));
    
    connect(sock,(struct sockaddr*)&addr,sizeof(addr));

    while(1)
    {
        fputs("input:",stdout);
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n")||!strcmp(buf,"Q\n"))
            break;
        write(sock,buf,BUF_SIZE);
        str_len=read(sock,buf,BUF_SIZE-1);
        buf[str_len]=0;
        printf("the message from serv:%s\n",buf);

    }
    close(sock);
    return 0;
}
