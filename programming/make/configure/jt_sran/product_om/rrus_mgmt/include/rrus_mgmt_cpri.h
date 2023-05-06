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
 *File name: rrus_mgmt_cpri.h
 *Description: the interface of cpri.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.25
 *
 ************************************************************************/


#ifndef SBS_INTERFACE_RRUMGMT_CPRI_H
#define SBS_INTERFACE_RRUMGMT_CPRI_H

#include "sbs_type.h"

#pragma pack(1)


typedef struct
{
	UINT8	des_mac[6];
	UINT8 	src_mac[6];
	UINT16	pro_type;
}CpriMacHead;

typedef struct
{
    UINT16  msg_type;    
    UINT16  msg_code;    
    UINT32  msg_len; 
    UINT32  fpga_id;
}OmCpriMsgHead;

typedef struct
{
	UINT8	fpga_id;
	unsigned fiber_num:4;
	unsigned rru_id:4;
}EqIndex;
/*
 * OmCpriMsgHead.msg_type value range
 */

#define     MSG_TYPE_CTRL_FPGA_MAIN_CTRL            0x5300          //message between MAIN_CTRL with CTRL_FPGA
#define     MSG_TYPE_CTRL_FPGA_FTPS_SERV            0x5301          //message between FTPS_SERV with CTRL_FPGA
#define     MSG_TYPE_CTRL_FPGA_
    
#define     MSG_TYPE_DATA_FPGA_MAIN_CTRL            0x5400          //message between MAIN_CTRL with DATA_FPGA
#define     MSG_TYPE_DATA_FPGA_RRUS_MGMT            0x5401          //message between RRUS_MGMT with DATA_FPGA
#define     MSG_TYPE_DATA_FPGA_
    
#define     MSG_TYPE_FPGA_MONT_MGMT                 0x5500          //message between FPGA with MONT_MGMT



/*
消息编号，取值范围：
数据FPGA的MAC通路检测信息，由数据FPGA每个10G数据口发出MAC包给OM数据接收口：
(MsgType 填：MSG_TYPE_DATA_FPGA_MAIN_CTRL。该过程保留接口，根据讨论，初始可以不实现)
001：FPGA2/3/4/5心跳消息 
 
（FPGA2/3/4/5其一填：MsgType填写MSG_TYPE_DATA_FPGA_MAIN_CTRL）
011：FPGA帧信息上报配置消息
012：FPGA帧信息上报配置响应
 
（FPGA2/3/4/5填：MsgType填写MSG_TYPE_DATA_FPGA_RRUS_MGMT）
CPRI接口MsgCode
101  CPRI接口初始化配置
102  CPRI接口初始化配置响应
103  CPRI AXC通道配置
104  CPRI AXC通道配置响应
105  CPRI接口链路状态查询
106  CPRI接口链路状态查询响应
107  时延测量请求
108  时延测量响应
109  时延配置命令
110  时延配置响应
111  环回测试请求
112  环回测试响应
113  误码率测试请求
114  误码率测试响应
 
（FPGA2/3/4/5填：MsgType填写MSG_TYPE_DATA_FPGA_RRUS_MGMT）
FPGAPHY接口MsgCode
201  FPGA_PHY工作模式配置
202  FPGA_PHY工作模式配置响应
203  FPGA单音信号配置
204  FPGA单音信号配置响应
205  IQ数据输出配置
206  IQ数据输出配置响应

 FPGA告警接口 
（FPGA2/3/4/5填：MsgType填写MSG_TYPE_FPGA_MONT_MGMT）
告警编码：（所有FPGA可能的告警编码可统一分类设置）
0X……
*/

#define CPRIMSGSUCCESS  0
//CPRI message 失败原因应宏定义在这里


// MsgType MSG_TYPE_DATA_FPGA_MAIN_CTRL
// MsgCode
// 011	FPGA帧信息上报配置	MAIN_CTRL_DATA_FPGA_FRNUM_REQ	      MAIN_CTRL-->DATA_FPGA 
// 012	FPGA帧信息配置响应	DATA_FPGA_MAIN_CTRL_FRNUM_RSP	      DATA_FPGA-->MAIN_CTRL
#define CODE_MAIN_CTRL_DATA_FPGA_FRNUM_REQ   11
#define CODE_DATA_FPGA_MAIN_CTRL_FRNUM_RSP   12




// MsgType MSG_TYPE_DATA_FPGA_RRUS_MGMT
// MsgCode
/*
101	CPRI接口初始化配置               RRUS_MGMT_DATA_FPGA_CPRI_INIT_CFG	            RRUS_MGMT-->DATA_FPGA
102	CPRI接口初始化配置响应             DATA_FPGA_RRUS_MGMT_CPRI_INIT_CFG_RSP	        DATA_FPGA-->RRUS_MGMT
103	CPRI AXC通道配置	          RRUS_MGMT_DATA_FPGA_AXC_CHANNEL_CFG	        RRUS_MGMT-->DATA_FPGA
104	CPRI AXC通道配置响应	          DATA_FPGA_RRUS_MGMT_AXC_CHANNEL_CFG_RSP	    DATA_FPGA-->RRUS_MGMT
105	CPRI接口链路状态查询	          RRUS_MGMT_DATA_FPGA_CPRI_STATE_QUERY	        RRUS_MGMT-->DATA_FPGA
106	CPRI接口链路状态查询响应	          DATA_FPGA_RRUS_MGMT_CPRI_STATE_QUERY_RSP	    DATA_FPGA-->RRUS_MGMT
107	时延测量请求                    RRUS_MGMT_DATA_FPGA_TIME_DELAY_MEASURE_REQ	RRUS_MGMT-->DATA_FPGA
108	时延测量响应                    DATA_FPGA_RRUS_MGMT_TIME_DELAY_MEASURE_RSP	DATA_FPGA-->RRUS_MGMT
109	时延配置命令                    RRUS_MGMT_DATA_FPGA_TIME_DELAY_CFG_REQ	    RRUS_MGMT-->DATA_FPGA
110	时延配置响应                    DATA_FPGA_RRUS_MGMT_TIME_DELAY_CFG_RSP	    DATA_FPGA-->RRUS_MGMT
111	环回测试请求                    RRUS_MGMT_DATA_FPGA_RING_TEST_REQ	            RRUS_MGMT-->DATA_FPGA
112	环回测试响应                    DATA_FPGA_RRUS_MGMT_RING_TEST_RSP	            DATA_FPGA-->RRUS_MGMT
113	误码率测试请求                   RRUS_MGMT_DATA_FPGA_SER_TEST_REQ	            RRUS_MGMT-->DATA_FPGA
114	误码率测试响应                   DATA_FPGA_RRUS_MGMT_SER_TEST_RSP	            DATA_FPGA-->RRUS_MGMT
*/
#define CODE_RRUS_MGMT_DATA_FPGA_CPRI_INIT_CFG               101          
#define CODE_DATA_FPGA_RRUS_MGMT_CPRI_INIT_CFG_RSP           102
#define CODE_RRUS_MGMT_DATA_FPGA_AXC_CHANNEL_CFG             103
#define CODE_DATA_FPGA_RRUS_MGMT_AXC_CHANNEL_CFG_RSP         104
#define CODE_RRUS_MGMT_DATA_FPGA_CPRI_STATE_QUERY            105
#define CODE_DATA_FPGA_RRUS_MGMT_CPRI_STATE_QUERY_RSP        106
#define CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_MEASURE_REQ      107
#define CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_MEASURE_RSP      108
#define CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_CFG_REQ          109
#define CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_CFG_RSP          110
#define CODE_RRUS_MGMT_DATA_FPGA_RING_TEST_REQ               111
#define CODE_DATA_FPGA_RRUS_MGMT_RING_TEST_RSP               112
#define CODE_RRUS_MGMT_DATA_FPGA_SER_TEST_REQ                113
#define CODE_DATA_FPGA_RRUS_MGMT_SER_TEST_RSP                114
#define CODE_RRUS_MGMT_DATA_FPGA_RRU_MAC_REQ                 115
#define CODE_DATA_FPGA_RRUS_MGMT_RRU_MAC_RSP                 116












/*
CPRI接口初始化配置
CPRI应支持BBU侧CPRI接口速率的配置。可选速率2.5G，5G，10G。
该过程根据高层具体应用场景，可以动态配置。
参数	类型	含义
FiberNum	    UINT32	光口号
                        表明该消息配置的具体为哪个光口的CPRI，
                        低8位代表8个CPRI，7-0位代表8-1的CPRI。
CpriSpeed	    UINT32	CPRI速率
                        0 - 2.5G，
                        1 - 5G，
                        2 - 10G，
                        默认5G，可配
RRU_ID_Start    UINT32	RRU ID 起始值。
*/
#define CPRISPEED2HALF5G    0
#define CPRISPEED5G         1
#define CPRISPEED10G        2

typedef struct 
{
    UINT32          fiber_num;
    UINT32          cpri_speed;
    UINT32          rru_id_start;
}CPRI_INIT_CFG;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_INIT_CFG           init_cfg;
}MSG_CPRI_INIT_CFG;



/*
CPRI接口初始化配置响应
设定CPRI接口速率后对OM的响应。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其它失败。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      result;
}CPRI_INIT_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_INIT_CFG_RSP       init_cfg_rsp;
}MSG_CPRI_INIT_CFG_RSP;



/*
CPRI接口AXC通道配置
对BBU侧CPRI口AXC接口是否有效的配置。小区建立的时候进行配置。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
AxcSpeed	UINT32	1.4M，3M，5M，10M，15M，20M。
                    默认20M
                    从0开始。
AxcInd	    UINT32	每个bit用于指示AXC接口是否有效，取值为0或1。0无效，1有效
                    从低到高bit有效，以当前配置为最新配置，全部有效。
*/
/*
接口      小区      A*C数量       第1级       第2级         第3级       第4级
10G     20M     8           0/1       2/3         4/5       6/7
        10M     16          0/1/2/3   4/5/6/7     8/9/10/11 12/13/14/15
        5M      32          0/1/-7    8/9-15      16/17-23  24/25-31
5G      20M     4           0/1 2/3     
        10M     8           0/1 2/3   4/5 6/7
        5M      16          0/1-3     4/5-7       8/9-11    12/13-15
2.5G    20M     2           0/1         
        10M     4           0/1       2/3     
        5M      8           0/1       2/3         4/5       6/7
*/
#define AXCSPEED1P4M        0
#define AXCSPEED3M          1
#define AXCSPEED5M          2
#define AXCSPEED10M         3
#define AXCSPEED15M         4
#define AXCSPEED20M         5

typedef struct 
{
    UINT32      fiber_num;
    UINT32      axc_speed;
    UINT32      axc_ind;
}CPRI_AXC_CHANNEL_CFG;

typedef struct
{
    OmCpriMsgHead               msg_head;
    CPRI_AXC_CHANNEL_CFG        axc_channel_cfg;
}MSG_CPRI_AXC_CHANNEL_CFG;



/*
CPRI接口AXC通道配置响应
设定CPRI接口工作模式后对OM的响应。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      result;
}CPRI_AXC_CHANNEL_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead                   msg_head;
    CPRI_AXC_CHANNEL_CFG_RSP        axc_channel_cfg_rsp;
}MSG_CPRI_AXC_CHANNEL_CFG_RSP;



/*
CPRI接口链路状态查询
OM需要查询BBU侧CPRI接口的状态，以及光纤链路的状态。
此功能为周期性查询功能，两个状态同时查询。 发现有问题，则产生告警。
【此过程的实现由FPGA-driver模块去查询，查询后直接返回给高层OM】
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
*/
typedef struct 
{
    UINT32      fiber_num;
}CPRI_STATE_QUERY;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_STATE_QUERY        state_query;
}MSG_CPRI_STATE_QUERY;



/*
CPRI接口链路状态查询响应
CPRI接口状态查询结果返回给OM。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
State	    UINT32	低字节表示接口状态：
                    0： State A
                    1： State B
                    2： State C
                    3： State D
                    4： State E
                    5： State F
                    6： State G
                    次低字节表示链路状态：
                    返回结果
                    0：正常
                    1：LOS
                    2：LOF 
*/
#define CPRISTATEA          0
#define CPRISTATEB          1
#define CPRISTATEC          2
#define CPRISTATED          3
#define CPRISTATEE          4
#define CPRISTATEF          5
#define CPRISTATEG          6

#define CPRISTATENORMAL     0
#define CPRISTATELOS        1
#define CPRISTATELOF        2

typedef struct 
{
    UINT32      fiber_num;
    UINT32      state;
}CPRI_STATE_QUERY_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_STATE_QUERY_RSP    state_query_rsp;
}MSG_CPRI_STATE_QUERY_RSP;



/*
时延测量请求
OM发起对BBU侧光纤链路的时延测量请求。消息体暂无。
发起情况：在BBU启动和RRU都启动完成后，小区建立之前需要完成时延测量。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
*/
typedef struct 
{
    UINT32      fiber_num;
}CPRI_TIME_DELAY_MEASURE_REQ;

typedef struct 
{
    OmCpriMsgHead                   msg_head;
    CPRI_TIME_DELAY_MEASURE_REQ     time_delay_measure;
}MSG_CPRI_TIME_DELAY_MEASURE_REQ;



/*
时延测量响应
CPRI将单个光口的测量结果上报至OM。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
T14	        UINT32	为xx个cycle。10G按照1/245.76MHz为单位，5G按照122.88MHz，2.5G按照61.44MHz为单位来，
                     都统一成时间单位然后再折算成时钟周期数。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      T14;
}CPRI_TIME_DELAY_MEASURE_RSP;

typedef struct 
{
    OmCpriMsgHead                   msg_head;
    CPRI_TIME_DELAY_MEASURE_RSP     time_delay_measure_rsp;
}MSG_CPRI_TIME_DELAY_MEASURE_RSP;



/*
时延配置命令
配置AXC对应RRU的时延补偿值到CPRI接口，此后帧头的同步由CPRI完成。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
AxCInd	    UINT32	Bit有效，当前小区的AXC。
TimeDelayUL
(T12 + T2a)	UINT32	为xx个cycle。10G按照1/245.76MHz为单位，5G按照122.88MHz，2.5G按照61.44MHz为单位来，
                    都统一成时间单位然后再折算成时钟周期数。
TimeDelayDL
(T12 + Ta3)	UINT32	为xx个cycle。10G按照1/245.76MHz为单位，5G按照122.88MHz，2.5G按照61.44MHz为单位来，
                    都统一成时间单位然后再折算成时钟周期数。
*/
typedef struct
{
    UINT32      fiber_num;
    UINT32      axc_ind;
    UINT32      time_delay_ul;
    UINT32      time_delay_dl;
}CPRI_TIME_DELAY_CFG;

typedef struct
{
    OmCpriMsgHead               msg_head;
    CPRI_TIME_DELAY_CFG         time_delay_cfg;
}MSG_CPRI_TIME_DELAY_CFG;



/*
时延配置响应
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      result;
}CPRI_TIME_DELAY_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead               msg_head;
    CPRI_TIME_DELAY_CFG_RSP     time_delay_cfg_rsp;
}MSG_CPRI_TIME_DELAY_CFG_RSP;



/*
环回测试请求
OM向CPRI发出链路环回测试请求，要求CPRI进行链路环回测试。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
*/
typedef struct 
{
    UINT32      fiber_num;
}CPRI_RING_TEST_REQ;

typedef struct
{
    OmCpriMsgHead           msg_head;
    CPRI_RING_TEST_REQ      ring_test;
}MSG_CPRI_RING_TEST_REQ;



/*
环回测试响应
CPRI收到链路环回测试请求后，进行CPRI到RRU之间链路环回测试，并将测试结构通过该消息响应给BBU。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      result;
}CPRI_RING_TEST_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_RING_TEST_RSP      ring_test_rsp;
}MSG_CPRI_RING_TEST_RSP;



/*
误码率测试请求
OM发起（或手动）对CPRI链路的误码率测试请求，CPRI进行光纤链路的误码率测试流程。
【该功能在业务正常过程中不需要启动， 为设备的性能指标。保留该测试功能。】
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
CtlWord	    UINT8	人为控制，手动命令。
                    0X 01：BBU测试开始，RRU发。
                    0X 02：BBU停止测试，BBU保存结果
                    0X 03：BBU上报测试结果
                    0X 11：RRU测试开始，BBU发
                    0X 12：RRU停止测试，RRU保存结果
                    0X 13：RRU上报测试结果到BBU，BBU再上报OM
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT8       ctl_word;
}CPRI_SER_TEST_REQ;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_SER_TEST_REQ       ser_test_req;
}MSG_CPRI_SER_TEST_REQ;



/*
误码率测试响应
光纤链路误码率测试结束后，CPRI上报误码率测试结果。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
TotalH	    UINT32	测量32位字的总数，高32位
TotalL	    UINT32	测量32位字的总数，低32位
ErrorH	    UINT32	测量32位字的错误数，高32位
ErrorL	    UINT32	测量32位字的错误数，低32位
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      total_high;
    UINT32      total_low;
    UINT32      error_high;
    UINT32      error_low;
}CPRI_SER_TEST_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    CPRI_SER_TEST_RSP       ser_test_rsp;
}MSG_CPRI_SER_TEST_RSP;


/*
RRU MAC address REQ

参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。

Level 	UINT8
Rrru_mac[6]	   UINT8		MAC address of RRU
*/
typedef struct
{
    UINT32      fiber_num;
    UINT8		level;
    UINT8		rru_mac[6];
}RRU_MAC_REQ;

typedef struct
{
    OmCpriMsgHead           msg_head;
    RRU_MAC_REQ       	rru_mac_req;
}MSG_RRU_MAC_REQ;


/*
RRU MAC address RSP

参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。

Level 	UINT8
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct
{
    UINT32      fiber_num;
    UINT8		level;
    UINT32		result;
}RRU_MAC_RSP;

typedef struct
{
    OmCpriMsgHead           msg_head;
    RRU_MAC_RSP       	rru_mac_rsp;
}MSG_RRU_MAC_RSP;





// MsgType MSG_TYPE_DATA_FPGA_RRUS_MGMT
// MsgCode
/*
消息
编号	消息名称	消息标识	消息
方向
201	FPGA_PHY工作模式配置              RRUS_MGMT_DATA_FPGA_WORK_MODE_CFG	        RRUS_MGMT-->DATA_FPGA
202	FPGA_PHY工作模式配置响应            DATA_FPGA_RRUS_MGMT_WORK_MODE_CONFIG_RSP	DATA_FPGA-->RRUS_MGMT
203	FPGA单音信号配置                  RRUS_MGMT_DATA_FPGA_SINGLE_SEND_CFG	        RRUS_MGMT-->DATA_FPGA
204	FPGA单音信号配置响应                DATA_FPGA_RRUS_MGMT_SINGLE_SEND_CFG_RSP	    DATA_FPGA-->RRUS_MGMT
205	IQ数据输出配置                    RRUS_MGMT_DATA_FPGA_IQ_OUT_CFG	            RRUS_MGMT-->DATA_FPGA
206	IQ数据输出配置响应                  DATA_FPGA_RRUS_MGMT_IQ_OUT_CFG_RSP	        DATA_FPGA-->RRUS_MGMT
*/
#define CODE_RRUS_MGMT_DATA_FPGA_WORK_MODE_CFG           201
#define CODE_DATA_FPGA_RRUS_MGMT_WORK_MODE_CFG_RSP       202
#define CODE_RRUS_MGMT_DATA_FPGA_SINGLE_SEND_CFG         203
#define CODE_DATA_FPGA_RRUS_MGMT_SINGLE_SEND_CFG_RSP     204
#define CODE_RRUS_MGMT_DATA_FPGA_IQ_OUT_CFG              205
#define CODE_DATA_FPGA_RRUS_MGMT_IQ_OUT_CFG_RSP          206










/*
FPGA PHY工作模式配置
此为配置FPGA PHY工作模式的配置信息。为整个FPGA的配置，跟光口无关。
在FPGA启动后可配置其工作模式。 同时该工作模式也要同步配置给CPU上的基带程序，保持两者工作模式一致。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
                    默认全部光口都有效。
FpgaPhyMode	UINT32	0 LTE-NORMAL，压缩
                    1 LTE-NORMAL，不压缩
                    2 CPRI-BYPASS，压缩
                    3 CPRI-BYPASS，不压缩 默认
*/
#define LTENORMALCPRES      0
#define LTENORMALNOCPRES    1
#define CPRIBYPASSCPRES     2
#define CPRIBYPASSNOCPRES   3

typedef struct 
{
    UINT32      fiber_num;      // 此字段对此条消息无效。
    UINT32      fpga_phy_mode;
}FPGA_WORK_MODE_CFG;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    FPGA_WORK_MODE_CFG      work_mode_cfg;
}MSG_FPGA_WORK_MODE_CFG;



/*
FPGA_PHY工作模式配置响应
此为配置FPGA_PHY工作模式的配置信息的响应。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32          fiber_num;
    UINT32          result;
}FPGA_WORK_MODE_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    FPGA_WORK_MODE_CFG_RSP  work_mode_cfg_rsp;
}MSG_FPGA_WORK_MODE_CFG_RSP;



/*
单音信号模式配置
此为配置FPGA_PHY工作模式的配置信息。跟FPGA上具体CPRI光口和AxC通路有关，跟是否建立小区无关。
该接口为独立测试接口，可独立配置FPGA某个光口某部分AxC是否执行单音工作模式，用于对FPGA的测试。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
AxCInd	    UINT32	每个字节用于指示AXC接口是否为单音模式工作，取值为0或1。
                    0无效，1有效
                    从低到高bit有效。
OnOrOff	    UINT32	1为开，
                    0为关。 
SingleMode	UINT32	LTE-SINGLE TYPE （单音类型）
                    0 – 正弦波
                    1 - LTE 
*/
#define SINGLEOFF       0
#define SINGLEON        1

#define SINGLEMODESIN   0
#define SINGLEMODELTE   1

typedef struct 
{
    UINT32      fiber_num;
    UINT32      axc_ind;
    UINT32      on_or_off;
    UINT32      single_mode;
}SINGLE_MODE_CFG;

typedef struct 
{
    OmCpriMsgHead       msg_head;
    SINGLE_MODE_CFG     single_mode_cfg;
}MSG_SINGLE_MODE_CFG;



/*
单音信号模式配置响应
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32          fiber_num;
    UINT32          result;
}SINGLE_MODE_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    SINGLE_MODE_CFG_RSP     single_mode_cfg_rsp;
}MSG_SINGLE_MODE_CFG_RSP;



/*
IQ数据输出配置
此为配置FPGA_PHY工作模式的配置信息。
该配置为进行业务小区调试阶段使用的信息。该接口为独立接口，业务过程中，界面选择某个业务小区需要进行IQ数据分析， 
则OM需要根据小区标识对应出其光口号及AxC标识，并对FPGA完成配置。将业务小区的IQ数据输出到指定的MAC地址进行分析。
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
AxCInd	    UINT32	每个字节用于指示AXC接口是否为单音模式工作，取值为0或1。
                    0无效，1有效
                    从低到高bit有效。
OnOrOff	    UINT32	1为开
                    0为关。
DestMac[6]	UINT8	FPGA将IQ数据发送到的目的MAC地址。
*/
#define IQOUTOFF    0
#define IQOUTON     1

typedef struct 
{
    UINT32      fiber_num;
    UINT32      axc_ind;
    UINT32      on_or_off;
    UINT8       dest_max[6];    // 0x...
}IQ_OUT_SET_CFG;

typedef struct 
{
    OmCpriMsgHead       msg_head;
    IQ_OUT_SET_CFG      iq_out_set_cfg;
}MSG_IQ_OUT_SET_CFG;



/*
IQ数据输出配置响应
参数	类型	含义
FiberNum	UINT32	光口号
                    表明该消息配置的具体为哪个光口的CPRI，
                    低8位代表8个CPRI，7-0位代表8-1的CPRI。
Result	    UINT32	0成功，
                    其他失败。并表示具体原因。
*/
typedef struct 
{
    UINT32      fiber_num;
    UINT32      result;
}IQ_OUT_SET_CFG_RSP;

typedef struct 
{
    OmCpriMsgHead           msg_head;
    IQ_OUT_SET_CFG_RSP      iq_out_set_cfg_rsp;
}MSG_IQ_OUT_SET_CFG_RSP;























#pragma pack()

#endif
