/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"


typedef struct
{
	UINT16		msg_type;
	UINT16		msg_code;
	UINT32		msg_len;
}OMHead;

typedef struct
{
	OMHead		om_head;
	UINT8		fpga_id;
	unsigned 	fiber_num:4;
	unsigned 	rru_id:4;
	UINT32		cpri_speed;
}INIT_INFO_CPRI_SPEED;

typedef struct
{
	OMHead			om_head;
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
}INIT_INFO_RRU;

typedef struct
{
	OMHead			om_head;
	UINT8			DesMac[6];		//CPU侧接收帧信息MAC地址
}CPRI_INIT_TRIGGER;

typedef struct
{
	OMHead			om_head;
	UINT8			fpga_id;
	unsigned 		fiber_num:4;
	unsigned 		rru_id:4;
    UINT32          axc_ind;
    UINT32          axc_speed;
}CPRI_AXC_CFG;

typedef struct
{
	OMHead			om_head;
	UINT8			fpga_id;
	unsigned 		fiber_num:4;
	unsigned 		rru_id:4;
}CPRI_RING_TEST;

typedef struct
{
	OMHead			om_head;
	UINT8			fpga_id;
	unsigned 		fiber_num:4;
	unsigned 		rru_id:4;
	UINT32			ctrl_word;
}CPRI_SER_TEST;

typedef struct
{
	OMHead			om_head;
	UINT16		   rru_id;
	UINT32		   ring_test_type;
    UINT16         check_period;
    UINT8          port_no;
}RRU_RING_TEST;

typedef struct
{
	OMHead			om_head;
	UINT16			rru_id;
}RRU_RESET;

typedef struct
{
	OMHead			om_head;
	UINT16			rru_id;
}RRU_VER_UPDATE;

//extern INT32 send_msg_to_module(MsgqType msgq/*UINT16 module_id*/,UINT32 socktype,char *msg_p,UINT32 msg_len);

//void *cfig_mgmt_entrance()
//{
//
//    log_msg(LOG_INFO, CFIG_MGMT, "CFIG_MGMT init success, enter while...\n");
//    sleep(3);
//	char buffer[MAX_MSGQ_MSG_LEN];
//	UINT32 msgCode;
//    while(1)
//    {
//    	sleep(1);
//    	/*send control data*/
//    	memset(buffer,0,sizeof(buffer));
//    	printf("input message:");
//    	fgets(buffer,MAX_MSGQ_MSG_LEN,stdin);
//    	msgCode = atoi(buffer);
//    	if(msgCode == 1)
//    	{
//    		INIT_INFO_CPRI_SPEED *init_cpri_cpeed = (INIT_INFO_CPRI_SPEED*)malloc(sizeof(INIT_INFO_CPRI_SPEED));
//    		init_cpri_cpeed->om_head.msg_type = MODULE_CFIG_MGMT;
//    		init_cpri_cpeed->om_head.msg_code = 1;
//    		init_cpri_cpeed->om_head.msg_len = sizeof(INIT_INFO_CPRI_SPEED);
//    		init_cpri_cpeed->fpga_id = 1;
//    		init_cpri_cpeed->fiber_num = 2;
//    		init_cpri_cpeed->rru_id = 0;
//    		init_cpri_cpeed->cpri_speed = 1;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)init_cpri_cpeed, sizeof(INIT_INFO_CPRI_SPEED));
//
//    		continue;
//    	}
//    	if(msgCode == 2)
//    	{
//    		INIT_INFO_RRU *init_cpri_rru = (INIT_INFO_RRU*)malloc(sizeof(INIT_INFO_RRU));
//    		memset(init_cpri_rru,0,sizeof(INIT_INFO_RRU));
//    		init_cpri_rru->om_head.msg_type = MODULE_CFIG_MGMT;
//    		init_cpri_rru->om_head.msg_code = 2;
//    		init_cpri_rru->om_head.msg_len = sizeof(INIT_INFO_RRU);
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)init_cpri_rru, sizeof(INIT_INFO_RRU));
//
//    		continue;
//    	}
//    	if(msgCode == 3)
//    	{
//    		CPRI_INIT_TRIGGER *cpri_init = (CPRI_INIT_TRIGGER*)malloc(sizeof(CPRI_INIT_TRIGGER));
//    		memset(cpri_init,0,sizeof(CPRI_INIT_TRIGGER));
//    		cpri_init->om_head.msg_type = MODULE_MAIN_CTRL;
//    		cpri_init->om_head.msg_code = 1;
//    		cpri_init->om_head.msg_len = sizeof(CPRI_INIT_TRIGGER);
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)cpri_init, sizeof(CPRI_INIT_TRIGGER));
//
//    		continue;
//    	}
//
//      	if(msgCode == 11)
//        {
//      		CPRI_AXC_CFG *axc_cfg = (CPRI_AXC_CFG*)malloc(sizeof(CPRI_AXC_CFG));
//        	memset(axc_cfg,0,sizeof(CPRI_AXC_CFG));
//        	axc_cfg->om_head.msg_type = MODULE_CELL_MGMT;
//        	axc_cfg->om_head.msg_code = 1;
//        	axc_cfg->om_head.msg_len = sizeof(CPRI_AXC_CFG);
//        	axc_cfg->fpga_id = 1;
//        	axc_cfg->fiber_num = 2;
//        	axc_cfg->axc_ind = 3;
//        	axc_cfg->axc_speed = 1;
//        	send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)axc_cfg, sizeof(CPRI_AXC_CFG));
//        	continue;
//        }
//
//    	if(msgCode == 12)
//    	{
//    		CPRI_RING_TEST *ring_test = (CPRI_RING_TEST*)malloc(sizeof(CPRI_RING_TEST));
//    		memset(ring_test,0,sizeof(CPRI_RING_TEST));
//    		ring_test->om_head.msg_type = MODULE_CFIG_MGMT;
//    		ring_test->om_head.msg_code = 3;
//    		ring_test->om_head.msg_len = sizeof(CPRI_RING_TEST);
//    		ring_test->fpga_id = 1;
//    		ring_test->fiber_num = 2;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)ring_test, sizeof(CPRI_RING_TEST));
//    		continue;
//    	}
//    	if(msgCode == 13)
//    	{
//    		CPRI_SER_TEST *ser_test = (CPRI_SER_TEST*)malloc(sizeof(CPRI_SER_TEST));
//    		memset(ser_test,0,sizeof(CPRI_SER_TEST));
//    		ser_test->om_head.msg_type = MODULE_CFIG_MGMT;
//    		ser_test->om_head.msg_code = 4;
//    		ser_test->om_head.msg_len = sizeof(CPRI_SER_TEST);
//    		ser_test->fpga_id = 1;
//    		ser_test->fiber_num = 2;
//    		ser_test->ctrl_word = 1;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)ser_test, sizeof(CPRI_SER_TEST));
//    		continue;
//    	}
//
//    	if(msgCode == 21)
//    	{
//    		RRU_RING_TEST *rru_ring = (RRU_RING_TEST*)malloc(sizeof(RRU_RING_TEST));
//    		memset(rru_ring,0,sizeof(RRU_RING_TEST));
//    		rru_ring->om_head.msg_type = MODULE_CFIG_MGMT;
//    		rru_ring->om_head.msg_code = 12;
//    		rru_ring->om_head.msg_len = sizeof(RRU_RING_TEST);
//    		rru_ring->rru_id = 0;
//    		rru_ring->ring_test_type = 0;
//    		rru_ring->check_period = 10;
//    		rru_ring->port_no = 0;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)rru_ring, sizeof(RRU_RING_TEST));
//    		continue;
//    	}
//    	if(msgCode == 22)
//    	{
//    		RRU_RESET *reset = (RRU_RESET*)malloc(sizeof(RRU_RESET));
//    		memset(reset,0,sizeof(RRU_RESET));
//    		reset->om_head.msg_type = MODULE_CFIG_MGMT;
//    		reset->om_head.msg_code = 13;
//    		reset->om_head.msg_len = sizeof(RRU_RESET);
//    		reset->rru_id = 0;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)reset, sizeof(RRU_RESET));
//    		continue;
//    	}
//    	if(msgCode == 23)
//    	{
//    		RRU_VER_UPDATE *rru_ver_update = (RRU_VER_UPDATE*)malloc(sizeof(RRU_VER_UPDATE));
//    		memset(rru_ver_update,0,sizeof(RRU_VER_UPDATE));
//    		rru_ver_update->om_head.msg_type = MODULE_CFIG_MGMT;
//    		rru_ver_update->om_head.msg_code = 14;
//    		rru_ver_update->om_head.msg_len = sizeof(RRU_VER_UPDATE);
//    		rru_ver_update->rru_id = 0;
//    		send_msg_to_module(MSGQ_ID_RRUS_MGMT, 0, (char*)rru_ver_update, sizeof(RRU_RESET));
//    		continue;
//    	}
//    }
//
//}
