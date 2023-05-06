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
 *	通用套接字选项 SO_BROADCAST so_broadcast
 *
 *		本选项开启或禁止进程发送广播消息的能力。
 */

/**
 *	通用套接字选项	SO_DEBUG so_debug
 *
 *		保存详细跟踪信息-仅支持TCP协议
 */
/**
 *	通用套接字选项	SO_LINGER	so_linger
 *
 *		指定close函数对面向连接的协议（TCP、SCTP，不是UDP）如何操作。
 *		默认操作是close立即返回，但是如果有数据残留在套接字发送缓冲区中，
 *		系统将是这把这些数据发送给对端。
 */
/**
 *	通用套接字选项	SO_RCVBUF  SO_SNDBUF
 *
 *		每个套接字都有一个发送缓冲区和一个就收缓冲区。
 */
/**
 *	通用套接字选项	SO_OOBINLINE so_oobinline
 *
 *		本选项开启时，带外数据将被留在正常的输入队列中（即在线留存），这种情况下
 *		接收函数的MSG_OOB标志不能用来读外带数据。
 */
/**
 *	通用套接字选项	SO_RCVLOWAT		SO_SNDLOWAT
 *
 *		每个套接字还有一个接收低水位标记和一个发送低水位标记。
 */
/**
 *	通用套接字选项	SO_RCVTIMEO SO_SNDTIMEO
 *
 *		这两个选项允许我们给套接字的接收和发送设置一个超时值。
 *
 *			访问他们的getsockopt和setsockopt函数的参数是指向timeval结构的指针，
 *		与select所用参数相同。这可让我们用秒数和微秒数来规定超时。我们通过设置
 *		其值为0s和0us来禁止超时。默认情况下两个超时都是禁止的。
 */
/**
 *	通用套接字选项	SO_USELOOPBACK
 *
 *		用于路由域（AF_ROUTE）的套接字。
 */
/**
 *	通用套接字选项	SO_TYPE so_type
 *
 *		本选项返回套接字的类型，返回的整数值是一个诸如SOCK_STREAM或SOCK_DGRAM
 *		之类的值。本选项通常由启动时继承了该套接字的进程使用。
 */
/**
 *	通用套接字选项	SO_REUSEADDR SO_REUSEPORT
 *
 *		SO_REUSEADDR起到下面四个功能：
 *			
 *			1.允许启动一个监听服务器并捆绑器众所周知端口，即使以前建立的将该端口用作
 *				他们的本地端口的链接仍存在。
 *			2.允许在同一端口上启动同一服务器的多个实例，只要每个实例捆绑一个不同的IP
 *				地址即可。
 *			3.允许单个进程捆绑同一个端口到多个套接字上；
 *			4.允许完全重复的捆绑。
 *
 *
 */
/**
 *	通用套接字选项	SO_KEEPALIVE so_keepalive
 *
 *		被设置保持存活，如果两小时内该套接字的任何一方向上都没有
 *		数据交换，TCP就自动给对端发送一个保持存活探测分节。
 */
/**
 *	通用套截止选项	SO_ERROR so_error
 *
 *		套接字上发生错误，
 */
/**
 *	通用套接字选项	SO_DONTROUTE so_dontroute
 *
 *		规定外出的分组绕过底层协议的正常路由机制
 */
