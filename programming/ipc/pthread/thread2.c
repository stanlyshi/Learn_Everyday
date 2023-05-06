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
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void* thread_main(void* param)
{
    int count = *((int*)param);
    for(int i=0;i<count;i++)
    {
        puts("thread run...");
    }

    char* msg = (char*)malloc(sizeof(char)*64);
    strcpy(msg, "hello, I'm thread~ \n");
    return (void*)msg;
}

int main(int argc, char* argv[])
{
    pthread_t t_id;
    int param = 5;
    void* retParam;
    
    if(0!=pthread_create(&t_id, NULL, &thread_main, &param))
    {
        perror("pthread_create error \n");
        exit(1);
    }

    if(0 != pthread_join(t_id, &retParam))
    {
        perror("pthread_join error \n");
        exit(1);
    }

    puts("in mainThread!");
    printf("Message from thread: %s \n", (char*)retParam);

    free(retParam);
    return 0;
}