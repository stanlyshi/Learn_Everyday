/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void timeout(int signo)
{
    if(signo == SIGALRM)
    {
        puts("time out!");
    }
    alarm(2);
}

void ketcontrol(int signo)
{
    if(signo == SIGINT)
    {
        puts("CTRL+C pressed!");
    }
}

/*这样就有问题了*/
void callback(int signo)
{
    if(signo == SIGALRM)
    {
        puts("time out!");
    }
    alarm(2);
    if(signo == SIGINT)
    {
        puts("CTRL+C pressed!");
    }
}

int main(int argc, char* argv[])
{
    signal(SIGALRM, timeout);
    signal(SIGINT, ketcontrol);
    alarm(2);
    
    int i;
    for(i=0;i<3;i++)
    {
        puts("wait...");
        sleep(100);
    }

    return 0;
}