/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:04 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:44 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<signal.h>
#include<string.h>
#define BUF_SIZE 1024

void error_handling(char* message);
void read_handling(int sock);
void write_handling(int sock);

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

//写进程
void write_handling(int sock)
{
    char buf[BUF_SIZE];
    while(1)
    {   
        memset(buf,0,BUF_SIZE);
        //fputs("Input:",stdout);
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n")||
           !strcmp(buf,"Q\n"))
        {
            shutdown(sock,SHUT_WR);
            return;
        }
        write(sock,buf,strlen(buf));
    }
}

//读进程
void read_handling(int sock)
{
    int str_len;
    char buf[BUF_SIZE];
    while(1)
    {
        str_len=read(sock,buf,BUF_SIZE-1);
        if(str_len<=0)
            return;
        buf[str_len]=0;
        printf("the message from server:%s\n",buf);
    }
}


void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
        
