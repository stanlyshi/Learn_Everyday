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
 *File name: rrus_mgmt_ir.h
 *Description: the interface of ir.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.04.18
 *
 ************************************************************************/


#ifndef SBS_INTERFACE_RRUAGENT_IR_H
#define SBS_INTERFACE_RRUAGENT_IR_H

#include "sbs_type.h"


#pragma pack(1)

/* Ir接口消息编号 */
typedef enum
{
	MSG_CODE_RRU_BBU_CHANNEL_BUILD_REQ          = 1,
	MSG_CODE_BBU_RRU_CHANNEL_BUILD_CFG          = 2,
	MSG_CODE_RRU_BBU_CHANNEL_BUILD_CFG_RSP      = 3,
	MSG_CODE_RRU_BBU_VER_UPDATE_RESULT_IND      = 4,
	MSG_CODE_BBU_RRU_VER_UPDATE_RESULT_RSP      = 5,

	MSG_CODE_BBU_RRU_VER_QUERY_REQ              = 11,
	MSG_CODE_RRU_BBU_VER_QUERY_RSP              = 12,

	MSG_CODE_BBU_RRU_VER_DOWNLOAD_REQ           = 21,
	MSG_CODE_RRU_BBU_VER_DOWNLOAD_RSP           = 22,
	MSG_CODE_RRU_BBU_VER_DOWNLOAD_RESULT_IND    = 23,

	MSG_CODE_BBU_RRU_VER_ACTIVATE_IND           = 31,
	MSG_CODE_RRU_BBU_VER_ACTIVATE_RSP           = 32,

	MSG_CODE_BBU_RRU_STATUS_QUERY_REQ           = 41,
	MSG_CODE_RRU_BBU_STATUS_QUERY_RSP           = 42,

	MSG_CODE_BBU_RRU_PARAM_QUERY_REQ            = 51,
	MSG_CODE_RRU_BBU_PARAM_QUERY_RSP            = 52,

	MSG_CODE_BBU_RRU_PARAM_CFG_REQ              = 61,
	MSG_CODE_RRU_BBU_PARAM_CFG_RSP              = 62,

	MSG_CODE_RRU_BBU_INIT_CAL_RESULT_REPORT     = 71,

	MSG_CODE_BBU_RRU_CAL_PARAM_CFG_REQ          = 81,
	MSG_CODE_RRU_BBU_CAL_PARAM_CFG_RSP          = 82,

	MSG_CODE_BBU_RRU_ANT_CAL_SWITCH_CFG_REQ     = 83,
	MSG_CODE_RRU_BBU_ANT_CAL_SWITCH_CFG_RSP     = 84,

	MSG_CODE_RRU_BBU_ANT_CAL_REQ                = 85,
	MSG_CODE_BBU_RRU_ANT_CAL_IND                = 86,
	MSG_CODE_RRU_BBU_ANT_CAL_RSP                = 87,

	MSG_CODE_BBU_RRU_ANT_CAL_END_IND            = 88,
	MSG_CODE_RRU_BBU_ANT_CAL_END_RSP            = 89,

	MSG_CODE_BBU_RRU_ANT_CAL_RESULT_QUERY_REQ   = 91,
	MSG_CODE_RRU_BBU_ANT_CAL_RESULT_QUERY_RSP   = 92,

	MSG_CODE_BBU_RRU_TIME_DELAY_MEASURE_REQ     = 101,
	MSG_CODE_RRU_BBU_TIME_DELAY_MEASURE_RSP     = 102,
	MSG_CODE_BBU_RRU_TIME_DELAY_CFG_REQ         = 103,
	MSG_CODE_RRU_BBU_TIME_DELAY_CFG_RSP         = 104,

	MSG_CODE_RRU_BBU_ALARM_REPORT               = 111,

	MSG_CODE_BBU_RRU_ALARM_QUERY_REQ            = 121,
	MSG_CODE_RRU_BBU_ALARM_QUERY_RSP            = 122,

	MSG_CODE_BBU_RRU_LOG_UPLOAD_REQ             = 131,
	MSG_CODE_RRU_BBU_LOG_UPLOAD_RSP             = 132,
	MSG_CODE_RRU_BBU_LOG_UPLOAD_RESULT_IND      = 133,

	MSG_CODE_BBU_RRU_RESET_IND                  = 141,

	MSG_CODE_BBU_RRU_REMOTE_RESET_IND           = 151,

	MSG_CODE_BBU_RRU_RING_TEST_REQ              = 161,
	MSG_CODE_RRU_BBU_RING_TEST_RSP              = 162,

	MSG_CODE_RRU_BBU_HB                         = 171,
	MSG_CODE_BBU_RRU_HB                         = 181,

	MSG_CODE_BBU_RRU_TDSCDMA_CELL_CFG_REQ       = 191,
	MSG_CODE_RRU_BBU_TDSCDMA_CELL_CFG_RSP       = 192,

	MSG_CODE_BBU_RRU_TDLTE_CELL_CFG_REQ         = 193,
	MSG_CODE_RRU_BBU_TDLTE_CELL_CFG_RSP         = 194,

	MSG_CODE_RRU_BBU_PERIOD_CAL_RESULT_REPORT   = 201,

	MSG_CODE_BBU_RRU_SINGLE_STEP_ANT_CAL_REQ    = 211,
	MSG_CODE_RRU_BBU_SINGLE_STEP_ANT_CAL_RSP    = 212,

	MSG_CODE_BBU_RRU_AC_RESULT_REQ              = 1300, /* AC校准结果查询 */
	MSG_CODE_RRU_BBU_AC_RESULT_RSP              = 1301, /* AC校准结果查询响应 */

	MSG_CODE_BBU_RRU_UP_POWER_REQ               = 1310, /* 功率查询 */
	MSG_CODE_RRU_BBU_UP_POWER_RSP               = 1311, /* 功率查询响应 */

	MSG_CODE_RRU_BBU_TANSPARENT_MSG_BEGIN                   = 221,

	MSG_CODE_RRU_BBU_TANSPARENT_MSG_EANT_CFG_RSP            = 221,  /* 221 电调天线配置响应 */
	MSG_CODE_RRU_BBU_TANSPARENT_MSG_STATIC_POWER_CFG_RSP    = 222,  /* 222 静态功率等级配置响应 */
	MSG_CODE_RRU_BBU_TANSPARENT_MSG_END                     = 230,

	MSG_CODE_BBU_RRU_TANSPARENT_MSG_BEGIN                   = 231,
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_EANT_CFG_REQ            = 231,  //231 电调天线配置
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_STATIC_POWER_CFG_REQ    = 232,  //232 静态功率等级配置
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_END                     = 240,

    MSG_CODE_RRU_BBU_RRU_IP_REQ                             = 500,
}IR_MSG_CODE;


/* IE Flag定义 */
typedef enum
{
	IE_FLAG_RRU_PRODUCT_ID                 = 1,
	IE_FLAG_CHANNEL_BUILD_REASON           = 2,
	IE_FLAG_RRU_ABILITY                    = 3,
	IE_FLAG_HW_TYPE_VER                    = 5,
	IE_FLAG_SW_VER                         = 6,

	IE_FLAG_SYS_TIME                       = 11,
	IE_FLAG_ACCESS_RSP_ADDR                = 12,
	IE_FLAG_RRU_OPER_MODE_SET              = 13,
	IE_FLAG_SW_VER_REQ                     = 14,

	IE_FLAG_CHANNEL_BUILD_CFG_RSP          = 21,
	IE_FLAG_VER_UPDATE_RESULT              = 31,

	IE_FLAG_VER_DOWNLOAD_RSP               = 101,
	IE_FLAG_RRU_VER_ACTIVATE_RSP           = 201,

	IE_FLAG_RF_PATH_STATE_QUERY_REQ        = 302,
	IE_FLAG_CLOCK_STATE_QUERY_REQ          = 305,

	IE_FLAG_RF_PATH_STATE                  = 352,
	IE_FLAG_CLOCK_STATE_QUERY_RSP          = 355,

	IE_FLAG_SYS_TIME_QUERY_REQ             = 401,
	IE_FLAG_CPU_USAGE_QUERY_REQ            = 402,
	IE_FLAG_CPU_USAGE_PERIOD_QUERY_REQ     = 403,
 	IE_FLAG_RRU_TEMPERATUR_QUERY_REQ       = 404,
	IE_FLAG_VSWR_STATE_QUERY_REQ           = 405,
	IE_FLAG_VSWR_THRESHOLD_QUERY_REQ       = 406,
	IE_FLAG_TEMP_THRESHOLD_QUERY_REQ       = 407,
	IE_FLAG_OUTPUT_POWER_QUERY_REQ         = 408,
	IE_FLAG_STATE_MACHINE_QUERY_REQ        = 409,

	IE_FLAG_CPU_USAGE_QUERY_RSP            = 451,
	IE_FLAG_CPU_USAGE_PERIOD_QUERY_RSP     = 452,
	IE_FLAG_RRU_TEMPERATUR_QUERY_RSP       = 453,
	IE_FLAG_VSWR_STATE_QUERY_RSP           = 454,
	IE_FLAG_VSWR_THRESHOLD_QUERY_RSP       = 455,
	IE_FLAG_TEMP_THRESHOLD_QUERY_RSP       = 456,
	IE_FLAG_OUTPUT_POWER_QUERY_RSP         = 457,
	IE_FLAG_STATE_MACHINE_QUERY_RSP        = 458,

	IE_FLAG_CPU_USAGE_PERIOD_CFG           = 502,
	IE_FLAG_VSWR_THRESHOLD_CFG             = 503,
	IE_FLAG_TEMP_THRESHOLD_CFG             = 505,
	IE_FLAG_ANT_CFG                        = 507,

	IE_FLAG_SYS_TIME_CFG_RSP               = 551,
	IE_FLAG_CPU_USAGE_PERIOD_CFG_RSP       = 553,
	IE_FLAG_VSWR_THRESHOLD_CFG_RSP         = 554,
	IE_FLAG_TEMP_THRESHOLD_CFG_RSP         = 556,
	IE_FLAG_RF_PATH_STATE_CFG_RSP          = 557,
	IE_FLAG_ANT_CFG_RSP                    = 559,

	IE_FLAG_RX_PATH_CAL_RESULT             = 601,
	IE_FLAG_TX_PATH_CAL_RESULT             = 602,

	IE_FLAG_TIME_DELAY_MEAS_REQ            = 901,
	IE_FLAG_TIME_DELAY_MEAS_RSP            = 911,

	IE_FLAG_TIME_DELAY_CFG_REQ             = 921,
	IE_FLAG_TIME_DELAY_CFG_RSP             = 931,

	IE_FLAG_ALARM_REPORT                   = 1001,
	IE_FLAG_ALARM_QUERY_REQ                = 1101,

	IE_FLAG_RESET_IND                      = 1301,

	IE_FLAG_RING_TEST_REQ                  = 1401,
	IE_FLAG_RING_TEST_RSP                  = 1411,

	IE_FLAG_CELL_CFG_REQ                   = 1501,
	IE_FLAG_FREQ_CFG_REQ                   = 1502,

	IE_FLAG_CELL_CFG_RSP                   = 1511,
	IE_FLAG_FREQ_CFG_RSP                   = 1512,

}IR_IE_FLAG;

typedef struct
{
    UINT32  msg_code;
    UINT32  msg_len;
    UINT32  rru_id;
    UINT32  bbu_id;
    UINT8   fiber_port;
    UINT32  stream_no;
}IrMsgHead;

typedef struct
{
	UINT16  ie_flag;
	UINT16  ie_len;
}IEHead;

/*
消息编号	        消息名称	                消息标识	                            消息方向
1	            通道建立请求	                RRU_BBU_CHANNEL_BUILD_REQ	        RRUBBU
2	            通道建立配置	                BBU_RRU_CHANNEL_BUILD_ CFG 	        BBURRU
3	            通道建立配置应答	            RRU_BBU_CHANNEL_BUILD_CFG_RSP	    RRUBBU
4	            版本更新结果指示	            RRU_BBU_VER_UPDATE_RESULT_IND	    RRUBBU
5	            版本更新结果指示应答	            BBU_RRU_VER_UPDATE_RESULT_RSP	    BBURRU
11	            RRU版本查询请求	            BBU_RRU_VER_QUERY_REQ	            BBURRU
12	            RRU版本查询应答	            RRU_BBU_VER_QUERY_RSP	            RRUBBU
21	            版本下载请求	                BBU_RRU_VER_DOWNLOAD_REQ	        BBURRU
22	            版本下载应答	                RRU_BBU_VER_DOWNLOAD_RSP	        RRUBBU
23	            版本下载结果指示	            RRU_BBU_VER_DOWNLOAD_RESULT_IND	    RRUBBU
31	            RRU版本激活指示	            BBU_RRU_VER_ACTIVATE_IND	        BBURRU
32	            RRU版本激活应答	            RRU_BBU_VER_ACTIVATE_RSP	        RRUBBU
41	            RRU状态查询	                BBU_RRU_STATUS_QUERY_REQ	        BBURRU
42	            RRU状态查询响应	            RRU_BBU_STATUS_QUERY_RSP	        RRUBBU
51	            RRU 参数查询	            BBU_RRU_PARAM_QUERY_REQ	            BBURRU
52	            RRU 参数查询响应	            RRU_BBU_PARAM_QUERY_RSP	            RRUBBU
61	            RRU 参数配置	            BBU_RRU_PARAM_CFG_REQ	            BBURRU
62	            RRU 参数配置响应	            RRU_BBU_PARAM_CFG_RSP	            RRUBBU
71	            初始化校准结果上报	            RRU_BBU_INIT_CAL_RESULT_REPORT	    RRUBBU
101	            时延测量请求	                BBU_RRU_TIME_DELAY_MEASURE_REQ	    BBURRU
102	            时延测量响应	                RRU_BBU_TIME_DELAY_MEASURE_RSP	    RRUBBU
103	            时延配置命令	                BBU_RRU_TIME_DELAY_CFG_REQ	        BBURRU
104	            时延配置命令响应	            RRU_BBU_TIME_DELAY_CFG_RSP	        RRUBBU
111	            告警上报请求	                RRU_BBU_ALARM_REPORT	            RRUBBU
121	            告警查询请求	                BBU_RRU_ALARM_QUERY_REQ	            BBURRU
122	            告警查询应答	                RRU_BBU_ALARM_QUERY_RSP	            RRUBBU
141	            复位指示	                BBU_RRU_RESET_IND	                BBURRU	
161	            回环请求	                BBU_RRU_RING_TEST_REQ	            BBURRU
162	            回环请求应答	                RRU_BBU_RING_TEST_RSP	            RRUBBU
171	            RRU在位心跳消息	            RRU_BBU_HB	                        RRUBBU
181	            BBU在位心跳消息	            BBU_RRU_HB	                        BBURRU
193	            小区配置	                BBU_RRU_CELL_CFG_REQ	            BBURRU
194	            小区配置响应	                RRU_BBU_CELL_CFG_RSP	            RRUBBU
*/

/*
 * RRU's IP Msg
 * RRU IP地址请求消息     RRU_BBU_RRU_IP_REQ          RRU  BBU
 * RRU IP地址分配消息     BBU_RRU_RRU_IP_RSP          BBU  RRU  
 */

/* 
功能：
RRU上电后通过UDP广播消息向BBU发送的请求获取IP地址的消息。
该消息在收到响应消息之前需要周期性发送，时间间隔默认为5秒钟，
如果n次后仍未收到BBU的响应消息，则RRU复位。目前是一直循环发送。
方向
RRU  BBU
消息头
UDP广播协议头
消息净荷
参数	类型	含义
RruId	    Uint32	RRU设备编号，取值范围为1～165。
RruMac[6]	Uint8	RRU MAC地址（注1）
注1：RRUMAC地址由RRU厂商按照自己规则自己定义，确保不冲突即可。
*/

typedef struct
{
    UINT32      rru_id;
    UINT8       rru_mac[6];
}RRU_BBU_RRU_IP_REQ;

/*
功能：
BBU收到RRU IP地址请求消息后，给RRU分配IP地址，并且通过UDP广播发送响应消息到RRU。
方向
BBU RRU
消息头
UDP广播协议头
消息净荷
参数	类型	含义
RruId	    Uint32	逻辑ID
BbuId	    Uint32	逻辑ID
RruMac[6]	Uint8	RRU的MAC地址
RruIp[4]	Uint8	分配给RRU的IP地址
BbuIp[4]	Uint8	BBU的IP地址
subMask[4]	Uint8	RRU的子网掩码
*/

typedef struct
{
    UINT32      rru_id;
    UINT32      bbu_id;
    UINT8       rru_mac[6];
    UINT8       rru_ip[4];
    UINT8       bbu_ip[4];
    UINT8       sub_mask[4];     // rru's submask.
}BBU_RRU_RRU_IP_RSP;


/* 
 * application layer's msg
 */





/* 
 * RRU_BBU_CHANNEL_BUILD_REQ
 */
/*
功能：
RRU获取IP地址后，初始化并且建立TCP连接，通过TCP协议发送给BBU通道建立请求消息。
如果RRU在15 （IR接口规定是5s）秒内没有收到BBU发送的通道建立配置响应消息，
RRU则以15秒为周期不断向BBU发送通道建立请求消息，直到BBU发送的通道建立配置响应消息。
该消息包含：
RRU产标示IE、通道建立原因IE、RRU能力IE、RRU级数IE、RRU硬件类型及版本信息IE、RRU软件版本信息IE
名称：RRU_BBU_CHANNEL_BUILD_REQ
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷
*/
/*
RRU产品标识IE
参数  类型  含义
IeFlag              Uint16  IE标识-值是1
IeLen               Uint16  IE长度
Manufacturer[16]    Int8    RRU制造商名称； 由制造商填写
Publisher[16]       Int8    RRU发行商名称； 由制造商填写
SerialNo[16]        Int8    产品序列号；  由制造商填写， 字符和数字组合
ManufactureDate[16] Int8    生产日期； 格式：yyyy-mm-dd
LastServiceDate[16] Int8    最近服务日期； 格式：yyyy-mm-dd
ExtraInfo[16]       Int8    附加信息；
*/

typedef struct
{
    UINT16      ie_flag;                    // value is 1.
    UINT16      ie_len;
    INT8        manufacturer[16];
    INT8        publisher[16];
    INT8        serial_no[16];
    INT8        manufacture_date[16];       // yyyy-mm-dd.
    INT8        last_service_date[16];      // yyyy-mm-dd.
    INT8        extra_info[16];
}IE_RRU_PRODUCT_INFO;



/*
通道建立原因IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是2
IeLen	    Uint16	IE长度
Reason	    Uint8	通道建立原因；
                    0：BBU侧发起的软件重启
                    1：RRU侧发起的软件重启
                    2：上电
                    其余保留
AlarmCode	Uint32	导致RRU侧发起软件重启的告警码 
*/

typedef enum
{
    RRU_ALARMCODE_BASE = 5900,
	RRU_ALM_FIBER_RECOVERED_CLOCK_UNLOCK,          	//5901
	RRU_ALM_ADC_OVERFLOW,                           //5902
	RRU_ALM_28V_POWER,                              //5903
	RRU_ALM_LOS,                                    //5904
	RRU_ALM_LOF,                                    //5905
	RRU_ALM_RAY_TRANSMITER,                         //5906
	RRU_ALM_RAY_RECEIVER,                           //5907
	RRU_ALM_TRXB_ABSENCE,                           //5908

	RRU_ALM_CASE_TEMP = 5916,                       //5916
//	RRU_ALM_TRXB_TEMP,                              //5917
	RRU_ALM_SPD_FAILURE = 5918,                     //5918
	RRU_ALM_RF_PLL_UNLOCK,                          //5919
	RRU_ALM_PA_TEMP,                                //5920
	RRU_ALM_PA_TEMP_OVER,                           //5921
	RRU_ALM_DPD,                                    //5922 
	RRU_BOARD_GENERAL_FAILURE_ALARM, 		        //5923
	RRU_ALM_ANT_VSWR,                               //5924
	RRU_ALM_EXPORT_POWER_UNDER,                   	//5925
	RRU_ALM_EXPORT_POWER_OVER,                      //5926
	RRU_ALM_CALIBRATE_CHANNEL,                      //5926

	RRU_ALM_UP_CHANNEL = 5932,						//5932
	RRU_ALM_DOWN_CHANNEL = 5935,					//5935
	RRU_ALM_TXANT_AMPLITUDE_PHASE = 5939,			//5939
	RRU_ALM_RXANT_AMPLITUDE_PHASE,					//5940
	RRU_ALM_FTPDOWN_SW_FILE_NULL,					//5941
	RRU_ALM_FTPDOWN_SW_TIMEOUT,						//5942
	RRU_ALM_FTPDOWN_SW_FILE_LARGER,					//5943
	RRU_ALM_POWNON_SELFCHECK,						//5944
	RRU_ALM_FAULT_RESET,							//5945
	RRU_ALM_CPU_OVERLOAD,							//5946
	RRU_ALM_APP_PROGRAM_RETRIEVE,					//5947
	RRU_ALM_FPGA_RETRIEVE,							//5948
	RRU_ALM_MEMORY_LESS,							//5949
	RRU_ALM_TASK_SUSPEND,							//5950
	RRU_ALM_TASK_EXCEPT,							//5951	
	RRU_ALM_RCU_FAILURE,
	RRU_ALM_PARTTAKE_BACKUP_FIBER =5955,
	RRU_ALM_IF_UNLOCK = 5958,
	RRU_ALM_END   
}ENUM_ALARM_CODE;

typedef struct
{
    UINT16              ie_flag;         // value is 2.
    UINT16              ie_len;  
    UINT8               reason;         // 0 is by BBU, 1 is by RRU, 2 is Power on, other reserve.
    ENUM_ALARM_CODE     alarm_code;
}IE_CHANNEL_BUILD_REASON;




/*
RRU能力IE
参数	类型	含义
IeFlag	            Uint16	IE标识-值是3
IeLen	            Uint16	IE长度
td_carrier_num      Uint32  支持的TD载波个数
lte_carrier_num     Uint32  支持的LTE载波个数
AntNum	            Uint8	支持的天线数 
MaxPower	        Uint16	通道最大发射功率； 单位：1/256dBm
MasterOrSlave	    Uint8	通信主从属性 0：主   1：从
MaxDwTimeDelay	    Uint32	RRU支持的下行时延补偿最大值.单位:ns
MaxUpTimeDelay	    Uint32	RRU支持的上行时延补偿最大值.单位:ns
support_mode        Uint16  支持的频段范围 Bit0: 支持TDD-LTEBit1：支持TD-SCDMA Bit2：支持FDD-LTE
ant_clbrt_flag      Uint8   RRU支持独立做天线校准 0：不支持 1：支持
main_send_eme_path  Uint8   主发校正通道
slave_send_eme_path Uint8   备发校正通道
main_recv_eme_path  Uint8   主收校正通道
slave_recv_eme_path Uint8   备收校正通道
*/

typedef struct
{
    UINT16      ie_flag;            // value is 3.
    UINT16      ie_len;
    UINT32      td_carrier_num;
    UINT32      lte_carrier_num;
    UINT8       ant_num;
    UINT16      max_power;
    UINT8       master_or_slave;    // 0 is master, 1 is slave.
    UINT32      max_dw_time_delay;  // the max downlink time delay fix supported by rru. /ns.
    UINT32      max_up_time_delay;  // the max uplink time delay fix supported by rru. /ns.
    UINT16      support_mode;       // 支持的频段范围 Bit0: 支持TDD-LTEBit1：支持TD-SCDMA Bit2：支持FDD-LTE
    UINT8       ant_clbrt_flag;     // RRU支持独立做天线校准 0：不支持 1：支持
    UINT8       main_send_eme_path; // 主发校正通道
    UINT8       slave_send_eme_path;// 备发校正通道
    UINT8       main_recv_eme_path; // 主收校正通道
    UINT8       slave_recv_eme_path;// 备收校正通道
}IE_RRU_ABILITY;

/*
RRU硬件类型及版本信息IE
参数	类型	含义
IeFlag	        Uint16	IE标识-值是5
IeLen	        Uint16	IE长度
HwType[32]	    Int8	RRU硬件类型；
                        1到16字节：厂家标识编号（后面补空格）（注1）
                        17到18字节：通道数
                        19到20字节：载波数
                        21到22字节：最大发射功率
                        23到32字节：其余厂家自定义
HwVersion[16]	Int8	RRU硬件版本号
注1：RRU厂商自己定义，需要以可打印的字符串作为标示，长度不足16字节时，后面补零。
*/

typedef struct
{
    UINT16      ie_flag;         // value is 5.
    UINT16      ie_len;
    UINT8       hw_type[32];     // RRU hardware type:
    INT8        hw_version[16];
}IE_RRU_HARD_TYPE_VER_INFO;



/*
RRU软件版本信息IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是6
IeLen	    Uint16	IE长度
SwVer [40]	Int8	RRU软件版本号（注1）
FwVer [40]	Int8	RRU固件版本号（注2）
注1：厂商标示编号+SW+版本号，长度不足时，后面补零。
注2：厂商标示编号+FW+版本号，长度不足时，后面补零。
*/

typedef struct
{
    UINT16      ie_flag;         // value is 6.
    UINT16      ie_len;
    INT8        sw_ver[40];
    INT8        fw_ver[40];
}IE_RRU_SOFT_VER_INFO;




typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_RRU_PRODUCT_INFO         ie_rru_product_info;
    IE_CHANNEL_BUILD_REASON     ie_channel_build_reason;
    IE_RRU_ABILITY              ie_rru_ability;
    IE_RRU_HARD_TYPE_VER_INFO   ie_rru_hard_type_ver_info;
    IE_RRU_SOFT_VER_INFO        ie_rru_soft_ver_info;
}MSG_RRU_BBU_CHANNEL_BUILD_REQ;





/*
 * BBU_RRU_CHANNEL_BUILD_ CFG
 */

/*
通道建立配置
功能：BBU收到RRU的通道建立请求消息后，由BBU发送通道建立配置消息给RRU，
该消息包含系统时间IE、接入应答地址IE、RRU操作模式设置IE、软件版本核对结果IE。
BBU如果发现RRU在“通道建立请求”消息中上报的软件版本与BBU要求的不一致，
则会在“软件版本核对结果”IE中通知RRU，并启动定时器等待RRU的“版本更新结果指示”消息。
名称：BBU_RRU_CHANNEL_BUILD_CFG
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷
*/

/*
系统时间IE
参数	类型	含义
IeFlag	Uint16	IE标志-值11
IeLen	Uint16	IE长度
Second	Uint8	秒 取值范围：0-59
Minute	Uint8	分 取值范围：0-59
Hour	Uint8	时 取值范围：0-23
Day	    Uint8	日 取值范围：1-31
Month	Uint8	月 取值范围：1-12
Year	Uint16	年 取值范围：1997-2099
*/

typedef struct
{
    UINT16      ie_flag;     // value is 11.
    UINT16      ie_len;
    UINT8       second;     // 0-59s.
    UINT8       minute;     // 0-59m.
    UINT8       hour;       // 0-23h.
    UINT8       day;        // 1-31d.
    UINT8       month;      // 1-12M.
    UINT16      year;       // 1997-2099y.
}IE_SYSTEM_TIME;

/*
接入应答地址IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值12
IeLen	    Uint16	IE长度
BbuFtpIp[4]	Uint8	BBU侧FTP服务器IP地址
*/

typedef struct
{
    UINT16      ie_flag;     // value is 12.
    UINT16      ie_len;
    UINT8       bbu_ftp_ip[4];
}IE_ACCESS_RESP_ADDRESS;

/*
RRU操作模式设置IE
参数	类型	含义
IeFlag	    Uint16	IE标识-13
IeLen	    Uint16	IE长度
RunMode 	Uint32	状态值 
                    0：RRU正常运行
                    1：测试状态（注1）
注1：目前只考虑正常模式，不考虑测试模式，预留扩展使用。
*/

typedef enum
{
    RRU_MODE_NORMAL,
    RRU_MODE_TEST,
    RRU_MODE_OTHER
}ENUM_RRU_RUNMODE;

typedef struct
{
    UINT16              ie_flag;     // value is 13.
    UINT16              ie_len;
    ENUM_RRU_RUNMODE    run_mode;
}IE_RRU_RUN_MODE_SET;



/*
软件版本核对结果IE
参数	类型	含义
IeFlag	        Uint16	IE标识-值14
IeLen	        Uint16	IE长度
SwVerType 	    Uint8	软件版本类型
                        0：软件
                        1：固件
CheckResult	    Uint32	返回结果
                        0：版本一致
                        1：版本不一致（RRU要发起软件更新请求）
FilePath[200]	Int8	文件路径
FileName[16]	Int8	文件名
FileLen	        Uint32	文件长度
FileTime[20]	Int8	文件时间信息
                        格式：yyyy-mm-dd hh:mm:ss
FileVer[40]	    Int8	文件版本
*/

typedef enum
{
    RRU_SOFT_VERSION_SAME,
    RRU_SOFT_VERSION_NOT_SAME,
    RRU_SOFT_VERSION_OTHER
}ENUM_RESULT_RRU_SOFT_CHECK;
    
typedef struct
{
    UINT16                  	ie_flag;     // value is 14.
    UINT16                  	ie_len;
    UINT8                   	sw_ver_type;
    ENUM_RESULT_RRU_SOFT_CHECK  check_result;
    INT8                    	file_path[200];
    INT8                    	file_name[16];
    UINT32                  	file_len;
    INT8                    	file_time[20];
    INT8                    	file_ver[40];
}IE_SOFT_VER_CHECK_RESULT;



typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_SYSTEM_TIME              ie_system_time;
    IE_ACCESS_RESP_ADDRESS      ie_access_resp_address;
    IE_RRU_RUN_MODE_SET         ie_rru_run_mode_set;
    IE_SOFT_VER_CHECK_RESULT    ie_soft_ver_check_result;
}MSG_BBU_RRU_CHANNEL_BUILD_CFG;





/*
 * RRU_BBU_CHANNEL_BUILD_CFG_RSP
 */

/*
通道建立配置应答
功能：RRU向BBU发送发送通道建立配置应答消息，通知BBU通道是否建立成功。
      RRU发出该消息后，开始周期性发出“RRU在位心跳消息”；定时启动启动定时器等待“BBU的在位心跳消息”。
名称：RRU_BBU_CHANNEL_BUILD_CFG_RSP
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷
参数  类型  含义
IeFlag      Uint16  IE标识-值21
IeLen       Uint16  IE长度
CfgResult   Uint32  返回结果
                    0：成功
                    1：失败
 */
 
typedef enum
{
    CHANNEL_BUILD_CFG_SUCCEED,
    CHANNEL_BUILD_CFG_FAILED,
    CHANNEL_BUILD_CFG_OTHER
}ENUM_RESULT_CHANNEL_BUILD_CFG;

typedef struct
{
    UINT16                          ie_flag;     // value is 21.
    UINT16                          ie_len;
    ENUM_RESULT_CHANNEL_BUILD_CFG   cfg_result;   
}IE_CHANNEL_BUILD_CFG_RET;

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_CHANNEL_BUILD_CFG_RET    ie_channel_build_cfg_ret;
}MSG_RRU_BBU_CHANNEL_BUILD_CFG_RSP;





/* 
 * RRU_BBU_VER_UPDATE_RESULT_IND
 */

/*
版本更新结果指示
功能：RRU收到BBU的通道建立配置消息，根据BBU侧的版本核对信息，准备软件、固件。
      如果需要更新RRU的软件、固件，RRU从Ftp下载新版本的软件固件，发送版本更新结果指示消息至BBU。
      FtpServer的IP可以从通道建立配置消息中接入应答地址IE中获得。
名称：RRU_BBU_VER_UPDATE_RESULT_IND
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷
参数	类型	含义
IeFlag	    Uint16	IE标识-值31
IeLen	    Uint16	IE长度
SwVerType	Uint8	软件类型
                    0：软件
                    1：固件
Result	    Uint32	返回结果
                    0：成功
                    1：文件不存在
                    2：超时
                    3：文件过大
                    4：其他原因
*/

typedef enum
{
    VER_UPDATE_SUCCEED,
    FILE_NOT_EXSIT,
    VER_UPDATE_TIMEOUT,
    FILE_TOO_LARGE,
    VER_UPDATE_OTHER
}ENUM_RESULT_VER_UPDATE;

typedef struct
{
    UINT16                      ie_flag;     // value is 31.
    UINT16                      ie_len;
    UINT8                       sw_ver_type;  // 0 is software, 1 is firmware.
    ENUM_RESULT_VER_UPDATE      update_result;
}IE_VER_UPDATE_RESULT_IND;

typedef struct
{   
    IrMsgHead                   ir_msg_head;
    IE_VER_UPDATE_RESULT_IND    ie_ver_update_result_ind;
}MSG_RRU_BBU_VER_UPDATE_RESULT_IND;





/*
 * BBU_RRU_VER_UPDATE_RESULT_RSP
 */

/*
版本更新结果指示应答
功能：BBU收到RRU的版本更新指示结果消息，发送消息内容为空的版本更新结果指示应答消息。
名称：BBU_RRU_VER_UPDATE_RESULT_RSP
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷
无
*/

typedef struct
{
    IrMsgHead       ir_msg_head;
}MSG_BBU_RRU_VER_UPDATE_RESULT_RSP;     // null body.





/*
 * BBU_RRU_VER_QUERY_REQ
 */

/*
RRU版本查询请求
功能：BBU向RRU发出RRU版本查询请求，要求RRU返回版本信息。包括RRU软件版本IE、RRU硬件版本类型及版本信息IE
名称：BBU_RRU_VER_QUERY_REQ
方向：BBU RRU
消息头：参见4.1.2TCP消息格式
消息净荷：
无。
*/

typedef struct
{
    IrMsgHead       ir_msg_head;
}MSG_BBU_RRU_VER_QUERY_REQ;             // null body.





/*
 * RRU_BBU_VER_QUERY_RSP
 */

/*
RRU版本查询应答
功能：BBU向RRU发出RRU版本查询请求，RRU返回版本信息。包括RRU软件版本IE、RRU硬件版本类型及版本信息IE
名称：BBU_RRU_VER_QUERY_RSP
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
RRU软件版本信息IE
参见RRU软件版本信息IE

RRU硬件类型及版本信息IE
参见RRU硬件类型及版本信息IE
*/

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_RRU_SOFT_VER_INFO        ie_rru_soft_ver_info;
    IE_RRU_HARD_TYPE_VER_INFO   ie_rru_hard_type_ver_info;  
}MSG_RRU_BBU_VER_QUERY_RSP;





/*
 * BBU_RRU_VER_DOWNLOAD_REQ
 */

/*
版本下载请求
功能：BBU向RRU发送软件更新请求消息,请求RRU更新软件。
名称：BBU_RRU_VER_DOWNLOAD_REQ
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷
软件版本核对结果IE
参见软件版本核对结果IE
*/

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_SOFT_VER_CHECK_RESULT    ie_soft_ver_check_result;
}MSG_BBU_RRU_VER_DOWNLOAD_REQ;





/*
 * RRU_BBU_VER_DOWNLOAD_RSP
 */

/*
版本下载应答
功能：BBU向RRU发出版本下载请求后，RRU比较更新的软件版本是否与本RRU系统中保留的版本相同，
      如相同则不更新，否则从指定FTP上下载软件。
名称：RRU_BBU_VER_DOWNLOAD_RSP
方向：RRU BBU
消息头：参见4.1.2TCP消息格式    
消息净荷：

参数	类型	含义
IeFlag	    Uint16	IE标识-值是101
IeLen	    Uint16	IE长度
SwVerType	Uint8	软件版本类型
                    0：软件
                    1：固件
Result	    Uint32	返回结果
                    0：可以下载
                    其余：拒绝下载的原因
*/

typedef enum
{
    VER_ALLOW_DOWNLOAD,
    VER_REJECT_DOWNLOAD_OTHER
}ENUM_RESULT_VER_DOWNLOAD_RSP;

typedef struct
{
    UINT16                      	ie_flag;     // value is 101.
    UINT16                      	ie_len;
    UINT8                       	sw_ver_type;
    ENUM_RESULT_VER_DOWNLOAD_RSP    download_result;
}IE_VER_DOWNLOAD_RSP;

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_VER_DOWNLOAD_RSP         ie_ver_download_rsp;
}MSG_RRU_BBU_VER_DOWNLOAD_RSP;





/*
 * RRU_BBU_VER_DOWNLOAD_RESULT_IND
 */

/*
版本下载结果指示
功能：该消息用于向BBU上报软件更新结果。当RRU从FTP下载完要更新的软件后，RRU内部进行软件更新操作，
      更新完成后向BBU发送该消息；或当RRU下载任何一个要更新的软件失败后，停止下载其它软件，并发送该消息；
      或当RRU内部在进行任何一个软件更新操作时失败，则停止更新，并发送该消息。如果软件更新失败，
      必须恢复到更新前的版本。包含版本下载传输完成指示IE。
名称：RRU_BBU_VER_DOWNLOAD_RESULT_IND
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	    Uint16	IE标识-值是31
IeLen	    Uint16	IE长度
SwVerType	Uint8	软件版本类型
                    0：软件
                    1：固件
Result	    Uint32	返回结果
                    0：下载完成
                    1：文件不存在
                    2：超时
                    3：文件过大
*/

typedef enum
{
    VER_DOWNLOAD_FINISH,
    VER_FILE_NOT_EXIST,
    VER_DOWNLOAD_TIMEOUT,
    VER_FILE_TOO_LARGE,
    VER_DOWNLOAD_OTHER
}ENUM_RESULT_VER_DOWNLOAD;

typedef struct
{
    UINT16                      ie_flag;     //value is 31.
    UINT16                      ie_len;
    UINT8                       sw_ver_type;
    ENUM_RESULT_VER_DOWNLOAD    download_result;
}IE_VER_DOWNLOAD_RESULT_IND;

typedef struct
{   
    IrMsgHead                       ir_msg_head;
    IE_VER_DOWNLOAD_RESULT_IND      ie_ver_download_result_ind;
}MSG_RRU_BBU_VER_DOWNLOAD_RESULT_IND;





/*
 * BBU_RRU_VER_ACTIVATE_IND
 */

/*
RRU版本激活指示
功能：BBU向RRU发送版本激活指示消息，要求RRU激活指定的软件版本。包含RRU软件版本信息IE
名称：BBU_RRU_VER_ACTIVATE_IND
方向：BBU RRU
消息头：参见4.1.2TCP消息格式
消息净荷：
RRU软件版本信息IE
参见RRU软件版本信息IE
*/

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_RRU_SOFT_VER_INFO        ie_rru_soft_ver_info;
}MSG_BBU_RRU_VER_ACTIVATE_IND;





/*
 * RRU_BBU_VER_ACTIVATE_RSP
 */

/*
RRU版本激活应答
功能：BBU向RRU发送软件软件激活指示消息，要求RRU激活指定的软件版本。RRU返回版本激活结果。（注1）
名称：RRU_BBU_VER_ACTIVATE_RSP
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
注1：取决于实现，建议重启。
*/

/*
RRU软件版本信息IE

参数	类型	含义
IeFlag	    Uint16	IE标识-值是201
IeLen	    Uint16	IE长度
SwVerType	Uint8	软件版本类型
                    0：软件
                    1：固件
Result	    Uint32	返回结果
                    0：成功
                    1：要激活的版本不存在
                    2：文件被破坏
                    3：软件版本与硬件版本不匹配
                    4：激活不成功
                    5：其他原因
*/

typedef enum
{
    VER_ACTIVATE_SUCCEED,
    VER_ACTIVATE_NOT_EXIST,
    VER_ACTIVATE_FILE_BROKEN,
    VER_ACTIVATE_SOFT_HARD_MISMATCH,
    VER_ACTIVATE_FAILED,
    VER_ACTIVATE_OTHER
}ENUM_RESULT_VER_ACTIVATE;

typedef struct
{
    UINT16                  	ie_flag;         // value is 201.
    UINT16                  	ie_len;
    UINT8                   	sw_ver_type;
    ENUM_RESULT_VER_ACTIVATE    activate_result;
}IE_VER_ACTIVATE_RSP;

typedef struct
{
    IrMsgHead               ir_msg_head;
    IE_VER_ACTIVATE_RSP     ie_ver_activate_rsp;
}MSG_RRU_BBU_VER_ACTIVATE_RSP;




/*
 * BBU_RRU_STATUS_QUERY_REQ
 */

/*
RRU状态查询
功能：BBU通过该消息获取RRU相关状态。根据查询的状态参数的不同填充不同的IE。
名称：BBU_RRU_STATUS_QUERY_REQ
方向：BBU RRU 
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
射频通道状态查询IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是302
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道号
                    1-8代表8个射频通道
*/

typedef struct
{
    UINT16          ie_flag;         // value is 302.
    UINT16          ie_len;
    UINT8           rf_path_no;
}IE_RF_PATH_STATE_QUERY;


/*
时钟状态IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是305
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16          ie_flag;
    UINT16          ie_len;          // value is 305.
}IE_SYSTEM_TIME_STATE;

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_RF_PATH_STATE_QUERY      ie_rf_path_state_query;
    IE_SYSTEM_TIME_STATE        ie_system_time_state;   
}MSG_BBU_RRU_STATUS_QUERY_REQ;





/*
 * RRU_BBU_STATUS_QUERY_RSP
 */

/*
RRU状态查询响应
功能：BBU通过该消息获取RRU当前可用状态。
名称：RRU_BBU_STATUS_QUERY_RSP
方向：RRU BBU 
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
射频通道状态查询响应IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是352
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道号
                    1-8代表8个射频通道
up_State	    Uint32	上行状态
                    0：未使能
                    1：使能，无故障
                    2：使能，有故障
dw_State	    Uint32	下行状态
                    0：未使能
                    1：使能，无故障
                    2：使能，有故障
*/

typedef enum
{
    RF_PATH_ENABLE,
    RF_PATH_ABLE_NO_ERROR,
    RF_PATH_ABLE_ERROR,
    RF_PATH_OTHER
}ENUM_RF_PATH_STATE;

typedef struct
{
    UINT16                  ie_flag;     // value is 352.
    UINT16                  ie_len;
    UINT8                   rf_path_no;
    ENUM_RF_PATH_STATE      up_path_state;
    ENUM_RF_PATH_STATE      dw_path_state;
}IE_RF_PATH_STATE_QUERY_RSP;



/*
时钟状态响应IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是355
IeLen	Uint16	IE长度
State	Uint32	状态
                0：同步
                1：失步
*/

typedef enum
{
    SYSTEM_CLOCK_SYNC,
    SYSTEM_CLOCK_LOSE_SYNC,
    SYSTEM_CLOCK_OTHER
}ENUM_SYSTEM_CLOCK_STATE;

typedef struct
{
    UINT16                      ie_flag;     // value is 355.
    UINT16                      ie_len;
    ENUM_SYSTEM_CLOCK_STATE     state;
}IE_SYSTEM_CLOCK_STATE_RSP;



typedef struct
{
    IrMsgHead                           ir_msg_head;
    IE_RF_PATH_STATE_QUERY_RSP          ie_rf_path_state_query_rsp;
    IE_SYSTEM_CLOCK_STATE_RSP           ie_system_clock_state_rsp;  
}MSG_RRU_BBU_STATUS_QUERY_RSP;





/*
 * BBU_RRU_RING_TEST_REQ
 */

/*
回环请求
功能：BBU向RRU发出链路环回测试请求，要求RRU进行链路环回测试
名称：BBU_RRU_RING_TEST_REQ
方向BBU RRU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	        Uint16	IE标识-值是1401
IeLen	        Uint16	IE长度
RingTestType	Uint32	回环类型
                        0：其他
                        1：下一级RRU回环
CheckPeriod	    Uint16	检测周期定时器 单位：ms
PortNo	        Uint8	光纤端口号
*/

typedef enum
{
    RING_TEST_OTHER,
    RING_TEST_NEXT_RRU,
    RING_TEST_MAX
}ENUM_RING_TEST_TYPE;

typedef struct
{
    UINT16                  ie_flag;            // value is 1401.
    UINT16                  ie_len;
    ENUM_RING_TEST_TYPE     ring_test_type;
    UINT16                  check_period;
    UINT8                   port_no;
}IE_RING_TEST_REQ;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_RING_TEST_REQ    ie_ring_test_req;
}MSG_BBU_RRU_RING_TEST_REQ;





/*
 * RRU_BBU_RING_TEST_RSP
 */

/*
回环请求应答
功能：RRU收到链路环回测试请求后，进行到下一级RRU之间链路环回测试，并将测试结果通过该消息响应给BBU。
名称：RRU_BBU_RING_TEST_RSP
方向RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	        Uint16	IE标识-值是1411
IeLen	        Uint16	IE长度
RingTestType	Uint32	回环类型
                        0：其他
                        1：下一级RRU回环
PortNo	        Uint8	光纤端口号
Result	        Uint32	返回结果
                        0：成功
                        1：失败
*/

typedef enum
{
    RING_TEST_SUCCESS,
    RING_TEST_FAILED,
    RING_TEST_RESULT_OTHER
}ENUM_RESULT_RINT_TEST;

typedef struct
{
    UINT16              	ie_flag;             // value is 1411.
    UINT16              	ie_len;
    ENUM_RING_TEST_TYPE     ring_test_type;
    UINT8               	port_no;
    ENUM_RESULT_RINT_TEST   ring_test_result;
}IE_RING_TEST_RSP;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_RING_TEST_RSP    ie_ring_test_rsp;
}MSG_RRU_BBU_RING_TEST_RSP;





/*
 * BBU_RRU_RESET_IND
 */

/*
复位指示
功能：BBU向RRU发出板卡复位请求消息
名称：BBU_RRU_RESET_IND
方向：BBU RRU
消息头：参见4.1.2TCP消息格式    
消息净荷：
参数	类型	含义
IeFlag	    Uint16	IE标识-值是1301
IeLen	    Uint16	IE长度
ResetType	Uint32	复位类型
                    0：不影响下一级的复位
                    1：影响下一级的复位
*/

typedef enum
{
    RRU_RESET_SELF,
    RRU_RESET_AND_NEXT,
    RRU_RESET_OTHER
}ENUM_RRU_RESET_TYPE;

typedef struct
{
    UINT16                  ie_flag;             // value is 1301.
    UINT16                  ie_len;  
    ENUM_RRU_RESET_TYPE     reset_type;
}IE_RESET_REQ;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_RESET_REQ        ie_reset_req;
}MSG_BBU_RRU_RESET_IND;





/*
 * BBU_RRU_CELL_CFG_REQ
 */

/*
小区配置
功能：BBU发送小区配置消息至RRU。包括小区建立和小区删除消息，小区重配（未做）。
      小区建立消息包括小区配置IE和频点配置IE；小区删除消息只包括小区配置IE
名称：BBU_RRU_CELL_CFG_REQ
方向：BBURRU
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
小区配置IE
参数  类型  含义
IeFlag                  Uint16  IE标识-值是1501
IeLen                   Uint16  IE长度
CellCfgCmd              Uint8   小区配置标识
                                0：建立
                                1：重配
                                2：删除
LcId                    Uint32  本地小区标识
SingleCarrierCellPower  Uint16  单载波小区功率 单位：1/256 dbm
AntSetNo                Uint8   天线组号 1-8
FreqIeNum               Uint8   频点数 指当前消息中包含的频点配置IE的数目
*/

typedef struct
{
    UINT16              ie_flag;             // value is 1501.
    UINT16              ie_len;
    UINT8               cell_cfg_cmd;
    UINT32              local_cell_id;
    UINT16              single_carrier_cell_power;
    UINT8               ant_set_no;
    UINT8               freq_ie_num;
}IE_CELL_CFG;



/*
频点配置IE
参数	类型	含义
IeFlag	            Uint16	IE标识-值是1502
IeLen	            Uint16	IE长度
FreqCfgCmd	        Uint8	频点配置标识
                            0：建立
                            1：删除
LcId	            Uint32	本地小区标识
CarrierNo	        Uint8	载波号
MidFreq	            Uint32	中心频率
                            单位：100kHz
FreqAttribute	    Uint32	频点的主辅特性
                            0：主频点
                            1：辅频点
TsInfo	            Uint8	时隙转换点0-6
SpecialSubframeCfg	Uint8	特殊时隙配置0-8(注1：)
CarrierWidth	    Uint32	载波带宽：值为：5, 10, 20
CircLenCh	        Uint8	循环前缀长度选择 常规CP-0 扩展CP-1
注1：和协议含义相同，参见3GPP 36.211-4.2
*/

typedef enum
{
    FREQATTR_MASTER,
    FREQATTR_SLAVE,
    FREQATTR_OTHER
}ENUM_FREQ_ATTRIBUTE;

typedef struct
{
    UINT16                  ie_flag;             // value is 1502.
    UINT16                  ie_len;
    UINT8                   freq_cfg_cmd;
    UINT32                  local_cell_id;
    UINT8                   carrier_no;
    UINT32                  mid_freq;
    ENUM_FREQ_ATTRIBUTE     freq_attribute;
    UINT8                   ts_info;
    UINT8                   special_subframe_cfg;  // 0-8
    UINT32                  carrier_width;
    UINT8                   circ_len_ch;          // 循环前缀长度选择 常规CP-0 扩展CP-1
}IE_FREQ_CFG;



typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_CELL_CFG         ie_cell_cfg;
    IE_FREQ_CFG         ie_freq_cfg;
}MSG_BBU_RRU_CELL_CFG_REQ_BUILD;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_CELL_CFG         ie_cell_cfg;
}MSG_BBU_RRU_CELL_CFG_REQ_DELETE;






/*
 * RRU_BBU_CELL_CFG_RSP
 */

/*
小区配置响应
功能：RRU发送小区配置响应消息至BBU。包括小区建立和小区删除响应消息。
      小区建立响应消息包括小区配置响应IE和频点配置响应IE；小区删除响应消息只包括小区配置响应IE
名称：BBU_RRU_CELL_CFG_RSP
方向：RRUBBU
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
小区配置IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是1511
IeLen	Uint16	IE长度
LcId	Uint32	本地小区标识
Result	Uint32	返回结果
                0：成功（包含主载频）
                1：失败
*/

typedef enum
{
    CELL_CFG_SUCCESS,
    CELL_CFG_FAILED,
    CELL_CFG_OTHER
}ENUM_RESULT_CELL_CFG;

typedef struct
{
    UINT16                  ie_flag;             // value is 1511.
    UINT16                  ie_len;
    UINT32                  local_cell_id;
    ENUM_RESULT_CELL_CFG    result;
}IE_CELL_CFG_RSP;

/*
频点配置IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是1512
IeLen	    Uint16	IE长度
FreqCfgCmd	Uint8	频点配置标识
                    0：建立
                    1：删除
LcId	    Uint32	本地小区标识
CarrierNo	Uint8	载波号
Result	    Uint32	返回结果
                    0：成功
                    1：失败
*/

typedef enum
{
    FREQ_CFG_SUCCESS,
    FREQ_CFG_FAILED,
    FREQ_CFG_OTHER
}ENUM_RESULT_FREQ_CFG;

typedef struct
{
    UINT16                  ie_flag;             // value is 1512.
    UINT16                  ie_len;
    UINT8                   freq_cfg_cmd;
    UINT32                  local_cell_id;
    UINT8                   carrier_no;
    ENUM_RESULT_FREQ_CFG    result;
}IE_FREQ_CFG_RSP;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_CELL_CFG_RSP     ie_cell_cfg_rsp;
    IE_FREQ_CFG_RSP     ie_freq_cfg_rsp;
}MSG_RRU_BBU_CELL_CFG_RSP_BUILD;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_CELL_CFG_RSP     ie_cell_cfg_rsp;
}MSG_RRU_BBU_CELL_CFG_RSP_DELETE;







/*
 * BBU_RRU_TIME_DELAY_MEASURE_REQ
 */

/*
时延测量请求
功能：BBU通过该消息通知RRU执行RRU内部处理时延测试。
名称：BBU_RRU_TIME_DELAY_MEASURE_REQ
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	Uint16	IE标识
IeLen	Uint16	IE长度
FiberNo	Uint8	光纤端口号
*/

typedef struct
{
    UINT16              ie_flag;             // value is 901.
    UINT16              ie_len;
    UINT8               fiber_no;
}IE_TIME_DELAY_REQ;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_TIME_DELAY_REQ   ie_time_delay_req;
}MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ;





/*
 * RRU_BBU_TIME_DELAY_MEASURE_RSP
 */

/*
时延测量响应
功能：RRU执行RRU内部处理时延测试之后，通过该消息告知BBU测试结果。
名称：RRU_BBU_TIME_DELAY_MEASURE_REQ
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	    Uint16	IE标识-值是911
IeLen	    Uint16	IE长度
FiberNo	    Uint8	光纤端口号
Toffset	    Uint32	单位：ns
TBdelayDL	Uint32	单位：ns
TBdelayUL	Uint32	单位：ns
T2a	        Uint32	单位：ns
Ta3	        Uint32	单位：ns
NFrame	    Uint32	单位: basic frame   注: 一个基本帧=260ns
*/

typedef struct
{
    UINT16              ie_flag;             // value is 911.
    UINT16              ie_len;
    UINT8               fiber_no;
    UINT32              t_offset;
    UINT32              tb_delay_dl;
    UINT32              tb_delay_ul;
    UINT32              t2a;
    UINT32              ta3;
    UINT32              n_frame;
}IE_TIME_DELAY_RSP;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_TIME_DELAY_RSP   ie_time_delay_rsp;
}MSG_RRU_BBU_TIME_DELAY_MEASURE_RSP;





/*
 * BBU_RRU_TIME_DELAY_CFG_REQ
 */

/*
时延配置命令
功能：BBU通过该消息设置RRU的链路传输时延。
名称：BBU_RRU_TIME_DELAY_CFG_REQ
方向：BBU  RRU 
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	    Uint16	IE标识-值是921
IeLen	    Uint16	IE长度
FiberNo	    Uint8	光纤号
Trxa_rxm	Uint32	单位：ns
Trxm_rxa	Uint32	单位：ns
TimeAdv	    Uint32	定时提前量 单位：ns
*/

typedef struct
{
    UINT16              ie_flag;             // value is 921.
    UINT16              ie_len;
    UINT8               fiber_no;
    UINT32              trxa_rxm;
    UINT32              trxm_rxa;
    UINT32              time_adv;
    UINT32              dl_cal_rru;     // RRU下行补偿值
    UINT32              ul_cal_rru;     // RRU上行补偿值
}IE_TIME_DELAY_CFG_REQ;

typedef struct
{
    IrMsgHead               ir_msg_head;
    IE_TIME_DELAY_CFG_REQ   ie_time_delay_cfg_req;
}MSG_BBU_RRU_TIME_DELAY_CFG_REQ;





/*
 * RRU_BBU_TIME_DELAY_CFG_RSP
 */

/*
时延配置命令响应
功能：RRU设置完链路传输时延之后，通过该消息告知BBU设置结果。
名称：RRU_BBU_TIME_DELAY_CFG_RSPEQ
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	Uint16	IE标识-值931
IeLen	Uint16	IE长度
Result	Uint32	返回结果
                0：成功
                1：失败
*/

typedef enum
{
    TIME_DELAY_CFG_SUCCESS,
    TIME_DELAY_CFG_FAILED,
    TIME_DELAY_CFG_OTHER
}ENUM_RESULT_TIME_DELAY_CFG_RSP;

typedef struct
{
    UINT16                          ie_flag;             // value is 931.
    UINT16                          ie_len;
    UINT8                           fiber_no;
    ENUM_RESULT_TIME_DELAY_CFG_RSP  cfg_result;
}IE_TIME_DELAY_CFG_RSP;

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_TIME_DELAY_CFG_RSP       ie_time_delay_cfg_rsp;
}MSG_RRU_BBU_TIME_DELAY_CFG_RSP;






/*
 * RRU_BBU_ALARM_REPORT
 */

/*
告警上报请求
功能：当RRU产生告警时，通过该消息向BBU发出告警。
名称：RRU_BBU_ALARM_REPORT
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	                        Uint16	IE标识-值是1001
IeLen	                        Uint16	IE长度
ValidFlag	                    Uint16	告警有效性
                                        0：有效
                                        1：告警不存在
AlarmCode	                    Uint32	告警码（注1）
AlarmSubCode	                Uint32	告警子码
AlarmState	                    Uint32	告警清除标志
                                        0：告警未清除
                                        1：告警已清除
TimeStamp[IR_ALARM_TIME_LEN]	Int8	时间戳 yyyy-mm-dd hh:mm:ss IR_ALARM_TIME_LEN = 20
ExtraInfo[IR_ALARM_EXTRA_LEN]	Int8	附加信息 IR_ALARM_EXTRA_LEN = 100
注1：厂商自己定义，后续扩展。
*/

#define IR_ALARM_TIME_LEN 20
#define IR_ALARM_EXTRA_LEN 100

typedef enum
{
    ALARM_SUB_OTHER
}ENUM_ALARM_SUB_CODE;

typedef enum
{
    ALARM_NOT_CLEAR,
    ALARM_CLEAR,
    ALARM_STATE_OTHER
}ENUM_ALARM_STATE;

typedef struct
{
    UINT16                      ie_flag;             // value is 1001.
    UINT16                      ie_len;
    UINT16                      valid_flag;
    ENUM_ALARM_CODE             alarm_code;
    ENUM_ALARM_SUB_CODE         alarm_sub_code;
    ENUM_ALARM_STATE            alarm_state;
    INT8                        time_stamp[IR_ALARM_TIME_LEN];
    INT8                        extra_info[IR_ALARM_EXTRA_LEN];
}IE_ALARM_REPORT;

typedef struct
{
    IrMsgHead           ir_msg_head;
    IE_ALARM_REPORT     ie_alarm_report;
}MSG_RRU_BBU_ALARM_REPORT;





/*
 * BBU_RRU_ALARM_QUERY_REQ
 */

/*
告警查询请求
功能：BBU向RRU发送该消息查询响应的告警状态。
名称：BBU_RRU_ALARM_QUERY_REQ
方向：BBU RRU
消息头：参见4.1.2TCP消息格式
消息净荷：
参数	类型	含义
IeFlag	        Uint16	IE标识-值是1101
IeLen	        Uint16	IE长度
AlarmCode	    Uint32	告警码
                        0xffffffff 表示查询所有告警
AlarmSubCode	Uint32	告警子码
                        0xffffffff 表示查询所有子告警
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 1101.
    UINT16                      ie_len;
    ENUM_ALARM_CODE             alarm_code;
    ENUM_ALARM_SUB_CODE         alarm_sub_code;
}IE_ALARM_QUERY_REQ;

typedef struct
{
    IrMsgHead                   ir_msg_head;
    IE_ALARM_QUERY_REQ          ie_alarm_query_req;
}MSG_BBU_RRU_ALARM_QUERY_REQ;





/*
 * RRU_BBU_ALARM_QUERY_RSP
 */

/*
告警查询应答
功能：RRU向BBU发送该消息响应的告警查询请求。包括告警上报IE
名称：BBU_RRU_ALARM_QUERY_RSP
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
参见告警上报IE
*/







/*
 * BBU_RRU_HB
 */
 
/*
BBU在位心跳消息
功能：在位检测的触发条件为通道建立完成后，BBU收到通道建立配置应答消息后发起的，
      RRU在发出通道建议配置应答消息后启动接收定时器。在位心跳消息定时发送，时间间隔为3s。
      RRU连续3次未收到心跳判断为BBU不存活。RRU连续3次未收到BBU在位心跳消息后处理流程：
      生成并保存导致RRU复位的告警，进入不影响下一级RRU的复位状态，重新进入通道建立过程，
      并在通道建立请求消息中及时上报保存的告警,以备操作人员对RRU复位原因进行分析,并及时排除导致RRU复位的故障。
名称：BBU_RRU_HB
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷：无
*/

typedef struct
{
    IrMsgHead           ir_msg_head;
}MSG_BBU_RRU_HB;        // null body.





/*
 * RRU_BBU_HB
 */

/*
RRU在位心跳消息
功能：RRU在位检测的触发条件为发出通道建立配置应答消息的时刻，发送在位心跳消息至BBU。
      在位心跳消息定时发送，时间间隔为3s。BBU连续3次未收到心跳判断为RRU不存活。
      BBU连续3次未收到RRU在位心跳消息后处理流程：停止发送BBU在位心跳消息，同时删除该RRU的上下文，
      通知相关信令模块删除该RRU的小区上下文。
名称：RRU_BBU_ HB
方向：RRU BBU
消息头：参见4.1.2TCP消息格式
消息净荷：无
*/

typedef struct
{
    IrMsgHead           ir_msg_head;
}MSG_RRU_BBU_HB;       // null body.





/*
 * BBU_RRU_PARAM_QUERY_REQ
 */

/*
RRU参数查询
功能：BBU通过该消息向RRU发送参数查询请求。根据要查询的不同参数填充不同的IE，每次只能查询一个参数。
名称：BBU_RRU_PARAM_QUERY_REQ
方向：BBU RRU 
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
系统时间查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是401
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 401.
    UINT16                      ie_len;
}IE_SYSTEM_TIME_QUERY;

/*
CPU占用率查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是402
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 402.
    UINT16                      ie_len;
}IE_CPU_USAGE_QUERY;

/*
CPU占用率统计周期查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是403
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 403.
    UINT16                      ie_len;
}IE_CPU_USAGE_CHECK_CYCLE_QUERY;

/*
RRU温度IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是404
IeLen	    Uint16	IE长度
TempType	Uint32	RRU温度类型
                    0：射频通道温度
*/

typedef enum
{
    RF_PASS_TEMP,
    RRU_TEMP_TYPE_OTHER
}ENUM_RRU_TEMP_TYPE;

typedef struct
{
    UINT16                      ie_flag;             // value is 404.
    UINT16                      ie_len;
    ENUM_RRU_TEMP_TYPE          temp_type;
}IE_RRU_TEMP_QUERY;

/*
驻波比状态IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是405
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道
                    1-8代表8个射频通道
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 405.
    UINT16                      ie_len;
    UINT8                       rf_path_no;
}IE_VSWR_STATE_QUERY;

/*
驻波比门限查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是406
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 406.
    UINT16                      ie_len;
}IE_VSWR_THRES_QUERY;

/*
过温门限查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是407
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 407.
    UINT16                      ie_len;
}IE_OVERTEMP_THRES_QUERY;

/*
输出功率IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是408
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道
                    1-8代表8个射频通道
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 408.
    UINT16                      ie_len;
    UINT8                       rf_path_no;
}IE_OUTPUT_POWER_QUERY;

/*
状态机查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是409
IeLen	Uint16	IE长度
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 409.
    UINT16                      ie_len;
}IE_STATE_MACHINE_QUERY;


typedef struct
{
    IrMsgHead               ir_msg_head;
    IE_SYSTEM_TIME_QUERY			ie_system_time_query; //flag:401
    IE_CPU_USAGE_QUERY				ie_cpu_usage_query;
    IE_CPU_USAGE_CHECK_CYCLE_QUERY	ie_cpu_usage_check_cycle_query;
    IE_RRU_TEMP_QUERY				ie_rru_temp_query;
    IE_VSWR_STATE_QUERY				ie_vswr_state_query;
    IE_VSWR_THRES_QUERY				ie_vswr_thres_query;
    IE_OVERTEMP_THRES_QUERY			ie_overtemp_thres_query;
    IE_OUTPUT_POWER_QUERY			ie_output_power_query;
    IE_STATE_MACHINE_QUERY          ie_state_machine_query;

}MSG_BBU_RRU_PARAM_QUERY_REQ;




/*
 * RRU_BBU_PARAM_QUERY_RSP
 */

/*
RRU参数查询响应
功能：RRU通过该消息返回RRU参数查询的相关信息，根据查询消息的IE回复响应的IE。
名称：RRU_BBU_PARAM_QUERY_RSP
方向：RRU  BBU 
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
系统时间查询IE
*/

/*
CPU占用率查询IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是451
IeLen	    Uint16	IE长度
CpuUsage	Uint32	CPU占用率
                    单位：%
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 451.
    UINT16                      ie_len;
    UINT32                      cpu_usage;
}IE_CPU_USAGE_RSP;

/*
CPU占用率统计周期查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是452
IeLen	Uint16	IE长度
Period	Uint32	数值
                单位：秒
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 452.
    UINT16                      ie_len;
    UINT32                      period;
}IE_CPU_USAGE_CHECK_CYCLE_RSP;

/*
RRU温度IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是453
IeLen	    Uint16	IE长度
TempType	Uint32	RRU温度类型
                    0：射频通道温度
RfPathNo	Uint8	射频通道
                    1-8代表8个射频通道
TempVal	    Uint32	温度数值 单位：℃
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 453.
    UINT16                      ie_len;
    ENUM_RRU_TEMP_TYPE          temp_type;
    UINT8                       rf_path_no;
    UINT32                      temp_val;
}IE_RRU_TEMP_RSP;

/*
驻波比状态IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是454
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道
                    1-8代表8个射频通道
VswrVal	    Uint32	数值 单位：1/10
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 454.
    UINT16                      ie_len;
    UINT8                       rf_path_no;
    UINT32                      vswr_val;
}IE_VSWR_STATE_RSP;

/*
驻波比门限查询IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是455
IeLen	    Uint16	IE长度
VswrThres1	Uint32	一级门限 单位：1/10
VswrThres2	Uint32	二级门限 单位：1/10
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 455.
    UINT16                      ie_len;
    UINT32                      vswr_thres1;
    UINT32                      vswr_thres2;
}IE_VSWR_THRES_RSP;

/*
过温门限查询IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是456
IeLen	    Uint16	IE长度
UpThres	    Int32	温度上门限值 单位：℃
LowThres	Int32	温度下门限值（注） 单位：℃
注1：低温门限，默认值是-45℃。
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 456.
    UINT16                      ie_len;
    INT32                       up_thres;
    INT32                       low_thres;
}IE_OVERTEMP_THRES_RSP;

/*
输出功率IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是457
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道
                    1-8代表8个射频通道
PowerVal	Uint16	数值 单位：1/256dbm
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 457.
    UINT16                      ie_len;
    UINT8                       rf_path_no;
    UINT16                      power_val;
}IE_OUTPUT_POWER_RSP;

/*
状态机查询IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是458
IeLen	Uint16	IE长度
State	Uint8	状态机的状态
                0：无线帧同步
                1：超组同步
                2：失步
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 458.
    UINT16                      ie_len;
    UINT8                       state;
}IE_STATE_MACHINE_RSP;


typedef struct
{
    IrMsgHead                       ir_msg_head;
    IE_SYSTEM_TIME						ie_system_time;
    IE_CPU_USAGE_RSP						ie_cpu_usage_rsp;
    IE_CPU_USAGE_CHECK_CYCLE_RSP		ie_cpu_usage_check_cycle_rsp;
    IE_RRU_TEMP_RSP						ie_rru_temp_rsp;
    IE_VSWR_STATE_RSP					ie_vswr_state_rsp;
    IE_VSWR_THRES_RSP					ie_vswr_thres_rsp;
    IE_OVERTEMP_THRES_RSP				ie_overtemp_thres_rsp;
    IE_OUTPUT_POWER_RSP					ie_output_power_rsp;
    IE_STATE_MACHINE_RSP            ie_state_machine_rsp;

}MSG_RRU_BBU_PARAM_QUERY_RSP;





/*
 * BBU_RRU_PARAM_CFG_REQ
 */

/*
RRU参数配置
功能：BBU配置除IQ数据以外的初始化参数给RRU。该消息包含除IQ数据，环形组网使能状态外的所有参数的IE。
      控制字与参数配置消息相同。
名称：BBU_RRU_PARAM_CFG_REQ
方向：BBU  RRU
消息头：参见4.1.2TCP消息格式
消息净荷
*/

/*
系统时间IE
*/

/*
CPU占用率统计周期配置IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是502
IeLen	Uint16	IE长度
Period	Uint32	统计周期 单位：秒
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 502.
    UINT16                      ie_len;
    UINT32                      period;
}IE_CPU_USAGE_CHECK_CYCLE_CFG;

/*
驻波比门限配置IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是503
IeLen	    Uint16	IE长度
VswrThres1	Uint32	一级门限 单位：1/10
VswrThres2	Uint32	二级门限 单位：1/10
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 503.
    UINT16                      ie_len;
    UINT32                      vswr_thres1;
    UINT32                      vswr_thres2;
}IE_VSWR_THRES_CFG;

/*
过温门限配置IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是505
IeLen	    Uint16	IE长度
TempType	Uint32	RRU温度类型
                    0：射频通道温度
UpThres	    Int32	温度上门限值 单位：1 ℃
LowThres	Int32	温度下门限值 单位：1 ℃
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 505.
    UINT16                      ie_len;
    ENUM_RRU_TEMP_TYPE          temp_type;
    INT32                       up_thres;
    INT32                       low_thres;
}IE_OVERTEMP_THRES_CFG;

/*
天线配置IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是507
IeLen	    Uint16	IE长度
AntMode	    Uint8	RRU天线模式
                    1：智能天线
                    2：从属智能天线
                    3：分布式天线
AntSetNo	Uint8	天线组号 
                    取值范围：1-8
ant_state_u	Uint8	上行天线状态
                    0：使能
                    1：去使能
ant_state_d	Uint8	下行天线状态
                    0：使能
                    1：去使能
*/

typedef struct
{
    UINT16                      ie_flag;             // value is 507.
    UINT16                      ie_len;
    UINT8                       ant_mode;
    UINT8                       ant_set_no;
    UINT8                       ant_state_u;    // 上行天线状态
    UINT8                       ant_state_d;    // 下行天线状态
}IE_ANTENNA_CFG;

/*
射频通道状态查询响应IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是352
IeLen	    Uint16	IE长度
up_path_state	    Uint32	状态
dw_path_state       0：未使能
                    1：使能，无故障
                    2：使能，有故障
*/

typedef struct
{
    UINT16                  ie_flag;     // value is 352.
    UINT16                  ie_len;
    UINT32					up_path_state[8];
    UINT32					dw_path_state[8];
}IE_RF_PATH_STATE_CFG;



typedef struct
{
    IrMsgHead                       ir_msg_head;
    IE_CPU_USAGE_CHECK_CYCLE_CFG    ie_cpu_usage_check_cycle_cfg;
    IE_VSWR_THRES_CFG               ie_vswr_thres_cfg;
    IE_OVERTEMP_THRES_CFG           ie_overtemp_thres_rsp;
    IE_ANTENNA_CFG                  ie_antenna_cfg;
    IE_RF_PATH_STATE_CFG            ie_rf_path_state_cfg;

}MSG_BBU_RRU_PARAM_CFG_REQ;



/*
 * RRU_BBU_PARAM_CFG_RSP
 */

/*
RRU参数配置响应-初始化阶段
功能：RRU收到BBU初始化参数配置消息，配置完成后发送响应消息到BBU，该消息包含每个参数配置结果的IE。
      控制字与参数配置响应消息相同。
名称：RRU_BBU_PARAM_CFG_RSP
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷
*/

/*
系统时间响应IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是551
IeLen	Uint16	IE长度
Result	Uint32	状态 
                0：成功
                1：失败
*/

typedef enum
{
    SYSTEM_TIME_RSP_SUCCESS,
    SYSTEM_TIME_RSP_FAILED,
    SYSTEM_TIME_RSP_OTHER
}ENUM_RESULT_SYSTEM_TIME_RSP;

typedef struct
{
    UINT16                          ie_flag;     // value is 551.
    UINT16                          ie_len;
    ENUM_RESULT_SYSTEM_TIME_RSP     result;
}IE_SYSTEM_TIME_RSP;

/*
CPU占用率统计周期配置响应IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是553
IeLen	Uint16	IE长度
Result	Uint32	状态 
                0：成功
                1：失败
*/

typedef enum
{
    CPU_CYCLE_CFG_SUCCESS,
    CPU_CYCLE_CFG_FAILED,
    CPU_CYCLE_CFG_OTHER
}ENUM_RESULT_CPU_USAGE_CHECK_CYCLE_RSP;

typedef struct
{
    UINT16                                  ie_flag;     // value is 553.
    UINT16                                  ie_len;
    ENUM_RESULT_CPU_USAGE_CHECK_CYCLE_RSP   result;
}IE_CPU_USAGE_CHECK_CYCLE_CFG_RSP;

/*
驻波比门限配置响应IE
参数	类型	含义
IeFlag	Uint16	IE标识-值是554
IeLen	Uint16	IE长度
Result	Uint32	状态 
                0：成功
                1：失败
*/

typedef enum
{
    VSWR_THRES_CFG_SUCCESS,
    VSWR_THRES_CFG_FAILED,
    VSWR_THRES_CFG_OTHER
}ENUM_RESULT_VSWR_THRES_RSP;

typedef struct
{
    UINT16                              ie_flag;     // value is 554.
    UINT16                              ie_len;
    ENUM_RESULT_VSWR_THRES_RSP          result;
}IE_VSWR_THRES_CFG_RSP;

/*
过温门限配置响应IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是556
IeLen	    Uint16	IE长度
TempType	Uint32	RRU温度类型
                    0：射频通道温度
Result	    Uint32	状态 
                    0：成功
                    1：失败
*/

typedef enum
{
    OVERTEMP_THRES_CFG_SUCCESS,
    OVERTEMP_THRES_CFG_FAILED,
    OVERTEMP_THRES_CFG_OTHER
}ENUM_RESULT_OVERTEMP_THRES_RSP;

typedef struct
{
    UINT16                              ie_flag;     // value is 556.
    UINT16                              ie_len;
    ENUM_RRU_TEMP_TYPE                  temp_type;
    ENUM_RESULT_OVERTEMP_THRES_RSP      result;
}IE_OVERTEMP_THRES_CFG_RSP;

/*
天线配置响应IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是559
IeLen	    Uint16	IE长度
AntSetNo	Uint8	天线组号
                    取值范围：1-8 
Result	    Uint32	状态 
                    0：成功
                    1：失败
*/

typedef enum
{
    ANTENNA_CFG_SUCCESS,
    ANTENNA_CFG_FAILED,
    ANTENNA_CFG_OTHER
}ENUM_RESULT_ANTENNA_RSP;

typedef struct
{
    UINT16                              ie_flag;     // value is 559.
    UINT16                              ie_len;
    UINT8                               ant_set_no;
    ENUM_RESULT_ANTENNA_RSP             result;
}IE_ANTENNA_CFG_RSP;

/*
射频通道状态配置响应IE
参数	类型	含义
IeFlag	    Uint16	IE标识-值是557
IeLen	    Uint16	IE长度
RfPathNo	Uint8	射频通道号
                    取值范围：1-8 
Result	    Uint32	状态 
                    0：成功
                    1：失败
*/

typedef enum
{
    RF_PATH_STATE_CFG_SUCCESS,
    RF_PATH_STATE_CFG_FAILED,
    RF_PATH_STATE_CFG_OTHER
}ENUM_RESULT_RF_PATH_STATE_RSP;

typedef struct
{
    UINT16                              ie_flag;     // value is 557.
    UINT16                              ie_len;
    UINT8                               rf_path_no;
    ENUM_RESULT_RF_PATH_STATE_RSP       result;
}IE_RF_PATH_STATE_CFG_RSP;


typedef struct
{
    IrMsgHead                       	ir_msg_head;
    IE_SYSTEM_TIME_RSP					ie_system_time_rsp;
	IE_CPU_USAGE_CHECK_CYCLE_CFG_RSP	ie_cpu_usage_check_cycle_cfg_rsp;
	IE_VSWR_THRES_CFG_RSP				ie_vswr_thres_cfg_rsp;
	IE_OVERTEMP_THRES_CFG_RSP			ie_overtemp_thres_cfg_rsp;
	IE_ANTENNA_CFG_RSP					ie_antenna_cfg_rsp;
	IE_RF_PATH_STATE_CFG_RSP			ie_rf_path_state_cfg_rsp;
}MSG_RRU_BBU_PARAM_CFG_RSP;

/*
 * RRU_BBU_INIT_CAL_RESULT_REPORT
 */

/*
初始化校准结果上报
功能：RRU完成参数配置后，发送初始化结果上报消息到BBU。
名称：RRU_BBU_INIT_CAL_RESULT_REPORT
方向：RRU  BBU
消息头：参见4.1.2TCP消息格式
消息净荷：
*/

/*
接收通道天线校准响应IE
参数	类型	含义
IeFlag	        Uint16	IE标识-值是601
IeLen	        Uint16	IE长度
RfPathNo	    Uint8	射频通道号
                        取值范围：1-8 
NormA	        Uint16	基带信号标定振幅
                        基带信号与标定输入的对应值
                        单位：1/256dBFS
AntNormalInput	Uint16	天线单元的标定输入
                        单位：1/256dBm
Result	        Uint32	返回结果
                        0：成功
                        1：失败
*/

typedef enum
{
    RECVPATH_ANTCALIB_SUCCESS,
    RECVPATH_ANTCALIB_FAILED,
    RECVPATH_ANTCALIB_OTHER
}ENUM_RESULT_RECVPATH_ANTCALIB_RSP;

typedef struct
{
    UINT16                                  ie_flag;     // value is 601.
    UINT16                                  ie_len;
    UINT8                                   rf_path_no;
    UINT16                                  norm_a;
    UINT16                                  ant_normal_input;
    ENUM_RESULT_RECVPATH_ANTCALIB_RSP       result;
}IE_RECVPATH_ANTCALIB_RSP;

/*
发射通道天线校准响应IE
参数	类型	含义
IeFlag	        Uint16	IE标识-值是602
IeLen	        Uint16	IE长度
RfPathNo	    Uint8	射频通道号
                        取值范围：1-8 
NormA	        Uint16	基带信号标定振幅
                        基带信号与标定输入的对应值
                        单位：1/256dBFS
AntNormalInput	Uint16	天线单元的标定输入
                        单位：1/256dBm
Result	        Uint32	返回结果
                        0：成功
                        1：失败
*/

typedef enum
{
    SENDPATH_ANTCALIB_SUCCESS,
    SENDPATH_ANTCALIB_FAILED,
    SENDPATH_ANTCALIB_OTHER
}ENUM_RESULT_SENDPATH_ANTCALIB_RSP;

typedef struct
{
    UINT16                                  ie_flag;     // value is 602.
    UINT16                                  ie_len;
    UINT8                                   rf_path_no;
    UINT16                                  norm_a;
    UINT16                                  ant_normal_input;
    ENUM_RESULT_SENDPATH_ANTCALIB_RSP       result;
}IE_SENDPATH_ANTCALIB_RSP;

typedef struct
{
    IrMsgHead                           ir_msg_head;
    IE_RECVPATH_ANTCALIB_RSP            ie_recvpath_antcalib_rsp;
    IE_SENDPATH_ANTCALIB_RSP            ie_sendpath_antcalib_rsp;
}MSG_RRU_BBU_INIT_CAL_RESULT_REPORT;












#pragma pack()

#endif
