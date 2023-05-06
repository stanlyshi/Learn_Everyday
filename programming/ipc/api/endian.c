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
 *	endian.c
 * big endian; little endian
 */
#include <stdio.h>
#include <stdlib.h>

int main()
{
	union{
		short s;
		char c[sizeof(short)];
	}un;

	un.s = 0x0102;

	if(sizeof(short) == 2)
	{
		if(un.c[0] == 1 && un.c[1] == 2)
		{
			printf("big\n");
		}
		else if(un.c[0] == 2 && un.c[1] == 1)
		{	
			printf("little\n");
		}	
		else
		{
			printf("unknown\n");
		}
	}
	printf("sizeof(short): %d\n", sizeof(short));
}
