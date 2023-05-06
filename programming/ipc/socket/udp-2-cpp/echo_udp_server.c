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
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void errorHandler(const char* message){
    fputs(message,stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_port = htons(atoi(argv[1]));

    if(-1 == bind(sock, (struct sockaddr*)&sockAddr, sizeof(sockAddr))){
        errorHandler("bind() error!");
    }

    struct sockaddr_in clntAddr;
    socklen_t clntAddrLen = sizeof(clntAddr);

    char buf[64];
    while(1){
        int recv_len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&clntAddr, &clntAddrLen);
        sendto(sock, buf, recv_len, 0, (struct sockaddr*)& clntAddr, sizeof(clntAddr));
    }

    close(sock);
    return 0;
}