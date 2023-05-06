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
/**
 *  Rong Tao 2018.10.16
 */

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include "common.h"

void errorHandler(const char* message)
{
    perror(message);
    exit(1);
}

int tcpsocket_server()
{
    int servSock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    if(-1 == bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr)))
    {
        perror("bind error!");
        return -1;
    }

    if(-1 == listen(servSock, LISTENQ))
    {
        perror("listen error!");
        return -1;
    }
    
    return servSock;
}

int tcpsocket_client(const char *ipv4_str)
{
    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ipv4_str);
    servAddr.sin_port = htons(PORT);

    if(-1 == connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)))
    {
        perror("connect error!");
        return -1;
    }
    return sock;
}



