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
 *	轮询（polling）
 */

#include <poll.h>

int poll(struct pollfd *fdarray,
		unsigned long nfds,
		int timeout);
/**
 *	poll函数起源于SVR3，最初局限于流设备，SVR3取消了这种限制，允许poll工作在任何
 *	描述符上。poll提供的功能与select类似，不过在处理流设备时，它能够提供额外的信息。
 *
 *	第一个参数值指向一个机构数组第一个元素的指针。每个数组元素都是一个pollfd结构，
 *	用于制定测试某个给定描述符fd的条件。
 *		
 *		struct pollfd{
 *			int fd;			//用于检查的描述符
 *			short events;	//fd上的事件
 *			short revents;	//fd上发生的事件
 *		}
 */

/** 百度百科 **
 *
 *	poll是linux中的字符设备驱动中的一个函数，linux2.5.44版本后，poll被epoll取代。
 *	和select实现的功能差不多，poll的作用是把当前的文件指针挂到等待队列。
 *
 *	作用：把当前的文件指针挂到等待队列；
 *	功能：和select差不多；
 *
 */
/**
  POLLIN        普通或优先级带数据可读
  POLLRDNORM    普通数据可读
  POLLRDBAND    优先级带数据可读
  POLLPRI       高优先级数据可读
  
  POLLOUT       普通数据可写
  POLLWRNORM    普通数据可写
  POLLWRBAND    优先级带数据可写
  
  POLLERR       发生错误
  POLLHUP       发生挂起
  POLLNVAL      描述符不是一个可打开的文件

 */