/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:17 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
/*
**使用边缘触发方式实现回声服务端
*/
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 64

void errorHandler(const char* message)
{
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

    int epfd = epoll_create(EPOLL_SIZE);

    epoll_event event;
    event.events = EPOLLIN|EPOLLET; //边缘触发方式
    event.data.fd = sockServ;
    
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockServ, &event);

    int sockClient, i;
    int eventCount;
    epoll_event* pEvents = (epoll_event*)malloc(sizeof(epoll_event)*EPOLL_SIZE);

    char buf[BUF_SIZE];
    while(1)
    {
        eventCount = epoll_wait(epfd, pEvents, EPOLL_SIZE, -1);
        
        for(i=0;i<eventCount;i++)
        {
            if(pEvents[i].data.fd == sockServ)
            { // new client connected...
                sockClient = accept(sockServ, 0, 0);
                
                if(sockClient == -1)
                {
                    puts("accept error!");
                    break;
                }
                else
                {
                    printf("Client %d: connected... \n", sockClient);
                }

                int flag = fcntl(sockClient, F_GETFL, 0);
                fcntl(sockClient, F_SETFL, flag|O_NONBLOCK);

                event.events = EPOLLIN;
                event.data.fd = sockClient;
                epoll_ctl(epfd, EPOLL_CTL_ADD, sockClient, &event);
            }
            else
            {   //receive message
                sockClient = pEvents[i].data.fd;
                while(1)
                {
                    int readLen = read(sockClient, buf, BUF_SIZE);
                    
                    if(readLen == -1)
                    {
                        if(errno == EAGAIN)break;
                    }
                    else if(readLen == 0)
                    {
                        printf("Client %d: disconnected...\n", sockClient);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, sockClient, 0);
                        close(sockClient);
                        break;
                    }
                    else
                    {
                        write(sockClient, buf, readLen);
                    }
                }
            }
        }
    }

    close(sockServ);
    close(epfd);
    return 0;
}