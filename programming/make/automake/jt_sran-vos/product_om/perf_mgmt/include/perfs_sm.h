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
** File name: perfs_sm.h
** Description:  performance statistics module's  state machine
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_SM_H
#define _PERFS_SM_H


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
typedef enum {
    PERF_MGMT_STATE_MIN = -1,
    PERF_INVALIDE_STATE,
    PERF_VALIDE_STATE,
    PERF_MGMT_STATE_MAX,
}PerfState;


typedef enum {
    PERF_MGMT_EVENT_MIN = -1,
    PERF_EVENT_CELL_OK,
    PERF_EVENT_CELL_NULL,
    PERF_EVENT_MAC_PERF_RSP,
    PERF_EVENT_SIMUPHY_PERF_RSP,
    PERF_EVENT_REALPHY_PERF_RSP,
    
    PERF_MGMT_EVENT_MAX,
}PERF_MGMT_EVENT;


/* Types -------------------------------------------------------------------------------------------------------------*/
typedef INT32 (*PerfSmHandleFn)(UINT32, void*);

typedef struct {
    char *Statename;
    char *Eventname;
    char *Handlername;
    PerfSmHandleFn handler;
}__attribute((packed)) perfs_sm_handler_t;


/* Macros ------------------------------------------------------------------------------------------------------------*/
#define add_perf_sm_entry(state, event, fn) \
            add_perf_mgmt_sm_entry(state, #state, event, #event, fn, #fn)

/* Globals -----------------------------------------------------------------------------------------------------------*/

/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 do_perf_mgmt_sm(UINT32 cell, PERF_MGMT_EVENT sm_event, void *arg);

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 add_perf_mgmt_sm_entry(PerfState state, char *statename, PERF_MGMT_EVENT event, char *eventname,
                            PerfSmHandleFn fn, char *fnname);



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 sm_handler_cell_ok(UINT32 cell_id, void*arg);
INT32 sm_handler_cell_null(UINT32 cell_id, void*arg);
INT32 sm_handler_mac_perf_rsp(UINT32 cell_id, void*arg);
INT32 sm_handler_simuphy_perf_rsp(UINT32 cell_id, void*arg);
INT32 sm_handler_realphy_perf_rsp(UINT32 cell_id, void*arg);


#endif /*<_PERFS_SM_H>*/
