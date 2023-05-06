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
 *	典型的兵法服务器程序轮廓
 */

int main()
{
	pid_t	pid;
	int		listenfd, connfd;

	listenfd	=	Socket(...);

	/*此处添加port和addr等信息赋值*/

	Bind(listenfd, ...);
	Listen(listenfd, LISTENQ);

	for(;;)
	{
		connfd = Accept(listenfd, ...);

		if((pid = fork()) == 0)
		{
			close(listenfd);
			doit(connfd);
			close(connfd);
			exit(0);
		}
		close(connfd);
	}
}
