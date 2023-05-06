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
** File name: vcm_module_register.h
** Description:
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao
** Date: 2019.02 - 
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"

#include "cell_mgmt_api.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
#define CELL_MGMT_REG_MODULE_MAXNUM     10 /* cell_mgmt support max number of register module */

typedef enum {
    REG_NULL = 0,
    REG_OK,
}module_state_e;

#define PIPE_READ   0
#define PIPE_WRITE  1

/* Types -------------------------------------------------------------------------------------------------------------*/
/** 
 *  For module register
 */
typedef struct {
    UINT32 flag;  
    UINT32 module_id;
    ULONG   mask;
    vcm_register_cb_f cb;
}_packed vcm_register_t;


/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
vcm_register_t          reg_modules[CELL_MGMT_REG_MODULE_MAXNUM];

/* already register module's number */
static UINT32           max_index_regModule = 0;

extern TaskCreateRet    vcm_task3; /* module register thread */

/**
 *  This is a no-name pipe, to make "cell main ctrl" and "module register" thread communicate with each other.
 *      Rong Tao: 2019.02
 *
 *  Procedure As follows:
 *  
 *              CELL_MGMT MODULE                             Other MODULE
 *                                                               Thread
 * "module register"         mainCtrlThread                         |
 *          |                       |                               |
 *          |                       |<----------------`cell_mgmt_module_register(module_id, mask, function1)`
 *          |                       |                               |
 *          |                       |                               |
 *          |               `pipe(pipe_fd)`                         |
 *          |                       |                               |
 *          |                       |                               |
 *          |                       |<-- one thing happen           |
 *          |                       |                               |
 *          |                       |                               |
 *          |               `write(pipe_fd[1])`                     |
 *          |<----------------------|                               |
 *  `read(pipe_fd[0])`              |                               |
 *          |                       |                               |
 *   `function1(...)`               |                               |
 *          |                       |                               |
 *          |                       |                               |
 *          |                       |                               | <------- jt_sran process terminate
 *          |                       |                 `cell_mgmt_module_unregister(module_id)`
 *          |                       |                               |
 *          |                       |                               |
 */
int vcm_module_reg_pipe_fd[2];

/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_module_register_init()
{
    __called_once();
    
    log_vcm_debug();

    UINT32 i;

    /**
     *  Initial vars
     */
    Memset(reg_modules, 0, sizeof(reg_modules));
    for(i=0; i<CELL_MGMT_REG_MODULE_MAXNUM; i++)
    {
        reg_modules[i].cb = NULL;
    }

    /**
     *  Create pipe
     */ 
    if (Pipe (vcm_module_reg_pipe_fd))
	{
		log_vcm_err("Create Pipe failed.\n");
        __vcm_set_errno(_VCM_EIPCPIPEERR);
		return VCM_ERR;
	}
    
    /**
     *  Task create in "cell_mgmt_task.c"
     */

    
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_task_module_register_ctrl(VOID *arg)
{
    log_vcm_info("%s module register ... pid(%d), tid(%d)\n", CELL_MGMT_NAME, getpid(), GET_TID());
    

    INT32 ret = VCM_ERR;

    int fd = vcm_module_reg_pipe_fd[PIPE_READ];

    vcm_inner_msg_pipe_t pipe_msg;

    UINT32 cell_id;
    
    ULONG mask;

    #if 1
    /**
     *  NON block file descriptor set, make Read msg bolck
     */
    int flags = Fcntl(fd, F_GETFL);
    if(flags == -1)
    {
        log_vcm_err("Fcntl error. F_GETFL\n");
        return VCM_ERR;
    }
    flags &= ~O_NONBLOCK;
    ret = Fcntl(fd, F_SETFL, flags);
    if(ret == -1)
    {
        log_vcm_err("Fcntl error. F_SETFL\n");
        __vcm_set_errno(_VCM_EFDSETBLOCKERR);
        return VCM_ERR;
    }
    #endif

    /**
     *  the loop to read message from pipe.
     */
    while(1)
    {
        /**
         *  Read msg from pipe
         */
        Memset(&pipe_msg, 0, sizeof(vcm_inner_msg_pipe_t));
        if(Read(fd, &pipe_msg, sizeof(pipe_msg)) <= 0)
        {
            continue;
        }
        cell_id = pipe_msg.cell_id;
        mask    = pipe_msg.event_mask;

        log_vcm_info("mask = %d\n", mask);

        /**
         *  Call the register callback function
         */
        INT32 i;
        for(i=0; i<CELL_MGMT_REG_MODULE_MAXNUM; i++)
        { 
            if(reg_modules[i].flag == 1 && reg_modules[i].cb != NULL)
            {
                if((reg_modules[i].mask&mask) >= CELLOK)
                {
                    vcm_register_cb_f cb = reg_modules[i].cb;
                    cell_info_t cell = {
                        .cell_id            = cell_id,
                        .cell_snmp_ind      = get_snmp_index(cell_id),
                        .cell_g_id          = get_g_id(cell_id),  
                        .cell_type          = get_cell_type(cell_id), 
                        .cell_is_uni        = get_is_unify(cell_id),
                        .cell_power         = get_cell_power(cell_id), 
                        .cell_usernum       = get_cell_usernum(cell_id),
                        .cell_bandwidth     = get_cell_bandwidth(cell_id),
                        .cell_mode          = get_cell_mode(cell_id), 
                        .cell_rru_id        = get_rru_id(cell_id),
                        .cell_axc_speed     = get_rru_axc_speed(cell_id), 
                        .cell_axc_ind       = get_rru_axc_index(cell_id),  
                        .cell_macsockfd     = get_mac_sockfd(cell_id),
                        .cell_physockfd     = get_cell_type(cell_id)==CELL_VIRTUAL?
                                                    get_phy_sockfd(cell_id):get_simu_phy_sockfd(cell_id),
                    };
                    Memset(cell.cell_rru_ip, 0, sizeof(cell.cell_rru_ip));
                    get_rru_ip(cell_id, cell.cell_rru_ip);  
                        
                    Memset(cell.cell_rru_sn, 0, sizeof(cell.cell_rru_ip));
                    get_rru_sn(cell_id, cell.cell_rru_sn);

                    /**
                     *  Callback
                     */
                    cb(cell_id, mask, &cell);
                }   
            }
        }
    }
    

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description: 
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_module_register(UINT32 module_id, ULONG mask, vcm_register_cb_f cb)
{
    log_vcm_debug();

    log_vcm_warn("register module(%ld), mask(%ld)\n", module_id, mask);

    if(max_index_regModule >= CELL_MGMT_REG_MODULE_MAXNUM)
    {
        log_vcm_err("register error, module number out of range.\n");
        return VCM_ERR;
    }

    UINT32 i;
    UINT32 ret = VCM_ERR;
    
    for(i=0; i<CELL_MGMT_REG_MODULE_MAXNUM; i++)
    {        
        if(reg_modules[i].flag == REG_NULL)
        {
            reg_modules[i].flag         = REG_OK;
            reg_modules[i].module_id    = module_id;
            reg_modules[i].mask         = mask;
            reg_modules[i].cb           = cb;
            max_index_regModule++;
            ret = VCM_OK;
            break;
        }
    }

    return ret;
}
weak_alias(vcm_module_register, cell_mgmt_module_register);



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_module_register_update(UINT32 module_id, ULONG mask, vcm_register_cb_f cb)
{
    log_vcm_debug();

    UINT32 i;
    UINT32 ret = VCM_ERR;
    
    for(i=0; i<CELL_MGMT_REG_MODULE_MAXNUM; i++)
    {        
        if(reg_modules[i].module_id == module_id)
        {
            if(reg_modules[i].mask == mask && (reg_modules[i].cb == cb || reg_modules[i].cb == NULL))
            {
                ret = VCM_ERR;
                break;
            }
        
            if(reg_modules[i].mask != mask) reg_modules[i].mask = mask;
            if(reg_modules[i].cb != cb && cb != NULL) reg_modules[i].cb = cb;
                
            ret = VCM_OK;
            break;
        }
    }
    return ret;
}
weak_alias(vcm_module_register_update, cell_mgmt_module_register_update);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_register_thing_happend(UINT32 cell_id, ULONG mask)
{
    log_vcm_debug();

    vcm_inner_msg_pipe_t msg = {
        .cell_id    = cell_id,
        .event_mask = mask,
    };
        
    int fd = vcm_module_reg_pipe_fd[PIPE_WRITE];
    
    if(Write(fd, &msg, sizeof(vcm_inner_msg_pipe_t)) <= 0)
    {
        log_vcm_err("write pipe error");
        __vcm_set_errno(_VCM_EIPCPIPERERR);
        return VCM_ERR;
    }
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_module_unregister(UINT32 module_id)
{
    log_vcm_debug();

    UINT32 i;
    UINT32 ret = VCM_ERR;
    
    for(i=0; i<CELL_MGMT_REG_MODULE_MAXNUM; i++)
    {        
        if(reg_modules[i].module_id == module_id)
        {
            Memset(&reg_modules[i], 0, sizeof(vcm_register_t));
            reg_modules[i].flag     = REG_NULL;
            reg_modules[i].cb       = NULL;
            max_index_regModule--;
            ret = VCM_OK;
            break;
        }
    }
    return ret;
}
weak_alias(vcm_module_unregister, cell_mgmt_module_unregister);




