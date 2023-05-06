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
#ifndef _COMMON_H
#define _COMMON_H


#define BUF_SIZE        256
#define NAME_SIZE       16
#define PORT            8888
#define MAX_CLNT        256
#define LISTENQ         5


void errorHandler(const char* message);

int tcpsocket_server();
int tcpsocket_client(const char *ipv4_str);


#endif /*<_COMMON_H>*/