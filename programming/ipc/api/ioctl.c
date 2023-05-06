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
 *	ioctl函数		P365
 *
 *	其中第三个参数总是一个指针，但指针的类型取决于request参数。
 */

#include <unistd.h>

//在处理“流”时，使用下面的头文件
#include <stropts.h>

int ioctl(int fd, int request, .../* void *arg */);

/** 百度百科
 *
 * ioctl是设备驱动程序中对设备的IO通道进行管理的函数。
 * 所谓IO通道管理，就是对设备的一些特性进行控制，例如串口的专属波特率、
 * 马达的转速等，他的参数如下：
 *
 * 		int ioctl(int fd, int cmd, ...);
 *
 * 			fd：用于牲畜打开设备时使用个open函数返回的文件标识符，
 * 			cmd：用户程序对设备的控制指令，
 * 			...：补充参数，一般最多一个，有或没有看cmd。
 *
 * 	中文名：ioctl
 * 	属性：控制IO设备
 * 	功能：进行管理的函数
 * 	特点：特性进行控制。
 */

/**	小结：**
 *
 * 	用于网络编程的ioctl命令可划分为6类：
 *
 * 		1.套接字操作：是否位于带外标记等；
 * 		2.文件操作：设置或清除非阻塞标志等；
 * 		3.接口操作：返回接口列表，获取广播地址等；
 * 		4.ARP表操作：创建、修改、获取、删除
 * 		路由表操作：增加或删除
 * 		流系统：待议
 */
