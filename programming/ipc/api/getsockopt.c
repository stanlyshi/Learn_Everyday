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
 * int getsockopt(	int s, 		//已经建立的socket的编号
 * 					int level,	//需要设置的网络层
 * 					int optname,//需要获取的选项
 * 					void *optval,	//取得的某个参数的返回值指针
 * 					socklen_t *optlen	//optval的内存长度
 * 				);
 * 	函数执行成功返回1，否则返回-1
 * 	需要包含头文件
 * 	sys/socket.h
 * 	sys/types.h
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int s;	
	int val, len;
	len = sizeof(int);

	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("connect");	
		exit(1);
	}
	else
	{
		printf("a socket was create.\n");
		printf("socket number is: %d\n", s);
	}

	getsockopt(s, SOL_SOCKET, SO_TYPE, &val, &len);
	perror("socket:");
	printf("optval = %d\n", val);
	printf("optlen = %d\n", len);

	/*用错误的方法取得一个信息*/
	getsockopt(100, SOL_SOCKET, SO_TYPE, &val, &len);
	perror("socket:");
	printf("optval = %d\n", val);
	printf("optlen = %d\n", len);

	
	

}
