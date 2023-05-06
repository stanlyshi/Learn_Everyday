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
 *	以下三个函数用于构造待发送的路由首部
 */

#include <netinet/in.h>

socklen_t inet6_rth_space(int type, int segments);

void *inet_rth_init(void *rthbuf, socklen_t rthlen,
					int type, int segment);

int inet6_rth_add(void *rthbuf, const struct in6_addr *addr);

/**
 * 以下三个函数用于处理所接收的路由首部
 */
#include <netinet/in.h>

int inet6_rth_reverse(const void *in, void *out);

int inet6_rth_segments(const void *rthbuf);

struct in6_addr *inet6_rth_getaddr(const void *rthbuf, int index);
