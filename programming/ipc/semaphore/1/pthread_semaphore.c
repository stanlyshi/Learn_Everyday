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
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct Struct
{
	sem_t *lock;
	int num;
}STRUCT;

void test(void *obj)
{
	STRUCT *point = (STRUCT*)obj;
	sem_t *semlock = NULL;
	semlock = point->lock;
	/*信号量>0立即返回，=0线程睡眠，直到值>0*/
	sem_wait(semlock);
	
	FILE *f = fopen("test.txt","a");
	
	if(f == NULL)
	{
		printf("fopen error\n");
	}
	printf("sem_wait() %d, %d\n", point->num, *point->lock);

	int j = 0;
	for(j=0; j<30; j++)
	{
		fprintf(f, "%c-", 'a'+point->num);
	}
	fprintf(f, "\n", 'a'+point->num);
	fclose(f);
	sem_post(semlock);
	
	return;
}

int main()
{
	pthread_t pid[20];
	
	int ret, i = 0;
	
	STRUCT obj[13];

	sem_t semlock;
	/*初始化无名信号量*/
	sem_init(&semlock, 0, 1);
	if(sem_init(&semlock, 0, 1) != 0)
	{
		printf("sem_init error\n");
	}
	if(sem_init(&semlock, 0, 1) != 0)
	{
		printf("sem_init error\n");
	}
	for(i=0;i<10;i++)
	{
		obj[i].num = i;
		obj[i].lock = &semlock;
		
		/*创建线程*/
		ret = pthread_create(&pid[i], NULL, (void *)test, &obj[i]);
		
		if(ret != 0)
		{
			printf("create thread wrong %d\n", i);
			return 0;
		}
	}
	for(i=0; i< 10; i++)
	{
		/*将线程加入结束线程*/
		pthread_join(pid[i], NULL);
	}
	
	return 1;
}
