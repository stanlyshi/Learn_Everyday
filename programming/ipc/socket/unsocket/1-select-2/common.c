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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "common.h"


int client[MAC_MAX_NUM];

int unsocket_init()
{
    int i;
    for(i=0; i<MAC_MAX_NUM; ++i)
	{
		client[i] = -1;
	}
    return 0;
}

int unsocket_server(const char *unix_path)
{
    int listenfd, ret = -1;
    struct sockaddr_un srv_addr;
    
    listenfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(listenfd < 0)
    {
        perror("sreate listening socket error.");
        return -1;
    }
    
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, unix_path, sizeof(srv_addr.sun_path)-1);
    
    ret = bind(listenfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
        perror("cannot bind server socket.");
        close(listenfd);
        unlink(unix_path);
        return -1;
    }
    
    ret = listen(listenfd, 1);
    
    if(ret == -1)
    {
        perror("cannot listen the client connect request.");
        close(listenfd);
        unlink(unix_path);
        return -1;
    }
    return listenfd;
}


int unsocket_client(const char *unix_path)
{
    int connect_fd, ret = -1;
    struct sockaddr_un srv_addr;
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(connect_fd < 0)
    {
        perror("create socket error.");
        return -1;
    }
    
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, unix_path);
    
    ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
    if(ret == -1)
    {
        perror("connect error");
        close(connect_fd);
        return -1;
    }
    return connect_fd;
}