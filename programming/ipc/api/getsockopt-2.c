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
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);

    exit(1);
}

void main()
{
    int sock;
    int send_buf,recv_buf,state;
    socklen_t buf_len;

    buf_len = sizeof(send_buf);
    sock = socket(PF_INET, SOCK_STREAM, 0);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &send_buf, &buf_len);
    if(state)
        error_handling("getsockopt() error!");
    printf("SO_SNDBUF: %d %d \n", send_buf, buf_len);

    sock = socket(PF_INET, SOCK_STREAM, 0);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recv_buf, &buf_len);
    if(state)
        error_handling("getsockopt() error!");
    printf("SO_RCVBUF: %d %d \n", recv_buf, buf_len);    
    
}