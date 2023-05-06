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
    printf("---\n");
    int n;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    
    struct timeval tvbrfore, tvafter;
    
    long int npkg = 0, nbyte=0;
    
    char sendline[MAXLINE + 1] = {"abcdefghijklmnopqrstuvwxyz"};
    char recvline[MAXLINE + 1];
    
    if(argc != 4) {
        perror("Usage: udpcli <IPAddress> <port> <nb_recvpkg>\n");
        exit(1);
    }
    int port  = atoi(argv[2]);
    int nb_recvpkg = atoi(argv[3]);
   
    printf("%s:%d pkg %d\n", argv[1], port, nb_recvpkg);
 
    sockfd = udpsocket_client(argv[1], port, &servaddr);
    
    npkg = 0, nbyte=0;
    memset(&tvbrfore, 0, sizeof(struct timeval));
    memset(&tvafter, 0, sizeof(struct timeval));

    if((n = sendto(sockfd, sendline, MAXLINE, 0, 
                (struct sockaddr *)&servaddr, servlen)) <= 0)
    {
        perror("sendto error");
        exit(1);
    }
    printf("Send len %d\n", n);
    
    /* 统计时间 */
    my_gettimeval(&tvbrfore);

    while(1)
    {        
        
        if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, &servlen)) < 0)
        {
            perror("recvfrom error");
            break;
        } else {
            //printf("Recv len %d, %d/%d\n", n, npkg, nb_recvpkg);
            nbyte += n;
            if(npkg++ > nb_recvpkg) {
                break;
            }

        }
        recvline[n] = '\0';
    }
    
    /* 统计时间 */
    my_gettimeval(&tvafter);
    
    /* 输出此段时间内的速率 */
    my_statistic_throughput("Recvfrom", &tvbrfore, &tvafter, nbyte, npkg-1);

    close(sockfd);

    return 1;
}
