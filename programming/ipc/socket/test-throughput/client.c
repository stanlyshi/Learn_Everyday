/**
 * client.c 测试吞吐量客户端代码
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"



int tcpsocket_client(const char *ipv4, struct sockaddr_in *servaddr, int port)
{
    int sockfd;
    
    if(ipv4 == NULL)
    {
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return -1;
	}
    
    bzero(servaddr, sizeof(struct sockaddr_in));
	servaddr->sin_family = AF_INET;
	servaddr->sin_port = htons(port);
	
	if(inet_pton(AF_INET, ipv4, &servaddr->sin_addr) < 0)
	{
		return -1;
	}
	
	if(connect(sockfd, (struct sockaddr*)servaddr, sizeof(struct sockaddr_in)) < 0)
	{
		return -1;
	}	
    return sockfd;
}



int udpsocket_client(const char *ipv4, struct sockaddr_in *servaddr, int port)
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


int tcp_client_run(struct parse_args *arguments)
{
	int sockfd;
	struct sockaddr_in servaddr;

    char addr[16] = {0};
    inet_ntop(AF_INET, &arguments->ipv4addr, addr, 16);

	if((sockfd = tcpsocket_client(addr, &servaddr, arguments->port)) == -1)
	{
		perror("socket error");
		exit(1);
	}
	int val = 1;
	setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));

	
	/*4.消息处理*/
	char sendline[MAX_LINE] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"};
	char recvline[MAX_LINE] = {0};
    
	ssize_t sendbytes = 0, recvbytes = 0, pkgcount = 0;
	struct timeval before, after;
	static int flag = 0;

	printf("Ready to send .\n");

	while(pkgcount<arguments->pkg_num)
	{
		if(flag == 0) {
			gettimeval(&before);
		}
		sendbytes += write(sockfd, sendline, arguments->pkg_size);
	
	    if(arguments->test_malloc_free)
		    test_malloc_and_free(sendline, arguments->pkg_size);
        
	    if(arguments->test_printf) {
            printf("Test printf: %ld/%ld\n", pkgcount+1, arguments->pkg_num);
        }
        
        if(arguments->recv_after_send)
            recvbytes += read(sockfd, recvline, MAX_LINE);
        
		flag = 1;
		pkgcount ++;
	}
	gettimeval(&after);

    statistic_throughput("Client Send", &before, &after, sendbytes);

    if(arguments->recv_after_send)
        statistic_throughput("Client Recv", &before, &after, recvbytes);

    
	close(sockfd);

	return 1; 
}


int udp_client_run(struct parse_args *arguments)
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    
    char addr[16] = {0};
    inet_ntop(AF_INET, &arguments->ipv4addr, addr, 16);
    
    sockfd = udpsocket_client(addr, &servaddr, arguments->port);
        
    int val = 1;
	setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    
	ssize_t sendbytes = 0, recvbytes = 0, pkgcount = 0;
	struct timeval before, after;
	static int flag = 0;
    
	char sendline[MAX_LINE] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"};
	char recvline[MAX_LINE] = {0};
    
	while(pkgcount<arguments->pkg_num)
	{
        sendbytes += sendto(sockfd, sendline, strlen(sendline), 0, 
                                (struct sockaddr *)&servaddr, servlen);
        if(flag == 0) {
            printf("sendto peer.\n");
            gettimeval(&before);
        }

	    if(arguments->test_malloc_free)
		    test_malloc_and_free(sendline, arguments->pkg_size);
        
	    if(arguments->test_printf) {
            printf("Test printf: %ld/%ld\n", pkgcount+1, arguments->pkg_num);
        }
        
        if(arguments->recv_after_send)
            recvbytes += recvfrom(sockfd, recvline, MAX_LINE, 0, NULL, &servlen);
        
		flag = 1;
		pkgcount ++;
    }

	gettimeval(&after);

    statistic_throughput("Client Send", &before, &after, sendbytes);

    if(arguments->recv_after_send)
        statistic_throughput("Client Recv", &before, &after, recvbytes);

    
	close(sockfd);

    return 1;
}

