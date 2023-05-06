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
** File name: perfs_handler.c
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
extern PerfCell        PerfCells[PERF_CELL_NUM];

extern vos_module_t    perfs_module;
extern ULONG           perfs_msgqid;     /* perf mgmt msgq id */


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_simuphy_perf_rsp(UINT32 cell_id, void*arg)
{
    log_perf_debug();

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    /**
     *  Save simulate performance parameters
     */
    perf_outer_msg_perf_check_simuphy_rsp perf_simuphy_rsp;
    Memcpy(&perf_simuphy_rsp, arg, sizeof(perf_outer_msg_perf_check_simuphy_rsp));

    Memcpy(&(PerfCells[cell_id-MIN_CELL_ID].simuphy), &(perf_simuphy_rsp.prfm), sizeof(perf_simuphy_t));
    
    return PERF_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_realphy_perf_rsp(UINT32 cell_id, void*arg)
{
    log_perf_debug();

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    /**
     *  Save virtual/real performance parameters
     */
    perf_outer_msg_perf_check_realphy_rsp perf_realphy_rsp;
    Memcpy(&perf_realphy_rsp, arg, sizeof(perf_outer_msg_perf_check_realphy_rsp));

    Memcpy(&(PerfCells[cell_id-MIN_CELL_ID].realphy), &(perf_realphy_rsp.prfm), sizeof(perf_realphy_t));

    Fprintf(STDOUT, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    Memdisplay(STDOUT, &perf_realphy_rsp, sizeof(perf_outer_msg_perf_check_realphy_rsp), 20);
    Fprintf(STDOUT, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    
    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
VOID timer_check_phy_performance_callback(VOID *arg)
{
    log_perf_debug();

    UINT32 cell_id _unused  = indexofarray(PerfCell, PerfCells, arg) + MIN_CELL_ID;

    /**
     *  Send message to (perf_mgmt msgqueue)main ctrl thread
     */
    #if 0
    perf_inner_msg msg_inner = {
        .msghdr.type = MODULE_PERF_MGMT,
        .msghdr.code = INNER_MSG_CODE_TIMER_CALLBACK_CHECK_PHY_PERFORMANCE,
        .msghdr.len  = sizeof(MsgPerfInner),
        .cell_id = cell_id,
    };
        
    if(-1 == send_msg_to_om(MODULE_PERF_MGMT, &msg_inner, sizeof(msg_inner)))
    {
        log_perf_err("Send msg to perf_mgmt msgqueue error.\n");
        return;
    }
    #endif
    
    return;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 check_phy_performance(const UINT32 cell_id)
{
    log_perf_debug();
    
    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }
    
    cell_type_t celltype = PerfCells[cell_id-MIN_CELL_ID].cell_type;
    INT sockfd = PerfCells[cell_id-MIN_CELL_ID].sockfd.phy;
    UINT16 msg_type;
    UINT16 msg_code;

    log_perf_info("Cell(%d)'s PHY sockfd is %d\n",cell_id, sockfd);
    
    if(celltype == CELL_VIRTUAL)
    {
        msg_type = MSG_TYPE_REALPHY_PERF;
        msg_code = MSG_CODE_REALPHY_PERF_STATISTICS_REQ;
    }
    else if(celltype == CELL_SIMULATE)
    {
        msg_type = MSG_TYPE_SIMUPHY_PERF;
        msg_code = MSG_CODE_SIMUPHY_PERF_STATISTICS_REQ;
    }
    else
        msg_code = 0;

    /**
     *  Here is send PERF_STATISTICS_REQ to MAC
     */
    perf_outer_msg_perf_check_req perf_req _unused = {
        .msghdr.type = msg_type,
        .msghdr.code = msg_code,
        .msghdr.len  = sizeof(perf_outer_msg_perf_check_req),
        .cell_id     = cell_id,
    };

    #if 0
   
    if(PERF_OK == send_msg_to_inet(MODULE_PERF_MGMT, SOCK_STREAM_INVALID, sockfd, IP_INVALID, SOCKADDR_NULL, 
                                    &perf_req, sizeof(PERF_STATISTICS_REQ)))
    {
        log_perf_info("send msg to phy success.\n"); 
    }
    else
    {
        log_perf_err("send msg to phy error.\n"); 
        return PERF_ERR;
    }
    #endif
    
    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_mac_perf_rsp(UINT32 cell_id, void*arg)
{
    log_perf_debug();

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    /**
     *  Save MAC performance parameters
     */
    perf_outer_msg_perf_check_mac_rsp perf_mac_rsp;
    Memcpy(&perf_mac_rsp, arg, sizeof(perf_outer_msg_perf_check_mac_rsp));

    Memcpy(&(PerfCells[cell_id-MIN_CELL_ID].mac), &(perf_mac_rsp.prfm), sizeof(perf_mac_t));

    Fprintf(STDOUT, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    Memdisplay(STDOUT, &perf_mac_rsp, sizeof(perf_outer_msg_perf_check_mac_rsp), 20);
    Fprintf(STDOUT, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    
    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
VOID timer_check_mac_performance_callback(VOID *arg)
{
    log_perf_debug();

    UINT32 cell_id _unused = indexofarray(PerfCell, PerfCells, arg) + MIN_CELL_ID;
    //log_perf_err("cell_id = %d\n", cell_id);

    /**
     *  Send message to (perf_mgmt msgqueue)main ctrl thread
     */
    #if 0
    perf_inner_msg msg_inner = {
        .msghdr.type = MODULE_PERF_MGMT,
        .msghdr.code = INNER_MSG_CODE_TIMER_CALLBACK_CHECK_MAC_PERFORMANCE,
        .msghdr.len  = sizeof(perf_inner_msg),
        .cell_id = cell_id,
    };
        
    if(-1 == send_msg_to_om(MODULE_PERF_MGMT, &msg_inner, sizeof(msg_inner)))
    {
        log_perf_err("Send msg to perf_mgmt msgqueue error.\n");
        return;
    }
    #endif
    
    return;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 check_mac_performance(const UINT32 cell_id)
{
    log_perf_debug();
    
    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }
    
    INT sockfd = PerfCells[cell_id-MIN_CELL_ID].sockfd.mac;

    log_perf_info("Cell(%d)'s MAC sockfd is %d\n",cell_id, sockfd);

    /**
     *  Here is send perf_outer_msg_perf_check_req to MAC
     */
    #if 0
    perf_outer_msg_perf_check_req perf_req = {
        .msghdr.type = MSG_TYPE_MAC_PERF,
        .msghdr.code = MSG_CODE_MAC_PERF_STATISTICS_REQ,
        .msghdr.len  = sizeof(perf_outer_msg_perf_check_req),
        .cell_id          = cell_id,
    };

    Fprintf(STDOUT, "_______________________________________________\n");
    mem2byteshow(&perf_req, sizeof(perf_outer_msg_perf_check_req), 10);
    Fprintf(STDOUT, "_______________________________________________\n");

    if(0 == send_msg_to_inet(MODULE_PERF_MGMT, SOCK_STREAM_VALID, sockfd, IP_INVALID, SOCKADDR_NULL, 
                                    &perf_req, sizeof(perf_outer_msg_perf_check_req)))
    {
        log_perf_info("send msg to mac success.\n"); 
    }
    else
    {
        log_perf_err("send msg to mac error.\n"); 
        return PERF_ERR;
    }
    #endif
    
    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
void register_to_cell_mgmt_callback(UINT32 cell_id, ULONG mask, cell_info_t *arg)
{
    log_perf_debug();
#if 0
#define REG_CELL_MGMT_INIT      1L<<0   /* cell_mgmt initial happend */
#define CELLOK           1L<<1   /* cell setup happend */
#define CELLNULL          1L<<2   /* cell delete happend */
#endif

    INT32               ret = PERF_ERR;
    UINT32              Cell_id = 0;
    PERF_MGMT_EVENT     sm_event = PERF_MGMT_EVENT_MIN;

    cell_info_t *cell = (cell_info_t *)arg; 

    /**
     *  'mask' is one of ERG_XXXX 
     */
    switch(mask)
    {
        /* CELLOK */
        case CELLOK:
            log_perf_info("deal with cell_mgmt module cell setup event\n");
            
            Cell_id = cell_id;
            sm_event = PERF_EVENT_CELL_OK;
            
            break;

        /* CELLNULL */
        case CELLNULL:
            log_perf_info("deal with cell_mgmt module cell deleted event\n");

            Cell_id = cell_id;
            sm_event = PERF_EVENT_CELL_NULL;
        
            break;

        default:
            log_perf_warn("i don't care this mask(%x)\n", mask);
            break;
    }

    ret = do_perf_mgmt_sm(Cell_id, sm_event, (VOID*)cell);
    if(ret != PERF_OK)
    {
        log_perf_err("do sm Error\n");
    }

    return;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_cell_ok(UINT32 cell_id, void*arg)
{
    log_perf_debug();

    /**
     *  Actually, "arg" is CELL_MGMT module vc_entity_table address
     */
    PerfCells[cell_id-MIN_CELL_ID].state = PERF_VALIDE_STATE;
    PerfCells[cell_id-MIN_CELL_ID].cell_id = cell_id;
    PerfCells[cell_id-MIN_CELL_ID].cell_type = ((cell_info_t*)arg)->cell_type;
    PerfCells[cell_id-MIN_CELL_ID].sockfd.mac = ((cell_info_t*)arg)->cell_macsockfd;
    PerfCells[cell_id-MIN_CELL_ID].sockfd.phy = ((cell_info_t*)arg)->cell_physockfd;

    Fprintf(STDOUT, "PRFM###: cell id %d\n", cell_id);
    Fprintf(STDOUT, "PRFM###: cell_type %s\n", ((cell_info_t*)arg)->cell_type==0?"virtual":"simulate");
    Fprintf(STDOUT, "PRFM###: cell sockfd mac %d, phy %d\n", 
                        ((cell_info_t*)arg)->cell_macsockfd, ((cell_info_t*)arg)->cell_physockfd);
    
    /**
     *  If the cell is running, means that MAC and PHY(virt/simu) is running,
     *  so, start the timer to check performance statistics. Rong Tao 2019.02
     */
    /**
     *  At first, i think create/initial timer in INIT, but is not good for timer handle thread.
     *  create timer when the cell setupped(CELLOK), that is to say, the timer is running
     *  while the cell is running(OK).      Rong Tao 2019.02
     */

    PerfCells[cell_id-MIN_CELL_ID].timer.mac = VOS_TimerCreate(perfs_module.moduleID, perfs_msgqid, Timeout_perf_ms, 
                        timer_check_mac_performance_callback, &PerfCells[cell_id-MIN_CELL_ID], VOS_TIMER_LOOP);
    //PerfCells[cell_id-MIN_CELL_ID].timer.mac = TimerCreateLoop( 
    //            timer_check_mac_performance_callback,&PerfCells[cell_id-MIN_CELL_ID], Timeout_perf_ms);

    PerfCells[cell_id-MIN_CELL_ID].timer.phy = TimerCreateLoop(
                timer_check_phy_performance_callback,&PerfCells[cell_id-MIN_CELL_ID], Timeout_perf_ms);
                
    log_perf_info("Create mac&phy timer\n");

    return PERF_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_cell_null(UINT32 cell_id, void*arg)
{
    log_perf_debug();

    INT32 ret = -1;

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    Memset(&PerfCells[cell_id-MIN_CELL_ID], 0, sizeof(PerfCell));
    
    PerfCells[cell_id-MIN_CELL_ID].state = PERF_INVALIDE_STATE;
    PerfCells[cell_id-MIN_CELL_ID].cell_id = cell_id;


    ret = TimerDelete(PerfCells[cell_id-MIN_CELL_ID].timer.mac);
    if(ret != PERF_OK)
    {
        log_perf_err("Delete mac perf check timer error.ret = %d, timerID = %ld\n", 
                        ret, PerfCells[cell_id-MIN_CELL_ID].timer.mac);
    }
    
    ret = TimerDelete(PerfCells[cell_id-MIN_CELL_ID].timer.phy);
    if(ret != PERF_OK)
    {
        log_perf_err("Delete phy perf check timer error.ret = %d, timerID = %ld\n", 
                        ret, PerfCells[cell_id-MIN_CELL_ID].timer.phy);
    }

    return PERF_OK;
}



/* Check Macros ------------------------------------------------------------------------------------------------------*/



