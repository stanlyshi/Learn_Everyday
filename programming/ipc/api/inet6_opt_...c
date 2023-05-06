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
 *	IPv6选项   《Unix网络编程》  P571
 *
 *	以下四个函数用于构造待发送的选项。
 */

#include <netinet/in.h>

int inet6_opt_init(void *extbuf, socklen_t extlen);
int inet6_opt_append(void *extbuf, socklen_t extlen,
					int offset, uint8_t type, socklen_t len,
					uint8_t align, void **databufp);
int inet6_opt_finish(void *extbuf, socklen_t extlen, int offset);
int inet6_opt_set_val(void *databuf, int offset, 
					const void *val, socklen_t vallen);


/**
 *	以下三个函数用于处理所接收的选项
 */
int inet6_opt_next(const void *extbuf, socklen_t extlen, int offset,
					uint8_t *type, socklen_t *lenp, void **databufp);
int inet6_opt_find(const void *extbuf, socklen_t extlen, int offset,
					uint8_t type, socklen_t *lenp, void **databufp);
int inet6_opt_get_val(const void *databuf, int offset,
					void *val, socklen_t vallen);
