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
** File name: perfs_msgs.h
** Description:  performance statistics module
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_MSGS_H
#define _PERFS_MSGS_H


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"
#include "perfs_l1l2l3_perf.h"


#include "cell_mgmt_api.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/

#define     MSG_TYPE_MAC_PERF_MGMT          0x5004  
#define     MSG_TYPE_PHY_PERF_MGMT          0x5103  
#define     MSG_TYPE_SIMU_PHY_PERF_MGMT     0X5202


typedef enum {
    MSG_TYPE_MAC_PERF       = MSG_TYPE_MAC_PERF_MGMT,
    MSG_TYPE_REALPHY_PERF   = MSG_TYPE_PHY_PERF_MGMT,
    MSG_TYPE_SIMUPHY_PERF   = MSG_TYPE_SIMU_PHY_PERF_MGMT,
}PERF_STATISTICS_MSG_TYPE;

typedef enum {
    MSG_CODE_MAC_MIN,
    MSG_CODE_MAC_PERF_STATISTICS_REQ = 001,
    MSG_CODE_MAC_PERF_STATISTICS_RSP = 002,
    MSG_CODE_MAC_MAX,
}PERF_MAC_MSG_CODE;

typedef enum {
    MSG_CODE_REALPHY_MIN,
    MSG_CODE_REALPHY_PERF_STATISTICS_REQ = 001,
    MSG_CODE_REALPHY_PERF_STATISTICS_RSP = 002,
    MSG_CODE_REALPHY_MAX,
}PERF_REALPHY_MSG_CODE;

typedef enum {
    MSG_CODE_SIMUPHY_MIN,
    MSG_CODE_SIMUPHY_PERF_STATISTICS_REQ = 001,
    MSG_CODE_SIMUPHY_PERF_STATISTICS_RSP = 002,
    MSG_CODE_SIMUPHY_MAX,
}PERF_SIMUPHY_MSG_CODE;


/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
	UINT16	type;
	UINT16	code;
	UINT32	len;
}_packed msghdr_t;


typedef struct {
    msghdr_t msghdr;
    UINT32  cell_id;    
}_packed perf_outer_msg_perf_check_req;

typedef struct {
    msghdr_t msghdr;
    UINT32  cell_id;    
    perf_realphy_t prfm;
}_packed perf_outer_msg_perf_check_realphy_rsp;

typedef struct {
    msghdr_t msghdr;
    UINT32  cell_id;    
    perf_simuphy_t prfm;
}_packed perf_outer_msg_perf_check_simuphy_rsp;

typedef struct {
    msghdr_t msghdr;
    UINT32  cell_id;    
    perf_mac_t prfm;
}_packed perf_outer_msg_perf_check_mac_rsp;

/**
 *  For callback of cell_mgmt module register
 */
#define cell_info_t cell_info_t



/**********************************************************************************************************************
 *  Inner message
 **********************************************************************************************************************/
/**
 *  Message type is MODULE_PERF_MGMT
 */
/**
 *  Message code as follows:
 */
#define INNER_MSG_CODE_TIMER_CALLBACK_CHECK_MAC_PERFORMANCE     001
#define INNER_MSG_CODE_TIMER_CALLBACK_CHECK_PHY_PERFORMANCE     002
/**
 *  Message as follows:
 */
typedef struct{
    msghdr_t msghdr;
    UINT32  cell_id; 
}_packed perf_inner_msg;

/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/* Check Macros ------------------------------------------------------------------------------------------------------*/




#endif /*<_PERFS_MSGS_H>*/
