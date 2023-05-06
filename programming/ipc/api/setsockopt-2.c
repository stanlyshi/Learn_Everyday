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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}

int main(int argc, char* argv[])
{
    if(argc!=2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    int sock_serv, sock_clnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_len;
    char message[256];
    size_t bytes_transferred;
    int option,optlen;

    sock_serv = socket(PF_INET, SOCK_STREAM, 0);
    if(sock_serv == -1)
        error_handling("socket() error!");

    option = 1;
    optlen = sizeof(option);
    setsockopt(sock_serv, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock_serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
        error_handling("bind() error!");

    if(listen(sock_serv, 5) == -1)
        error_handling("listen() error!");

    clnt_addr_len = sizeof(clnt_addr);
    sock_clnt = accept(sock_serv, (sockaddr*)&serv_addr, &clnt_addr_len);

    while(bytes_transferred = read(sock_clnt,message, sizeof(message))){
        write(sock_clnt, message, bytes_transferred);
        write(1, message, bytes_transferred);
    }

    close(sock_clnt);
    close(sock_serv);

    return 0;
}