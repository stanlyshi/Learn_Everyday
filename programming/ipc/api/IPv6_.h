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
 *	IPv6套接字选项	
 *
 */

IPV6_CHECKSUM
/*本选项指定用户数据中校验和所处位置的字节偏移。*/

IPV6_DONTFRAG
/*禁止为UDP套接字货源是套接字自动插入分片首部，外出分组中大小超过发送接口MTU
 * 的那些分组将被丢弃
 */

IPV6_NEXTHOP
/*将外出数据包的下一跳地址指定为一个套接字地址结构。这是一个特权操作。*/

IPV6_PATHMTU
/*本选项不能设置，只能获取。
 * 获取本选项时，返回值为有路径MTU发向功能确定的当前MTU。*/

IPV6_RECVDSTOPTS
/*开启本选项表明：任何接收到的IPv6目的地址项都将由recvmsg作为辅助数据返回。
 * 本选项默认为关闭。*/

IPV6_RECVHOPLIMIT
/*开启本选项表明：任何接收到的跳限字段都将由recvmsg作为辅助数据返回。
 * 本选项默认为关闭。*/

IPV6_RECVHOPORTS
/*开启本选项表明：任何接收到的IPv6步跳选项都将由recvmsg作为辅助数据返回
 * 本选项默认为关闭*/

IPV6_RECVPATHMTU
/*表明：某条路径的路径MTU在发生变化时将由recvmsg作为辅助数据返回*/

IPV6_RECVPKTINFO
/*表明：接收到的IPv6数据包的一下两条信息将由recvmsg作为辅助数据返回，
 * 目的IPv6地址和到达接口索引。*/

IPV6_RECVRTHDR
/*表明：接收到的IPv6路由首部将由recvmsg作为辅助数据返回。本选项默认为关闭。*/

IPV6_RECVTCLASS
/*表明：接收到的流通类别将由recvmsg作为辅助数据返回，本选项默认关闭*/

IPV6_UNICAST_HOPS
/*类似于IPv4的IP_TTL套接字选项。*/

IPV6_USE_MIN_MTU
/*本选项设置为1表明：路径MTU法相功能不必执行，为避免分片，
 * 					分组就使用IPv6的最小MTU发送；
 * 			0表明：	路径MTU发现功能对于所有目的地都得执行。
 * 			-1表明：路径MTU法相功能仅对单播目的地执行，
 * 					对于多播目的地就使用最小MTU。
 * 		本选项默认为-1.
 */

IPV6_V6ONLY
/*在一个AF_INET6套接字上开启本选项将闲置它只执行IPv6通信。本选项默认关闭状态。*/

IPV6_XXX
/*大多数用于修改协议首部的IPv6选项假设：
 * 就UDP套接字而言，信息由recvmsg和sendmsg作为辅助数据内核和应用进程之间传递；
 * 就TCP套接字而言，同样的信息改用getsockopt和setsockopt获取的设置。
 * 套接字选项和辅助数据的类型一致，并且访问套接字选项的缓冲区所含的信息和辅助数据中窜访的信息也一致。*/













