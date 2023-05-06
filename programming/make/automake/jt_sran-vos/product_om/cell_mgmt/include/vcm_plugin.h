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
** File name: vcm_plugin.h 
** Description: CELL_MGMT plug-in unit
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _VCM_PLUG_IN_UNIT_H
#define _VCM_PLUG_IN_UNIT_H  1

/**
 *  This file for further more developement.
 *  2019.03.19
 */

  
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_types.h"
#include "vcm_config.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
typedef enum vcm_Plugin_type_t {
    VCM_PLUGIN_MIN,
    VCM_PLUGIN_CELL_EXTEND,
    /*...*/
    VCM_PLUGIN_MAX,
}vcm_plugin_type_t;

/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct vcm_Plugin_operation_t {
    int (*install)();
    int (*uninstall)();
}_packed vcm_plugin_operation_t;

typedef struct vcm_Plugin_unit_t {
    char plugin_name[VCM_STR_LEN];
    vcm_plugin_operation_t  operator;
}_packed vcm_plugin_unit_t;


/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/* Check Macros ------------------------------------------------------------------------------------------------------*/




#endif /*<_VCM_PLUG_IN_UNIT_H>*/
