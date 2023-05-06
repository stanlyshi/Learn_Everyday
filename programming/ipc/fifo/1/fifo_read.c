/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:09 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:01 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:42 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
/**
 *	fifo 
 * 2018.10 RongTao rt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "common.h"

int main()
{
	FILE *fp;
	int i = 0;
	char buff[20];
	printf("read\n");
	
	//mkfifo("/home/RongTao/ipc/fifo/tmp", 0777);
	
	fp = fopen("tmp", "rb");
	
	for(i=0; i<10; i++)
	{
		memset(buff, 0, strlen(buff));
		fread(buff, 1, strlen(buff), fp);
		printf("read: %s\n", buff);
	}
	
	//sleep(15);
	fclose(fp);
		
	return 0;
}
