/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:15 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:04 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 11日 星期一 08:30:18 CST. */
//1. The socket function creates a raw socket when the second argument is SOCK_RAW. The third argument (the protocol) is normally nonzero. For example, to create an IPv4 raw socket we would write
//2.
//3.
//4.
//5. int sockfd;
//6.
//7. sockfd = socket(AF_INET, SOCK_RAW, protocol);
//8.

sockfd = socket(AF_INET, SOCK_RAW, protocol);

//where protocol is one of the constants, IPPROTO_xxx, defined by including the <netinet/in.h> header, such as IPPROTO_ICMP.
//
//Only the superuser can create a raw socket. This prevents normal users from writing their own IP datagrams to the network.
