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
 *	readline():
 *	
 *		从一个描述符读文本行，一次1个字节
 */

ssize_t
readline(int fd, void *vptr, size_t maxlen)
/*从一个描述符读文本行，一次读一个字节*/
{
	ssize_t	n, rc;
	char 	c, *ptr;

	ptr = vptr;

	for(n=1; n<maxlen; n++)
	{
again:
		if((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
			{
				break;/*new line*/
			}
			else if(rc == 0)
			{		
				*ptr = 0;
				return n-1;
			}	
			else
			{
				if(errno == EINTR)
				{
					goto again;
				}
				return -1;
			}
		}
	}
	*ptr = 0;
	return n;
}
