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
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

/**
# 复制文件描述符
```
#include<unistd.h>
int dup(int filedes);
int dup2(int filedes, int filedes2);
//成功返回复制后的文件描述符，失败时返回-1
```
- filedes:需要复制的文件描述符
- filedes2:明确指定的文件描述符整数值
*/

int main(int argc, char *argv[])
{
    char str1[] = "hello~\n";
    char str2[] = "I love programming!\n";

    int fd1 = dup(1);
    int fd2 = dup2(1, 88);

    printf("fd1: %d  fd2: %d \n", fd1, fd2);

    write(fd1, str1, sizeof(str1));
    write(fd2, str2, sizeof(str2));

    close(fd1);
    close(fd2);

    puts(str1);

    close(1);

    puts(str2);

    return 0;
}