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
#include<stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include<pthread.h>

#define NUM_THREAD 10

long long sum = 0;

void* thread_increase(void* arg)
{
    printf("%d\n", pthread_self());
    for(int i=0;i<50000000;i++)
    {
        sum+=1;
    }
    return 0;
}

void* thread_decrease(void* arg)
{
    printf("%d\n", pthread_self());
    for(int i=0;i<50000000;i++)
    {
        sum+=1;
        
    }
    return 0;
}

int main()
{
    pthread_t t_id[NUM_THREAD];

    for(int i=0;i<NUM_THREAD;i++)
    {
        if(i%2 == 0)
        {
            pthread_create(&t_id[i], NULL, thread_increase, NULL);
        }
        else
        {
            pthread_create(&t_id[i], NULL, thread_decrease, NULL);
        }
    }

    for(int i=0;i<NUM_THREAD;i++)
    {
        pthread_join(t_id[i], NULL);
    }

    printf("sizeof(long long): %d \n", sizeof(long long));
    printf("sum: %lld \n", sum);

    return 0;
}