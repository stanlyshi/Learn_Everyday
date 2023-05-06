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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <arpa/inet.h>
#include "sbs_err.h"
#include "sbs_type.h"
#include "sbs_sema.h"
#include "sbs_log.h"
#include "mnt_mgmt_api.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "msg_receive_fn.h"
#include "fpga_msg_receive_fn.h"
#include "om_common_h.h"
#include "inet_mgmt.h"


/*******************************************************************************************

 Description :handle function of FPGA fan threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_fan_thres_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of handle_fn_fpga_fan_thres_cfg_req() is invalid.\n");
		return FUNC_ERR;
	}
	FpgaServerFanThresConf *pMsg = (FpgaServerFanThresConf*)arg3;

	savefpgaserverhealthresinfo.old_fpga_server_fan_thres = savefpgaserverhealthresinfo.curr_fpga_server_fan_thres;
	savefpgaserverhealthresinfo.curr_fpga_server_fan_thres = pMsg->fpga_server_fan_thres;

	memset(&fanthrescfgreq,0,sizeof(fanthrescfgreq));
	fanthrescfgreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	fanthrescfgreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_FAN_THRES_CFG_REQ);
	fanthrescfgreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	fanthrescfgreq.msg_head.msg_len = htonl(sizeof(fanthrescfgreq));
	fanthrescfgreq.fpga_id = htonl(savefpgaserverid.fpga_server_id);
	fanthrescfgreq.thres_value = htons(pMsg->fpga_server_fan_thres);

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&msghead,sizeof(fanthrescfgreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure FPGA fan threshold to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}
	return 0;
}

/*******************************************************************************************

 Description :handle function of FPGA temperature threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_temp_thres_cfg_req(INT8 *arg3)
{
	UINT32 index;
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_temp_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	FpgaTempThresConf *pMsg = (FpgaTempThresConf*)arg3;
	index = pMsg->fpga_index;

	savefpgahealthresinfo[index-1].old_fpga_temp_thres = savefpgahealthresinfo[index-1].curr_fpga_temp_thres;
	savefpgahealthresinfo[index-1].curr_fpga_temp_thres = pMsg->fpga_temp_thres;

	memset(&tempthrescfgreq,0,sizeof(tempthrescfgreq));
	tempthrescfgreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	tempthrescfgreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_TEMP_THRES_CFG_REQ);
	tempthrescfgreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	tempthrescfgreq.msg_head.msg_len = htonl(sizeof(tempthrescfgreq));
	tempthrescfgreq.fpga_id = htonl(savefpgaid[index-1].fpga_id);
	tempthrescfgreq.thres_value = htonl(pMsg->fpga_temp_thres);

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&tempthrescfgreq,sizeof(tempthrescfgreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure FPGA temperature threshold to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of FPGA voltage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_vol_thres_cfg_req(INT8 *arg3)
{
	UINT32 index;
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_vol_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	FpgaVolThresConf *pMsg = (FpgaVolThresConf*)arg3;
	index = pMsg->fpga_index;

	savefpgahealthresinfo[index-1].old_fpga_vol_thres = savefpgahealthresinfo[index-1].curr_fpga_vol_thres;
	savefpgahealthresinfo[index-1].curr_fpga_vol_thres = pMsg->fpga_vol_thres;

	memset(&volthrescfgreq,0,sizeof(volthrescfgreq));
	volthrescfgreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	volthrescfgreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_VOL_THRES_CFG_REQ);
	volthrescfgreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	volthrescfgreq.msg_head.msg_len = htonl(sizeof(volthrescfgreq));
	volthrescfgreq.fpga_id = htonl(savefpgaid[index-1].fpga_id);
	volthrescfgreq.thres_value = htons(pMsg->fpga_vol_thres);

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&volthrescfgreq,sizeof(volthrescfgreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure FPGA voltage threshold to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of CPU fan threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_fan_thres_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_cpu_fan_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	CpuServerFanThresConf *pMsg = (CpuServerFanThresConf*)arg3;

	savecpuserverhealinfo.old_cpu_server_fan_thres = savecpuserverhealinfo.curr_cpu_server_fan_thres;
	savecpuserverhealinfo.curr_cpu_server_fan_thres = pMsg->cpu_server_fan_thres;

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of CPU voltage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_vol_thres_cfg_req(INT8 *arg3)
{
	UINT32 index;
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_cpu_vol_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	CpuVolThresConf *pMsg = (CpuVolThresConf*)arg3;
	index = pMsg->cpu_index;

	savecpuhealinfo[index-1].old_cpu_vol_thres = savecpuhealinfo[index-1].curr_cpu_vol_thres;
	savecpuhealinfo[index-1].curr_cpu_vol_thres = pMsg->cpu_vol_thres;

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of CPU temperature threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_temp_thres_cfg_req(INT8 *arg3)
{
	UINT32 index;
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_cpu_temp_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	CpuTempThresConf *pMsg = (CpuTempThresConf*)arg3;
	index = pMsg->cpu_index;

	savecpuhealinfo[index-1].old_cpu_temp_thres=savecpuhealinfo[index-1].curr_cpu_temp_thres;
	savecpuhealinfo[index-1].curr_cpu_temp_thres=pMsg->cpu_temp_thres;

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of CPU usage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_usg_thres_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_cpu_usg_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	CpuUsgThresConf *pMsg = (CpuUsgThresConf*)arg3;

	savecpuservercomputesourceinfo.old_cpu_usg_thres = savecpuservercomputesourceinfo.curr_cpu_usg_thres;
	savecpuservercomputesourceinfo.curr_cpu_usg_thres = pMsg->cpu_usg_thres;

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of memory usage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_mem_usg_thres_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_mem_usg_thres_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	MemUsgThresConf *pMsg = (MemUsgThresConf*)arg3;

	savecpuservercomputesourceinfo.old_mem_usg_thres = savecpuservercomputesourceinfo.curr_mem_usg_thres;
	savecpuservercomputesourceinfo.curr_mem_usg_thres = pMsg->mem_usg_thres;

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of FPGA GPS position mode configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_gps_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_gps_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	PosModeConf *pMsg = (PosModeConf*)arg3;

	savetimesyncstate.old_pos_mode = savetimesyncstate.curr_pos_mode;
	savetimesyncstate.curr_pos_mode = pMsg->pos_mode_cfg;

	memset(&loccfgreq,0,sizeof(loccfgreq));
	loccfgreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	loccfgreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_GPS_CFG_REQ);
	loccfgreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	loccfgreq.msg_head.msg_len = htonl(sizeof(loccfgreq));
	loccfgreq.PosMode = pMsg->pos_mode_cfg;

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&loccfgreq,sizeof(loccfgreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure clock synchronization position mode to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of FPGA GPS clock source configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_clc_source_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_clc_source_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	CloSreConf *pMsg = (CloSreConf*)arg3;

	savetimesyncstate.old_clo_sre = savetimesyncstate.curr_clo_sre;
	savetimesyncstate.curr_clo_sre = pMsg->clo_sre_cfg;

	memset(&closourcfgreq,0,sizeof(closourcfgreq));
	closourcfgreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	closourcfgreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_CLOCK_SOURCE_CFG_REQ);
	closourcfgreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	closourcfgreq.msg_head.msg_len = htonl(sizeof(closourcfgreq));
	closourcfgreq.CloSource = pMsg->clo_sre_cfg;

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&closourcfgreq,sizeof(closourcfgreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure clock synchronization clock source to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of FPGA board power configure massage from CFIG_MGMT
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_brd_power_cfg_req(INT8 *arg3)
{
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_brd_power_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	FpgaServerPowerConf *pMsg = (FpgaServerPowerConf*)arg3;

	savefpgaserverpowerstate.old_fpga_server_power_state = savefpgaserverpowerstate.curr_fpga_server_power_state;
	savefpgaserverpowerstate.curr_fpga_server_power_state = pMsg->fpga_server_power_ctl;

	memset(&fpgaserverpowerctlreq,0,sizeof(fpgaserverpowerctlreq));
	fpgaserverpowerctlreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	fpgaserverpowerctlreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_BOARD_POWER_REQ);
	fpgaserverpowerctlreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	fpgaserverpowerctlreq.msg_head.msg_len = htonl(sizeof(fpgaserverpowerctlreq));
	fpgaserverpowerctlreq.fpga_server_id = htonl(savefpgaserverid.fpga_server_id);
	fpgaserverpowerctlreq.powerctl = pMsg->fpga_server_power_ctl;

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&fpgaserverpowerctlreq,sizeof(fpgaserverpowerctlreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure FPGA board power to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/*******************************************************************************************

 Description :handle function of FPGA chip power configure massage from CFIG_MGMT
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_chip_power_cfg_req(INT8 *arg3)
{
	UINT32 index;
	if(NULL == arg3)
	{
		log_msg(LOG_ERR, MAIN, "the input pointer of handle_fn_fpga_brd_power_cfg_req() is NULL.\n");
		return FUNC_ERR;
	}
	FpgaPowerConf *pMsg = (FpgaPowerConf*)arg3;
	index = pMsg->fpga_index;

	savefpgapowerstate[index-1].old_fpga_power_state = savefpgapowerstate[index-1].curr_fpga_power_state;
	savefpgapowerstate[index-1].curr_fpga_power_state = pMsg->fpga_power_ctl;

	memset(&fpgachippowerctlreq,0,sizeof(fpgachippowerctlreq));
	fpgachippowerctlreq.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	fpgachippowerctlreq.msg_head.msg_code = htons(BORD_MGMT_OM_RX_CHIP_POWER_REQ);
	fpgachippowerctlreq.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	fpgachippowerctlreq.msg_head.msg_len = htonl(sizeof(fpgachippowerctlreq));
	fpgachippowerctlreq.fpga_id = htonl(savefpgaid[index-1].fpga_id);
	fpgachippowerctlreq.powerctl = pMsg->fpga_power_ctl;

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&fpgachippowerctlreq,sizeof(fpgachippowerctlreq))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to configure FPGA chip power to control FPGA failed!\n");
		//trap api send a failed trap msg
		return FUNC_ERR;
	}

	return FUNC_OK;
}
