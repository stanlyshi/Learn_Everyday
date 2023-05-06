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
//IPV6_CHECKSUM Socket Option
//For an ICMPv6 raw socket, the kernel always calculates and stores the checksum in the ICMPv6 header. This differs from an ICMPv4 raw socket, where the application must do this itself (compare Figures 28.14(See 9.17.5) and 28.16(See 9.17.5)). While ICMPv4 and ICMPv6 both require the sender to calculate the checksum, ICMPv6 includes a pseudoheader in its checksum (we will discuss the concept of a pseudoheader when we calculate the UDP checksum in Figure 29.14(See 9.18.7)). One of the fields in this pseudoheader is the source IPv6 address, and normally the application lets the kernel choose this value. To prevent the application from having to try to choose this address just to calculate the checksum, it is easier to let the kernel calculate the checksum.
//For other raw IPv6 sockets (i.e., those created with a third argument to socket other than IPPROTO_ICMPV6), a socket option tells the kernel whether to calculate and store a checksum in outgoing packets and verify the checksum in received packets. By default, this option is disabled, and it is enabled by setting the option value to a nonnegative value, as in
//
int offset = 2;
if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_CHECKSUM,
			&offset, sizeof(offset)) < 0)
//	error
//
//This not only enables checksums on this socket, it also tells the kernel the byte offset of the 16-bit checksum: 2 bytes from the start of the application data in this example. To disable the option, it must be set to -1. When enabled, the kernel will calculate and store the checksum for outgoing packets sent on the socket and also verify the checksums for packets received on the socket.
