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
 *	pthread_getspecific
 *	pthread_setspecific
 */

#include <pthread.h>

void pthread_getspecific(pthread_key_t key);
	//返回：指向线程特定数据的指针（有可能是个空指针）
int pthread_setspecific(pthread_key_t key, const void *value);
	//返回：若成功则为0，若出错则为正的Exxx值  
	

/**
 * pthread_setspecific
 *
 * 		使用该函数可以为指定线程特定数据键设置线程特定绑定。
 *
 */

