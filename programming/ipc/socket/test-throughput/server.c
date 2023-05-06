/**
 *  server.c  测试吞吐量服务器端代码
 *   rongtao@sylincom.com
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


int tcpsocket_server(int port)
{
    struct sockaddr_in servaddr;
    
    int listenfd;
    
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
		return -1;
    } 
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
		return -1;
    }
    
    if(listen(listenfd, LISTENQ) < 0)
    {
		return -1;
    }  
    
    return listenfd;
}


int udpsocket_server(int port)
{
    int sockfd;
    
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;    
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
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


int tcp_server_run(struct parse_args *arguments)
{
    struct sockaddr_in cliaddr;
    
    int listenfd, connfd;
    pid_t childpid;

    char buf[MAX_LINE];
    
    socklen_t clilen;

    if((listenfd = tcpsocket_server(arguments->port))<0)
    {
		perror("socket error");
		exit(1);
    }
	
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,NULL,0);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,NULL,0);
	clilen = sizeof(cliaddr);

    int val = 1;
	setsockopt(listenfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    
    for(;;)
    {
        if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
        {
            perror("accept error");
            exit(1);
        }
		if((childpid = fork()) == 0)
		{
	    	close(listenfd);
			setsockopt(connfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));

            struct timeval before, after;
            static int flag = 0;

	    	ssize_t recvbytes = 0, sendbytes = 0, n;
            
            memset(buf, 0, MAX_LINE);
	    	while((n = read(connfd, buf, MAX_LINE)) > 0)
	    	{
        		if(flag == 0) {
        			gettimeval(&before);
        		}
                
	    	    recvbytes += n;
                
	    	    if(arguments->test_malloc_free)
				    test_malloc_and_free(buf, n);
                
	    	    if(arguments->test_printf) {
//                    test_printf(buf, n);
                    printf("Test printf: recv bytes %ld\n", recvbytes);
                }
                if(arguments->reply_when_recv)
                    sendbytes += write(connfd, buf, n);

                
                flag = 1;
	    	}
        	gettimeval(&after);

            statistic_throughput("Server Recv", &before, &after, recvbytes);
            
            if(arguments->reply_when_recv)
                statistic_throughput("Server Send", &before, &after, sendbytes);

            
	    	exit(0);
		}
		close(connfd);
    }    
    
    close(listenfd);   

}



int udp_server_run(struct parse_args *arguments)
{
    int n;
    socklen_t len;

    int sockfd;
    struct sockaddr_in cliaddr;

    char buf[MAX_LINE];

    sockfd = udpsocket_server(arguments->port);

    int val = 1;
	setsockopt(sockfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    

    struct timeval before, after;
    static int flag = 0;
	ssize_t recvbytes = 0, sendbytes = 0;
    
    while((n = recvfrom(sockfd, buf, MAX_LINE, 0, 
                     (struct sockaddr *)&cliaddr, &len)) > 0)
    {
        if(flag == 0) {
            printf("Start counting time.\n");
			gettimeval(&before);
		}
        
	    recvbytes += n;
        
        if(arguments->test_malloc_free)
		    test_malloc_and_free(buf, n);
        
	    if(arguments->test_printf) {
//                    test_printf(buf, n);
            printf("Test printf: recv bytes %ld\n", recvbytes);
        }
        if(arguments->reply_when_recv)
            sendbytes += sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, len);
        
        flag = 1;
    }
    
    gettimeval(&after);

    statistic_throughput("Server Recv", &before, &after, recvbytes);

    if(arguments->reply_when_recv)
        statistic_throughput("Server Send", &before, &after, sendbytes);
    
    return 0;
}
