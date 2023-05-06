/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:19 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:06 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:47 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
/**
 * UDP socket client.c
 *
The client does not establish a connection with the server. Instead, the client just sends a datagram to the server using the sendto function (described in the next section), which requires the address of the destination (the server) as a parameter. Similarly, the server does not accept a connection from a client. Instead, the server just calls the recvfrom function, which waits until data arrives from some client. recvfrom returns the protocol address of the client, along with the datagram, so the server can send a response to the correct client.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "common.h"

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    socklen_t servlen = sizeof(servaddr);
    
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    
    if(argc != 2)
    {
        perror("Usage: udpcli <IPAddress>");
        exit(1);
    }

    sockfd = udpsocket_client(argv[1], &servaddr);
    
    printf("Client Input:");
    
    while(fgets(sendline, MAXLINE, stdin) != NULL)
    {
        /*
        ssize_t sendto(int sockfd, const void *buff, size_t nbytes, int flags, 
                        const struct sockaddr *to, socklen_t addrlen);
        */
        if(sendto(sockfd, sendline, strlen(sendline), 0, 
                    (struct sockaddr *)&servaddr, servlen) < 0)
        {
            perror("sendto error");
            exit(1);
        }
        else
        {
            printf("Client send %d: %s", sockfd, sendline);
        }
        
        /*
        #include <sys/socket.h>
        ssize_t recvfrom(int sockfd, void *buff, size_t nbytes, int flags, 
                        struct sockaddr *from, socklen_t *addrlen);
        */
        if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, &servlen)) < 0)
        {
            perror("recvfrom error");
            exit(1);
        }
        else
        {
            printf("Client recv %d: %s", sockfd, recvline);
        }
        recvline[n] = '\0';
        
        printf("Client Input:");
    }

    return 1;
}
