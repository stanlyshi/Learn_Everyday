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
 *	信号量 P179
 *
 *		信号量（信号灯），有时被称为信号灯，是在多线程环境下使用的一种设施。
 *	是可以用来保证两个或多个关键代码段不被并发调用。
 *		在进入一段代码之前，线程必须获取一个信号量，一旦该关键代码完成了，
 *	那么该线程必须释放信号量。
 *		其他向进入该段代码的线程必须等待直到第一个线程释放信号量。
 *		为了完成这个过程，需要创建一个信号量VI，然后将Acquire Semaphore VI
 *	以及Release Semaphore VI分别放置在每个关键代码段的首末，确认这些信号量VI引用的
 *	是初始创建的信号量。如在这个停车场系统中，车位是公共资源，每辆车好比一个线程，
 *	看门人气的是信号量的作用。
 *
 * 	合理比喻：
 *
 *		车位：		公共资源
 *		车：		线程
 *		看门人：	信号量
 */
#include <semaphore.h>

sem_t *sem_open(const char *name, int oflag, ...
				/*mode_t mode, unsigned int value*/);
	//成功：返回指向信号量的指针
	//错误：SEM_FAILED
	

int sem_close(sem_t *sem);
	//成功：0
	//错误：-1
	

/**
 *	删除信号量
 */
int sem_unlink(const char *name);
	//成功：0
	//错误：-1
	

/**
 *	sem_wait函数测试所指定信号量的值：
 *		>0	将它-1立即返回
 *		=0	调用线程就被投入睡眠，知道改值变为大于0，这是再将它-1，函数随后返回。
 */
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
	//成功：0
	//错误：-1
	

/**
 * 使用外某个信号量后，调用sem_post
 */

int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem, int valp);
	//成功：0
	//错误：-1
	

/**
 *	上面的内容是有名信号量，这些信号量由一个name标识，它通常指代文件系统的某个文件
 *	然而Posix也提供基于内存的信号量，他们由应用程序分配信号量的内存空间（也就是
 *	分配一个sem_t数据类型的内存空间），然后由系统初始化他们的值。
 */
int sem_init(sem_t *sem, int shared, unsigned int value);
	//出错：返回-1

int sem_destory(sem_t *sem);
	//成功：0
	//出错：-1
	




















