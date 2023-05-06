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
#include<unistd.h>
#include<stdio.h>

int main ()
{
    int fds[2];

    pipe(fds);

    pid_t pid = fork();
    
    if(pid == 0)
    {
        char str[] = "I'm from child proc";
        char buf[32];

        puts("child proc will write data");
        write(fds[1], str, sizeof(str));
        sleep(2);

        read(fds[0], buf, sizeof(buf));
        printf("pid=0: %s\n", buf);
    }
    else
    {
        char str[] = "I'm from parent proc";
        char buf[32];

        read(fds[0], buf, sizeof(buf));
        printf("pid!=0: %s\n", buf);

        puts("parent will write data");
        write(fds[1], str, sizeof(str));
        sleep(2);
    }
    return 0;
}