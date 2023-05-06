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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:30 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_handle_main_entrance.c
 *Description: search all msg handle in rrus_mgmt.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/
/* Dependencies ------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
    
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_api_cell_mgmt.h"
#include "rrus_mgmt_api_cfig_mgmt.h"
#include "rrus_mgmt_api_main_ctrl.h"
#include "rrus_mgmt_api_mont_mgmt.h"
#include "rrus_mgmt_api_mac.h"
#include "rrus_mgmt_api_inet_recv.h"
#include "rrus_mgmt_api_inet_send.h"
#include "rrus_mgmt_api_timer.h"
#include "rrus_mgmt_api_rru.h"
#include "rrus_mgmt_api_data_fpga.h"
#include "rrus_mgmt_ir.h"
#include "rrus_mgmt_rru_entity_state_machine.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_common.h"


/*******************************declarations*****************************/
INT32    handle_from_cell_mgmt_msg(char*, int);
INT32    handle_from_cfig_mgmt_msg(char*, int);
INT32    handle_from_main_ctrl_msg(char*, int);
INT32    handle_from_mont_mgmt_msg(char*, int);
INT32    handle_from_inet_mgmt_msg(char*, int);
INT32	 handle_from_timer_msg(char*, int);


INT32    handle_from_mac_msg(char*, int);
INT32    handle_from_data_fpga_msg(char*, int);
INT32    handle_from_rru_msg(char*, int);


/* Functions ------------------------------------------------------------- */
/**********************************************
** search handler of cell_mgmt msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
/*
#define CODE_ICER_CPRI_AXC_CFG          001
#define CODE_ICER_CPRI_AXC_CFG_RET      002
#define CODE_ICER_RRU_REG_REQ           003
#define CODE_ICER_RRU_REG_RSP           004
*/

INT32 handle_from_cell_mgmt_msg(char *buf, int len)
{
    UINT32 msg_code;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buffer is NULL.\n");

        return FUNC_ERR;
    }
    msg_code = ((CommMsgHead*)buf)->msg_code;

    //buf = buf + sizeof(common_head);
    switch (msg_code)
    {
        case CODE_ICER_CPRI_AXC_CFG:
        {
            if (handle_icer_cpri_axc_cfg((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icer_cpri_axc_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code %d in handle_from_cell_mgmt_msg.\n", msg_code);
            break;
        }
            
    }

    
    return FUNC_OK;
}





/**********************************************
** search handler of cfig_mgmt msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
/*
#define CODE_ICR_INIT_INFO_CPRI                 001         // CPRI��ʼ����Ϣ
#define CODE_ICR_INIT_INFO_RRU                  002         // RRU��ʼ����Ϣ
#define CODE_ICR_RRU_INIT_RESULT_TRAP           003         // RRU��ʼ�������Ϣ�ϱ�
    
#define CODE_ICR_CPRI_RING_TEST_REQ             004         // CPRI���ز�������
#define CODE_ICR_CPRI_RING_TEST_RSP             005         // CPRI���ز�����Ӧ
#define CODE_ICR_CPRI_SER_TEST_REQ              006         // �����ʲ�������
#define CODE_ICR_CPRI_SER_TEST_FIN_RSP          007         // �����ʲ��������Ӧ
#define CODE_ICR_CPRI_SER_TEST_RET_QUERY        008         // �����ʲ��Խ����ѯ
#define CODE_ICR_CPRI_TIME_DELAY_RET_REPORT     009         // CPRIʱ�ӽ���ϱ�
#define CODE_ICR_CPRI_SINGLE_MODE_CFG           010         // ����ģʽ����
#define CODE_ICR_CPRI_SINGLE_MODE_CFG_RET       011         // ����ģʽ���ý��
#define CODE_ICR_FPGA_WORK_MODE_CFG             012         // FPGAPHY����ģʽ����
#define CODE_ICR_FPGA_WORK_MODE_CFG_RSP         013         // FPGAPHY����ģʽ������Ӧ
#define CODE_ICR_IQ_DATA_OUT_CFG                014         // IQ�����������
#define CODE_ICR_IQ_DATA_OUT_CFG_RSP            015         // IQ�������������Ӧ
    
#define CODE_ICR_RRU_PARAM_CFG                  016         // RRU��������
#define CODE_ICR_RRU_PARAM_CFG_RSP              017         // RRU����������Ӧ
#define CODE_ICR_RRU_RING_TEST_REQ              018         // RRU�ػ���������
#define CODE_ICR_RRU_RING_TEST_RSP              019         // RRU�ػ�������Ӧ
#define CODE_ICR_RRU_RESET_REQ                  020         // RRU��λ����
#define CODE_ICR_RRU_RESET_RSP                  021         // RRU��λ��Ӧ
#define CODE_ICR_SOFT_UPDATE_REQ                022         // RRU�����������
#define CODE_ICR_SOFT_UPDATE_RSP                023         // RRU���������Ӧ
*/

INT32 handle_from_cfig_mgmt_msg(char *buf, int len)
{
    UINT32 msg_code;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }

    msg_code = ((CommMsgHead*)buf)->msg_code;
    //buf = buf + sizeof(common_head);
    switch (msg_code)
    {
        case SET_INIT_INFO_CPRI_SPEED:
        {
            if (handle_icr_init_info_cpri((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_init_info_cpri return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_INIT_INFO_RRU:
        {
            if (handle_icr_init_info_rru((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_init_info_rru return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_CPRI_RING_TEST:
        {
            if (handle_icr_cpri_ring_test_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_cpri_ring_test_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_CPRI_SER_TEST:
        {
            if (handle_icr_cpri_ser_test_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_cpri_ser_test_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }


        case SET_CPRI_TIME_DELAY_CFG:
        {
            if (handle_icr_cpri_time_cfg_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_cpri_ser_test_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_SINGLE_SEND_CFG:
        {
            if (handle_icr_single_send_cfg((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_cpri_single_mode_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

//        case CODE_ICR_FPGA_WORK_MODE_CFG:
//        {
//            if (handle_icr_fpga_work_mode_cfg((void*)buf, len) == FUNC_ERR)
//            {
//                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_fpga_work_mode_cfg return error.\n");
//                return FUNC_ERR;
//            }
//            break;
//        }
//
//        case CODE_ICR_IQ_DATA_OUT_CFG:
//        {
//            if (handle_icr_iq_data_out_cfg((void*)buf, len) == FUNC_ERR)
//            {
//                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_iq_data_out_cfg return error.\n");
//                return FUNC_ERR;
//            }
//            break;
//        }

        case SET_RRU_PARAM_CPU_USAGE_PERIOD:
        {
            if (handle_icr_rru_param_cpu_usage_period((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_param_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_PARAM_VSWR_THRES:
        {
            if (handle_icr_rru_param_vswr_thres((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_param_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_PARAM_TEMP_THRES:
        {
            if (handle_icr_rru_param_temp_thres((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_param_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_PARAM_ANT_CFG:
        {
            if (handle_icr_rru_param_ant_cfg((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_param_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_PARAM_CHANNEL_STATE:
        {
            if (handle_icr_rru_param_channel_state((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_param_cfg return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_RING_TEST:
        {
            if (handle_icr_rru_ring_test_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_ring_test_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_RESET_REQ:
        {
            if (handle_icr_rru_reset_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_reset_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_SOFTWARE_UPDATE_REQ:
        {
            if (handle_icr_rru_soft_update_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_rru_soft_update_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case SET_RRU_TIME_DELAY_CFG:
         {
             if (handle_icr_rru_time_delay_cfg((void*)buf, len) == FUNC_ERR)
             {
                 log_msg(LOG_ERR, RRUS_MGMT, "Function handle_icr_cpri_ser_test_req return error.\n");
                 return FUNC_ERR;
             }
             break;
         }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code in handle_from_cfig_mgmt_msg.\n");
            break;
        }
    }

    
    return FUNC_OK;
}




/**********************************************
** search handler of main_ctrl msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
/*
#define CODE_IMAR_CPRI_INIT   001
*/

INT32 handle_from_main_ctrl_msg(char *buf, int len)
{
    UINT32 msg_code;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }
    msg_code = ((CommMsgHead*)buf)->msg_code;

    //buf = buf + sizeof(common_head);
    switch (msg_code)
    {
        case CODE_IMAR_CPRI_INIT:
        {
            if (handle_imar_cpri_init((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_imar_cpri_init return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code %d in handle_from_main_ctrl_msg.\n");
            break;
        }
           
        
        
    }
    
    return FUNC_OK;
}


/**********************************************
** search handler of mont_mgmt msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 handle_from_mont_mgmt_msg(char *buf, int len)
{
    return FUNC_OK;
}



/**********************************************
** search handler of inet_mgmt msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 handle_from_inet_mgmt_msg(char *buf, int len)
{
    return FUNC_OK;
}


/**********************************************
** search handler of mac msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
/*
#define CODE_IMR_CELL_BUILD_CFG_REQ    001
#define CODE_IMR_CELL_BUILD_CFG_RSP    002
#define CODE_IMR_CELL_DELETE_REQ       003
#define CODE_IMR_CELL_DELETE_RSP       004
*/

INT32 handle_from_mac_msg(char *buf, int len)
{
    UINT32 msg_code;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }

    msg_code = ((CommMsgHead*)buf)->msg_code;
    //buf = buf + sizeof(common_head);
    switch (msg_code)
    {
        case CODE_IMR_CELL_BUILD_CFG_REQ:
        {
            if (handle_imr_cell_build_cfg_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_imr_cell_build_cfg_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_IMR_CELL_DELETE_REQ:
        {
            if (handle_imr_cell_delete_req((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_imr_cell_delete_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code %d in handle_from_mac_msg.\n", msg_code);
            break;
        }
    }
    
    return FUNC_OK;
}


/**********************************************
** search handler of data_fpga msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/

/*
#define CODE_MAIN_CTRL_DATA_FPGA_FRNUM_REQ                      011
#define CODE_DATA_FPGA_MAIN_CTRL_FRNUM_RSP                      012
#define CODE_RRUS_MGMT_DATA_FPGA_CPRI_INIT_CFG                  101          
#define CODE_DATA_FPGA_RRUS_MGMT_CPRI_INIT_CFG_RSP              102
#define CODE_RRUS_MGMT_DATA_FPGA_AXC_CHANNEL_CFG                103
#define CODE_DATA_FPGA_RRUS_MGMT_AXC_CHANNEL_CFG_RSP            104
#define CODE_RRUS_MGMT_DATA_FPGA_CPRI_STATE_QUERY               105
#define CODE_DATA_FPGA_RRUS_MGMT_CPRI_STATE_QUERY_RSP           106
#define CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_MEASURE_REQ         107
#define CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_MEASURE_RSP         108
#define CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_CFG_REQ             109
#define CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_CFG_RSP             110
#define CODE_RRUS_MGMT_DATA_FPGA_RING_TEST_REQ                  111
#define CODE_DATA_FPGA_RRUS_MGMT_RING_TEST_RSP                  112
#define CODE_RRUS_MGMT_DATA_FPGA_SER_TEST_REQ                   113
#define CODE_DATA_FPGA_RRUS_MGMT_SER_TEST_RSP                   114
#define CODE_RRUS_MGMT_DATA_FPGA_WORK_MODE_CFG                  201
#define CODE_DATA_FPGA_RRUS_MGMT_WORK_MODE_CFG_RSP              202
#define CODE_RRUS_MGMT_DATA_FPGA_SINGLE_SEND_CFG                203
#define CODE_DATA_FPGA_RRUS_MGMT_SINGLE_SEND_CFG_RSP            204
#define CODE_RRUS_MGMT_DATA_FPGA_IQ_OUT_CFG                     205
#define CODE_DATA_FPGA_RRUS_MGMT_IQ_OUT_CFG_RSP                 206
*/

INT32 handle_from_data_fpga_msg(char *buf, int len)
{
	UINT16 msg_type,msg_code;
    UINT32 fpga_id,fibernum;
    UINT16 cpri_index;
    INT32 entity_id;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }
  //  common_head+OmCpriMsgHead+body -->cpri_index
    buf = buf + sizeof(CommMsgHead);
    msg_type = ntohs(((OmCpriMsgHead*)buf)->msg_type);
    if(msg_type != MSG_TYPE_DATA_FPGA_RRUS_MGMT)
    {
	   log_msg(LOG_ERR, RRUS_MGMT, "Not a valid msg_type from data fpga.\n");
	   return FUNC_ERR;
    }

    msg_code = ntohs(((OmCpriMsgHead*)buf)->msg_code);

	EqIndex *cpri = (EqIndex*)malloc(sizeof(EqIndex));

	memcpy(&fpga_id, buf + sizeof(OmCpriMsgHead) - sizeof(UINT32), sizeof(UINT32));
	memcpy(&fibernum, buf + sizeof(OmCpriMsgHead), sizeof(UINT32));
	cpri->fpga_id = ntohl(fpga_id);
	cpri->fiber_num = ntohl(fibernum);
	cpri->rru_id = 0;

	memcpy(&cpri_index, cpri,sizeof(UINT16));
	printf("fpga_id:%d\n",cpri->fpga_id);
	printf("fiber_num:%d\n",cpri->fiber_num);
    free(cpri);

	
	if((entity_id = id_to_cpri_entity(cpri_index)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}


    switch (msg_code)
    {
        case CODE_DATA_FPGA_MAIN_CTRL_FRNUM_RSP:
        {
            if (handle_data_fpga_frnum_rsp((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_frnum_req return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_CPRI_INIT_CFG_RSP:
        {
            if (handle_data_fpga_cpri_init_cfg_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_cpri_init_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_AXC_CHANNEL_CFG_RSP:
        {
            if (handle_data_fpga_axc_channel_cfg_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_axc_channel_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_CPRI_STATE_QUERY_RSP:
        {
            if (handle_data_fpga_cpri_state_query_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_cpri_state_query_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_MEASURE_RSP:
        {
            if (handle_data_fpga_time_delay_measure_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_time_delay_measure_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_TIME_DELAY_CFG_RSP:
        {
            if (handle_data_fpga_time_delay_cfg_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_time_delay_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_RING_TEST_RSP:
        {
            if (handle_data_fpga_ring_test_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_ring_test_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_SER_TEST_RSP:
        {
            if (handle_data_fpga_ser_test_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_ser_test_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_WORK_MODE_CFG_RSP:
        {
            if (handle_data_fpga_work_mode_cfg_rsp((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_work_mode_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_SINGLE_SEND_CFG_RSP:
        {
            if (handle_data_fpga_single_send_cfg_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_single_send_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_IQ_OUT_CFG_RSP:
        {
            if (handle_data_fpga_iq_out_cfg_rsp((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_iq_out_cfg_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_DATA_FPGA_RRUS_MGMT_RRU_MAC_RSP:
        {
            if (handle_data_fpga_rru_mac_rsp(cpri_index, (void*)buf) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_data_fpga_rru_mac_rsp return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code %d in handle_from_data_fpga_msg.\n", msg_code);
            break;
        }
    }
    return FUNC_OK;
}



/**********************************************
** search handler of rru msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/

/*
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

	MSG_CODE_BBU_RRU_AC_RESULT_REQ              = 1300, // ACУ׼�����ѯ 
	MSG_CODE_RRU_BBU_AC_RESULT_RSP              = 1301, // ACУ׼�����ѯ��Ӧ 

	MSG_CODE_BBU_RRU_UP_POWER_REQ               = 1310, // ���ʲ�ѯ 
	MSG_CODE_RRU_BBU_UP_POWER_RSP               = 1311, // ���ʲ�ѯ��Ӧ 

	MSG_CODE_RRU_BBU_TANSPARENT_MSG_BEGIN                   = 221,

	MSG_CODE_RRU_BBU_TANSPARENT_MSG_EANT_CFG_RSP            = 221,  // 221 �������������Ӧ 
	MSG_CODE_RRU_BBU_TANSPARENT_MSG_STATIC_POWER_CFG_RSP    = 222,  // 222 ��̬���ʵȼ�������Ӧ 
	MSG_CODE_RRU_BBU_TANSPARENT_MSG_END                     = 230,

	MSG_CODE_BBU_RRU_TANSPARENT_MSG_BEGIN                   = 231,
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_EANT_CFG_REQ            = 231,  //231 �����������
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_STATIC_POWER_CFG_REQ    = 232,  //232 ��̬���ʵȼ�����
	MSG_CODE_BBU_RRU_TANSPARENT_MSG_END                     = 240,
}IR_MSG_CODE;
*/


INT32 handle_from_rru_msg(char *buf, int len)
{
    UINT16 msg_code;
    UINT32 rru_id;
    INT32   event = -1;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }


    char *pir = buf + sizeof(CommMsgHead);

    if (len == RRU_IP_BROADCAST_LENGTH + sizeof(CommMsgHead))
    {
        if (handle_msg_rru_ip_req((void*)pir) == FUNC_ERR)
        {
            log_msg(LOG_ERR, RRUS_MGMT, "Function handle_rru_rru_ip_req return error.\n");
            return FUNC_ERR;
        }
        return FUNC_OK;
    }
    
    if ((msg_code = get_msg_code_from_ir((void*)pir, len - sizeof(CommMsgHead))) == FUNC_ERR)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Function get_msg_code_from_ir return error.\n");
        return FUNC_ERR;
    }


    if ((rru_id = get_rru_id_from_ir((void*)pir, len - sizeof(CommMsgHead))) == FUNC_ERR)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Function get_rru_id_from_id return error.\n");
        return FUNC_ERR;
    }

    switch(msg_code)
    {
        case MSG_CODE_RRU_BBU_HB:
        {
            if (handle_msg_rru_hub(rru_id, (void*)pir) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_rru_rru_hub error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case MSG_CODE_RRU_BBU_ALARM_REPORT:
        {
            if (handle_msg_alarm_report(rru_id, (void*)pir) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_rru_alarm_report error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case MSG_CODE_RRU_BBU_STATUS_QUERY_RSP:
        {
            print_ir_msg(msg_code, (UINT8*)pir);

            if (handle_msg_status_query_rsp(rru_id, (void*)pir) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_msg_status_query_rsp error.\n");
                return FUNC_ERR;
            }
            break;
        }
        case MSG_CODE_RRU_BBU_PARAM_QUERY_RSP:
        {
            print_ir_msg(msg_code, (UINT8*)pir);

            if (handle_msg_param_query_rsp(rru_id, (void*)pir) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_msg_param_query_rsp error.\n");
                return FUNC_ERR;
            }
            break;
        }
        default:
        {
            print_ir_msg(msg_code, (UINT8*)pir);

            if ((event = get_rru_event_from_ir((void*)pir, len - sizeof(CommMsgHead))) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function get_rru_event_from_ir.\n");
                return FUNC_ERR;
            }

            if (exec_rru_sm(rru_id, event, (void*)pir) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function exec_rru_sm return error.\n");
                return FUNC_ERR;
            }
            break;
        }
    }
    
    return FUNC_OK;
}


/**********************************************
** search handler of timer msg.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/

/*
#define CODE_TIMER_RRU_HUB_TIMEOUT                  0              
#define CODE_TIMER_BBU_HUB                          1
#define CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT         2
#define CODE_TIMER_RRU_NORMAL_MSG_RSP_TIMEOUT       3
#define CODE_TIMER_RRU_PARAM_QUERY                  4
#define CODE_TIMER_RRU_STATE_QUERY                  5
#define CODE_TIMER_CPRI_STATE_QUERY                 6
#define CODE_TIMER_CPRI_MSG_RSP_TIMEOUT             7
*/

INT32 handle_from_timer_msg(char *buf, int len)
{
    UINT16 msg_code;

    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Receive buf is NULL.\n");
        return FUNC_ERR;
    }

    msg_code = ((CommMsgHead*)buf)->msg_code;
    switch (msg_code)
    {
        case CODE_TIMER_RRU_HUB_TIMEOUT:
        {
            if (handle_timer_rru_hub_timeout((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_rru_hub_timeout return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_BBU_HUB:
        {

            if (handle_timer_bbu_hub_msg((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_bbu_hub return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT:
        {
            if (handle_timer_rru_cell_cfg_rsp_timeout((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_rru_cell_cfg_rsp_timeout return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_RRU_NORMAL_MSG_RSP_TIMEOUT:
        {
            if (handle_timer_rru_normal_msg_rsp_timeout((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_rru_normal_msg_rsp_timeout return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_RRU_PARAM_QUERY:
        {
            if (handle_timer_rru_param_query((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_rru_param_query return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_RRU_STATE_QUERY:
        {
            if (handle_timer_rru_state_query((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_rru_state_query return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_CPRI_STATE_QUERY:
        {
            if (handle_timer_cpri_state_query((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_cpri_state_query return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        case CODE_TIMER_CPRI_MSG_RSP_TIMEOUT:
        {
            if (handle_timer_cpri_msg_rsp_timeout((void*)buf, len) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Function handle_timer_cpri_msg_rsp_timeout return error.\n");
                return FUNC_ERR;
            }
            break;
        }

        default:
        {
            log_msg(LOG_WARNING, RRUS_MGMT, "Receive error msg_code %d in handle_from_timer_msg.\n", msg_code);
            break;
        }
    }
    return FUNC_OK;
}






