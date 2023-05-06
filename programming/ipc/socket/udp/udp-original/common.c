/**
 *  测试F-Stack的UDP发包速率
 *  作者：荣涛 <rongtao@sylincom.com>
 *  时间：2020年7月16日10:08:34
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/ip.h>

#include "common.h"


int udpsocket_server()
{
    int sockfd;
    
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;    
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return -1;
    }

    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        return -1;
    }
    
    return sockfd;
}

int udpsocket_client(const char *ipv4, int port, struct sockaddr_in *servaddr)
{
    int sockfd, t;
    
    bzero(servaddr, sizeof(struct sockaddr_in));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(port);
    
    if((t = inet_pton(AF_INET, ipv4, &servaddr->sin_addr)) <= 0)
    {
        return -1;
    }
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        return -1;
    }
    
    return sockfd;
}


inline long int my_gettimeval(struct timeval *tv)
{
    gettimeofday(tv, NULL);    
}
inline void my_statistic_throughput(char *description, 
            struct timeval *before, struct timeval *after, unsigned long int bytes, long int npkg)
{
//    printf("\t -- before time: %ld, %ld\n", before->tv_sec, before->tv_usec);
//    printf("\t --  after time: %ld, %ld\n", after->tv_sec, after->tv_usec);
    printf("-- %s: Total %.3lf Mbps, bytes = %ld(bits:%ld), npkg = %ld.\n", 
                            description?description:"Unknown Description", 
                            8*bytes*1.0/((after->tv_sec-before->tv_sec)*1000000
    						            +after->tv_usec-before->tv_usec), bytes, bytes*8, npkg);
}

