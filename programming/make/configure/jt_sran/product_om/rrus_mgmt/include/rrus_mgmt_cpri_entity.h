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
 *File name: rrus_mgmt_cpri_entity.h
 *Description: the infomation of rru.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.06.01
 *
 ************************************************************************/


#ifndef SBS_INTERFACE_RRUAGENT_CPRIENTITY_H
#define SBS_INTERFACE_RRUAGENT_CPRIENTITY_H

#include "sbs_type.h"

typedef struct
{
	UINT16		cpri_index;		//CPRI ID(高8位是FpgaId，低8是fiberNo)
	UINT8		cpri_mac[6];	//Mac address of cpri
	UINT32		CpriSpeed;		//2.5G，5G，10G，默认5G，可配
	UINT32		RRU_ID_Start;	//RRU ID 起始值
	UINT32		AxcSpeed[32];	//1.4M，3M，5M，10M，15M，20M。默认20M 从0开始。
	UINT32		State;			//低字节表示接口状态：次低字节表示链路状态：
	UINT32		T14;			//为xx个cycle。都统一成时间单位然后再折算成时钟周期数。

    UINT32      total_high;		//ser result
    UINT32      total_low;
    UINT32      error_high;
    UINT32      error_low;
}CpriEntity;



#define MAX_CPRI_NUM 16
extern CpriEntity* g_cpri_entity_table[MAX_CPRI_NUM];





#endif
