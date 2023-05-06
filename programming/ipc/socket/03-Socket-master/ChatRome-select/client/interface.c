/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:11 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:02 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:43 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:15 CST. */
/*******************************************************************************
* 客户端界面设计
* 2015-12-15 yrr实现
*
********************************************************************************/

#include "config.h"

/***************************************************
函数名:mainInterface
功能:登录界面
传入参数:无
返回值:无
***************************************************/
int mainInterface()
{
   
		printf("-------------------------------------\n");
		printf("    欢迎进入小Q聊天室～	      \n");
		printf("	   1.注册                 \n");
		printf("	   2.登陆			      \n");
		printf("	   3.帮助			      \n");
		printf("	   4.退出			      \n");
		printf("-------------------------------------\n\n\n");
}

/***************************************************
函数名:helpInterface
功能:主界面的帮助选项
传入参数:无
返回值:无
***************************************************/
int helpInterface()
{
   
		printf("-------------------------------------\n");
		printf("         欢迎进入小帮助～	      \n");
		printf("	           ^_^                 \n");
		printf("        请在主界面选择操作～     \n");
		printf("	           ^_^			      \n");
		printf("-------------------------------------\n\n\n");
}

/***************************************************
函数名:helpInterface
功能:主界面的帮助选项
传入参数:无
返回值:无
***************************************************/
void chatInterface(char userName[])
{
		printf("------------------------------------------\n");
		printf("你好，%s                       \n", userName);
		printf("	     1. 查看在线用户                 \n");
		printf("	     2. 私聊                         \n");
		printf("	     3. 群聊                         \n");
		printf("	     4. 查看聊天记录                 \n");
		printf("	     5. 退出                         \n");
		printf("请选择操作～                    \n");
		printf("------------------------------------------\n\n\n");
}

