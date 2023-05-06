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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "tcp_common.h"

/*
$ ./client -h 10.21.3.242 -p 8888
    client: connecting to  ...
    server: got connection from 10.21.3.242
    client: received 'Hello, world!'

*/

void help() 
{
    printf("client: invalid hostname or port!\n");
}

int main(int argc, char *argv[])
{
    char buf[MAXDATASIZE];
    char* hostname = NULL;
    int csock, numbytes;  
    char* port = NULL;

    while (1) 
    {
        char c;

        c = getopt (argc, argv, "h:p:");
        

        switch (c) 
        {
            case 'h':
                hostname = optarg;
                break;
                
            case 'p':
                port = optarg;   
                break;
                
            case '?':
            default:
                hostname = "127.0.0.1";
                port = PORT;
                break;
        }
        if (c == -1) 
        {
            break;
        }
    }

    if (!hostname || !port) 
    {
        help();
        exit(2);
    }
    
    csock = tcp_connect(hostname, port);
    
    if (csock < 0) 
    {
        return -1;
    }

    if ((numbytes = recv(csock, buf, MAXDATASIZE-1, 0)) == -1) 
    {
        perror("recv");
        exit(1);
    }

    buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);

    close(csock);

    return 0;
}









