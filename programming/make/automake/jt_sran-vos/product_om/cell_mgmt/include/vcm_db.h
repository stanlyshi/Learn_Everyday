/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
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
** File name: vcm_db.h 
** Description: CELL_MGMT's databases
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03
**
***********************************************************************************************************************/
#ifndef _VCM_DB_H
#define _VCM_DB_H 1


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_config.h"
#include "vcm_types.h"
#include "vcm_errno.h"
#include "vcm_sys_wrapper.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#define VCM_DB_CELLS


#define VCM_CREATE_MAC_INFO_TABLE \
    "CREATE TABLE " VCM_DB_MACS_TABLE "(" \
    "snmp_index INTEGER,"\
    "pid INTEGER,"\
    "status INTEGER,"\
    "cfg_pth CHAR(256),"\
    "ether CHAR(16),"\
    "enb_ipaddr CHAR(16),"\
    "gtpu_recv_addr CHAR(16),"\
    "vbs_eth CHAR(256),"\
    "sockfd INTEGER)"

#define VCM_INSERT_INTO_MAC_TABLE



/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/* Check Macros ------------------------------------------------------------------------------------------------------*/




#endif /*<_VCM_DB_H>*/

