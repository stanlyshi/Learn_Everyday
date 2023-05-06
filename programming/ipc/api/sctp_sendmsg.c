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
 *	sctp_sendmsg
 *
 *	使用伴随辅助数据的sendmsg函数，应用进程能够控制sctp的各种特性，
 *	然而既然是用辅助数据可能不太方便，许多sctp实现提供了一个辅助函数库调用
 *	（有可能作为系统调用实现），以方便应用进程使用sctp的高级特性。
 */
#include <netinet/sctp.h>

ssize_t sctp_sendmsg(int sockfd, const void *msg, size_t msgsz,
					const struct sockaddr *to, socklen_t tolen,
					uint32_t ppid, uint32_t flags, uint16_t stream,
					uint32_t timetolive, uint32_t context);

/**
 *	sockfd:
 *	msg：长度为msgsz字节的缓冲区
 *	msgsz：msg字节数
 *	tolen：是定存放在to中的地址长度
 *	ppid：指定将随数据块传递的净荷协议标识符。
 *	flags：传递给SCTP栈，用于标识任何SCTP选项
 *	stream：调用者在stream参数中指定一个sctp流号。
 *	timetolive：以毫秒问单位指定消息的生命期，其中0表示无线生命期
 *	context：指定可能有的用户上下文
 *
 *	例子：
 *		ret = sctp_sendmsg(sockfd, data, datasz, &dest, sizeof(dest),
 *							24, MSG_PR_SCTP_TTL, 1 ,1000, 52);
 */
