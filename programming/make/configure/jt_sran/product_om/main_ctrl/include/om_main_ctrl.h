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
/**********************************************
** Copyright (c) .
** All rights reserved.
**
** File name:om_main_ctrl.h
** Description:header file of om_main_ctrl.
**
** Current Version: 1.0
** Author: wangyuanyuan
** Date: 201809
***********************************************/

#ifndef OM_MAIN_CTRL_H
# define OM_MAIN_CTRL_H

/* Dependencies ------------------------------------------------------------- */
#include "sbs_type.h"
#include "om_common_h.h"

extern INT32 handle_module_phy_msg(void*, UINT32);

extern INT32 handle_module_bord_mgmt_msg(void*, UINT32);

extern INT32 parse_om_config_xml();

extern INT32 update_om_by_ftp();

extern INT32 cfig_mgmt_cfig_recover();

extern INT32 rrus_mgmt_module_init();

extern INT32 timer_module_init();

extern INT32 cfig_mgmt_module_init();

extern INT32 inet_mgmt_module_init();

extern INT32 mnt_mgmt_init();

extern INT32 start_phy();




#pragma pack(1)
/*
 * Api of main_ctrl with bord_mgmt
 */
#define CODE_BORD_MGMT_MAIN_CTRL_FPGA_INIT_RSLT_IND 	1
/*
FPGA启动结果上报
参数	类型	含义
FpgaInitRslt	UINT32	FPGA启动结果上报
0：成功
非0：具体的错误类型。
*/
typedef struct
{
	CommMsgHead		head;
	UINT32			fpga_init_rslt;
}BORD_MGMT_MAIN_CTRL_FPGA_INIT_RSLT_IND;

extern INT32 handle_msg_fpga_init_rslt_ind(void*, UINT32);

/*
 * Api of main_ctrl with rrus_mgmt
 */
#define CODE_MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ	1
/*
CPRI初始化请求
*/
typedef struct
{
	CommMsgHead		head;
	//UINT8			DesMac[6];		//CPU侧接收帧信息MAC地址
}MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ;


/*
 * Api of main_ctrl with phy
 */
#define CODE_PHY_MAIN_CTRL_PHY_LOAD_RSP		1
/*
 基带实体加载响应	 
 
参数			类型		含义
InitRslt	INT32	基带实体加载结果
					0：成功
					非0：失败, 不同的数据代表具体的错误类型。由PHY给出定义
PhyPid		UINT32	基带程序进程ID
PortNum		UINT8	DPDK绑定端口的个数，取值为1-4，指示以下几个MAC地址有效
MacAddr1[6]	UINT8	DPDK绑定端口1的MAC地址
MacAddr2[6]	UINT8	DPDK绑定端口2的MAC地址
MacAddr3[6]	UINT8	DPDK绑定端口3的MAC地址
MacAddr4[6]	UINT8	DPDK绑定端口4的MAC地址
*/
typedef struct 
{
	CommMsgHead		head;
	UINT32			cell_virt_id;
	INT32			init_rslt;
	UINT32			phy_pid;
	UINT8			port_num;
	UINT8			mac_addr1[6];
	UINT8			mac_addr2[6];
	UINT8			mac_addr3[6];
	UINT8			mac_addr4[6];
}PHY_MAIN_CTRL_PHY_LOAD_RSP;

extern INT32 handle_msg_phy_load_rsp(void*, UINT32);

#pragma pack()

#endif
