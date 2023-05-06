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
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>

#define NUM_THREAD 10

/**
互斥段代码只允许一个线程在一个时刻访问，互斥锁
*/

long long sum = 0;
pthread_mutex_t mutex;

void* thread_increase(void* arg)
{
    pthread_mutex_lock(&mutex);
    for(int i=0;i<500;i++)
    {
        sum += 1;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

void* thread_decrease(void* arg)
{
    pthread_mutex_lock(&mutex);
    for(int i=0;i<500;i++)
    {
        sum -= 1;
        printf("pthread_t = %d, sum = %d\n", pthread_self(), sum);
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t t_id[NUM_THREAD];

    for(int i=0;i<NUM_THREAD;i++)
    {
        if(i%2 == 0){
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

    printf("sum: %lld \n", sum);

    pthread_mutex_destroy(&mutex);
    return 0;
}