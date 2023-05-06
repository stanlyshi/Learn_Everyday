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
    socklen_t clt_addr_len;
    int listen_fd;
    int com_fd; 
    int ret = 0;
    int i;
    int len;
    struct sockaddr_un clt_addr, srv_addr;
    
    listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(listen_fd < 0)
    {
        perror("sreate listening socket error.");
        exit(1);
    }
    else
    {
        srv_addr.sun_family = AF_UNIX;
        strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path)-1);
        
        unlink(UNIX_DOMAIN);
        
        ret = bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
        
        if(ret == -1)
        {
            perror("cannot bind server socket.");
            close(listen_fd);
            
            unlink(UNIX_DOMAIN);
            
            exit(1);
        }
        
        ret = listen(listen_fd, 1);
        if(ret == -1)
        {
            perror("cannot listen the client connect request.");
            close(listen_fd);
            unlink(UNIX_DOMAIN);
            exit(1);
        }
        
        chmod(UNIX_DOMAIN, 00777);
        
        while(1)
        {
            //当有连接请求时，调用accept函数建立连接
            len = sizeof(clt_addr);
            com_fd = accept(listen_fd, (struct sockaddr*)&clt_addr, &len);
            
            if(com_fd < 0)
            {
                perror("cannot accept client connect request.");
                close(listen_fd);
                unlink(UNIX_DOMAIN);
                break;
            }
            //读取并输出客户端发送过来的链接信息
            memset(recv_php_buf, 0, 256);
            recv_php_num = read(com_fd, recv_php_buf, sizeof(recv_php_buf));
            printf("----info----\n%d:  ", com_fd);
            for(i=0; i<recv_php_num; i++)
            {
                printf("%d, ", recv_php_buf[i]);
            }
            printf("\n");
            
            
            close(com_fd);
        }
    }
}