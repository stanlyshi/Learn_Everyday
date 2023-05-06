/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:45 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * cm_api.h
 *
 *  Created on: Aug 20, 2018
 *      Author: root
 */

#ifndef CM_API_H_
#define CM_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "om_common_h.h"

#define ULONG unsigned long
#define USHORT unsigned short
#define UCHAR  unsigned char




typedef struct CONFIG_VCMAddState_s
{
	CommMsgHead stSysMsg;
	ULONG ulIndex;
	UCHAR ucRes;
	ULONG ulError_type;

}CONFIG_VCMAddState_t;

typedef struct CONFIG_VCMDelState_s
{
	CommMsgHead stSysMsg;
	ULONG ulIndex;
	UCHAR ucRes;
	ULONG ulError_type;

}CONFIG_VCMDelState_t;

typedef struct CONFIG_NESetup_s
{
	CommMsgHead         stSysMsg;
    UCHAR   ucNESetupStatus;
}CONFIG_NESetup_t;

typedef struct CONFIG_TimeSyncState_s
{
	CommMsgHead         stSysMsg;
    UCHAR   ucTimeSync;
	UCHAR   ucErrCode;
	UCHAR   ucPosMode;
	UCHAR   ucClosource;

}CONFIG_TimeSyncState_t;


#define CM_QUE_LEN   200
#define MODULE_CM_ID 0x1000
#define CM_TASK_NAME tCM
#define CM_SUBAGENT_TASK_NAME tCMSubAgent
#define TIMER_CM 1000

#define CONFIG_CONTRL_REPLY_NESETUP   1
#define CONFIG_CONTRL_REPLY_TIMESYNC  2
#define CONFIG_VCM_REPLY_ADDCELL  3
#define CONFIG_VCM_REPLY_DELCELL  4

#define MODULE_VCM        1
#define MODULE_CTL        2

#define VCM_CELL_NOTIFY   1
#define CTL_STATUS_NOTIFY 2


#endif /* CM_API_H_ */
