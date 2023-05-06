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
 *	gai_strerror函数
 *
 *		由getaddrinfo函数返回的非0错误值的名字和含义。
 *		gai_strerror以这些值为他的唯一参数，返回一个只想对应的
 *	出错信息串的指针。
 */

#include <netdb.h>

const char *gai_strerror(int error);

/** getaddrinfo函数的返回的非0错误常量
 *	
 *	EAI_AGAIN:		名字解析中临时失败
 *	EAI_BADFLAGS:	ai_family的值无效
 *	EAI_FAIL:		名字解析中不可恢复的失败；
 *					（这个依次往下移动一行）不支持ai_family
 *	EAI_FAMILY:		内存分配失败
 *	EAI_MEMORY:		hostname或service未提供，或者不可知
 *	EAI_NONAME:		用户参数缓冲区溢出
 *	EAI_OVERFLOW:
 *	EAI_SERVICE:	不支持ai_socktype类型的service
 *	EAI_SOCKTYPE:	不支持ai_socktype
 *	EAI_SYSTEM:		在errno变量中由系统错误返回
 *
 */
