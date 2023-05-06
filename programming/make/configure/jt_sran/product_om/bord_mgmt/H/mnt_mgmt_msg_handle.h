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
 * mnt_mgmt_msg_handle.h
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#ifndef MNT_MGMT_MSG_HANDLE_H_
#define MNT_MGMT_MSG_HANDLE_H_
#include <mnt_mgmt_api.h>
#include <om_common_h.h>
#include "sbs_task.h"
#pragma pack(1)

#define 	HANDLE_HB_TIME 				30
#define	    QUERY_FPGA_TIME 			10

#define 	PortNum 					6

#define 	FpgaNum 					5
#define 	CpuNum						2


#define 	MsgHeadBufLen 				12
#define 	MacHeadBufLen				14
#define 	FpgaServerInfoBufLen		80
#define 	CtrlFpgaInfoBufLen			95
#define     DataFpgaInfoBufLen 			125

#define 	FpgaServerHealLen 			6
#define 	FpgaHealInfoLen 			10

#define     IpType 						 0X00   //IPv4 type

#define     CtrlFpgaIpNode   			1


/*****************massage code between control FPGA and board management module **********************/

#define  OM_RX_BORD_MGMT_BROADCAST_INFO_REQ         0X0001
#define  BORD_MGMT_OM_RX_BROADCAST_INFO_RSP         0X0002
#define  OM_RX_BORD_MGMT_FPGA_IP_CFG_RSP			0X0003
#define  OM_RX_BORD_MGMT_TIME_SYNC_REPORT           0X0004
#define  BORD_MGMT_OM_RX_TIME_SYNC_ACK              0X0005
#define  BORD_MGMT_OM_RX_CHAN_MONI_TRIGGER          0X0011
#define  BORD_MGMT_OM_RX_HEAL_THRES_QUERY_REQ       0X0021
#define  OM_RX_BORD_MGMT_HEAL_THRES_QUERY_RSP       0X0022
#define  BORD_MGMT_OM_RX_HEAL_INFO_QUERY_REQ        0X0023
#define  OM_RX_BORD_MGMT_HEAL_INFO_QUERY_RSP        0X0024
#define  BORD_MGMT_OM_RX_TEMP_THRES_CFG_REQ         0X0031
#define  OM_RX_BORD_MGMT_TEMP_THRES_CFG_RSP         0X0032
#define  BORD_MGMT_OM_RX_VOL_THRES_CFG_REQ          0X0033
#define  OM_RX_BORD_MGMT_VOL_THRES_CFG_RSP          0X0034
#define  BORD_MGMT_OM_RX_FAN_THRES_CFG_REQ          0X0035
#define  OM_RX_BORD_MGMT_FAN_THRES_CFG_RSP          0X0036
#define  BORD_MGMT_OM_RX_GPS_CFG_REQ                0X0037
#define  OM_RX_BORD_MGMT_GPS_CFG_RSP                0X0038
#define  BORD_MGMT_OM_RX_CLOCK_SOURCE_CFG_REQ       0X0039
#define  OM_RX_BORD_MGMT_CLOCK_SOURCE_CFG_RSP       0X003A
#define  BORD_MGMT_OM_RX_FW_UPDATE_REQ              0X0041
#define  OM_RX_BORD_MGMT_FW_UPDATE_RSP              0X0042
#define  OM_RX_BORD_MGMT_CHANNEL_MONITOR            0X0051
#define  BORD_MGMT_OM_RX_OM_CHANNEL_MONITOR         0X0052
#define  BORD_MGMT_OM_RX_BOARD_POWER_REQ            0X0071
#define  OM_RX_BORD_MGMT_BOARD_POWER_RSP            0X0072
#define  BORD_MGMT_OM_RX_CHIP_POWER_REQ             0X0073
#define  OM_RX_BORD_MGMT_CHIP_POWER_RSP             0X0074

/*********************massage code between data  FPGA and board management module********************/
#define  OM_RX_BORD_MGMT_DATA_CHANNEL_MONITOR       001
#define  BORD_MGMT_DATA_FPGA_FRNUM_REQ			    011
#define  DATA_FPGA_BORD_MGMT_FRNUM_RSP			    012

/******************************************massage code of timer*************************************/

#define  HEART_BEAT_BORD_TIMER_MSG        		    0x0091
#define  QUERY_FPGA_CPU_HEAL_TIMER_MSG      0x0092

/***************massage code between board management module and cell management module*************/
#define  FPGA_START_STATE_REPORT			0x00B0
#define  MSG_TYPE_DATA_FPGA_BORD_MGMT				0x5400


typedef enum
{
	BORD_MGMT_CTRL_FPGA,   //massage from OM board management module to control FPGA
	BORD_MGMT_DATA_FPGA,   //massage from OM board management module to data FPGA
	RRU_MGMT_DATA_FPGA,    //massage from OM RRU Management module to data FPGA

	CTRL_FPGA_BORD_MGMT,   //control FPGA to OM board management module
	CTRL_FPGA_SBS_FTP,
	DATA_FPGA_BORD_MGMT,    //DATA FPGA to OM board management module
	DATA_FPGA_RRU_MGMT,     //DATA FPGA to OM RRU Management module
	FPGA_ALARM_MGMT,        //FPGA alarm OM to OM alarm management module
	TIMER_MSG,
	CFG_MGMT_BORD_MGMT,
}MsgType;

typedef struct
{
	UINT16 msg_type;
	UINT16 msg_code;
	UINT32 msg_len;
	UINT32 fpga_id;
}MsgHead;
extern MsgHead msghead;

typedef struct
{
	UINT8 des_mac[6];
	UINT8 sre_mac[6];
	UINT16 pro_type;
}MacHead;

/*********************************define massage body information*****************************************/

typedef struct
{
	MacHead mac_head;
	MsgHead msg_head;
	UINT16 product_type;
	UINT16 board_type;
	UINT8 fpga_server_sn[32];
	UINT8 fpga_server_name[16];
	UINT8 fpga_server_version[16];
	UINT8 fpga_server_manuf_data[12];
}FpgaServerAssetInfo;  //FPGA server asset information

typedef struct
{
	UINT8 fpga_slot;
	UINT8 fpga_name[16];
	UINT8 fpga_version[24];
	UINT8 fpga_sn[32];
	UINT8 fpga_fw_version[16];
	UINT8 fpga_mac[6];
}CtrlFpgaAssetInfo;  //control FPGA asset information

typedef struct
{
	MacHead mac_head;
	MsgHead msg_head;
	UINT8 des_mac[6]; //
}FrameInfoReportCfgInfo;//configure data FPGA for data FPGA frame information report
extern FrameInfoReportCfgInfo frameinforeportcfginfo;

typedef struct
{
	MacHead mac_head;
	MsgHead msg_head;
	UINT8 result;
}FrameInfoReportCfgRsp;//frame report configuration respond information
extern FrameInfoReportCfgRsp frameinforeportcfgrsp;

typedef struct
{
	MsgHead msg_head;
	FpgaState fpga_start_state;
}FpgaStartStateReport;
extern FpgaStartStateReport fpgastartstatereport;

typedef struct
{
	UINT8 fpga_slot;
	UINT8 fpga_name[16];
	UINT8 fpga_version[24];
	UINT8 fpga_sn[32];
	UINT8 fpga_fw_version[16];
	UINT8 fpga_mac1[6];
	UINT8 fpga_mac2[6];
	UINT8 fpga_mac3[6];
	UINT8 fpga_mac4[6];
	UINT8 fpga_mac5[6];
	UINT8 fpga_mac6[6];
}DataFpgaAssetInfo;//data FPGA asset information

extern DataFpgaAssetInfo* datafpgaassetinfo[FpgaNum-1];

typedef struct{
	MacHead mac_head;
	MsgHead msg_head;
	UINT8 iptype;
	UINT8 fpga1_addr[16];
	UINT8 ftp_server_addr[16];
	UINT8 cpu_om_addr[16];
	UINT32 fpga_server_id;
	UINT32 fpga1_id;
	UINT32 fpga2_id;
	UINT32 fpga3_id;
	UINT32 fpga4_id;
	UINT32 fpga5_id;
}ResCtrlFpga;//respond information for control FPGA broadcast massage

extern ResCtrlFpga resctrlfpga;

typedef struct
{
	MacHead mac_head;
	MsgHead msg_head;
	UINT8 fpga_addr_cfg_result;
}FpgaAddrCfgReault;//control FPGA IP address configuration result


typedef struct
{
	MsgHead msg_head;
	UINT8 timesync;
	UINT8 errcode;
	UINT8 posmode;
	UINT8 closource;
}CloSyncReport;//clock synchronization information

typedef struct{
	MsgHead msg_head;
	UINT8 cpu_data_mac1[6];   //the destination AMC of heart heat packet(data FPGA data channel monitor)
}FpgaChanMoniTrig;

extern FpgaChanMoniTrig fpgachanmonitrig;

typedef struct
{
	MsgHead msghead;
	UINT32 fpga_server_id;
	UINT16 fpga_server_fan_thres;
}FpgaServerHealThresInfo;//FPGA server health threshold information

typedef struct
{
	UINT32 fpga_id;
	UINT32 fpga_temp_thres;
	UINT16 fpga_vol_thres;
}FpgaHealThresInfo;//FPGA health threshold information
extern FpgaHealThresInfo* fpgahealthresinfo[FpgaNum];

typedef struct
{
	MsgHead msghead;
	UINT32 fpga_server_id;
	UINT16 fpga_server_fan;
}FpgaServerHealInfo;//FPGA server health information

typedef struct
{
	UINT32 fpga_id;
	UINT32 fpga_temp;
	UINT16 fpga_vol;
}FpgaHealInfo;//FPGA health information
extern FpgaHealInfo* fpgahealinfo[FpgaNum];

/***cfg info***/
typedef struct
{
	MsgHead msg_head;
	UINT32 fpga_id;
	UINT32 thres_value;
}TempThresCfgReq;//FPGA temperature threshold configure request
extern TempThresCfgReq tempthrescfgreq;


typedef struct
{
	MsgHead msg_head;
	UINT32 fpga_id;
	UINT16 thres_value;
}VolFanThresCfgReq;//FPGA voltage threshold configure request

extern VolFanThresCfgReq fanthrescfgreq;
extern VolFanThresCfgReq volthrescfgreq;

typedef struct{
	MsgHead msg_head;
	UINT32 fpga_id;
	UINT8 result;
}HealThresCfgRsp;//respond


typedef struct
{
	MsgHead msg_head;
	UINT8 PosMode;
}LocCfgReq; //clock synchronization position mode configure request
 extern LocCfgReq loccfgreq;

typedef struct{
	MsgHead msg_head;
	UINT8 cfgresult;
}LocCfgRsp;//position mode configure respond

typedef struct
{
	MsgHead msg_head;
	UINT8 CloSource;
}CloSouCfgReq; //clock synchronization clock source configure request
extern  CloSouCfgReq closourcfgreq;

typedef struct
{
	MsgHead msg_head;
	UINT8 cfgresult;
}CloSouCfgRsp; //clock source configure respond

typedef struct{
	MsgHead msg_head;
	UINT8 FilePath[200];
	UINT8 FwVersion[16];
	UINT32 FwLength;
}CtrlFpgaFwUpdateReq; //control FPGA firmware update request

typedef struct
{
	MsgHead msg_head;
	UINT8 result;
}CtrlFpgaFwUpdateRsp; //control FPGA firmware update respond

typedef struct
{
	MsgHead msg_head;
	UINT32 fpga_server_id;
	UINT8 powerctl;
}FpgaServerPowerCtlReq; //FPGA server power control request
 extern FpgaServerPowerCtlReq fpgaserverpowerctlreq;

typedef struct
{
	MsgHead msg_head;
	UINT8 result;
}FpgaBoardPowerCtlRsp; //FPGA server power control respond

typedef struct
{
	MsgHead msg_head;
    UINT32 fpga_id;
	UINT8 powerctl;
}FpgaChipPowerCtlReq; //FPGA chip power control request
extern FpgaChipPowerCtlReq fpgachippowerctlreq;

typedef struct
{
	MsgHead msg_head;
	UINT32 fpga_id;
	UINT8 result;
}FpgaChipPowerCtlRsp; //FPGA chip power control respond


typedef struct
{
//UINT32 fpga_id;
//UINT8  port_id;
UINT32 heartb_count;
//UINT32 fpga_state; // 0 is not use,1 is use;
UINT32 fpga_port_state; //0 is not use,1 is use;
}FpgaHeartBeatRcd;//FPGA心跳记录

typedef struct
{
	struct sbs_timer_list* timer;
	UINT32 seconds;
}heartb_timer;

typedef struct
{
	INT8 cpu_name[20];
   UINT32 user;/*user's time from the start of system*/
   UINT32 nice;/*the time with negative nice value occupy the time */
   UINT32 system;/*the time of system */
   UINT32 idle;/*idle time*/
   UINT32 iowait;/*hard disk IO time*/
   UINT32 irq;/*hardware interrupt time*/
   UINT32 softirg;/*software interrupt time*/
}Cpu_Time;

typedef struct
{
	UINT32 MemTotal;//KB
	UINT32 MemFree;
	UINT32 Buffers;
	UINT32 Cached;
	UINT32 SwapTotal;
	UINT32 SwapFree;
	UINT32 Shmem;
	UINT32 HugePages_Free;
	UINT32 Hugepagesize;
}Mem_Info;
typedef struct
{
	struct sbs_timer_list* timer;
	//Cpu_Time* start;
	UINT32 seconds;
}query_fpga_timer;


/*****read and write lock******/
extern pthread_rwlock_t brd_mnt_rwlock;

extern FpgaHeartBeatRcd fpgaheartbeatrcd[FpgaNum][PortNum];
extern Cpu_Time cpu_time_start;
extern Cpu_Time cpu_time_end;






/************************************************************************************************

 Description :handle MAC broadcast message from control FPGA
 1.save FPGA asset information
 2.allocation IP for control FPGA
 3.update fpga1/2/3/4/5 firmware
 4.trigger FPGA send heart beat massage
 5.send health threshold query request
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_broadcast_info_req(INT8 *buf);

/************************************************************************************************

 Description :handle function of control FPGA broadcast rspond

 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_broadcast_rsp();

/**************************************************************************************************

 Description :handle function of clock synchronization report message from control FPGA:
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fn_time_sync_report_req(INT8 *buf);

/***************************************************************************************************

 Description :handle function of FPGA health information threshold query respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fpga_heal_thres_query_rsp(INT8 *buf);

/***************************************************************************************************

 Description :handle function of FPGA health information query respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fn_fpga_heal_info_query_rsp (INT8 *buf);

/***************************************************************************************************

 Description :handle function of FPGA temperature threshold configuration result from control FPGA.
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***************************************************************************************************/
INT32 handle_fn_fpga_temp_thres_cfg_rsp (INT8 *buf);

/*************************************************************************************************

 Description :handle function of FPGA voltage threshold configuration result from control FPGA.
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_vol_thres_cfg_rsp (INT8 *buf);

/*************************************************************************************************

 Description :handle function of FPGA voltage threshold configuration result from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_fpga_fan_thres_cfg_rsp (INT8 *buf);

/********************************************************************************************

 Description :handle function of  GPS position mode configuration result respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_gps_cfg_rsp(INT8 *buf);

/*******************************************************************************************

 Description :handle function of FPGA clock source configuration result respond from control FPGA
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *******************************************************************************************/
INT32 handle_fn_clock_source_cfg_rsp( INT8 *buf);

/********************************************************************************************

 Description :handle function of control FPGA heart beat message:
 input :massage pointer
 output:NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga1_heartbeat_msg( INT8 *buf);

/*********************************************************************************************

 Description :handle function of control FPGA IP configuration result from control FPGA:
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *******************************************************************************************/
INT32 handle_fn_om_udp_msg(INT8 *buf);

/********************************************************************************************

 Description :handle function of FPGA board power control result from control FPGA:
 input : massage pointer
 output: null
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga_board_power_rsp( INT8 *buf);

/********************************************************************************************

 Description :handle function of signal FPGA power control result respond from control fpga:
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga_chip_power_rsp( INT8 *buf);

/********************************************************************************************

 Description :handle function of data FPGA heart beat message from FPGA2/3/4/5:
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_data_fpga_heartbeat_msg( INT8 *buf);

/*********************************************************************************************

 Description :handle function of configuring frame report to data FPGA
 input : NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_frame_info_report_cfg_req();

/*********************************************************************************************

 Description :handle function of frame report configuration respond from data FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_frame_info_report_cfg_rsp(INT8 *buf);

/*********************************************************************************************

 Description :handle heart beat timeout message from heart beat monitor timer
 input :NULL
 output:NULL
 return:success FUNC_OK
        false FUNC_ERR

 *******************************************************************************************/
INT32 handle_fn_heartb_timer_msg();

/*******************************************************************************************

 Description :handle function of timeout message from query FPGA information timer
 input :NULL
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 handle_fn_query_fpga_cpu_heal_info_timer_msg();

/*******************************************************************************************

 Description :send query FPGA healthy information to MODULE_INET_MGMT
 input : NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ******************************************************************************************/
INT32 query_fpga_heal_info();

/***********************************************************************************************

 Description :send check fpga heartbeat counts timeout message to own message queue(callback function)
 input: NULL
 output:NULL
 return:void

 ***********************************************************************************************/
void send_heartb_bord_timeout_msg(void *data);

/************************************************************************************************

 Description :send query FPGA timeout message to own message queue(callback function)
 input: NULL
 output:NULL
 return: void

 ************************************************************************************************/
void send_query_fpga_timeout_msg(void *data);

/***********************************************************************************************

 Description :add heart beat  count timer
 input : time(second)
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 add_handle_heartb_msg_timer(UINT32 seconds);

/***********************************************************************************************

 Description :add query FPGA timer
 input : time(second)
 output:NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 add_query_fpga_timer(UINT32 seconds);

/***********************************************************************************************

 Description :get CPU server computing resource usage:cpu_usage,memory_usage
 input :NULL
 output:NULL
 return:success FUNC_OK
        false FUNC_ERR

 ***********************************************************************************************/
void get_cpu_server_res_usage();

/**********************************************************************************************

 Description :get cpu server healthy info
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ************************************************************************************************/
INT32 get_cpu_server_heal_info();

/************************************************************************************************

 Description :get cpu used time from /proc/stat,at first time.
 input: NULL
 output: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 get_cpu_start_time();

/************************************************************************************************

 Description :converts the character type to an integer
 input: a character
 output:NULL
 return:success integer
         false FUNC_ERR

 ************************************************************************************************/
INT32 char_trans_int(char ch);

/*************************************************************************************************

 Description :get cpu server and BMC asset info
 input: NULL
 output: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 get_cpu_server_asset_info();

/**********************************************************************************************

 Description :get cpu server healthy info
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ************************************************************************************************/
INT32 get_cpu_server_heal_info();
#pragma pack()
#endif /* MNT_MGMT_MSG_HANDLE_H_ */
