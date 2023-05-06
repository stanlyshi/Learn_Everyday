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
** File name: perfs_cell_perf.h
** Description:  performance statistics module's  cell performance
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_CELL_PERF_H
#define _PERFS_CELL_PERF_H


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"
#include "perfs_l1l2l3_perf.h"
#include "perfs_sm.h"

#include "cell_mgmt_api.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/

/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
    Timer_t mac;
    Timer_t phy;
}_packed PrfmTimer;

typedef struct {
    INT mac;
    INT phy;
}_packed Sockfd;

typedef struct {
    UINT32          cell_id;
    UINT32          cells_num;
    cell_type_t        cell_type;    //simulated cell or not, 0-virt; 1-simu
    
    PerfState       state;
    
    perf_mac_t      mac;
    union {
    perf_realphy_t  realphy;
    perf_simuphy_t  simuphy;
    };
    PrfmTimer       timer;
    Sockfd          sockfd;
}_packed PerfCell;
#define PERF_CELL_NUM   CELL_NUM

/* Macros ------------------------------------------------------------------------------------------------------------*/



/* Globals -----------------------------------------------------------------------------------------------------------*/

/* Functions ---------------------------------------------------------------------------------------------------------*/



#endif /*<_PERFS_CELL_PERF_H>*/
