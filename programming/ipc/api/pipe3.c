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
#include<stdlib.h>

int main()
{
    char str1[] = "who are you?";
    char str2[] = "Thank you for your message.";

    char buf[32];

    int fds1[2], fds2[2];

    pipe(fds1), pipe(fds2);

    pid_t pid = fork();
    
    if(pid == 0)
    {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, sizeof(buf));

        printf("child proc output: %s \n", buf);
    }
    else
    {
        read(fds1[0], buf, sizeof(buf));
        printf("parent proc output: %s\n", buf);
        
        write(fds2[1], str2, sizeof(str2));
    }

    return 0;
}