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
 * 网际IPv4套接字地址结构
 *
 * #include <netinet/in.h>
 *
 */
struct in_addr
{
	in_addr_t s_addr; /*32bit IPv4地址, uint32_t*/
}
struct sockaddr_in
{
	uint8_t			sin_len;		/*这个结构体的长度*/
	sa_family_t		sin_family;		/*AF_INET, unsigned short int*/
	in_port_t		sin_port;		/*16-bit的TCP | UDP端口号, uint16_t*/
	struct in_addr	sin_addr;		/*32-bit IPv4地址*/
	char 			sin_zero[8];	/*unused*/
}
