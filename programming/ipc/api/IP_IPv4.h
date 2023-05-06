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
 *	IPv4套接字选项	IP_HDRINCL
 *
 *		如果本现象是给一个原始IP套接字设置的，那么我们必须为所有的该原始套接字
 *		上发送的数据包构造自己的IP首部。一般情况下，在原始套接字上发送的数据包
 *		其IP首部是由内核构造的，不过有些应用程序需要构造自己的IP首部，以取代IP
 *		置于该首部中的某些字段。
 *
 *		当本现象开启时，我们构造完整的IP首部。
 */
/**
 *	IPv4套接字选项	IP_OPTIONS
 *
 *		允许我们在IPv4首部中设置IP选项
 */
/**
 *	IPv4套接字选项	IP_RECVDSTADDR
 *
 *		该选项导致所受到的UDP数据包的目的IP地址由recvmsg函数作为辅助数据返回。
 */
/**
 *	IPv4套接字选项	IP_TOS
 *
 *		本套接字选项允许我们为TCP、UDP和SCTP套接字设置IP首部中的服务类型字段。
 */
/**
 *	IPv4套接字选项	IP_TTL
 *
 *		使用本选项设置或获取系统用在从某个给定套接字发送的单播分组上的默认TTL值。
 */
/**
 *	IPv4套接字选项	IP_RECVIF
 *
 *		该选项导致所收到的UDP数据包的接收接口索引由recvmsg函数作为辅助函数返回。
 */
