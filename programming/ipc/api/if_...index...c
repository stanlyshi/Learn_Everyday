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
 *	接口名字和索引函数
 *
 *		这几个貌似也是《UNIX网络编程》书里面作者遍的函数好像
 */

#include <net/if.h>

unsigned int if_nametoindex(const char *ifname);
	/**
	 * 返回：
	 * 		成功：正的接口索引
	 * 		错误：0
	 */
char *if_indextoname(unsigned int ifindex,  char *ifname);
	/**
	 *	返回：
	 *		成功：指向接口名字的指针，
	 *		错误：NULL
	 */
struct if_nameindex *if_nameindex(void);
	/**
	 *	返回：
	 *		成功：空指针
	 *		错误：NULL
	 */
void if_freenameindex(struct if_nameindex *ptr);
	

/**
 *	if_nametoindex：返回名字为ifname的借口的索引；
 *	if_indextoname：返回索引为ifindex的接口的名字；
 *					ifname参数指向一个大小为IFNAMSIZ的缓冲区
 *	if_nameindex返回一个指向if_nameindex结构数组的指针，
 *			结构体定义如下：
 *				struct if_nameindex {
 *					unsigned int if_index; //1,2,3,4....
 *					char *if_name;  //null terminated name:"le0",...
 *				}
 *
 *
 */
