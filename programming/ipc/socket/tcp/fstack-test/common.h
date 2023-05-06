/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#ifndef _COMMON_H
#define _COMMON_H

#include <netinet/tcp.h>

#define MAX_LINE       1500
#define PORT           2152
#define BACKLOG        10
#define LISTENQ        6666
#define MAX_CONNECT    20

int tcpsocket_server();

static long int gettimeval(struct timeval *tv)
{
    gettimeofday(tv, NULL);    
}

#endif /*<_COMMON_H>*/
