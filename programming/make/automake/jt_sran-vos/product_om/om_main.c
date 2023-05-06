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
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: om_main.c
** Description: 
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.01 - 
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "cell_mgmt_api.h"
#include "inet_mgmt_api.h"
#include "perf_mgmt_api.h"


#include "vos_common.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
vos_module_t module_table[] = {
    {0x100,"PRODUCT_MOD_1"},
    {0x200,"PRODUCT_MOD_2"},
    {0x300,"PRODUCT_MOD_3"},
};

vos_module_t module_table2[] = {
    {0x400,"MODULE_CFIG_MGMT"},
    {0x500,"MODULE_MAIN_CTRL"},
    {0x600,"MODULE_BORD_MGMT"},
    {0x700,"MODULE_CELL_MGMT"},
    {0x800,"MODULE_RRUS_MGMT"},
    {0x900,"MODULE_FTPS_MGMT"},
    {0xa00,"MODULE_ALAR_MGMT"},
    {0xb00,"MODULE_INET_MGMT"},
    {0xc00,"MODULE_TIMER"},
};


/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/

void sig_handler(int signum)
{
    /**
     *  I add VOS to OM project, however, when i terminate the jt_sran execution,
     *  linux terminal is not display, which not show the command that input from
     *  keyboard until press ENTER. So i add this command to deal with the issue.
     *  Maybe it's because VOS console change the stdout and the stdin that we don't
     *  know, and it must be solved further. Rong Tao
     */
    system("stty sane");

    if(cell_mgmt_clean_up() == -1)
	{
		printf("###############clean failed of CELL MGMT.\n");
	}
    
    printf("System Terminate...............\n");
    
    exit(0);
}

void test_cell_mgmt_register_callback(UINT32 cell_id, ULONG mask, struct cell_info *cell)
{
    
    printf("########################### MAIN_CTRL: id = %d, mask = %ld, \n", cell_id, mask);


    return;
}

#define VOS_CFG_FILE    "/mnt/hgfs/shared/jt_sran-vos/vos.conf"

int main(int argc, char *argv[])
{
    LONG ret = -1;
    
    ret = VOS_Init(argv[0], VOS_CFG_FILE, module_table, sizeof(module_table)/sizeof(module_table[0]));
    if(ret != VOS_OK)
    {
        VOS_Printf("VOS_Init error. %s\n", VOS_CFG_FILE);
        return VOS_ERROR;
    }
#if 1
    /**
     *  INET_MGMT
     */
    ret = inet_mgmt_module_init();
    if(ret != VOS_OK)
    {
        VOS_Printf("inet_mgmt_module_init error.\n");
        return VOS_ERROR;
    }
    
    
    /**
     *  CELL_MGMT
     */
    ret = cell_mgmt_module_init();
    if(ret != VOS_OK)
    {
        VOS_Printf("cell_mgmt_module_init error.\n");
        return VOS_ERROR;
    }
    /**
     *  Test CELL_MGMT module register
     */
    cell_mgmt_module_register(0x1234, CELLOK|CELLNULL, test_cell_mgmt_register_callback);
#endif
#if 1
    /**
     *  Perf_mgmt module initial
     */
    ret = perf_mgmt_module_init();
    if(ret != VOS_OK)
    {
        VOS_Printf("perf_mgmt_module_init error.\n");
        return VOS_ERROR;
    }
#endif
    
    signal(SIGINT, sig_handler);

    while(1)
    {
        //Printf("sys running.\n");
        sleep(3);
    }


    system("stty sane");

    return 0;
}
