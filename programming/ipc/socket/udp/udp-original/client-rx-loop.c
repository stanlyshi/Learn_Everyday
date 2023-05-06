/**
 *  测试F-Stack的UDP发包速率
 *  作者：荣涛 <rongtao@sylincom.com>
 *  时间：2020年7月16日10:08:34
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "common.h"

int sockfd = 0;

void sig_handler(int signum)
{
    printf("Catch ctrl+C(SIGINT) signal.\n");
    close(sockfd);
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_handler);

    int n;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    
    struct timeval tvbrfore, tvafter;
    
    long int npkg = 0, nbyte=0;
    
    char sendline[MAXLINE] = {"9999999999999999999999"}, recvline[MAXLINE + 1];
    
    if(argc != 2) {
        perror("Usage: udpcli <IPAddress>");
        exit(1);
    }

    sockfd = udpsocket_client(argv[1],PORT, &servaddr);
        
    printf("Ready to send>> %s\n", sendline);
    
    if(sendto(sockfd, sendline, strlen(sendline), 0, 
                (struct sockaddr *)&servaddr, servlen) <= 0)
    {
        perror("sendto error");
        exit(1);
    } else {
        printf("Client send to >> %s(%s)\n", argv[1], strerror(errno));
    }
    
    while(1)
    {
        npkg = 0, nbyte=0;
        memset(&tvbrfore, 0, sizeof(struct timeval));
        memset(&tvafter, 0, sizeof(struct timeval));
        
        /* 统计时间 */
        my_gettimeval(&tvbrfore);
        
        while(1) {
            if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, &servlen)) < 0)
            {
                perror("recvfrom error");
                break;
            } else {
//                printf("Client recv %d: %s", sockfd, recvline);
                nbyte += n;
                if(npkg++ > 10000) {
                    break;
                }

            }
            recvline[n] = '\0';
        }
        
        /* 统计时间 */
        my_gettimeval(&tvafter);
        
        /* 输出此段时间内的速率 */
        my_statistic_throughput("Recvfrom", &tvbrfore, &tvafter, nbyte, npkg-1);

        /* 准备下次触发f_stack服务端 进行发包测试 */
        printf("Continue... ");
    }

    close(sockfd);

    return 1;
}

