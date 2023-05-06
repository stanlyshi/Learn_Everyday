/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:21 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "common.h"

int connect_fd;

void handle_sigint(int signum)
{
    printf("Catch the SIGINT signal.\n");
    write(connect_fd, GOODBYE, 20);
    close(connect_fd);
    exit(1);
}

int main()
{
    int ret = 0;
    struct sockaddr_un srv_addr;
    int i;
    
    char msg[256] = HELLO;
    int msglen = 0;
    
    signal(SIGINT, handle_sigint);
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(connect_fd < 0)
    {
        perror("create socket error.");
        exit(1);
    }
    
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, PATH);
    
    ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
        perror("connect error");
        close(connect_fd);
        exit(1);
    }
    
    while(1)
    {
        write(connect_fd, msg, 20);
        
        memset(msg, 0 , sizeof(msg));
        msglen = read(connect_fd, msg, sizeof(msg));
        
        printf("%s\n", msg);
        
        memset(msg, 0 , sizeof(msg));
        printf("Input: ");
        scanf("%s", msg);
        
        if(strcmp((char*)msg, QUIT) == 0)
        {
            raise(SIGINT);
        }
    }
    
    
    close(connect_fd);

}