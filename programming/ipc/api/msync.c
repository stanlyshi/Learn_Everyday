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
 *	有时候我们希望硬盘上的文件内容与内存映射区中的内容一致。
 *	调用msync函数
 */

#include <sys/mman.h>

int msync(void *addr, size_t len, int flags);
	//成功：0
	//错误：-1
	
/**
 *	addr和len参数通常指代内存中的整个内存映射区，
 *	flags参数为：
 *			MS_ASYNC	执行异步写
 *			MS_SYNC		执行同步写
 *			MS_INVALIDATE	执行高速缓存的数据失效
 */

