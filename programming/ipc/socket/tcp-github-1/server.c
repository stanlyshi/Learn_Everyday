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
#include <signal.h>
#include <sys/wait.h> 

#include "tcp_common.h"

int main(int argc, char* argv[])
{
    char* port;
    int backlog;

    if (argc == 2) {
        if (argv[1] == "-h") {
            print_usage(argv[0]);
            exit(0);
        }

        port = argv[1];
    }
    else {
        //set up the defaults
        port = PORT;
        backlog = BACKLOG;
    }

    //this call will block
    start_server_select(port, backlog);

    //we will never reach here
    printf("server is exiting ...\n");
    
    return 0;
}

