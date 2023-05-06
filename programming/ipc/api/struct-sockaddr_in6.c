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
 *	netinet/in.h
 *
 */
in_addr_t		//IPv4地址，一般为uint32_t
in_port_t		//TCP或UDP端口，一般为uint16_t

/*IPv6套接字地址结构*/
struct in6_addr
{
	uint8_t 	s6_addr[16];	/*128-bit IPv6地址*/
};

#define SIN6_LEN

struct sockaddr_in6
{
	uint8_t			sin6_len;		//结构体的长度
	sa_family_t		sin6_family;	//AF_INET6
	in_port_t		sin6_port;		//运输层端口号
	
	uint32_t		sin6_flowinfo;	//流信息
	struct in6_addr	sin6_addr;		//IPv6 地址
	
	uint32_t		sin6_scope_id;	//审视接口
}
