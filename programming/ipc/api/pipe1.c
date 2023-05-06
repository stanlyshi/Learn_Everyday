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

int main()
{
    int filedes[2];
    pipe(filedes);

    char str[] = "who are you?";

    pid_t pid = fork();
    
    if(pid == 0)
    {
        puts("child will write data");
        write(filedes[1], str, sizeof(str));
    }
    else
    {
        char buf[32];
        puts("parent will read data");
        read(filedes[0], buf, sizeof(buf));
        puts(buf);
    }

    
    
    return 0;
}