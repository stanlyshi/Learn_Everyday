/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<netdb.h>

void errorHandler(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    if(argc =!2){
        printf("Usage: %s <addr> \n", argv[0]);
        exit(1);
    }

    hostent* host = gethostbyname(argv[1]);
    if(!host){
        errorHandler("gethostbyname() error!");
    }

    printf("Official name:%s \n", host->h_name);

    for(int i=0;host->h_aliases[i];i++){
        printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);
    }

    if(host->h_addrtype == AF_INET){
        printf("Address type: AF_INET \n");
    }
    else{
        printf("Address type: AF_INET6 \n");
    }

    for(int i=0;host->h_addr_list[i];i++){
        printf("IP address %d: %s \n", i+1, inet_ntoa(*(in_addr*)host->h_addr_list[i]));
    }

    return 0;
}