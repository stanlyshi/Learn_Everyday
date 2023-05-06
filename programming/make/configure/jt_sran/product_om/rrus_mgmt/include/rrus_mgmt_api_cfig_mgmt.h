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
 *File name: rrus_mgmt_cfig_mgmt_api.h
 *Description: the api of rrus_mgmt with cfig_mgmt.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.06.05
 *
 ************************************************************************/
#ifndef RRUS_MGMT_API_CFIG_MGMT_H
#define RRUS_MGMT_API_CFIG_MGMT_H

#include "sbs_type.h"
#include "om_common_h.h"

#pragma pack(1)



typedef enum
{
	SET_INIT_INFO_CPRI_SPEED = 1,
	SET_INIT_INFO_RRU,
	SET_CPRI_RING_TEST,
	SET_CPRI_SER_TEST,
	SET_CPRI_TIME_DELAY_CFG,
	SET_SINGLE_SEND_CFG,
	SET_RRU_PARAM_CPU_USAGE_PERIOD,
	SET_RRU_PARAM_VSWR_THRES,
	SET_RRU_PARAM_TEMP_THRES,
	SET_RRU_PARAM_ANT_CFG,
	SET_RRU_PARAM_CHANNEL_STATE,
	SET_RRU_RING_TEST,
	SET_RRU_RESET_REQ,
	SET_RRU_SOFTWARE_UPDATE_REQ,
	SET_RRU_TIME_DELAY_CFG
}CFG2RRUMsgCode;

typedef enum
{
	TRAP_RRU_INIT_RESULT = 1,
	TRAP_CPRI_RING_TEST,
	TRAP_CPRI_SER_TEST,
	TRAP_CPRI_TIME_DELAY_CFG,
	TRAP_SINGLE_SEND_RESULT,
	TRAP_RRU_RING_TEST,
	TRAP_RRU_SOFTWARE_UPDATE
}RRU2CFGMsgCode;

typedef struct
{
	INT8			serial_no[16];
    UINT32          period;
	UINT32          vswr_thres1;
	UINT32          vswr_thres2;
	INT32			up_thres;
	INT32			low_thres;
	UINT8			ant_mode;
	UINT8			ant_set_no;
	UINT16			ant_state;
	UINT32			up_path_state[8];
	UINT32			dw_path_state[8];
}RRU_PARAM_TABLE;
#define MAX_RRU_PARAM_NUM 64
RRU_PARAM_TABLE* g_rru_param_table[MAX_RRU_PARAM_NUM];

/*************配置恢复函数**************************/
//初始信息配置——CPRI速率	配置CPRI速率
INT32 handle_icr_init_info_cpri(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16		    cpri_index;
    UINT32          cpri_speed;
}ICR_INIT_INFO_CPRI_SPEED;

//初始信息配置——RRU参数	配置RRU初始参数
INT32 handle_icr_init_info_rru(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	INT8			serial_no[16];
    UINT32          period;
	UINT32          vswr_thres1;
	UINT32          vswr_thres2;
	INT32			up_thres;
	INT32			low_thres;
	UINT8			ant_mode;
	UINT8			ant_set_no;
	UINT16			ant_state;
	UINT32			up_path_state[8];
	UINT32			dw_path_state[8];
}ICR_INIT_INFO_RRU;




//RRU初始化完成上报		相应RRU完成初始化流程后上报，提供RRU序列号与RRU_ID绑定
//INT32 handle_icr_rru_init_result_trap(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16			rru_id;
    INT8			serial_no[16];
	UINT32		    result;
}ICR_RRU_INIT_RESULT_TRAP;

//获取第一个RRU的ID		返回第一个RRU的ID
INT32 get_first_rru_id(UINT16 *rru_id);

//获取下一个RRU的ID		读取输入参数的RRU_ID，赋值为下一个RRU的ID
INT32 get_next_rru_id(UINT16 *rru_id);

//检查RRU的ID是否存在
INT32 check_exist_rru_id(UINT16 rru_id);

/*****************CPRI消息触发函数*****************/
//CPRI查询消息

//CPRI接口链路状态查询	CFIG_MGMT触发，查询CPRI接口链路状态信息
INT32 get_cpri_channel_state(UINT16 cpri_index , void *msg);

//CPRI时延测量信息查询	CFIG_MGMT触发，查询CPRI侧光纤链路时延测量信息
INT32 get_cpri_time_delay_measure(UINT16 cpri_index , void *msg);

//CPRI触发消息

//CPRI环回测试请求配置	由CFIG_MGMT模块触发CPRI链路环回测试。   
INT32 handle_icr_cpri_ring_test_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16		    cpri_index;
}ICR_CPRI_RING_TEST;


//CPRI环回测试上报		进行CPRI到RRU之间链路环回测试，并将测试结构通过该消息响应给CFIG_MGMT
//INT32 trap_cpri_ring_test(UINT16 cpri_index, void *msg);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32		    result;
}ICR_CPRI_RING_TEST_RESULT;


//误码率测试请求配置	CFIG_MGMT触发对CPRI链路的误码率测试请求，CPRI进行光纤链路的误码率测试流程
//INT32 set_cpri_ser_test(UINT16 cpri_index, void *msg);
INT32 handle_icr_cpri_ser_test_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT8			ctrl_word;
}ICR_CPRI_SER_TEST;

//误码率测试完成结果上报	光纤链路误码率测试结束后，上报网管
//INT32 trap_cpri_ser_test(UINT16 cpri_index, void *msg);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32		    result;
}ICR_CPRI_SER_TEST_RESULT;


//误码率测试查询	上报误码率测试结果
INT32 get_cpri_ser_test_result(UINT16 cpri_index ,UINT8 pos, void *msg);
/*typedef struct 
{
	UINT32			totalh;
    UINT32          totall;
	UINT32			errorh;
	UINT32			errorl;
}ICR_SER_TEST_RESULT;*/

//CPRI时延配置	CFIG_MGMT触发对CPRI链路的时延配置
INT32 handle_icr_cpri_time_cfg_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32			axc_ind;
    UINT32          time_delay_ul;
	UINT32			time_delay_dl;
}ICR_CPRI_TIME_DELAY_CFG;

//CPRI时延配置结果上报	上报CPRI时延配置结果
//INT32 trap_cpri_time_delay_cfg(UINT16 cpri_index, void *msg);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32		    result;
}ICR_CPRI_TIME_DELAY_CFG_RESULT;


//单音信号模式配置	独立配置FPGA某个光口某部分AxC是否执行单音工作模式，用于对FPGA的测试
//INT32 set_single_send_cfg(UINT16 cpri_index, void *msg);
INT32 handle_icr_single_send_cfg(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32			axc_ind;
    UINT32          on_or_off;
	UINT32			single_mode;
}ICR_SINGLE_SEND_CFG;

//单音信号模式配置结果上报	上报CPRI单音信号模式配置结果
//INT32 trap_single_send_result(UINT16 cpri_index, void* msg);
typedef struct 
{
	CommMsgHead			head;
	UINT16          cpri_index;
	UINT32		    result;
}ICR_SINGLE_SEND_RESULT;



/*********************RRU消息触发函数*********************/
//RRU查询消息

//RRU逻辑信息查询	CFIG_MGMT通过该消息触发RRU逻辑信息查询
INT32 get_rru_logic_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len);
/*typedef struct 
{
	UINT32			net_unit_id;
    UINT32          bbu_id;
	UINT32			fpga_id;
	UINT8			cpri_port;
	UINT8			bbu_ip[16];
	UINT8			rru_ip[16];
	UINT8			rru_mac[18];
	UINT8			sub_mask[16];
}ICR_RRU_LOGIC_INFO;*/

//RRU产品信息查询	CFIG_MGMT通过该消息触发RRU逻辑信息查询
INT32 get_rru_product_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len);
/*typedef struct 
{
	UINT32			net_unit_id;
    INT8         	manufactuere[16];
	INT8         	publisher[16];
	INT8			serial_no[16];
	INT8			manufacture_date[16];
	INT8			last_service_date[16];
	INT8			extra_info[16];
}ICR_RRU_PRODUCT_INFO;*/

//最近一次通道建立原因查询	CFIG_MGMT通过该消息触发最近一次通道建立原因查询
INT32 get_rru_channel_build_reason(UINT16 rru_id, UINT8 pos, void *msg);
/*typedef struct 
{
	UINT8			reason;
    UINT32         	alarm_code;
}ICR_RRU_CHANNEL_BUILD_REASON;*/

//RRU能力信息查询	CFIG_MGMT通过该消息触发RRU能力信息查询
INT32 get_rru_ability_info(UINT16 rru_id, UINT8 pos, void *msg);
/*typedef struct 
{
	UINT32			td_carrier_num;
    UINT32          lte_carrier_num;
	UINT8         	ant_num;
	UINT16			max_power;
	UINT8			master_or_slave;
	UINT32			max_dw_time_delay;
	UINT32			max_up_time_delay;
	UINT16 			support_mode;
	UINT8           ant_clbrt_flag;
	UINT8           main_send_eme_path;
	UINT8           slave_send_eme_path;
	UINT8           main_recv_eme_path;
	UINT8           slave_recv_eme_path;
}ICR_RRU_ABILITY_INFO;*/

//RRU软硬件信息查询	CFIG_MGMT通过该消息触发RRU软硬件信息查询
INT32 get_rru_ware_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len);
/*typedef struct 
{
	INT8			hw_type[32];
    INT8         	hw_version[16];
	INT8         	sw_ver[40];
	INT8         	fw_ver[40];
}ICR_RRU_WARE_INFO;*/

//CFIG_MGMT通过该消息触发RRU参数查询请求，根据要查询的不同参数提供多个函数
//RRU参数查询——CPU占用率查询	
INT32 get_rru_param_cpu_usage(UINT16 rru_id, void *msg);

//RRU参数查询——CPU占用率统计周期查询
INT32 get_rru_param_cpu_usage_period(UINT16 rru_id, void *msg);

//RRU参数查询——RRU射频通道温度查询
INT32 get_rru_param_temp_val(UINT16 rru_id, void *msg, UINT32 *len);

//RRU参数查询——RRU射频通道驻波比状态查询
INT32 get_rru_param_vswr_val(UINT16 rru_id, void *msg, UINT32 *len);

//RRU参数查询——驻波比门限查询
INT32 get_rru_param_vswr_thres(UINT16 rru_id, UINT8 pos, void *msg);
/*
typedef struct 
{
	UINT32			VswrThres1;
    UINT32         	VswrThres2;
}ICR_RRU_PARAM_VSWR_THRES;
*/

//RRU参数查询——过温门限查询响应
INT32 get_rru_param_temp_thres(UINT16 rru_id, UINT8 pos, void *msg);
/*
typedef struct 
{
	INT32			UpThres;
    INT32         	LowThres;
}ICR_RRU_PARAM_TEMP_THRES;
*/

//RRU参数查询——射频通道输出功率查询响应
INT32 get_rru_param_power_val (UINT16 rru_id, void *msg, UINT32 *len);

//RRU参数查询——状态机查询响应
INT32 get_rru_param_state(UINT16 rru_id, void *msg);

//RRU射频状态查询	CFIG_MGMT通过该消息触发获取RRU射频通道状态
INT32 get_rru_channel_state(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len);
/*
typedef struct 
{
	UINT32			up_path_state[8];
    UINT32         	dw_path_state[8];
}ICR_RRU_CHANNEL_STATE;
*/

//RRU时钟状态查询	CFIG_MGMT通过该消息触发获取RRU时钟状态
INT32 get_rru_clock_state(UINT16 rru_id, void *msg);

//RRU时延信息查询	CFIG_MGMT通过该消息触发获取RRU时延信息
INT32 get_rru_time_delay_measure(UINT16 rru_id, UINT8 pos, void *msg);
/*typedef struct 
{
	UINT8			fiber_no;
	UINT32			t_offset;
    UINT32         	tb_delay_dl;
	UINT32          tb_delay_ul;
	UINT32         	t2a;
	UINT32         	t3a;
}ICR_RRU_TIME_DELAY_MEASURE;*/

//RRU时延配置信息查询	CFIG_MGMT通过该消息触发获取RRU时延配置信息
INT32 get_rru_time_delay_cfg(UINT16 rru_id, UINT8 pos, void *msg);
/*typedef struct 
{
	UINT8			fiber_no;
	UINT32			trxa_rxm;
    UINT32          trxm_rxa;
	UINT32          time_adv;
	UINT32         	dl_cal_rru;
	UINT32         	ul_cal_rru;
}ICR_RRU_TIME_DELAY_CFG;*/

//RRU触发消息

//CFIG_MGMT通过该消息触发RRU参数配置，根据配置参数的不同提供多个函数
//RRU参数配置——CPU占用率统计周期配置		
INT32 handle_icr_rru_param_cpu_usage_period(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT32          period;
}ICR_RRU_PARAM_CPU_USAGE_PERIOD;


//RRU参数配置——驻波比门限配置
INT32 handle_icr_rru_param_vswr_thres(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT32			VswrThres1;
    UINT32         	VswrThres2;
}ICR_RRU_PARAM_VSWR_THRES;


//RRU参数配置——过温门限配置
INT32 handle_icr_rru_param_temp_thres(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	INT32			UpThres;
    INT32         	LowThres;
}ICR_RRU_PARAM_TEMP_THRES;


//RRU参数配置——天线配置
INT32 handle_icr_rru_param_ant_cfg(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT8			AntMode;
    UINT8         	AntSetNo;
	UINT16			AntState;
}ICR_RRU_PARAM_ANT_CFG;

//RRU参数配置——射频通道状态配置
INT32 handle_icr_rru_param_channel_state(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT32			up_path_state[8];
    UINT32         	dw_path_state[8];
}ICR_RRU_PARAM_CHANNEL_STATE;

//RRU回环测试请求配置	CFIG_MGMT通过该消息触发链路环回测试，要求RRU进行链路环回测试
INT32 handle_icr_rru_ring_test_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT32			ring_test_type;
    UINT16         check_period;
	UINT8          port_no;
}ICR_RRU_RING_TEST;

//RRU回环测试上报	RRU收到链路环回测试请求后，进行到下一级RRU之间链路环回测试，并将测试结果通过该消息返回
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT8           port_no;
	UINT32		    result;
}ICR_RRU_RING_TEST_RESULT;

//复位请求配置		CFIG_MGMT通过该消息触发RRU板卡复位请求消息
INT32 handle_icr_rru_reset_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
}ICR_RRU_RESET_REQ;


//RRU软件更新请求配置	CFIG_MGMT通过该消息触发RRU软件更新流程，包括以下IR消息流程：RRU版本查询、版本下载、RRU版本激活
INT32 handle_icr_rru_soft_update_req(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
}ICR_RRU_SW_UPDATE_REQ;


//RRU软件更新上报	上报RRU软件更新结果
//INT32 trap_rru_software_update(UINT16 rru_id, void *msg);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT32              VerUpdate;
	UINT32              ErrType;
}ICR_RRU_SW_UPDATE_RESULT;


//RRU时延配置    由CFIG_MGMT模块触发RRU时延配置
//INT32 set_rru_time_delay_cfg(UINT16 rru_id, void *msg);
INT32 handle_icr_rru_time_delay_cfg(void* msg, UINT32 len);
typedef struct 
{
	CommMsgHead			head;
	UINT16          rru_id;
	UINT8			fiber_no;
	UINT32			trxa_rxm;
    UINT32          trxm_rxa;
	UINT32          time_adv;
	UINT32         	dl_cal_rru;
	UINT32         	ul_cal_rru;
}ICR_RRU_TIME_DELAY_CFG;



#pragma pack()


#endif
