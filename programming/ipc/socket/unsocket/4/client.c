/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:22 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:08 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include "common.h"

void main()
{
    int connect_fd;
    int ret = 0, len;

    connect_fd  = unsocket_client(UNIX_DOMAIN);
    int val = 1;
	setsockopt(connect_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
    printf("fd: %d\n", connect_fd);
	while(1)
	{
		scanf("%s", buf);
		//buf[strlen(buf)] = '\r';
		//buf[strlen(buf)] = '\n';
		int i;
		for(i=0;i<5;i++)
		{
			ret = write(connect_fd, buf, strlen(buf)); 
			//ret = write(connect_fd, buf, MSG_LENGTH);//知道接收长度
			//ret = send(connect_fd, buf, strlen(buf), 0);
			printf("send bytes = %d: %s\n", ret, buf);
			//sleep(1);
			//flush(connect_fd);

		}
	}

    len = read(connect_fd, buf, MSG_LENGTH);
    
    printf("%d:%s\n", len, buf);
    
    close(connect_fd);

}
