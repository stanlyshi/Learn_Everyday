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
 *	SCTP套接字选项
 *
 *	PS：SCTP流控制传输协议
 */

SCTP_ADAPTION_LAYER
/*在关联初始化期间，任何一个断电都可能指定一个适配层指示。
 * 这个指示是一个32位无符号数，可有两端的应用进程用来协调任何本地应用适配层。
 * 本选项允许调用者获取或设置将由本端提供给对端的适配层指示。*/

SCTP_ASSOCINFO
/*本套接字选项可用于一下三个目的：
 * 1.获取关于某个现有关联的信息；
 * 2.改变某个已有关联的参数；
 * 3.为未来的关联设置默认信息。
 *
 * 在获取关于某个现有关联的信息时，应该使用sctp_opt_info函数而不是getsockopt函数。
 * 作为本选项的输入的是sctp_assocparams结构体：
 * 		struct sctp_assocparams{
 *			u_int16_t	sasoc_asocmaxrxt;
 *			u_int16_t	sasoc_number_peer_destinations;
 *			u_int32_t	sasoc_peer_rwnd;
 *			u_int32_t	sasoc_local_rwnd;
 *			u_int32_t	sasoc_cookie_life;
 * 		};
 */



SCTP_AUTOCLOSE
/*本选项允许我们获取或设置一个SCTP端点的自动关闭时间。
 * 自动关闭时间是一个SCTP关联在空闲时保持打开的秒数。SCTP协议栈把空闲定义
 * 为一个关联的连个端点都没有在发送或接收用户数据的状态。*/

SCTP_DEFAULT_SEND_PARAM
/*本选项接收sctp_sndrcvinfo结构体作为输入。结构体定义为：
 *
 * struct sctp_sndrcvinfo{
 *		u_int16_t	sinfo_stream;
 *		u_int16_t	sinfo_ssn;
 *		u_int16_t	sinfo_flags;
 *		u_int32_t	sinfo_ppid;
 *		u_int32_t	sinfo_context;
 *		u_int32_t	sinfo_timetolive;
 *		u_int32_t	sinfo_tsn;
 *		u_int32_t	sinfo_cumtsn;
 *		sctp_assoc_t	sinfo_assoc_id;
 * };
 */

SCTP_DISABLE_FRAGMENTS
/*SCTP通常把太大而不适合置于单个SCTP分组中的用户消息分割成多个DATA块。
 * 开启本选项将在发送端禁止这种行为。被禁止后，SCTP将为此向用户返送EMSGSIZE
 * 错误，并且不发送用户信息。
 * SCTP的默认行为与本选项被禁止等效。*/

SCTP_EVENTS
/*本套接字选项允许调用者获取、开启或禁止各种SCTP通知。*/

SCTP_GET_PEER_ADDR_INFO
/*本选项仅用于获取某个给定对端地址的相关信息*/

SCTP_I_WANT_MAPPED_V4_ADDR
/*用于为AF_INET6类型的套接字开启或禁止IPv4映射地址，其默认状态为开启。*/

SCTP_INITMSG
/*本套接字选项用于获取或设置某个SCTP套接字在发送INIT消息时所用的默认初始参数。*/

/** 注意了！！！ ***
 *
 * 这个地方太多了，用vim打字太累，详情请见
 * 《UNIX网络编程：卷1 socket编程》 P178继续
 *
 *
 */
























