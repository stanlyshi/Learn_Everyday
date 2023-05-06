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
 *	popen：创建一个管道并启动另外一个进程，该进程要么从该管道独处标准输入，
 *			要么网该管道写入标注输出。
 */	

#include <stdio.h>
#include <string.h>

FILE *popen(const char *command, const char *type);
	//返回：成功：文件指针；失败：NULL

int pclose(FILE *stream);
	//返回：成功：shell的终止状态；失败：-1

int main()
{
	size_t n;
	char buff[1000], command[1000];

	fgets(buff, 1000, stdin);

	n = strlen(buff);

	if(buff[n-1] == '\n')
	{
		n--;
	}

	snprintf(command, sizeof(command), "cat %s", buff);

	FILE *fp = popen(command, "r");

	while(fgets(buff, 1000, fp) != NULL)
	{
		fputs(buff, stdout);
	}
	pclose(fp);

	exit(0);
}
