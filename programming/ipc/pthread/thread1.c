/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:10 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:02 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:42 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

void* thread_proc(void* arg)
{
    int count = *(int*)arg;
    for(int i=0;i<count;i++)
    {
        puts("thread run");
        sleep(1);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    pthread_t t_id;
    int num = 5;
    
    if(pthread_create(&t_id, NULL, &thread_proc, &num) !=0)
    {
        perror("pthread_create error!");
        exit(1);
    }

    sleep(10);

    return 0;
}