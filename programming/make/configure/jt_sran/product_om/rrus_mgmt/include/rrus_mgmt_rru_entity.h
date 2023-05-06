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
 *File name: rru_agent_rru_entity.h
 *Description: the infomation of rru.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.06.01
 *
 ************************************************************************/


#ifndef SBS_INTERFACE_RRUAGENT_RRUENTITY_H
#define SBS_INTERFACE_RRUAGENT_RRUENTITY_H

#include "sbs_type.h"
#include "rrus_mgmt_cpri.h"
#include "rrus_mgmt_ir.h"
#include "rrus_mgmt_rru_entity_state_machine.h"

typedef struct
{
	UINT8		shelf_id;
	UINT8		vbbu_id;
	UINT8		chip_id;
	unsigned	fiber_num:4;
	unsigned	rru_level:4;
}RRU_ID;


typedef struct
{
    UINT32      rru_id;          // 超级基站系统内RRU的标识，此处使用4个字节大小表�?系统初始配置，RRU自动获取后上报�?
    UINT32      fpga_id;         // RRU所连接FPGA的ID
    UINT32      bbu_id;          // RRU所连接的BBU的ID
    UINT16		cpri_index;
    UINT8       cpri_port;       // RRU所在Fpga的cpri_port
    UINT8		rru_level;		 // RRU level
    UINT8       bbu_ip[16];      // RRU所连接的BBU的ip
    UINT8       rru_ip[16];      // RRU通信用ip地址
    UINT8       rru_mac[6];      // RRU通信用MAC地址
    UINT8       sub_mask[4];     // RRU通信用子网掩�?
    INT32       rru_fd;          // RRU通信用fd
    UINT32      stream_no;       // 流水�?
    UINT8       rru_entity_id;   // rru实体id，g_rru_entity_table的索�?
    RruStateMachine rru_cur_state;  // rru 实体现在的状态；
	UINT8       lost_hub_count;  // rru失去心跳的计数，大于3次进行告警，并对RRU试着进行复位

}RruLogicInfo;

typedef struct
{
	INT8	manufacturer[16];
	INT8	publisher[16];
	INT8	serial_no[16];
	INT8	manufacture_date[16];
	INT8	last_service_date[16];
	INT8	extra_info[16];
}ProductInfo;

typedef struct
{
	UINT8	reason;
	UINT32  AlarmCode;
}ChannelBuildInfo; 

typedef struct
{
	UINT32	td_carrier_num;
	UINT32	lte_carrier_num;
	UINT8	ant_num;
	UINT16	max_power;
	UINT8	master_or_slave;
	UINT32	max_dw_time_delay;
	UINT32	max_up_time_delay;
	UINT16	support_mode;
	UINT8	ant_clbrt_flag;
	UINT8	main_send_eme_path;
	UINT8	slave_send_eme_path;
	UINT8	main_recv_eme_path;
	UINT8	slave_recv_eme_path;
}AbilityInfo;

typedef struct
{
	INT8	hw_type[32];
	INT8	hw_version[16];
}HardWareInfo;

typedef struct
{
	INT8	sw_ver[40];
	INT8	fw_ver[40];
}SoftWareInfo;

typedef struct
{
	UINT32  check_result;
    INT8    file_path[200];
    INT8    file_name[16];
    UINT32  file_len;
    INT8    file_time[20];
    INT8    file_ver[40];
}SoftVerCheckResultInfo;

typedef struct
{
	UINT32	up_path_state[8];
	UINT32  dw_path_state[8];
	UINT32	clock_state;
}RruStateInfo;

typedef struct
{
	// CellCfgInfo
	UINT8               cell_cfg_cmd;
    UINT32              local_cell_id;
    UINT16              single_carrier_cell_power;
    UINT8               ant_set_no;
    UINT8               freq_ie_num;
    // FreqCfgInfo
    UINT8               freq_cfg_cmd;
    UINT8               carrier_no;
    UINT32              mid_freq;
    UINT32     			freq_attribute;
    UINT8               ts_info;
    UINT8               special_subframe_cfg;
    UINT32              carrier_width;
    UINT8               circ_len_ch;
}CellCfgInfo;

typedef struct
{
	UINT8               fiber_no;
    UINT32              t_offset;
    UINT32              tb_delay_dl;
    UINT32              tb_delay_ul;
    UINT32              t2a;
    UINT32              ta3;
    UINT32              n_frame;
}TimeDelayInfo;

typedef struct
{
	UINT8               fiber_no;
    UINT32              trxa_rxm;
    UINT32              trxm_rxa;
    UINT32              time_adv;
    UINT32              dl_cal_rru;     
    UINT32              ul_cal_rru;
}TimeDelayCfgInfo;

typedef struct
{
	UINT16              valid_flag;
    UINT32             	alarm_code;
    UINT32         		alarm_sub_code;
    UINT32            	alarm_state;
    INT8                time_stamp[IR_ALARM_TIME_LEN];
    INT8                extra_info[IR_ALARM_EXTRA_LEN];
}RruAlarmInfo;

typedef struct
{
	UINT8                       ant_mode;
    UINT8                       ant_set_no;
    UINT8                       ant_state_u;    // 上行天线状�?
    UINT8                       ant_state_d;    // 下行天线状�?
}RruAntennaInfo;

typedef struct
{
	UINT32                      cpu_usage;
	UINT32                      cpu_usage_period;
	UINT32                      period;
	UINT32                      temp_val[8];
	UINT32                      vswr_val[8];
	UINT32                      vswr_thres1;
    UINT32                      vswr_thres2;
    INT32                       up_thres;
    INT32                       low_thres;
    UINT16                      power_val[8];
    UINT8                       state;   
}RruParamInfo;

typedef struct
{
    ProductInfo             product_info;
    ChannelBuildInfo        channel_build_info;
    AbilityInfo             ability_info;
    HardWareInfo            hard_ware_info;
    SoftWareInfo            soft_ware_info;
    SoftVerCheckResultInfo  soft_ver_check_result_info;
    RruStateInfo            rru_state_info;
    CellCfgInfo             cell_cfg_info;
    TimeDelayInfo           time_delay_info;
    TimeDelayCfgInfo        time_delay_cfg_info;
    RruAlarmInfo            rru_alarm_info[20];
    RruAntennaInfo          rru_antenna_info;
    RruParamInfo            rru_param_info;
}RruComInfo;

typedef struct
{
    RruLogicInfo    logic_info;
    RruComInfo      com_info;   
}RruEntity;

#define MAX_RRU_NUM 64
extern RruEntity* g_rru_entity_table[MAX_RRU_NUM];

extern RruEntity* g_rru_cfig_table[MAX_RRU_NUM];




#endif
