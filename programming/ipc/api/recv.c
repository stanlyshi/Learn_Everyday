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
 * 数据接收函数recv
 * int recv(int s, void *buf, int len, unsigned int flags);
 * 	s: 已经建立的socket
 * 	buf：一个指针，指向一个数组
 * 	len：数组的长度
 * 	flags：一般设置为0，其他可能的赋值
 * 		MSG_OOB：接收以out-of-band送出的数据
 * 		MSG_PEEK：返回来的数据并不会在系统内删除，如果在调用recv时，
 * 					会返回相同的数据内容
 * 		MSG_WAITALL：强迫接收到的len大小的数据后才返回，除非有错误或信号产生
 * 		MSG_NOSIGNAL：信号中断
 * recv函数如果接收到数据，会把这些数据保存在buf中，然后返回接收到字符的个数
 * 如果发生错误会返回-1.
 * 函数可能发生的错误，可以被errno来捕获：
 * 		EBADF：参数s不是一个合法的socket
 * 		EFAULT：参数中的指针指向了无法读取的内存空间
 * 		ENOTSOCK：参数s是文件描述词，而不是一个socket
 * 		EINTR：被信号中断
 * 		EAGAIN：此动作会阻断今晨个，餐参数s的socket不可阻断
 * 		ENOBUFS：系统的缓冲内存不足
 * 		EINVAL：参数不正确
 * 头文件
 * 	#include <sys/types.h>
 * 	#include <sys/socket.h>
 */
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>

#define PORT 21
#define REMOTE_IP "127.0.0.1"

int main()
{
	int s;
	struct sockaddr_in addr;
	char mybuffer[256];

	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("socket created\n");
		printf("socket id: %d\n", s);
	}

	bzero((struct sockaddr*)&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_IP);

	if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("connected ok \n");
		printf("remote ip: %s\n", REMOTE_IP);
		printf("remote prot: %d\n", PORT);
	}
	
	/*接收信息*/
	recv(s, mybuffer, sizeof(mybuffer), 0);
	printf("mybuffer: %s\n", mybuffer);
}
