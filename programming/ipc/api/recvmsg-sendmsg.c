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
 *	recvmsg	和 sendmsg函数时最常用的两个IO函数，实际上我们可以把所有的
 *	read，readv，recv，recvfrom调用替换成recvmsg调用。
 *	类似的，各种输出函数调用也可以替换成sendmsg函数。
 */

#include <sys/socket.h>

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags);

/**
 *	msghdr结构体
 */
struct msghdr
{
	void			*msg_name;		//protocol address
	socklen_t		msg_namelen;	//size of protocol address
	struct iovec 	*msg_iov;		//scatter/ gather array
	int				msg_iovlen;		//elements in msg_iov
	void			*msg_control;	//ancillary data
	socklen_t		msg_controllen;	//length of ancillary data
	int				msg_flags;		//falgs returned by recvmsg
};
