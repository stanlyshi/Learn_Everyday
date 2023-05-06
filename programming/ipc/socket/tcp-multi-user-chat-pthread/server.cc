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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include "common.h"

int clntSocks[MAX_CLNT];
int clntCount = 0;
pthread_mutex_t mutex;


void sendMsg2Clients(char* buf, int len)
{
    pthread_mutex_lock(&mutex);
    for(int i=0;i<clntCount;i++)
    {
        write(clntSocks[i], buf, len);
    }
    pthread_mutex_unlock(&mutex);
}

void* clntHandler(void* arg)
{
    int sock = *(int*)arg;
    int readLen = 0;
    char buf[BUF_SIZE];
    
    while(readLen = read(sock, buf, BUF_SIZE))
    {
        sendMsg2Clients(buf, readLen);
    }

    pthread_mutex_lock(&mutex);
    
    for(int i=0;i<clntCount;i++)
    {
        if(clntSocks[i]==sock)
        {
            int j = i;
            while(j<clntCount-1)
            {
                clntSocks[j] = clntSocks[j+1];
                j++;
            }
            clntCount--;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
    close(sock);
    return 0;
}

int main(int argc, char*argv[])
{
    if(argc !=1)
    {
        printf("Usage: %s \n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);

    int servSock = tcpsocket_server();

    int clntSock;
    sockaddr_in clntAddr;
    socklen_t clntAddrLen = sizeof(clntAddr);
    pthread_t t_id;
    
    while(1)
    {
        clntSock = accept(servSock, (sockaddr*)&clntAddr, &clntAddrLen);
        
        if(clntSock == -1)
        {
            puts("accept error!");
            continue;
        }
        else
        {
            printf("Connected client ip: %s:%d \n", 
                    inet_ntoa(clntAddr.sin_addr), ntohs(clntAddr.sin_port));
        }

        pthread_mutex_lock(&mutex);
        clntSocks[clntCount++] = clntSock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, clntHandler, (void*)&clntSock);
        pthread_detach(t_id);
    }

    pthread_mutex_destroy(&mutex);
    close(servSock);
    return 0;
}