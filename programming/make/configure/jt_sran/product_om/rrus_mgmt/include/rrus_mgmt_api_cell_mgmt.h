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
 *File name: rrus_mgmt_cell_mgmt_api.h
 *Description: the api of rrus_mgmt with cell_mgmt.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.06.05
 *
 ************************************************************************/


//RRU管理模块与CELL_MGMT模块交互接口
#ifndef CELL_RRUM_H
#define CELL_RRUM_H

#include "sbs_type.h"
#include "om_common_h.h"

/* Message Code of RRUS_MGMT with CELL_MGMT */
#define CODE_ICER_CPRI_AXC_CFG          1
#define CODE_ICER_CPRI_AXC_CFG_RET      2
#define CODE_ICER_RRU_REG_REQ           3


//CPRI-AxC配置请求  建立小区时，由CELL_MGMT模块调用，根据小区RRU对应的信息，触发对应小区AxC通路、时延的配置�?
//删除小区时，由CELL_MGMT模块调用，设置对应小区AxC通路无效�?
//extern INT32 set_cpri_axc(void *msg, int);

extern INT32 handle_icer_cpri_axc_cfg(void*, int);

typedef struct 
{ 
	CommMsgHead		head;
    UINT32          rru_id;
    UINT32          axc_speed;
    UINT32			cfg_ind;
}ICER_CPRI_AXC_CFG;


//CPRI-AxC配置响应  配置响应，回复成果，或错误原�?
//extern INT32 trap_cpri_axc_result(void *msg, int);
typedef struct 
{
	CommMsgHead		head;
	UINT32          rru_id;
	UINT32          axc_ind;
    UINT32          cfg_rslt;
    UINT32          err_type;
}ICER_CPRI_AXC_CFG_RESULT;


//RRU注册请求   当RRU_MGMT模块发生RRU的增删，RRU管理模块调用注册接口，向VCM提供给RRU最新状�?


typedef struct 
{
	CommMsgHead		head;
	UINT8			rru_sn[16];
    UINT32          rru_id;
    UINT32          rru_reg;
}ICER_RRU_REG;


#endif
