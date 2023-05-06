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
 * readline.c 的改进版本
 *
 */

static int	read_cnt;
static char	*read_ptr;
static char read_buf[MAXLINE];

static ssize_t 
my_read(int fd, char *ptr)
{
	if(read_cnt <= 0)
	{
again:
		if((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0)
		{
			if(errno == EINTR)
			{
				goto again;
			}
			return -1;
		}
		else if(read_cnt == 0)
		{
			return 0;
		}
		read_ptr = read_buf;
	}	
	read_cnt --;
	*ptr = *read_ptr++;
	return 1;
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;

	for(n=1; n<maxlen; n++)
	{
		if((rc = my_read(fd, &c)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
			{
				break;
			}	
			else if(rc == 0)
			{
				*ptr = 0;
				return n-1;
			}
			else
			{
				return -1;
			}
		}
	}
	*ptr = 0;
	return n;
}

ssize_t 
readlinebuf(void **vptrptr)
{
	if(read_cnt)
	{
		*vptrptr = read_ptr;
	}
	return read_cnt;
}
