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


int main(int argc, char *argv[])
{
    int n;
	socklen_t len;
    
	int sockfd;
	struct sockaddr_in cliaddr;

    struct timeval tvbrfore, tvafter;
    
    long int npkg = 0, nbyte=0;

    char buf[MAXLINE];
    
    sockfd = udpsocket_server();
    
	for(;;)
	{
        memset(buf, 0, sizeof(buf));
		len = sizeof(cliaddr);
        
		if((n = recvfrom(sockfd, buf, MAXLINE, 0, 
                         (struct sockaddr *)&cliaddr, &len)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}
		else
		{
            printf("");
			printf("Server recv %d: %s", sockfd, buf);
		}
		
        memset(buf, 0, sizeof(buf));
//		fgets(buf, MAXLINE, stdin);
        npkg = 0, nbyte=0;
        memset(&tvbrfore, 0, sizeof(struct timeval));
        memset(&tvafter, 0, sizeof(struct timeval));
		
        /* 统计时间 */
        my_gettimeval(&tvbrfore);
        
        while(1) {
    		if((n = sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, len)) < 0)
    		{
    			perror("sendto error");
                close(sockfd);
    			exit(1);
    		} else {
//              printf("Server send %d: %s", sockfd, buf);
                nbyte += n;
                if(npkg++ > 10000) {
                    break;
                }
    		}
        }
        /* 统计时间 */
        my_gettimeval(&tvafter);
        
        /* 输出此段时间内的速率 */
        my_statistic_throughput("Sendto", &tvbrfore, &tvafter, nbyte, npkg-1);

        /* 准备下次触发f_stack服务端 进行发包测试 */
        printf(">>");
	}

	return 1;
}
