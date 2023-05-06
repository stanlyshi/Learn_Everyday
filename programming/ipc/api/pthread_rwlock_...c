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
 *	读写锁
 */
#include <pthread.h>

/**
 *	获取与释放读写锁
 */

int pthread_rwlock_rdlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_unlock(pthread_rwlock_t *rwptr);
	//成功：返回0
	//错误：返回正的Exxx值
	
/**
 *	获取一个读出锁或写入锁,
 *		但是如果该锁不能马上取得，那就返回一个EBUSY错误，
 *		而不是把调用线程投入睡眠
 */
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwptr);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwptr);
	//成功：返回0
	//错误：Exxx
	

/**
 *	读写锁属性
 */
int pthread_rwlock_init(pthread_rwlock_t *rwptr,
						const pthread_rwlockattr_t *attr);
int pthread_rwlock_destory(pthread_rwlock_t *rwptr);
	//成功：0
	//错误：Exxx
	
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr,
									int *valptr);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr);
	//成功：0
	//错误：Exxx
	

/**
 * pthread_rwlock_t结构体
 */
typedef struct {

	pthread_mutex_t		rw_mutex;
	pthread_cond_t		rw_condreaders;
	pthread_cond_t		rw_condwriters;

	int		rw_magic;
	int		rw_nwaitreaders;
	int		rw_nwaitwriters;
	int		rw_reffcount;

}pthread_rwlock_t;










