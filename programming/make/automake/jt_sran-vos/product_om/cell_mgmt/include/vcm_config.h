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
** File name: vcm_config.h 
** Description: CELL_MGMT "#define"
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
#ifndef _VCM_CONFIG_H
#define _VCM_CONFIG_H  1
  
  
/* Dependencies ------------------------------------------------------------------------------------------------------*/
/* Constants ---------------------------------------------------------------------------------------------------------*/
#define CELL_MGMT_NAME                  "Cell Management Module"
#define CELL_MGMT_MAIN_TASK_NAME        "Cell Management Main Task"
#define CELL_MGMT_REGISTER_TASK_NAME    "Cell Management Register Task"

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#define _VCM_LOGGING                    /* turn on logging message */
#define _VCM_DEBUG                      /* turn on cell_mgmt debug information on linux terminal */
#define _VCM_DISPLAY                    /* show cell entiry, state machine on stdout\stderr .etc. */
#define _VCM_TURN_ON_TIMER              /* turn on the timers of cell_mgmt in sm produre */


/* Globals -----------------------------------------------------------------------------------------------------------*/
/**
 *  CELL information define
 */
#define VCM_MIN_CELL_NUM                1
#define VCM_MAX_CELL_NUM                20
#define VCM_CELL_NUM                    (VCM_MAX_CELL_NUM-VCM_MIN_CELL_NUM+1)
#define VCM_ALL_CELLS                   0x00fffff1  //make sure is unique in jt_sran

/**
 *  Internet Interface information define
 */
#define VCM_IF_IP_NUM                   25 //max numbers of IPv4 for mac

#define VCM_IF_NAME                     "ens33"
#define VCM_IF_NETMASK                  "255.255.0.0"
#define VCM_IF_IP_BASE                  "10.21.11.17"

#define VCM_IP_LEN                      16
#define VCM_HDADDR_LEN                  18

/**
 *  Configuration of cells
 */
#define VCM_CELLS_FOLDER_PATH           "/home/rongtao/om/Cells"   

/**
 *  l1l2l3 UNIX Socket define 
 */
#define L1_UNIX_SOCK_PATH               "/etc/jt_product/jt_sran/un_sock/PHY_UNSOCKET_PATH"
#define L2L3_UNIX_SOCK_PATH             "/etc/jt_product/jt_sran/un_sock/L2L3_UNSOCKET_PATH"

/**
 *  MAC information define 
 */
#define MAC_MAX_NUM                     VCM_CELL_NUM /* This is the number of L2L3(cells) */

#define MAC_SRC_DEFAULT_PATH            "/home/rongtao/om/mac/default"
#define MAC_EXECUTION_SCRIPT            "start.sh"
#define MAC_CFG_FOLDER                  "cfgfile"
#define MAC_CFG_VbsMainConfig           "VbsMainConfig.xer"
#define MAC_CFG_PHYConfig               "PHYConfig.xer"
#define MAC_MEMORY_SIZE                 "123"


/**
 *  PHY information define
 */
#define REAL_PHY_INDEX_START            0
#define REAL_PHY_INDEX_END              7
#define REAL_PHY_INDEX_NUM              (REAL_PHY_INDEX_END-REAL_PHY_INDEX_START+1)

#define SIMU_PHY_INDEX_START            8   // index of simu phy starts from 8
#define SIMU_PHY_INDEX_END              16  // index of simu phy end to 16
#define SIMU_PHY_INDEX_NUM              (SIMU_PHY_INDEX_END-SIMU_PHY_INDEX_START+1)

#define PHY_INDEX_NUM                   (SIMU_PHY_INDEX_END+1)

#define SIMU_PHY_SRC_PATH               "/home/rongtao/om/mac/default/simulatePhy"
#define SIMU_PHY_FOLDER                 "simulatePhy"
#define SIMU_PHY_CFIG                   "SimuPhyConfig.xer"
#define SIMU_PHY_EXECUTION_SCRIPT       "start_simu_phy.sh"
#define SIMU_PHY_BASE_PORT              10810
#define SIMU_PHY_DSP_PORT               "4007"
#define SIMU_PHY_GTS_PORT               "5007"

/**
 *  RRU information define
 */
#define VCM_RRU_NUM                     64
#define VCM_RRU_SN_LEN                  16


/**
 *  Common information define
 */
#define VCM_STR_LEN                     256
#define VCM_MAX_MSGQ_MSG_NUM            256


/**
 *  MySQL configure
 */
#define VCM_DB_HOST                  "localhost"
#define VCM_DB_USER                  "root"
#define VCM_DB_PASSWD                "root "
#define VCM_DB_DB                    "vcm_db"
#define VCM_DB_CELLS_TABLE           "cells"
#define VCM_DB_MACS_TABLE            "macs"
#define VCM_DB_PHYS_TABLE            "phys"
#define VCM_DB_RRUS_TABLE            "rrus"



/**
 *  There is some config choices
 */
//#define _VCM_PRIVATE_SYNC_TO_API        1 /* this is to synchronize to API */


/* Functions ---------------------------------------------------------------------------------------------------------*/


/* Check Macros ------------------------------------------------------------------------------------------------------*/
#if defined(_VCM_LOGGING) && !defined(_VCM_DEBUG)
 #warning "You turn on the debug logging, but not define _CELL_MGMT_LOGGING."
#endif

#if VCM_MAX_CELL_NUM < VCM_MIN_CELL_NUM
 #error "VCM_MAX_CELL_NUM must bigger than VCM_MIN_CELL_NUM"
#endif

#if (REAL_PHY_INDEX_END) < (REAL_PHY_INDEX_START)
 #error "REAL_PHY_INDEX_END must higher than REAL_PHY_INDEX_START"
#endif
#if (SIMU_PHY_INDEX_END) < (SIMU_PHY_INDEX_START)
 #error "SIMU_PHY_INDEX_END must heigher than SIMU_PHY_INDEX_START"
#endif
#if (SIMU_PHY_INDEX_START) < (REAL_PHY_INDEX_END)
 #error "SIMU_PHY_INDEX_START must heighter than REAL_PHY_INDEX_END"
#endif


#endif /*<_VCM_CONFIG_H>*/
