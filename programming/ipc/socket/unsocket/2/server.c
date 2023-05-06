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
    int listen_fd;
    int com_fd; 
    int ret = 0;
    int i;
    int len;
    struct sockaddr_un clt_addr;
    
    listen_fd = unsocket_server(UNIX_DOMAIN);
    
    while(1)
    {
        len = sizeof(clt_addr);
        
        com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);
        
        if(com_fd < 0)
        {
            perror("cannot accept client connect request.");
            close(listen_fd);
            unlink(UNIX_DOMAIN);
            break;
        }
        
        memset(buf, 0, MSG_LENGTH);
        len = read(com_fd, buf, MSG_LENGTH);
        
        printf("%d: %d:%s\n", com_fd, len, buf);
        
        buf[0] = 'X';
        
        write(com_fd, buf, MSG_LENGTH);
        
        close(com_fd);
    }
}