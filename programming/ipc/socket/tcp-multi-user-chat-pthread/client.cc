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
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>

#include "common.h"

char name[] ="[DEFAULT]";

void* sendThread(void* arg)
{
    int sock = *(int*)arg;
    char buf[BUF_SIZE];
    char msg[NAME_SIZE+BUF_SIZE];
    
    while(fgets(buf, BUF_SIZE, stdin))
    {
        if(!strcmp(buf, "Q\n") || !strcmp(buf, "q\n"))
        {
            close(sock);
            exit(0);
        }
        sprintf(msg, "%s %s", name, buf);
        write(sock, msg, NAME_SIZE+BUF_SIZE);
    }
    return 0;
}

void* receiveThread(void* arg)
{
    int sock = *(int*)arg;
    char msg[BUF_SIZE+NAME_SIZE];
    int recvLen;
    
    while(1)
    {
        recvLen = read(sock, msg, NAME_SIZE+BUF_SIZE-1);
        if(recvLen == -1)
        {
            break;
        }
        msg[recvLen] = 0;
        fputs(msg, stdout);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <name> \n", argv[0]);
        exit(1);
    }
    
    sprintf(name, "[%s]", argv[2]);
    
    int sock = tcpsocket_client(argv[1]);

    pthread_t t_id[2];
    pthread_create(&t_id[0], NULL, sendThread, (void*)&sock);
    pthread_create(&t_id[1], NULL, receiveThread, (void*)&sock);
    pthread_join(t_id[0], NULL);
    pthread_join(t_id[1], NULL);

    close(sock);
    return 0;
}