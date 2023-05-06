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
* 客户端用户登陆处理实现文件
* 2015-12-14 yrr实现
*
********************************************************************************/

#include "config.h"

/*********************************************
函数名：loginUser
功能：用户登陆函数实现
参数：套接字描述符
返回值：成功登陆返回SUCCESS 否则返回异常类型
**********************************************/
int loginUser(int sockfd)
{
	int ret;
	/*声明用户登陆信息*/
	User user;
	char buf[MAX_LINE];
	Message message;
	/*获取用户输入*/
	printf("请输入用户名：\n");
	memset(user.userName , 0 , sizeof(user.userName));
	scanf("%s" , user.userName);
	printf("user.UserName = %s\n" , user.userName);
	
	printf("请输入用户密码：\n");
	memset(user.password , 0 , sizeof(user.password));
	scanf("%s" , user.password);
	printf("user.password = %s\n" , user.password);

	/*发送用户登陆信息到服务器*/
	memset(buf , 0 , MAX_LINE);
	memcpy(buf , &user , sizeof(user));
	send(sockfd , buf , sizeof(buf) , 0);

	/*接收登陆结果*/
	memset(buf , 0 , MAX_LINE);
	recv(sockfd , buf , sizeof(buf) , 0);
	memset(&message , 0 , sizeof(message));
	memcpy(&message , buf , sizeof(buf));
	
	printf("%s\n",message.content);
	
	/*如果登陆成功，则进入聊天界面*/
	if(SUCCESS == message.msgRet)
	{
		enterChat(&user , sockfd);
	}//if
	return message.msgRet;
}


