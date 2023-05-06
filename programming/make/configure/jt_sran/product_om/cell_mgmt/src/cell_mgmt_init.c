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
/******************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: cell_mgmt_init.c
** Description: some of CELL_MGMT module's initialized function
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "sbs_type.h"
#include "sbs_log.h"

#include "util_systemer.h"
#include "util_biter.h"
#include "util_printer.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"

#include "inet_mgmt.h"
#include "rrus_mgmt_rru_entity.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
//MsgqType        MSGQ_ID_CELL_MGMT;              /*msg queue id*/
//TaskType        THREAD_CELL_MGMT;               /*cell_mgmt thread id*/
//UINT8           flag_thread_cell_mgmt;          /*Task flag defined in OM*/
TaskType        CELL_MGMT_TIMER_CTRL_TASK;      /*timer ctrl task*/

VcEntityNode    vc_entity_table[MAX_CELL_NUM];  
RruInfo         rru_info_table[MAX_RRU_NUM];    
INT8            phy_state_table[MAX_PHY_INDEX]; 

INT32           unsockfd_mac_server, unsockfd_phy_server;

/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: cell mgmt module initialized function for "main()"
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_module_init()
/*log_style: WRITE2FILE, log_format:LOG_COMPLETE, log_level:LOG_INFO */
{    
    /**
     * Test MAC
     */
#define __CELL_MGMT_TEST_MAC__
#ifdef  __CELL_MGMT_TEST_MAC__
    int cvid;
    scanf("%d", &cvid);
    sh_cmd("rm -rf %s/*", PATH_CELLS);
    sh_cmd("mkdir %s/%d", PATH_CELLS, cvid);
    getchar();
    sh_cmd("cp -a %s/* %s/%d ", MAC_SRC_PATH_DEFAULT, PATH_CELLS, cvid);
    getchar();
    set_mac_init(cvid);
    getchar();
    set_mac_kill(cvid);
    exit(1);
#endif

    log_msg(LOG_INFO, CELL_MGMT, "OM_VCM Starting... %s\n", get_system_time());

    /**************************************************
     * open the message queue for the cell_mgmt_msgq.
     **************************************************/    
    info("%s:%d cell_mgmt open_msgq: %s, num:%d, len:%d\n", __func__, __LINE__, 
            CELL_MGMT_MSGQ_NAME, MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);

	MSGQ_ID_CELL_MGMT = open_msgq(CELL_MGMT_MSGQ_NAME, 
                                    MAX_MSGQ_MSG_NUM, 
                                    MAX_MSGQ_MSG_LEN);
	if (-1 == MSGQ_ID_CELL_MGMT)
	{
		log_msg(LOG_ERR, CELL_MGMT, "can't open cell_mgmt_msgq. %s\n", 
                CELL_MGMT_MSGQ_NAME);
                
        error("%s:%d cell_mgmt can't open cell_mgmt_msgq %s\n",
                __func__, __LINE__, CELL_MGMT_MSGQ_NAME);
		log_cleanup();

		return FUNC_ERR;
	}
    else 
    {
		log_msg(LOG_INFO, CELL_MGMT, "cell_mgmt open_msgq success %s\n", 
                CELL_MGMT_MSGQ_NAME);

        success("%s:%d cell_mgmt open_msgq success %s\n",
                __func__, __LINE__, CELL_MGMT_MSGQ_NAME);
    }
    
    /****************************************
     * cell_mgmt state machine initialized
     ****************************************/ 
    info("%s:%d State Machine Initializing...\n", __func__, __LINE__);

    if(FUNC_ERR == cell_mgmt_sm_init())
    {
        error("%s:%d State Machine Initialized Error.\n", __func__, __LINE__);
    }
    else
    {
        success("%s:%d State Machine Initialized Successfully.\n",
                __func__, __LINE__);
    }
    
    log_msg(LOG_INFO, CELL_MGMT, "cell_mgmt_sm_init Init is done. \n");
    
    /********************************
     * CPRI-AxC table Initialized
     ********************************/  
#if 0 /* pass follow code if 0-endif*/
    info("CPRI-AxC Check Table Initializing...\n");
    cpri_axc_check_table_init();
    success("CPRI-AxC Check Table Initialized Successfully.\n");
    log_msg(LOG_INFO, CELL_MGMT, "cpri_axc_table_init Init is done. \n");
#endif
      
    /***************************
     * cell entity initialized
     ***************************/
    if(FUNC_ERR == cell_mgmt_cell_entity_init())
    {
        error("%s:%d Cell Entity Initialized Error.\n", __func__, __LINE__);
    }
    else
    {
        success("%s:%d Cell Entity Initialized.\n", __func__, __LINE__);
    }
    
    log_msg(LOG_INFO, CELL_MGMT, "Cell Entity Initialized successful.\n");

    /*******************************************
     *  Initialized of unsocket for MAC and PHY
     *******************************************/
    if(FUNC_OK == cell_mgmt_unsocket_init())
    {
        success("%s:%d inet initial SUCCESS!\n", __func__, __LINE__);
        log_msg(LOG_INFO, CELL_MGMT, "inet initial SUCCESS\n");
    }
    else
    {
        error("%s:%d inet initial ERROR!\n", __func__, __LINE__);
        log_msg(LOG_ERR, CELL_MGMT, "inet initial Error\n");
        return FUNC_ERR;
    }
    
    
    /****************************************
     *  CELL_MGMT momdule main ctrl function
     ****************************************/
    info("Create Task Thread for cell_mgmt_main_ctrl ...\n");

    THREAD_CELL_MGMT = create_pthread(1,  
                                       SCHED_RR,  
                                       TASK_JOIN,  
                                       (FUNCPTR)cell_mgmt_module_main_ctrl, 
                                       NULL);
                                       
	if (-1 == THREAD_CELL_MGMT)
	{
		log_msg(LOG_ERR, CELL_MGMT, 
                "can't create cell_mgmt_module_main_ctrl thread \n");

		close_msgq(CELL_MGMT_MSGQ_NAME, MSGQ_ID_CELL_MGMT);

        error("%s:%d Create Task Thread Failed.\n", __func__, __LINE__);

		return FUNC_ERR;
	}
	else
	{
		log_msg(LOG_INFO, CELL_MGMT, 
                "create create_pthread success, the thread_id is %d. \n", 
                THREAD_CELL_MGMT);
        success("%s:%d Create Task main Ctrl Thread Successfully.\n",
                __func__, __LINE__);
	}
    
    //pthread_join(THREAD_CELL_MGMT, NULL);
    
    success("%s:%d Quit cell_mgmt_module!\n", __func__, __LINE__);

    log_msg(LOG_INFO, CELL_MGMT, 
            "%s:%d Quit cell_mgmt_module!\n", __func__, __LINE__);

    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity table initialized
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_cell_entity_init()
{
    debug();

    warning("%s:%d memset(..., 0, ...)\n", __func__, __LINE__);

    INT32 cell_virt_id, rru_id, phy_id;

    /**
     *  cell mode string array init
     */
    vc_mode_to_str(VC_MODE_MIN);
    vc_mode_to_str(GSM);
    vc_mode_to_str(GMR);
    vc_mode_to_str(WCDMA);
    vc_mode_to_str(TD_SCDMA);
    vc_mode_to_str(TDD_LTE);
    vc_mode_to_str(FDD_LTE);
    vc_mode_to_str(VC_MODE_MAX);

    /**
     * cell parameters initial
     */
    for(cell_virt_id=MIN_CELL_NUM; cell_virt_id<=MAX_CELL_NUM; cell_virt_id++)
    {
        if(NULL == memset(&vc_entity_table[cell_virt_id-MIN_CELL_NUM], 
                          0, sizeof(VcEntityNode)))
        {
            error("%s:%d memset Error\n", __func__, __LINE__);
            return FUNC_ERR;
        }
    }
    
    /**
     * rru state initial
     */
    for(rru_id=1; rru_id<=MAX_RRU_NUM; rru_id++)
    {
        if(NULL == memset(&rru_info_table[rru_id-1], 0, sizeof(RruInfo)))
        {
            error("%s:%d memset Error\n", __func__, __LINE__);
            return FUNC_ERR;
        }
        rru_info_table[rru_id-1].rru_ip[0] = '\0';
    }
    /**
     * phy state initial
     */
    for(phy_id=1; phy_id<=MAX_PHY_INDEX; phy_id++)
    {
        phy_state_table[phy_id-1] = 0;
    }
    
    
    
    success("vc_entity_table init done. %s\n", get_system_time());
    
    log_msg(LOG_INFO, CELL_MGMT, "cell_mgmt_cell_entity_init is done. %s\n",
            get_system_time());

    return FUNC_OK;
}

/******************************************************************************
 * Description: mac and phy's unsocket initialized
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_unsocket_init()
{
    debug();
    
    
    /*register mac unsocket. some val defined in "cell_mgmt_INET.h"*/
    unsockfd_mac_server = unsocket_register(CELL_MGMT_MSGQ_INVALID, //0
                                            callback_mac_inet,      //cb fn
                                            CALLBACK_FUNC_VALID,    //1
                                            MAC_SUN_PATH,           //
                                            SOCK_STREAM_VALID);     //1
                                            
    if(unsockfd_mac_server == -1)
    {
        error("%s:%d mac unsocket_register error!\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    /*register phy unsocket. some val defined in "cell_mgmt_INET.h"*/
    unsockfd_phy_server = unsocket_register(CELL_MGMT_MSGQ_INVALID, //0
                                            callback_phy_inet,      //cb fn
                                            CALLBACK_FUNC_VALID,    //1
                                            PHY_SUN_PATH,           //
                                            SOCK_STREAM_VALID);     //1

    if(unsockfd_phy_server == -1)
    {
        error("%s:%d phy unsocket_register error!\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    success("%s:%d cell_mgmt's inet initial success!\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell_mgmt clean up function for "ctrl-c". etc.
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_clean_up()
{
    debug();
    
	if(socket_unregister(unsockfd_mac_server) == -1)
	{
        error("%s:%d mac socket unregister error!\n", __func__, __LINE__);
		return FUNC_ERR;
	}
	if(socket_unregister(unsockfd_phy_server) == -1)
	{
        error("%s:%d phy socket unregister error!\n", __func__, __LINE__);
		return FUNC_ERR;
	}
    success("%s:%d mac&phy socket unregister success!\n", __func__, __LINE__);
	return FUNC_OK;
}

