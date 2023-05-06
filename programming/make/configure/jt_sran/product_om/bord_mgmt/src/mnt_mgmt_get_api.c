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
/*
 ============================================================================
 Name        : mnt_mgmt_api.c
 Author      :
 Version     : v1.0
 Copyright   :
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "sbs_task.h"
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
/**************************************************************************************************
 * Description: for cell management module to get data fpga mac_address
 *
 * Input:  pointer points to an array (fpga mac addr)
 *
 * Output: NULL
 * return: FUNC_OK   success
 *         FUNC_ERR  faile
 *************************************************************************************************/

INT32 get_data_fpga_mac(UINT8 (*fpga_mac)[6])
{
	if(NULL == fpga_mac)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of get_data_fpga_mac() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(fpga_mac, savedatafpgamac[1].fpga_mac2,6);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Device_Get_produce_type(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_produce_type() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, &savefpgaserverassetinfo.product_type,sizeof(UINT16));

    pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_FPGA_Device_Get_board_type(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_board_type() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, &savefpgaserverassetinfo.board_type,sizeof(UINT16));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Device_Get_fpga_server_name(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_fpga_server_name() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaserverassetinfo.fpga_server_name,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;

}

INT32 CTRL_FPGA_Device_Get_fpga_server_sn(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_fpga_server_sn() is invalid. \n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaserverassetinfo.fpga_server_sn,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Device_Get_fpga_server_version(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_fpga_server_version() is invalid. \n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaserverassetinfo.fpga_server_version,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Device_Get_fpga_server_manuf_data(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Device_Get_fpga_server_manuf_data() is invalid. \n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,savefpgaserverassetinfo.fpga_server_manuf_data,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;

}

INT32 CTRL_FPGA_DeviceInfo_Get_fpga_id(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviceInfo_Get_fpga_id() is invalid. \n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviceInfo_Get_fpga_id() is invalid. \n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, &savefpgaassetinfo[index-1].fpga_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_DeviveInfo_Get_fpga_slot(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviveInfo_Get_fpga_slot() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviveInfo_Get_fpga_slot() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, &savefpgaassetinfo[index-1].fpga_slot,sizeof(UINT8));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_DeviceInfo_Get_fpga_name(UINT32 index,char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviceInfo_Get_fpga_name() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviceInfo_Get_fpga_name() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaassetinfo[index-1].fpga_name,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_DeviceInfo_Get_fpga_version(UINT32 index,char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviceInfo_Get_fpga_version() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviceInfo_Get_fpga_version() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaassetinfo[index-1].fpga_version,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);

	return FUNC_OK;
}

INT32 CTRL_FPGA_DeviceInfo_Get_fpga_sn(UINT32 index,char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviceInfo_Get_fpga_sn() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviceInfo_Get_fpga_sn() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaassetinfo[index-1].fpga_sn,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_DeviceInfo_Get_fpga_fw_version(UINT32 index,char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_DeviceInfo_Get_fpga_fw_version() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_DeviceInfo_Get_fpga_fw_version() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value, savefpgaassetinfo[index-1].fpga_fw_version,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}



INT32 CTRL_FPGA_Healthres_Get_fpga_server_id(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Healthres_Get_fpga_server_id() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgaserverhealthresinfo.fpga_server_id,sizeof(UINT32));

    pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}


INT32 CTRL_FPGA_Healthres_Get_fpga_server_fan_thres(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Healthres_Get_fpga_server_fan_thres() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgaserverhealthresinfo.curr_fpga_server_fan_thres,sizeof(UINT16));

    pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}

INT32 CTRL_FPGA_Health_Get_fpga_server_id(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Health_Get_fpga_server_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgaserverhealinfo.fpga_server_id,sizeof(UINT32));

    pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Health_Get_fpga_server_fan(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Health_Get_fpga_server_fan() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgaserverhealinfo.fpga_server_fan,sizeof(UINT16));

    pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_FPGA_Healthres_Get_fpga_id(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Healthres_Get_fpga_id() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_Healthres_Get_fpga_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealthresinfo[index-1].fpga_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_FPGA_Healthres_Get_fpga_temp_thres(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Healthres_Get_fpga_temp_thres() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_Healthres_Get_fpga_temp_thres() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealthresinfo[index-1].curr_fpga_temp_thres,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Healthres_Get_fpga_vol_thres(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Healthres_Get_fpga_vol_thres() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_Healthres_Get_fpga_vol_thres() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealthresinfo[index-1].curr_fpga_vol_thres,sizeof(UINT16));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}

INT32 CTRL_FPGA_HealthInfo_Get_fpga_id(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_HealthInfo_Get_fpga_id() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_HealthInfo_Get_fpga_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealinfo[index-1].fpga_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_HealthInfo_Get_fpga_temp(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_HealthInfo_Get_fpga_temp() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_HealthInfo_Get_fpga_temp() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealinfo[index-1].fpga_temp,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_HealthInfo_Get_fpga_vol(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_HealthInfo_Get_fpga_vol() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_HealthInfo_Get_fpga_vol() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgahealinfo[index-1].fpga_vol,sizeof(UINT16));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Clock_Get_PosModeCfg(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Clock_Get_PosModeCfg() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savetimesyncstate.curr_pos_mode,sizeof(UINT8));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_FPGA_Clock_Get_CloSreCfg(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Clock_Get_CloSreCfg() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savetimesyncstate.curr_clo_sre,sizeof(UINT8));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Power_Get_fpgaServerPowerCtl(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Power_Get_fpgaServerPowerCtl() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgaserverpowerstate.curr_fpga_server_power_state,sizeof(UINT8));


	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_FPGA_Power_Get_fpgaPowerCtl(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_FPGA_Power_Get_fpgaPowerCtl() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > FpgaNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the fpga index of CTRL_FPGA_Power_Get_fpgaPowerCtl() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savefpgapowerstate[index-1].curr_fpga_power_state,sizeof(UINT8));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}


INT32 CTRL_CPU_Device_Get_cpu_id(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_cpu_id() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_Device_Get_cpu_id() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuassetinfo[index-1].cpu_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}


INT32 CTRL_CPU_Device_Get_cpu_name(UINT32 index,char *value,UINT32 ulength)
{

	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_cpu_name() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_Device_Get_cpu_name() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,savecpuassetinfo[index-1].cpu_name,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Device_Get_bios_version(UINT32 index,char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_bios_version() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_Device_Get_bios_version() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,savecpuassetinfo[index-1].bios_version,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
	}

INT32 CTRL_CPU_Device_Get_bmc_id(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_bmc_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savebmcassetinfo.bmc_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Device_Get_bmc_name(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_bmc_name() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,savebmcassetinfo.bmc_name,ulength);


	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Device_Get_bmc_fw_version(char *value,UINT32 ulength)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_bmc_fw_version() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,savebmcassetinfo.bmc_fw_version,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}
INT32 CTRL_CPU_Device_Get_bmc_addr(UINT8 *addr,UINT32 ulength)
{
	if(NULL == addr)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Device_Get_bmc_addr() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(addr,savebmcassetinfo.bmc_addr,ulength);

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Health_Get_cpu_server_id(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Health_Get_cpu_server_id() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuserverhealinfo.cpu_server_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Health_Get_cpu_server_fan_thres(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Health_Get_cpu_server_fan_thres() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuserverhealinfo.curr_cpu_server_fan_thres,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Health_Get_cpu_server_fan(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Health_Get_cpu_server_fan() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuserverhealinfo.cpu_server_fan,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_HealthInfo_Get_cpu_id(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_HealthInfo_Get_cpu_id() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_HealthInfo_Get_cpu_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuhealinfo[index-1].cpu_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_HealthInfo_Get_cpu_temp_thres(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_HealthInfo_Get_cpu_temp_thres() is invalid.\n");
		return FUNC_ERR;
	}

	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_HealthInfo_Get_cpu_temp_thres() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuhealinfo[index-1].curr_cpu_temp_thres,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_CPU_HealthInfo_Get_cpu_temp(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_HealthInfo_Get_cpu_temp() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_HealthInfo_Get_cpu_temp() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuhealinfo[index-1].cpu_temp,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_HealthInfo_Get_cpu_vol_thres(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_HealthInfo_Get_cpu_vol_thres() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_HealthInfo_Get_cpu_vol_thres() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuhealinfo[index-1].curr_cpu_vol_thres,sizeof(UINT16));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_HealthInfo_Get_cpu_vol(UINT32 index,UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_HealthInfo_Get_cpu_vol() is invalid.\n");
		return FUNC_ERR;
	}
	if((index > CpuNum) || (0 == index))
	{
		log_msg(LOG_ERR, BORD_MGMT, "the cpu index of CTRL_CPU_HealthInfo_Get_cpu_vol() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuhealinfo[index-1].cpu_vol,sizeof(UINT16));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Cal_Get_cpu_server_id(UINT32 *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Cal_Get_cpu_server_id() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuservercomputesourceinfo.cpu_server_id,sizeof(UINT32));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}
INT32 CTRL_CPU_Cal_Get_cpu_usg_thres(float *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Cal_Get_cpu_usg_thres() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuservercomputesourceinfo.curr_cpu_usg_thres,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}


INT32 CTRL_CPU_Cal_Get_cpu_usg(float *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Cal_Get_cpu_usg() is invalid.\n");
		return FUNC_ERR;
	}

	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuservercomputesourceinfo.cpu_usg,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Cal_Get_mem_usg_thres(float *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Cal_Get_mem_usg_thres() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuservercomputesourceinfo.curr_mem_usg_thres,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}

INT32 CTRL_CPU_Cal_Get_mem_usg(float *value)
{
	if(NULL == value)
	{
		log_msg(LOG_ERR, BORD_MGMT, "the input pointer of CTRL_CPU_Cal_Get_mem_usg() is invalid.\n");
		return FUNC_ERR;
	}
	pthread_rwlock_rdlock(&brd_mnt_rwlock);

	memcpy(value,&savecpuservercomputesourceinfo.mem_usg,sizeof(float));

	pthread_rwlock_unlock(&brd_mnt_rwlock);
	return FUNC_OK;
}








