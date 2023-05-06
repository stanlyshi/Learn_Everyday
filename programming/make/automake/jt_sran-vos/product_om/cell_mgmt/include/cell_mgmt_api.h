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
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: cell_mgmt_api.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
** 
***********************************************************************************************************************/
#ifndef _CELL_MGMT_API_H
#define _CELL_MGMT_API_H 1
   
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_types.h"
#include "vcm_config.h"
#include "vcm_tools.h"
#include "vcm_sys_wrapper.h"
  
/* Constants ---------------------------------------------------------------------------------------------------------*/
#define ALL_CELLS   VCM_ALL_CELLS
#define CELL_NUM    VCM_CELL_NUM
#define MIN_CELL_ID VCM_MIN_CELL_NUM
#define MAX_CELL_ID VCM_MAX_CELL_NUM

/**
 *  Mask code, support totally 64 codes(1L is 64bit in 64bit system)
 *      Rong Tao: 2019.02
 */
#define CELLOK      CELLSETUP   /* cell setup successfully */
#define CELLNULL    CELLDELETE  /* cell delete successfully */


/* Types -------------------------------------------------------------------------------------------------------------*/
/**
 *  For register module callback function argument
 *      Rong Tao: 2019.02
 */
typedef struct cell_info {
    /* some information */
    UINT32  cell_id;
    UINT32  cell_snmp_ind;
    UINT32  cell_g_id;
    UINT8   cell_type;
    UINT8   cell_is_uni;
    UINT32  cell_power;
    UINT8   cell_usernum;
    UINT8   cell_bandwidth;
    UINT8   cell_mode;
    UINT16  cell_rru_id; 
    UINT8   cell_rru_sn[VCM_RRU_SN_LEN];    
    UINT8   cell_rru_ip[VCM_RRU_SN_LEN];   
    UINT16  cell_rru_port;   
    UINT32  cell_axc_speed; 
    UINT32  cell_axc_ind;  
    INT32   cell_macsockfd;
    INT32   cell_physockfd;
}__attribute__((packed))cell_info_t;

typedef void(*vcm_register_cb_f)(UINT32 cell_id, ULONG mask, cell_info_t *cell);

typedef vcm_register_cb_f RegCallbackFn;

/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description: cell management module initialized function
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
extern INT32 cell_mgmt_module_init();

/***********************************************************************************************************************
 * Description: cell management module clean up function
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
extern INT32 cell_mgmt_clean_up();
 
/***********************************************************************************************************************
 * Description: cell management module perror function, like glibc's perror() interface
 *
 * Input: script: a string to descripte 
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 *
 *  Example:
         vcmPerror("perror");
         __vcm_set_errno(_VCM_EICMACETHER);
         vcmPerror("perror");
         __vcm_set_errno(_VCM_EICRRUSTATUS);
         vcmPerror("perror");
     example output:
        perror:Vcm Success
        perror:Invalid cell mac's ether interface
        perror:Invalid cell rru's status
 **********************************************************************************************************************/
extern INT32 cell_mgmt_perror(CONST CHAR *script);

/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
extern INT32 cell_mgmt_module_register(UINT32 module_id, ULONG mask, vcm_register_cb_f cb);

/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
extern INT32 cell_mgmt_module_register_update(UINT32 module_id, ULONG mask, vcm_register_cb_f cb);


/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
extern INT32 cell_mgmt_module_unregister(UINT32 module_id);




#endif /*<_CELL_MGMT_API_H>*/
