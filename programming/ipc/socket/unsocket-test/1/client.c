/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:22 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:08 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include "common.h"

void main()
{
    int connect_fd;
    int ret = 0;
    static struct sockaddr_un srv_addr;
    
    connect_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(connect_fd < 0)
    {
        perror("create socket error.");
        exit(1);
    }
    else
    {
        srv_addr.sun_family = AF_UNIX;
        strcpy(srv_addr.sun_path, UNIX_DOMAIN);
        
        ret = connect(connect_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
        
        if(ret == -1)
        {
            close(connect_fd);
            exit(1);
        }
        
        write(connect_fd, recv_php_buf, 20);
        
        close(connect_fd);
            
    }
}