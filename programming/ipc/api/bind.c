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
 * 绑定端口函数
 * int bind(int sockfd, struct sockaddr* my_addr, int addrlen);
 * 	sockfd: 已经创建的socket编号
 * 	sockaddr: 是一个指向sockaddr结构体类型的指针
 * 		struct sockaddr
 * 		{
 * 			unsigned short int sa_family;
 * 			char sa_data[14];
 * 		};
 * 		sa_family: 为调用socket()时的domain参数，AF_xxxx值
 * 		sa_data: 最多使用14各字符长度，含有IP地址和端口信息
 * 	如果建立socket时使用的是AF_INET参数，则sockaddr结构体的定义方法如下：
 * 		struct sockaddr_in
 * 		{
 *			unsigned short int sin_family;
 *			uint16_t sin_port;
 *			struct in_addr sin_addr;
 *			unsigned char sin_zero[8];
 * 		};
 * 		其中:
 * 		struct in_addr
 * 		{
 *			uint32_t s_addr;
 * 		};
 * 		在这些结构体中，成员变量的作用于含义如下所示：
 * 		sin_family: 调用Socket()时的domain参数
 * 		sin_port：使用的端口号
 * 		sin_addr.s_addr: IP地址
 * 		sin_zero: 未使用的字段，填充为0
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*定义一个表示端口的常量*/
#define PORT 5678

int main()
{
	int sockfd, nersockfd, fd;
	struct sockaddr_in addr;

	int addr_len = sizeof(struct sockaddr_in);
	fd_set myreadfds;
	
	char msgbuffer[256];

	/*建立一个socket*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket: ");
		exit(1);
	}
	else
	{
		printf("socket created\n");
		printf("socket id: %d \n", sockfd);
	}	
	/*清空表示地址的结构体变量*/
	bzero(&addr, sizeof(addr));
	/*设置addr的成员*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*bind函数绑定端口*/
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else 
	{
		printf("connected\n");
		printf("local port: %d\n", PORT);
	}

	exit(0);
}
