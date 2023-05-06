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
/**
 *	pthread_once
 *
 *		处理线程特定数据时，通常首先调用
 *		pthread_once和pthread_key_create两个函数。
 */

#include <pthread.h>

int pthread_once(pthread_once_t *onceptr, void (*init)(void));
int pthread_key_create(pthread_key_t *keyptr, void (*destructor)(void *value));

/**
 *	每当一个使用线程特定数据的函数被调用时，pthread_once函数通常转而被该函数调用，
 *	不过pthread_once使用有onceptr参数指向的变量中的值，确保init参数所指向的函数
 *	在进程范围内只被调用一次。
 *
 *	在进程范围内对于一个给定键，pthread_key_create只能被调用一次。
 *	所创建的键通过keyptr参数指针返回，如果destructor指针不为空指针，它所指的函数
 *	将由为该键存放过耨个之的每个线程在终止时调用。
 *
 *	
 */

/** pthread_once **
 *   只执行一次！！！
 */
/** pthread_key_create
 * 		为线程创建了同一名不同值的全局变量
 */



//两个函数的典型用法：
pthread_key_t r1_key;
pthread_once_t r1_once = PTHREAD_ONCE_INIT;

void readline_once(void)
{
	pthread_key_create(&r1_key, readline_dextructor);
}
ssize_t readline(...)
{
	...
	pthread_once(&r1_once, readline_once);
	if((ptr = pthread_getspecific(r1_key)) == NULL)
	{
		ptr = malloc(...);
		pthread_setspecific(r1_key, ptr);
		/*初始化内存指针*/
	}
	/*指针指向值*/
}
