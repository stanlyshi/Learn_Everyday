/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:34 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:46 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_main_ctrl_api.h
 *Description: the all api of rrus_mgmt with main_ctrl.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.06.05
 *
 ************************************************************************/



//RRU管理模块与MAIN_CTRL模块交互接口
#ifndef MAIN_RRUM_H
#define MAIN_RRUM_H

#include "sbs_type.h"


/* Message Code of RRUS_MGMT with MAIN_CTRL */
#define CODE_IMAR_CPRI_INIT   1


//CPRI初始化请求	FPGA server启动完成后，由MAIN_CTRL触发配置CPRI，成功后等待处理RRU的初始化
//extern INT32 set_cpri_init_trigger(void *msg);
typedef struct 
{
	UINT8			DesMac[6];		//CPU侧接收帧信息MAC地址
}IMAR_CPRI_INIT_TRIGGER;

extern INT32 handle_imar_cpri_init(void*, UINT32);
extern INT32 handle_msg_cpri_init_cfg(UINT16 cpri_index, void* msg);


#endif
