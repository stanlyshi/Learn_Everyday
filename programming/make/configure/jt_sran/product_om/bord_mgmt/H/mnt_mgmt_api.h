/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:44 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * mnt_mgmt_api.h
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#ifndef MNT_MGMT_API_H_
#define MNT_MGMT_API_H_
#include "sbs_type.h"
#include "om_common_h.h"

#pragma pack(1)

#define FpgaNum 5
#define CpuNum 2

#define  CFG_MGMT_BORD_MGMT_FPGA_FAN_THRES_CFG_REQ               0x0080
#define  CFG_MGM_BORD_MGMT_FPGA_TEMP_THRES_CFG_REQ               0x0081
#define  CFG_MGMT_BORD_MGMT_FPGA_VOL_THRES_CFG_REQ               0x0082

#define  CFG_MGMT_BORD_MGMT_CPU_FAN_THRES_CFG_REQ                0x0083
#define  CFG_MGMT_BORD_MGMT_CPU_VOL_THRES_CFG_REQ                0x0084
#define  CFG_MGMT_BORD_MGMT_CPU_TEMP_THRES_CFG_REQ               0x0085

#define  CFG_MGMT_BORD_MGMT_CPU_USG_THRES_CFG_REQ                0x0086
#define  CFG_MGMT_BORD_MGMT_MEM_USG_THRES_CFG_REQ                0x0087

#define  CFG_MGMT_BORD_MGMT_FPGA_GPS_CFG_REQ                     0x0088
#define  CFG_MGMT_BORD_MGMT_FPGA_CLOCK_SOURCE_CFG_REQ            0x0089

#define  CFG_MGMT_BORD_MGMT_FPGA_BOARD_POWER_CFG_REQ             0x008A
#define  CFG_MGMT_BORD_MGMT_FPGA_CHIP_POWER_CFG_REQ              0x008B

#define	BORD_MGMT_MSG_NE_SYNC_STATE	  0x00A0 /*BORD_MGMT 2 CELL_MGMT*/



/********************************************************************************************
*for set
*
*
********************************************************************************************/
typedef struct
{
	CommMsgHead msghead;
	UINT16 fpga_server_fan_thres;
}FpgaServerFanThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT32 fpga_index;
	UINT16 fpga_vol_thres;
}FpgaVolThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT32 fpga_index;
	UINT32 fpga_temp_thres;
}FpgaTempThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT16 cpu_server_fan_thres;
}CpuServerFanThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT32 cpu_index;
	UINT16 cpu_vol_thres;
}CpuVolThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT32 cpu_index;
	UINT32 cpu_temp_thres;
}CpuTempThresConf;

typedef struct
{
	CommMsgHead msghead;
	float cpu_usg_thres;
}CpuUsgThresConf;

typedef struct
{
	CommMsgHead msghead;
	float mem_usg_thres;
}MemUsgThresConf;

typedef struct
{
	CommMsgHead msghead;
	UINT8 fpga_server_power_ctl;
}FpgaServerPowerConf;

typedef struct
{
	CommMsgHead msghead;
	UINT32 fpga_index;
	UINT8 fpga_power_ctl;
}FpgaPowerConf;


typedef struct
{
	CommMsgHead msghead;
	UINT8 pos_mode_cfg;
}PosModeConf;


typedef struct
{
	CommMsgHead msghead;
	UINT8 clo_sre_cfg;
}CloSreConf;


/***********************************  set api***********************************************/
/*******************************************************************************************

 Description :handle function of FPGA fan threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_fan_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA temperature threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_temp_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA voltage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_vol_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of CPU fan threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_fan_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of CPU voltage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_vol_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of CPU temperature threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_temp_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of CPU usage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_cpu_usg_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of memory usage threshold configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_mem_usg_thres_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA GPS position mode configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_gps_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA GPS clock source configure massage from CFIG_MGMT
 input :massage pointer
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_clc_source_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA board power configure massage from CFIG_MGMT
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_brd_power_cfg_req(INT8 *arg3);

/*******************************************************************************************

 Description :handle function of FPGA chip power configure massage from CFIG_MGMT
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_chip_power_cfg_req(INT8 *arg3);



/******************************************************************************************
*for get
*
*
*******************************************************************************************/

typedef struct
{
UINT16 product_type;
UINT16 board_type;
UINT8 fpga_server_name[16];
UINT8 fpga_server_sn[32];
UINT8 fpga_server_version[16];
UINT8 fpga_server_manuf_data[12];
}SaveFpgaServerAssetInfo;
extern SaveFpgaServerAssetInfo savefpgaserverassetinfo;

typedef struct
{
	UINT32 fpga_id;
	UINT8 fpga_slot;
	UINT8 fpga_name[16];
	UINT8 fpga_version[24];
	UINT8 fpga_sn[32];
	UINT8 fpga_fw_version[16];
}SaveFpgaAssetInfo;
extern SaveFpgaAssetInfo savefpgaassetinfo[FpgaNum];

typedef struct
{
	UINT8 ctrl_fpga_mac[6];
}SaveCtrlFpgaMac;
extern SaveCtrlFpgaMac savectrlfpgamac;

typedef struct
{
	UINT8 fpga_mac1[6];
	UINT8 fpga_mac2[6];
	UINT8 fpga_mac3[6];
	UINT8 fpga_mac4[6];
	UINT8 fpga_mac5[6];
	UINT8 fpga_mac6[6];
}SaveDataFpgaMac;
extern SaveDataFpgaMac savedatafpgamac[FpgaNum-1];


typedef struct
{
	UINT8 ctrl_fpga_addr[16];
}SaveFpgaAddr;
extern SaveFpgaAddr savefpgaaddr;

typedef struct
{
	UINT32 fpga_server_id;
	UINT16 curr_fpga_server_fan_thres;
	UINT16 old_fpga_server_fan_thres;
}SaveFpgaServerHealThresInfo;
extern SaveFpgaServerHealThresInfo   savefpgaserverhealthresinfo;

typedef struct
{
	UINT32 fpga_id;
	UINT32 curr_fpga_temp_thres;
	UINT32 old_fpga_temp_thres;
	UINT16 curr_fpga_vol_thres;
	UINT16 old_fpga_vol_thres;
}SaveFpgaHealThresInfo;
extern SaveFpgaHealThresInfo     savefpgahealthresinfo[FpgaNum];

typedef struct
{
	UINT32 fpga_server_id;
    UINT16 fpga_server_fan;
}SaveFpgaServerHealInfo;
extern SaveFpgaServerHealInfo  savefpgaserverhealinfo;

typedef struct{
	UINT32 fpga_id;
    UINT32 fpga_temp;
    UINT16 fpga_vol;
}SaveFpgaHealInfo;
extern SaveFpgaHealInfo   savefpgahealinfo[FpgaNum];

typedef struct
{
	UINT32 fpga_server_id;
	UINT8  curr_fpga_server_power_state;
	UINT8  old_fpga_server_power_state;
}SaveFpgaServerPowerState;
extern SaveFpgaServerPowerState savefpgaserverpowerstate;

typedef struct
{
	UINT32 fpga_id;
	UINT8  curr_fpga_power_state;
	UINT8  old_fpga_power_state;
}SaveFpgaPowerState;
extern SaveFpgaPowerState	savefpgapowerstate[FpgaNum];


typedef struct
{
	UINT8 curr_pos_mode;
	UINT8 old_pos_mode;
	UINT8 curr_clo_sre;
	UINT8 old_clo_sre;
}SaveTimeSyncState;
extern SaveTimeSyncState	savetimesyncstate ;

//save FPGA/CPU physical id
typedef struct
{
	UINT32 fpga_id;
}SaveFpgaId;
extern SaveFpgaId savefpgaid[FpgaNum];

typedef struct
{
	UINT32 fpga_server_id;
}SaveFpgaServerId;
extern SaveFpgaServerId savefpgaserverid;

typedef struct
{
	UINT32 cpu_id;
}SaveCpuId;
extern SaveCpuId savecpuid[CpuNum];

typedef struct
{
	UINT32 cpu_server_id;
	UINT32 bmc_id;
}SaveCpuServerId;
extern SaveCpuServerId savecpuserverid;

//cpu save information for get api
typedef struct
{
	UINT8 cpu_om_addr[16];
	UINT8 cpu_data_addr1[16];
	UINT8 cpu_data_addr2[16];
	UINT8 cpu_data_addr3[16];
}SaveCpuAddr;
extern SaveCpuAddr savecpuaddr;

typedef struct
{
	UINT8 cpu_om_mac[6];
	UINT8 cpu_data_mac[6];
	UINT8 cpu_data_mac2[6];
	UINT8 cpu_data_mac3[6];
}SaveCpuMac;
extern SaveCpuMac  savecpumac;

typedef struct
{
	UINT32 	cpu_id;
	UINT8   cpu_name[48];
	UINT8   bios_version[16];
}SaveCpuAssetInfo;
extern SaveCpuAssetInfo  savecpuassetinfo[CpuNum];

typedef struct
{
    UINT32  bmc_id;
	UINT8   bmc_name[24];
	UINT8   bmc_fw_version[24];
	UINT8   bmc_addr[16];
}SaveBmcAssetInfo;
extern SaveBmcAssetInfo  savebmcassetinfo;

typedef struct
{
	UINT32 cpu_server_id;
	float curr_cpu_server_fan_thres;
	float old_cpu_server_fan_thres;
	float cpu_server_fan;
}SaveCpuServerHealInfo;
extern SaveCpuServerHealInfo savecpuserverhealinfo;

typedef struct
{
    UINT32 cpu_id;
    UINT32 curr_cpu_temp_thres;
    UINT32 old_cpu_temp_thres;
    UINT32 cpu_temp;
    UINT16 curr_cpu_vol_thres;
    UINT16 old_cpu_vol_thres;
    UINT16 cpu_vol;
}SaveCpuHealInfo;
extern SaveCpuHealInfo  savecpuhealinfo[CpuNum];

typedef struct
{
	UINT32 cpu_server_id;
	float  cpu_usg;
	float  mem_usg;
	float  curr_cpu_usg_thres;
    float  old_cpu_usg_thres;
    float  curr_mem_usg_thres;
    float  old_mem_usg_thres;
}SaveCpuServerComputeSourceInfo;
extern SaveCpuServerComputeSourceInfo savecpuservercomputesourceinfo;

//trap massage
typedef struct
{
UINT32 NeId;
UINT32 	NE_start_state;
}NeStartStateRep;

typedef struct
{
	UINT8 TimeSync;
	UINT8 ErrCode;
	UINT8 PosMode;
	UINT8 CloSource;
}SaveClcSyncStateRep;




INT32 CTRL_FPGA_Device_Get_produce_type(UINT32 *value);
INT32 CTRL_FPGA_Device_Get_board_type(UINT32 *value);
INT32 CTRL_FPGA_Device_Get_fpga_server_name(char *value,UINT32 ulength);
INT32 CTRL_FPGA_Device_Get_fpga_server_sn(char *valne,UINT32 ulength);
INT32 CTRL_FPGA_Device_Get_fpga_server_version(char *value,UINT32 ulength);
INT32 CTRL_FPGA_Device_Get_fpga_server_manuf_data(char *value,UINT32 ulength);

/*fpga ----asset information get api*/
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_id(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_DeviveInfo_Get_fpga_slot(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_name(UINT32 index,char *value,UINT32 ulength);
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_version(UINT32 index,char *value,UINT32 ulength);
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_sn(UINT32 index,char *value,UINT32 ulength);
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_fw_version(UINT32 index,char *value,UINT32 ulength);

/*fpga server----heath_thres information get api*/
INT32 CTRL_FPGA_Healthres_Get_fpga_server_id(UINT32 *value);
INT32 CTRL_FPGA_Healthres_Get_fpga_server_fan_thres(UINT32 *value);

/*fpga server----heath information get api*/
INT32 CTRL_FPGA_Health_Get_fpga_server_id(UINT32 *value);
INT32 CTRL_FPGA_Health_Get_fpga_server_fan(UINT32 *value);

/*fpga----heath_thres information get api*/
INT32 CTRL_FPGA_Healthres_Get_fpga_id(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_Healthres_Get_fpga_temp_thres(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_Healthres_Get_fpga_vol_thres(UINT32 index,UINT32 *value);

/*fpga----heath information get api*/
INT32 CTRL_FPGA_HealthInfo_Get_fpga_id(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_HealthInfo_Get_fpga_vol(UINT32 index,UINT32 *value);
INT32 CTRL_FPGA_HealthInfo_Get_fpga_temp(UINT32 index,UINT32 *value);

/*fpga server----clock module information get api*/

INT32 CTRL_FPGA_Clock_Get_PosModeCfg(UINT32 *value);
INT32 CTRL_FPGA_Clock_Get_CloSreCfg(UINT32 *value);

/*fpga server----power state get api*/

INT32 CTRL_FPGA_Power_Get_fpgaServerPowerCtl(UINT32 *value);
INT32 CTRL_FPGA_Power_Get_fpgaPowerCtl(UINT32 index,UINT32 *value);

/*cpu  server---asset information get api*/

INT32 CTRL_CPU_Device_Get_cpu_id(UINT32 index,UINT32 *value);
INT32 CTRL_CPU_Device_Get_cpu_name(UINT32 index,char *value,UINT32 ulength);
INT32 CTRL_CPU_Device_Get_bios_version(UINT32 index,char *value,UINT32 ulength);

INT32 CTRL_CPU_Device_Get_bmc_id(UINT32 *value);
INT32 CTRL_CPU_Device_Get_bmc_name(char *value,UINT32 ulength);
INT32 CTRL_CPU_Device_Get_bmc_fw_version(char *value,UINT32 ulength);
INT32 CTRL_CPU_Device_Get_bmc_addr(UINT8 *addr,UINT32 ulength);

/*cpu  server---health information get api*/

INT32 CTRL_CPU_Health_Get_cpu_server_id(UINT32 *value);
INT32 CTRL_CPU_Health_Get_cpu_server_fan_thres(UINT32 *value);
INT32 CTRL_CPU_Health_Get_cpu_server_fan(UINT32 *value);

INT32 CTRL_CPU_HealthInfo_Get_cpu_id(UINT32 index,UINT32 *value);
INT32 CTRL_CPU_HealthInfo_Get_cpu_temp_thres(UINT32 index,UINT32 *value);
INT32 CTRL_CPU_HealthInfo_Get_cpu_temp(UINT32 index,UINT32 *value);
INT32 CTRL_CPU_HealthInfo_Get_cpu_vol_thres(UINT32 index,UINT32 *value);
INT32 CTRL_CPU_HealthInfo_Get_cpu_vol(UINT32 index,UINT32 *value);

/*cpu  server---computing information get api*/

INT32 CTRL_CPU_Cal_Get_cpu_server_id(UINT32 *value);
INT32 CTRL_CPU_Cal_Get_cpu_usg_thres(float *value);
INT32 CTRL_CPU_Cal_Get_cpu_usg(float *value);
INT32 CTRL_CPU_Cal_Get_mem_usg_thres(float *value);
INT32 CTRL_CPU_Cal_Get_mem_usg(float *value);

/***********for cell management**************/

INT32 get_data_fpga_mac(UINT8 (*fpga_mac)[6]);


/********************************************************************************************************
*for cell_mgmt
*
*
********************************************************************************************************/

/* Types -------------------------------------------------------------------- */
typedef struct {
	CommMsgHead     msghead;
	NeSyncState     bord_syn;
}__attribute__((packed))MsgBordSyn;

extern MsgBordSyn msgbordsyn;


#pragma pack()

#endif /* MNT_MGMT_API_GET_H_ */
