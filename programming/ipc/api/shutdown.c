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
#include <sys/socket.h>

/**
 *	终止网络连接的方法是close函数。不过close有两个限制：
 *		1.close把描述符的引用计数-1，仅在该计数变为0时才关闭套接字；
 *		2.close终止读和写两个方向的数据传送。
 */
int shutdown(int sockfd, int howto);

/**
 *	shutdown依赖于howto参数的值：
 *		
 *		SHUT_RD：关闭套接字读的这一半。
 *				套接字中不再有数据可接收，而且套接字接收缓冲区中的现有数据都被丢弃。
 *				进程不能再对这样的套接字调用任何读函数。对于TCP套接字这样调用
 *				shutdown函数后，由该套接字接收的来自对端的任何数据都被确认，然后
 *				悄然丢弃。
 *
 *		SHUT_WR：关闭连接的写的这一半。
 *				对于TCP套接字，这称为半关闭。
 *
 *		SHUT_RDWR：连接的读半部和写半部都关闭。
 *
 *
 *
 *
 *	SHUT_RD
 *		The same semantics as for TCP discussed in Section 6.6(See 8.4.6); no SCTP 
 *		protocol action is taken.
 *	SHUT_WR
 *		Disables further send operations and initiates the SCTP shutdown procedures, 
 *		which will terminate the association. Note that this option does not provide 
 *		a half-closed state, but does allow the local endpoint to read any queued data 
 *		that the peer may have sent prior to receiving the SCTP SHUTDOWN message.
 *	SHUT_RDWR
 *		Disables all read and write operations, and initiates the SCTP shutdown procedure. 
 *		Any queued data that was in transit to the local endpoint will be acknowledged and 
 *		then silently discarded.
 */
