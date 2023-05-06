/**
 *  测试F-Stack的UDP发包速率
 *  作者：荣涛 <rongtao@sylincom.com>
 *  时间：2020年7月16日10:08:34
 */
#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <stdint.h>

#define PORT        2152
#define MAXLINE     1500 

int udpsocket_server();
int udpsocket_client(const char *ipv4,int port, struct sockaddr_in *servaddr);

inline long int my_gettimeval(struct timeval *tv);

inline void my_statistic_throughput(char *description, 
            struct timeval *before, struct timeval *after, unsigned long int bytes, long int npkg);


#endif /*<_COMMON_H>*/
