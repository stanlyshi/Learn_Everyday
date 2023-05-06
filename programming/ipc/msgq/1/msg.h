/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:10 CST. */
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
/*用于创建一个唯一的key*/
#define MSG_FILE "/home/RongTao/ipc"


typedef struct 
{
	int a;
	int b;
	int c;
	
}ptr_msg;

/*消息结构*/
struct msg_form
{
	long mtype;
	char mtext[256];
	float f;
	void *ptr;
};

int msg_print(const char *ptr, const struct msg_form msg)
{
	printf("--------\n");
	printf("%s: %d", ptr, sizeof(msg));
	printf("%s: %s\n", ptr, msg.mtext);
	printf("%s: %f\n", ptr, msg.f);
	
	return 0;
}