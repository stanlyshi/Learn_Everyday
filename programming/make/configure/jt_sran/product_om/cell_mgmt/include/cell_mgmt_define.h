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
** File name: cell_mgmt_define.h 
** Description: CELL_MGMT "#define"
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_DEFINE_H
#define _MODULE_CELL_MGMT_DEFINE_H

/* Dependencies --------------------------------------------------------------*/
/* Constants -----------------------------------------------------------------*/

/******************************************************************************
 * CELL_MGMT 
 ******************************************************************************/
#define MAX_STR_LEN                     256
#define MIN_CELL_NUM                    1
#define MAX_CELL_NUM                    128
#define MAX_PHY_INDEX                   8

#define MAC_SRC_PATH                    SYS_PRODUCT_DIR"/l2_l3/"     
#define MAC_SRC_PATH_DEFAULT            MAC_SRC_PATH"/default/"
#define PATH_CELLS                      MAC_SRC_PATH"/cells/"             

#define MAC_MEMORY_SIZE                 "215888480"

#define IP_BASE                         "10.21.2.242"
#define IFCONFIG_INTERFACE_NAME         "virbr0"

#define Timeout_Wait_Seconds            5   /*(s)*/

/******************************************************************************
 * inet_mgmt init: register unsocket with mac
 ******************************************************************************/
#define MAC_SUN_PATH                    SYS_PRODUCT_DIR"/un_sock/L2L3_UNSOCKET_PATH"
#define PHY_SUN_PATH                    SYS_PRODUCT_DIR"/un_sock/PHY_UNSOCKET_PATH"

#define CALLBACK_FUNC_VALID             1
#define CALLBACK_FUNC_INVALID           0
#define CELL_MGMT_MSGQ_VALID            1
#define CELL_MGMT_MSGQ_INVALID          0
#define SOCK_STREAM_VALID               1
#define SOCK_STREAM_INVALID             0
#define IP_VALID                        1
#define IP_INVALID                      0
#define SOCKADDR_NULL                   NULL

/******************************************************************************
 * PHY 
 ******************************************************************************/

/******************************************************************************
 * MAC
 ******************************************************************************/

/******************************************************************************
 * CFIG_MGMT
 ******************************************************************************/

/******************************************************************************
 * BORD_MGMT
 ******************************************************************************/

/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

#endif /*<_MODULE_CELL_MGMT_DEFINE_H>*/