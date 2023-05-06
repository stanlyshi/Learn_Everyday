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
 *	流
 *
 *	当对于不同优先级的支持随SVR4被增加到流系统时，下面两个函数也被引入
 */

#include <stropts.h>

int getpmsg(int fd, struct strbuf *ctlptr, struct strbuf *dataptr, 
			int *bandp, int *flagsp);
int putpmsg(int fd, const struct strbuf *ctlptr, const struct strbuf *dataptr,
			int band, int flags);
/**
 *	band参数必须在0-255之间，
 *
 *	返回：
 *		成功：非负数
 *		出错：-1
 */

/**
 *	strbuf结构体
 */
struct strbuf
{
	int maxlen;
	int len;
	char *buf;
};
