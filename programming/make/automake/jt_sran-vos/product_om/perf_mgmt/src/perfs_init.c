/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: perfs_init.c
** Description:  performance statistics module's  init
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/

/* Globals -----------------------------------------------------------------------------------------------------------*/
vos_module_t    perfs_module;
ULONG           perfs_msgqid;     /* perf mgmt msgq id */

PerfCell        PerfCells[PERF_CELL_NUM];

Sem_t           perf_cell_entity_sem;

TaskCreateRet   perf_task_ctrl;

perfs_sm_handler_t  perfs_sm[PERF_MGMT_STATE_MAX][PERF_MGMT_EVENT_MAX];


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_module_init()
{
    log_perf_debug();

    INT32 ret = PERF_ERR;

    /**
     *  Open messages queue
     */
    ret = perfs_msgqueue_init();
    if(ret == PERF_ERR)
    {
        log_perf_err("open message queue error.\n");
        return PERF_ERR;
    }

    /**
     *  This is one of input of "VOS_module_register"
     */
    if(perfs_msgqid == 0)
    {
        log_perf_err("msgid is zero\n");
        return PERF_OK;
    }
    perfs_module.queueID = perfs_msgqid;
    
    /**
     *  Register cell_mgmt module
     */
    ret = VOS_module_register(PERF_MGMT_NAME, &perfs_module);
    if(ret != PERF_OK)
    {
        log_perf_err("%s module register error.\n", PERF_MGMT_NAME);
        return PERF_ERR;
    }
    log_perf_succ("%s module register success.\n", PERF_MGMT_NAME);

    
    /**
     *  init cell entity
     */
    ret = perfs_cells_init();
    if(ret == PERF_ERR)
    {
        log_perf_err("init cell entity error.\n");
        return PERF_ERR;
    }

    /**
     *  Create main control task thread
     */
    ret = perfs_task_init();
    if(ret == PERF_ERR)
    {
        log_perf_err("create task error.\n");
        return PERF_ERR;
    }
    
    /**
     *  State Machine initial
     */
    ret = perfs_sm_init();
    if(ret == PERF_ERR)
    {
        log_perf_err("State Machine initial error.\n");
        return PERF_ERR;
    }

    /**
     *  Register to CELL_MGMT module, need to some CELL_MGMT event\message
     */
    ret = cell_mgmt_module_register(perfs_module.moduleID, CELLOK|CELLNULL, 
                                    register_to_cell_mgmt_callback);
    if(ret == PERF_ERR)
    {
        log_perf_err("maybe cell_mgmt_module_register error.\n");
        //return PERF_ERR;
    }
    log_perf_succ("maybe cell_mgmt_module_register success.\n");


    return PERF_OK;
}
weak_alias(perfs_module_init, perf_mgmt_module_init);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_clean_up()
{
    log_perf_debug();

    Sem_destroy(perf_cell_entity_sem);


    

    return PERF_OK;
}
weak_alias(perfs_clean_up, perf_mgmt_clean_up);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32   perfs_msgqueue_init()
{
    log_perf_debug();

	perfs_msgqid = MsgqCreate();
	if (0 == perfs_msgqid)
	{
        log_perf_err("create msgq error.\n");

		return PERF_ERR;
	}
    log_perf_info("create msgq success.\n");

    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_cells_init()
{
    log_perf_debug();

    /**
     *  Memset 0
     */
    Memset(PerfCells, 0, sizeof(PerfCells));

    /**
     *  Create semaphore to mutex cells 
     */
    perf_cell_entity_sem = Sem_init();
    if(perf_cell_entity_sem == 0)
    {
        log_perf_err("init semaphore error.\n");
        return PERF_ERR;
    }


    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_sm_init()
{
    log_perf_debug();

    INT32 ret = PERF_ERR;

    /**
     *  Memset 0
     */
    Memset(perfs_sm, 0, sizeof(perfs_sm));

    
#define _add_perf_sm_entry(state, event, handler) \
    do{\
        ret = add_perf_sm_entry(state, event, handler); \
        if(ret != PERF_OK) goto quit_error;\
    }while(0)

    _add_perf_sm_entry(PERF_INVALIDE_STATE, PERF_EVENT_CELL_OK, sm_handler_cell_ok);
    _add_perf_sm_entry(PERF_VALIDE_STATE, PERF_EVENT_CELL_NULL, sm_handler_cell_null);
    _add_perf_sm_entry(PERF_VALIDE_STATE, PERF_EVENT_MAC_PERF_RSP, sm_handler_mac_perf_rsp);
    _add_perf_sm_entry(PERF_VALIDE_STATE, PERF_EVENT_SIMUPHY_PERF_RSP, sm_handler_simuphy_perf_rsp);
    _add_perf_sm_entry(PERF_VALIDE_STATE, PERF_EVENT_REALPHY_PERF_RSP, sm_handler_realphy_perf_rsp);

    log_perf_succ("add_perf_sm_entry Success.\n");

    return PERF_OK;

quit_error:
    log_perf_err("add_prfm_sm_entry Error.\n");
    return PERF_ERR;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 add_perf_mgmt_sm_entry(PerfState state, char *statename, PERF_MGMT_EVENT event, char *eventname,
                            PerfSmHandleFn fn, char *fnname)
{
    log_perf_debug();

    if(state <= PERF_MGMT_STATE_MIN || state >= PERF_MGMT_STATE_MAX)
    {
        log_perf_err("error state (%d)\n", state);
        return PERF_ERR;
    }
    if(event <= PERF_MGMT_EVENT_MIN || event >= PERF_MGMT_EVENT_MAX)
    {
        log_perf_err("error event (%d)\n", event);
        return PERF_ERR;
    }
    if(statename == NULL || eventname == NULL || fnname == NULL)
    {
        log_perf_err("NULL pointer error\n");
        return PERF_ERR;
    }
    //log_perf_info("\n State: %s\nEvent: %s\n Handler:%s\n",statename,eventname,fnname);
    perfs_sm[state][event].Statename    = statename;
    perfs_sm[state][event].Eventname    = eventname;
    perfs_sm[state][event].Handlername  = fnname;
    perfs_sm[state][event].handler      = fn;

    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 do_perf_mgmt_sm(UINT32 cell_id, PERF_MGMT_EVENT sm_event, void *arg)
{
    
    if((cell_id < MIN_CELL_ID || cell_id > MAX_CELL_ID) && cell_id != ALL_CELLS)
    {
        log_perf_err("cell id out of range (%d)\n", cell_id);
        return PERF_ERR;
    }

    if(sm_event <= PERF_MGMT_EVENT_MIN || sm_event >= PERF_MGMT_EVENT_MAX)
    {
        log_perf_err("error event (%d)\n", sm_event);
        return PERF_ERR;
    }

    if(arg == NULL)
    {
        log_perf_err("NULL pointer error\n");
        return PERF_ERR;
    }

    if(cell_id == ALL_CELLS)
    {
        INT32 ret = PERF_ERR;
        UINT32 id, n;
        n = CELL_NUM;//g_perf_cell_number>CELL_NUM?CELL_NUM:g_perf_cell_number;
        
        for(id=0; id <= n; id++)
        {
            PerfState state = PerfCells[id-MIN_CELL_ID].state;
            PerfSmHandleFn fn = perfs_sm[state][sm_event].handler;

            if(fn) 
            {
                ret = fn(id, arg);
                if(ret == PERF_ERR)
                {
                    log_perf_err("handler state(%d) event(%d) id(%d) Error.\n", state, sm_event, id);
                    return PERF_ERR;
                }
            }
            else
            {
                continue;
            }
        }
    }
    else
    {
        INT32 ret = PERF_ERR;
        PerfState state = PerfCells[cell_id-MIN_CELL_ID].state;
        PerfSmHandleFn fn = perfs_sm[state][sm_event].handler;
        if(fn) 
        {
            ret = fn(cell_id, arg);
            if(ret == PERF_ERR)
            {
                log_perf_err("handler state(%d) event(%d) id(%d) Error.\n", state, sm_event, cell_id);
                return PERF_ERR;
            }
        }
    }
    display_cell_performance(STDOUT, cell_id);
    
    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_check_paras_valid()
{
    log_perf_debug();

    

    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_task_init()
{
    log_perf_debug();

    perf_task_ctrl = TaskCreate(PERF_MGMT_MAIN_TASK_NAME, perfs_task_main_ctrl, NULL);

    if (NULL == perf_task_ctrl)
	{
		log_perf_err("Can't create perfs_task_main_ctrl thread \n");
		return PERF_ERR;
	}
	log_perf_info("create create task success, the thread_id is %d. \n", PERF_MGMT_MAIN_TASK_NAME);



    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 perfs_task_main_ctrl(VOID *arg)
{
    log_perf_info("pid(%d), tid(%d)\n", getpid(), GET_TID());

    while(1)
    {
        //log_perf_info("Main Ctrl ...\n");


        

        
        Sleep(1);
    }

    return PERF_OK;
#if 0
Error_loop:

    perfs_clean_up();
    return PERF_ERR;
#endif
}





