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
#include<string.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 30
#define EVENT_SIZE 30
#define EPOLL_SIZE 100

void error_handler(const char* message);

int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_addr_sz;
    
    char buf[BUF_SIZE];
    int str_len;

    int ep_fd,evt_num,i;//epoll的文件描述符
    struct epoll_event event;//用于注册socket的事件
    struct epoll_event* pevents;//用于保存激活事件的数组


    serv_sock=socket(AF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handler("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handler("listen() error");

    ep_fd=epoll_create(EPOLL_SIZE);
    event.events=EPOLLIN;
    event.data.fd=serv_sock;

    epoll_ctl(ep_fd,EPOLL_CTL_ADD,serv_sock,&event);
    
    pevents=malloc(sizeof(struct epoll_event)*EVENT_SIZE);

    while(1)
    {
        evt_num = epoll_wait(ep_fd,pevents,EVENT_SIZE,-1);
        for(i=0;i<evt_num;i++)
        {
            if(serv_sock==pevents[i].data.fd)
            {
                clnt_addr_sz=sizeof(clnt_addr);
                clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(ep_fd,EPOLL_CTL_ADD,clnt_sock,&event);
            }
            else
            {
                str_len=read(pevents[i].data.fd,buf,BUF_SIZE);
                if(str_len<=0)
                {
                    epoll_ctl(ep_fd,EPOLL_CTL_DEL,pevents[i].data.fd,NULL);
                    continue;
                }
                write(pevents[i].data.fd,buf,str_len);
            }
        }
    }
    epoll_ctl(ep_fd,EPOLL_CTL_DEL,pevents[i].data.fd,NULL);
    close(ep_fd);
    free(pevents);
    return 0;
}

void error_handler(const char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
