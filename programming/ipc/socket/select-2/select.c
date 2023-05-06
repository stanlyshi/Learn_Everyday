/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:16 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:45 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/time.h>

#define BUF_SIZE 32

int main()
{
    char buf[BUF_SIZE];

    fd_set readSet, tmpSet;
    FD_ZERO(&readSet);
    FD_SET(0, &readSet);

    struct timeval timeout;
    /*
    timeout.tv_sec = 5;
    timeout.tv_usec = 5000;
    */

    while(1)
    {
        tmpSet = readSet;

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int result = select(1, &tmpSet, 0, 0, &timeout);
        
        if(result == -1)
        {
            puts("select() error!");
            break;
        }
        else if(result == 0)
        {
            puts("time out 5ssssssssss!");
            //break;
        }
        else
        {
            if(FD_ISSET(0, &tmpSet))
            {
                int len = read(0, buf, BUF_SIZE);
                buf[len] = 0;

                printf("Message from consle %s", buf);
            }
        }
    }

    return 0;
}