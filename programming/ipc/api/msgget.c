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
 *	msgget函数用于创建一个新的消息队列或访问一个已存在的消息队列
 */

#include <sys/msg.h>

int msgget(key_t key, int oflag);
	//成功：非负标识符
	//错误：-1
/**
 *	key：可以使ftok返回值，也可以是IPC_PROVATE
 *	oflag：读写权限值的组合
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int c, oflag, mqid;

	oflag = SVMSG_MODE | IPC_CREAT;

	while((c = getopt(argc, argv, "e")) != -1)
	{
		switch(c)
		{
			case 'e':
				oflag |= IPC_EXCL;
				break;
		}
	}
	if(optind != argc - 1)
		exit(0);

	exit(1);
}
