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
** File name: vcm_cell.c
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
cell_node_t    Cells[VCM_CELL_NUM];  /*cell entity */
rru_info_t     Rrus[VCM_RRU_NUM];  /* rrus */


CHAR*       vcm_mode_str[VC_MODE_MAX];  /* cell mode string */
CHAR*       vcm_rru_status_str[RRU_STATUS_MAX];  /* cell mode string */
CHAR*       vcm_mac_status_str[MAC_STATUS_MAX];  /* cell mode string */


ipv4_t      ipv4_used[VCM_IF_IP_NUM]; /* configure MAC's ipv4, make sure it's not reuse the same ip in history */
LONG        __vcm_errno; /* this is the cell_mgmt error number for "cell_mgmt_errno.h" */
CHAR        __vcm_errno_string[VCM_STR_LEN] = {"Vcm Success"};

vcm_set     rrus_set; /* for rru register recording */
vcm_set     all_cell_set, virt_cell_set, simu_cell_set, normal_cell_set;


Sem_t       sem_lock_cells, sem_lock_errno;     /* lock of cells, For set and get api */

#define     __SEM_CELL_LOCK     do{Sem_wait(sem_lock_cells);/*log_vcm_debug();*/}while(0);
#define     __SEM_CELL_UNLOCK   do{Sem_post(sem_lock_cells);/*log_vcm_debug();*/}while(0);



/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_cell_add_mode(cell_mode_t mode, CHAR *Smode)
{
    /**
     *  Check parameter valiable
     */
    if(mode <= VC_MODE_MIN || mode >= VC_MODE_MAX || Smode == NULL)
    {
        log_vcm_err("mode invaliable(%d)\n", mode);
        __vcm_set_errno(_VCM_EICMODE);
        return VCM_ERR;
    }

    vcm_mode_str[mode] = Smode;
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_cell_add_rru_status(RRU_STATUS status, CHAR *Sstatus)
{
    /**
     *  Check parameter valiable
     */
    if(status <= RRU_STATUS_MIN || status >= RRU_STATUS_MAX || Sstatus == NULL)
    {
        log_vcm_err("status invaliable(%d)\n", status);
        __vcm_set_errno(_VCM_EICRRUSTATUS);
        return VCM_ERR;
    }

    vcm_rru_status_str[status] = Sstatus;
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_cell_add_mac_status(MAC_STATUS status, CHAR *Sstatus)
{
    /**
     *  Check parameter valiable
     */
    if(status <= MAC_STATUS_MIN || status >= MAC_STATUS_MAX || Sstatus == NULL)
    {
        log_vcm_err("status invaliable(%d)\n", status);
    
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }

    vcm_mac_status_str[status] = Sstatus;
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_cell_id_available(CONST UINT32 cell_id)
{
    if((cell_id < VCM_MIN_CELL_NUM || cell_id > VCM_MAX_CELL_NUM ) && cell_id != VCM_ALL_CELLS)
    {
        log_vcm_err("cell_id(%d) out of range.\n", cell_id);
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_id(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    
    __SEM_CELL_LOCK

    Cells[cell_id-VCM_MIN_CELL_NUM].id = cell_id;
    
    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}




/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cell_id(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT32 id = Cells[cell_id-VCM_MIN_CELL_NUM].id;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return id;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_snmp_index_available(CONST UINT32 snmp_index)
{
    
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_snmp_index(CONST UINT32 cell_id, CONST UINT32 snmp_index)
{
    if(VCM_ERR == is_snmp_index_available(snmp_index))
    {
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].snmp_index = snmp_index;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_snmp_index(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    UINT32 index = Cells[cell_id-VCM_MIN_CELL_NUM].snmp_index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return index;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_g_id(CONST UINT32 cell_id, CONST UINT32 g_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].g_id = g_id;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_g_id(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT32 g_id = Cells[cell_id-VCM_MIN_CELL_NUM].g_id;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return g_id;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_cell_type_available(cell_type_t type)
{
    if(type <= CELL_TYPE_MIN || type >= CELL_TYPE_MAX)
    {
        log_vcm_err("cell type(%d) out of range.\n", type);
        __vcm_set_errno(_VCM_EICTYPE);
        return VCM_ERR;
    }
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_type(CONST UINT32 cell_id, cell_type_t type)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(VCM_ERR == is_cell_type_available(type))
    {
        __vcm_set_errno(_VCM_EICTYPE);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].type = type;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cell_type(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    cell_type_t type = Cells[cell_id-VCM_MIN_CELL_NUM].type;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return type;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_cell_unify(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(get_is_unify(cell_id) == UNIFY_OFF)
    {
        return VCM_ERR;
    }
    else if(get_is_unify(cell_id) == UNIFY_ON)
    {
        return VCM_OK;
    }
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_ERR;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_is_unify(CONST UINT32 cell_id, cell_role_t is_unify)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].is_unify = is_unify;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_is_unify(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    cell_role_t is_unify = Cells[cell_id-VCM_MIN_CELL_NUM].is_unify;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return is_unify;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_power(CONST UINT32 cell_id, CONST UINT8 power)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].power = power;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT8 get_cell_power(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT8   power = Cells[cell_id-VCM_MIN_CELL_NUM].power;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return power;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_usernum(CONST UINT32 cell_id, CONST UINT8 usernum)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].usernum = usernum;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT8 get_cell_usernum(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT8   usernum = Cells[cell_id-VCM_MIN_CELL_NUM].usernum;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return usernum;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_bandwidth(CONST UINT32 cell_id, CONST UINT8 bandwidth)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].bandwidth = bandwidth;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT8 get_cell_bandwidth(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT8   bandwidth = Cells[cell_id-VCM_MIN_CELL_NUM].bandwidth;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return bandwidth;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_cell_mode_available(cell_mode_t mode)
{
    if(mode <= VC_MODE_MIN || mode >= VC_MODE_MAX )
    {
        log_vcm_err("cell mode(%d) out of range.\n", mode);
        __vcm_set_errno(_VCM_EICMODE);
        return VCM_ERR;
    }
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_mode(CONST UINT32 cell_id, CONST cell_mode_t mode)
{
    if(VCM_ERR == is_cell_mode_available(mode))
    {
        __vcm_set_errno(_VCM_EICMODE);
        return VCM_ERR;
    }
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].mode = mode;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cell_mode(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    cell_mode_t mode = Cells[cell_id-VCM_MIN_CELL_NUM].mode;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return mode;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_cell_state_available(vcm_sm_state_t state)
{
    if(state <= VC_STATE_MIN_NUM || state >= VC_STATE_MAX_NUM )
    {
        log_vcm_err("cell state(%d) out of range.\n", state);
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_cell_state(CONST UINT32 cell_id, CONST vcm_sm_state_t state)
{
    if(VCM_ERR == is_cell_state_available(state))
    {
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].state = state;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cell_state(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    vcm_sm_state_t state = Cells[cell_id-VCM_MIN_CELL_NUM].state;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return state;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_rru_id(CONST UINT32 cell_id, CONST UINT16 rru_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].rru.id = rru_id;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT16 get_rru_id(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT16 id = Cells[cell_id-VCM_MIN_CELL_NUM].rru.id;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return id;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example:
        CONST UINT8 sn[] = {"15962"};
        set_rru_sn(5, sn);
 **********************************************************************************************************************/
INT32 set_rru_sn(CONST UINT32 cell_id, CONST UINT8 rru_sn[VCM_RRU_SN_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)rru_sn) <= 0 || Strlen((VOID*)rru_sn) > VCM_RRU_SN_LEN || (VOID*)rru_sn == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EICRRUSN);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID* ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, (VOID*)rru_sn, VCM_RRU_SN_LEN);
    
    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_rru_sn(CONST UINT32 cell_id, UINT8 rru_sn[VCM_RRU_SN_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID* ret = Strncpy( (VOID*)rru_sn, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, VCM_RRU_SN_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_rru_status(CONST UINT32 cell_id, RRU_STATUS status)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(status <= RRU_STATUS_MIN || status >= RRU_STATUS_MAX)
    {
        log_vcm_err("Rrus status(%d) out of range error.\n", status);
        __vcm_set_errno(_VCM_EICRRUSTATUS);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].rru.status = status;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_rru_status(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    RRU_STATUS status = Cells[cell_id-VCM_MIN_CELL_NUM].rru.status;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return status;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 *  Example
        CONST UINT8 ip[] = {"10.21.3.242"};
        set_rru_ip(5, ip);
 **********************************************************************************************************************/
INT32 set_rru_ip(CONST UINT32 cell_id, CONST UINT8 ip[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)ip) <= 0 || Strlen((VOID*)ip) > VCM_IP_LEN || (VOID*)ip == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].rru.ip, (VOID*)ip, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_rru_ip(CONST UINT32 cell_id, UINT8 ip[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)ip, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].rru.ip, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_rru_port(CONST UINT32 cell_id, CONST UINT16 port)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].rru.port = port;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT16 get_rru_port(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT16 port = Cells[cell_id-VCM_MIN_CELL_NUM].rru.port;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);

    return port;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_rru_axc_speed(CONST UINT32 cell_id, CONST UINT32 axc_speed)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].rru.axc_speed = axc_speed;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_rru_axc_speed(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT32 axc_speed = Cells[cell_id-VCM_MIN_CELL_NUM].rru.axc_speed;
    
    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return axc_speed;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_rru_axc_index(CONST UINT32 cell_id, CONST UINT32 axc_index)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].rru.axc_index = axc_index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_rru_axc_index(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT32 axc_index = Cells[cell_id-VCM_MIN_CELL_NUM].rru.axc_index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return axc_index;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_snmp_index(CONST UINT32 cell_id, CONST UINT32 snmp_index)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].mac.snmp_index = snmp_index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_snmp_index(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT32 snmp_index = Cells[cell_id-VCM_MIN_CELL_NUM].mac.snmp_index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return snmp_index;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_pid(CONST UINT32 cell_id, CONST UINT32 pid)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].mac.pid = pid;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_pid(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT32   pid = Cells[cell_id-VCM_MIN_CELL_NUM].mac.pid;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return pid;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_status(CONST UINT32 cell_id, MAC_STATUS status)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(status <= MAC_STATUS_MIN || status >= MAC_STATUS_MAX)
    {
        log_vcm_err("Mac status(%d) out of range error.\n", status);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].mac.status = status;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
    
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_status(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    MAC_STATUS status = Cells[cell_id-VCM_MIN_CELL_NUM].mac.status;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return (INT32)status;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_cfg_pth(CONST UINT32 cell_id, CONST CHAR cfg_pth[VCM_STR_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)cfg_pth) <= 0 || Strlen((VOID*)cfg_pth) > VCM_STR_LEN || (VOID*)cfg_pth == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.cfg_pth, (VOID*)cfg_pth, VCM_STR_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_cfg_pth(CONST UINT32 cell_id, CHAR cfg_pth[VCM_STR_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)cfg_pth, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.cfg_pth, VCM_STR_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_ether(CONST UINT32 cell_id, CONST CHAR ether[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    
    if(Strlen((VOID*)ether) <= 0 || Strlen((VOID*)ether) > VCM_IP_LEN)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.ether, (VOID*)ether, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_ether(CONST UINT32 cell_id, CHAR ether[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)ether, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.ether, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_enb_ipaddr(CONST UINT32 cell_id, CONST CHAR enb_ipaddr[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)enb_ipaddr) <= 0 || Strlen((VOID*)enb_ipaddr) > VCM_IP_LEN || (VOID*)enb_ipaddr == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.enb_ipaddr, (VOID*)enb_ipaddr, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_enb_ipaddr(CONST UINT32 cell_id, CHAR enb_ipaddr[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)enb_ipaddr, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.enb_ipaddr, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_gtpu_recv_addr(CONST UINT32 cell_id, CONST CHAR gtpu_recv_addr[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)gtpu_recv_addr) <= 0 || Strlen((VOID*)gtpu_recv_addr) > VCM_IP_LEN || (VOID*)gtpu_recv_addr == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.gtpu_recv_addr, (VOID*)gtpu_recv_addr, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_gtpu_recv_addr(CONST UINT32 cell_id, CHAR gtpu_recv_addr[VCM_IP_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)gtpu_recv_addr, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.gtpu_recv_addr, VCM_IP_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_vbs_eth(CONST UINT32 cell_id, CONST CHAR vbs_eth[VCM_STR_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)vbs_eth) <= 0 || Strlen((VOID*)vbs_eth) > VCM_STR_LEN || (VOID*)vbs_eth == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.vbs_eth, (VOID*)vbs_eth, VCM_STR_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_vbs_eth(CONST UINT32 cell_id, CHAR vbs_eth[VCM_STR_LEN])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID *ret = Strncpy( (VOID*)vbs_eth, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].mac.vbs_eth, VCM_STR_LEN);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_mac_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(sockfd < 0)
    {
        log_vcm_err("Socket file descriptor number(%d) error.\n", sockfd);
        __vcm_set_errno(_VCM_EICMACSOCKFD);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].mac.sockfd = sockfd;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_mac_sockfd(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT32 sockfd = Cells[cell_id-VCM_MIN_CELL_NUM].mac.sockfd;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return sockfd;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_phy_index(CONST UINT32 cell_id, CONST UINT8 index)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(index < 0)
    {
        log_vcm_err("Phy index(%d) error.\n", index);
        __vcm_set_errno(_VCM_EICPHYINDEX);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.index = index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT8 get_phy_index(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return (INT8)VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT8    index = Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return index;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_phy_FpgaMac(CONST UINT32 cell_id, CONST UINT8 FpgaMac[6])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(Strlen((VOID*)FpgaMac) <= 0 || Strlen((VOID*)FpgaMac) > 6 || (VOID*)FpgaMac == NULL)
    {
        log_vcm_err("String length out of range error.\n");
        __vcm_set_errno(_VCM_EOORSTRING);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID* ret = Strncpy((VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.FpgaMac, (VOID*)FpgaMac, 6);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_phy_FpgaMac(CONST UINT32 cell_id, UINT8 FpgaMac[6])
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    VOID* ret = Strncpy( (VOID*)FpgaMac, (VOID*)Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.FpgaMac, 6);

    __SEM_CELL_UNLOCK
    
    if(NULL == ret)
    {
        log_vcm_err("String copy error.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    else
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_phy_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(sockfd < 0)
    {
        log_vcm_err("Socket file descriptor number(%d) error.\n", sockfd);
        __vcm_set_errno(_VCM_EICPHYSOCKFD);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.sockfd = sockfd;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_phy_sockfd(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT32 sockfd = Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.sockfd;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return sockfd;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_simu_phy_index(CONST UINT32 cell_id, CONST UINT8 index)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(index < SIMU_PHY_INDEX_START || index > SIMU_PHY_INDEX_END)
    {
        __vcm_set_errno(_VCM_EICPHYINDEX);
        log_vcm_err("Simulate phy index(%d) out of range error.\n", index);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.index = index;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT8 get_simu_phy_index(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT8    index = Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.index;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return index;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_simu_phy_pid(CONST UINT32 cell_id, CONST UINT32 pid)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.pid = pid;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_simu_phy_pid(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT32 pid = Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.pid;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return pid;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_simu_phy_tcpServerPort(CONST UINT32 cell_id, UINT64 tcpServerPort)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.tcpServerPort = tcpServerPort;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT64 get_simu_phy_tcpServerPort(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return (INT64)VCM_ERR;
    }

    __SEM_CELL_LOCK

    UINT64 tcpServerPort = Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.tcpServerPort;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return tcpServerPort;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_simu_phy_status(CONST UINT32 cell_id, SIMU_PHY_STATUS status)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(status <= SIMU_PHY_STATUS_MIN || status >= SIMU_PHY_STATUS_MAX)
    {
        __vcm_set_errno(_VCM_EICPHYSTATUS);
        log_vcm_err("Simulate phy status(%d) out of range error.\n", status);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.status = status;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_simu_phy_status(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    SIMU_PHY_STATUS status = Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.status;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return (INT32)status;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 set_simu_phy_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    if(sockfd < 0)
    {
        log_vcm_err("Socket file descriptor number(%d) error.\n", sockfd);
        __vcm_set_errno(_VCM_EICPHYSOCKFD);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.sockfd = sockfd;

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_simu_phy_sockfd(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK

    INT32 sockfd = Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu.sockfd;

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return sockfd;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cellnum_of_celltype(CONST cell_type_t type)
{
    if(VCM_ERR == is_cell_type_available(type))
    {
        __vcm_set_errno(_VCM_EICTYPE);
        return VCM_ERR;
    }

    UINT32 count = 0;
    UINT32 id = 0;

    __SEM_CELL_LOCK
    
    for(id=VCM_MIN_CELL_NUM; id<=VCM_MAX_CELL_NUM; id++)
    {
        if(get_cell_type(id) == type)
        {
            count++;
        }
    }

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);

    return count;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cellnum_of_cellstate(CONST vcm_sm_state_t state)
{
    if(VCM_ERR == is_cell_state_available(state))
    {
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }
    
    UINT32 count = 0;
    UINT32 id = 0;

    __SEM_CELL_LOCK
    
    for(id=VCM_MIN_CELL_NUM; id<=VCM_MAX_CELL_NUM; id++)
    {
        if(get_cell_state(id) == state)
        {
            count++;
        }
    }

    __SEM_CELL_UNLOCK
        
    __vcm_set_errno(_VCM_ESUCCESS);
    return count;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cellnum_of_cellmode(CONST cell_mode_t mode)
{
    if(VCM_ERR == is_cell_mode_available(mode))
    {
        __vcm_set_errno(_VCM_EICMODE);
        return VCM_ERR;
    }

    UINT32 count = 0;
    UINT32 id = 0;

    __SEM_CELL_LOCK
    
    for(id=VCM_MIN_CELL_NUM; id<=VCM_MAX_CELL_NUM; id++)
    {
        if(get_cell_mode(id) == mode)
        {
            count++;
        }
    }

    __SEM_CELL_UNLOCK

    __vcm_set_errno(_VCM_ESUCCESS);

    return count;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_mode(CONST UINT32 cell_id, CONST cell_mode_t mode)
{
    if(VCM_ERR == is_cell_mode_available(mode))
    {
        __vcm_set_errno(_VCM_EICMODE);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    cell_mode_t mode2 = get_cell_mode(cell_id);
    __SEM_CELL_UNLOCK
    
    if(mode2 == mode)
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
    __vcm_set_errno(_VCM_EICMODE);
    return VCM_ERR;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_state(CONST UINT32 cell_id, CONST vcm_sm_state_t state)
{
    if(VCM_ERR == is_cell_state_available(state))
    {
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    vcm_sm_state_t state2 = get_cell_state(cell_id);
    __SEM_CELL_UNLOCK
        
    if(state2 == state)
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
    __vcm_set_errno(_VCM_EICSTATE);
    return VCM_ERR;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_type(CONST UINT32 cell_id, CONST cell_type_t type)
{
    if(VCM_ERR == is_cell_type_available(type))
    {
        __vcm_set_errno(_VCM_EICTYPE);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    cell_type_t type2 = get_cell_type(cell_id);
    __SEM_CELL_UNLOCK
        
    if(type2 == type)
    {
        __vcm_set_errno(_VCM_ESUCCESS);
        return VCM_OK;
    }
    __vcm_set_errno(_VCM_EICTYPE);
    return VCM_ERR;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 cell_recover(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }
    /**
     *  If you wanna recover a cell from STOP state, the cell must be STOP state.
     */
    vcm_sm_state_t state = get_cell_state(cell_id);
    if(state != CELL_STOP || state == VCM_ERR)
    {
        log_vcm_err("Can not recover cell(%d) from STOP.\n", cell_id);
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }

    /**
     *  Maybe there are some other handler.
     *  For example:
     *      1.When receive a message, but cell State is STOP, this message
     *        can storge in Cells[cell_id-VCM_MIN_CELL_NUM].recover.arg. And 
     *        wait for recover.
     */
    
    return set_cell_state(cell_id, Cells[cell_id-VCM_MIN_CELL_NUM].recover.state);
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 cell_stop(CONST UINT32 cell_id)
{
    if(VCM_ERR == is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS)
    {
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    /**
     *  If state is VC_STATE_MIN_NUM, means this cell is ZERO
     */
    vcm_sm_state_t state = get_cell_state(cell_id);
    if(state == VCM_ERR || state == VC_STATE_MIN_NUM)
    {
        log_vcm_err("Can not stop cell(%d) from Invalid State(%d).\n", cell_id, state);
        __vcm_set_errno(_VCM_EICSTATE);
        return VCM_ERR;
    }

    __SEM_CELL_LOCK
    
    Cells[cell_id-VCM_MIN_CELL_NUM].recover.state = state;
    set_cell_state(cell_id, CELL_STOP);


    __SEM_CELL_UNLOCK

    /**
     *  Maybe there are some other handler.
     *  For example:
     *      1.Send message Cells[cell_id-VCM_MIN_CELL_NUM].recover.arg to own msgq.
     */
    //TODO:
    
    __vcm_set_errno(_VCM_ESUCCESS);
    return VCM_OK;    
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 cell_info_display(FILE *fp, CONST UINT32 cell_id)
{
#ifdef _VCM_DISPLAY
    if(VCM_ERR == is_cell_id_available(cell_id))
    {
        __vcm_set_errno(_VCM_EICID);
        log_vcm_err("cell_id out of range error. Don't display cell(s).\n");
        return VCM_ERR;
    }
    if(fp == NULL)
    {
        log_vcm_err("NULL pointer error. Don't display cell(s).\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }
    
    __SEM_CELL_LOCK
    
    /* display all cells */
    if(cell_id == VCM_ALL_CELLS)
    {
        UINT32 nc, nv, ns, nn;
        Fprintf(fp, "+-%sShow all cells%s-------------------------------------------{\n", VCM_ANSI_RED, VCM_ANSI_RESET);
        Fprintf(fp, "|    Cells range : [%d - %d] / %d  \n", VCM_MIN_CELL_NUM, VCM_MAX_CELL_NUM, VCM_CELL_NUM);
        Fprintf(fp, "|Cells set range : [%d - %d] / %d  \n", 0, VCM_CELL_NUM-1, VCM_CELL_NUM);
        Fprintf(fp, "|  Current cells : "); nc=vcm_set_display(fp, all_cell_set, 0, VCM_CELL_NUM, 'c'); Fprintf(fp, "\n");
        Fprintf(fp, "|  Virtual cells : "); nv=vcm_set_display(fp, virt_cell_set, 0, VCM_CELL_NUM,'v'); Fprintf(fp, "\n");
        Fprintf(fp, "| Simulate cells : "); ns=vcm_set_display(fp, simu_cell_set, 0, VCM_CELL_NUM,'s'); Fprintf(fp, "\n");
        Fprintf(fp, "|   Normal cells : "); nn=vcm_set_display(fp, normal_cell_set, 0, VCM_CELL_NUM,'n'); Fprintf(fp, "\n");
        Fprintf(fp, "|       Subtotal : all[%d], virtual[%d], simulate[%d], normal[%d]\n", nc, nv, ns, nn);
        Fprintf(fp, "+----------------------------------------------------------}\n");
    }
    /* display one cell */
    else
    {
        Fprintf(fp, "+-%sShow cell %d%s---------------------------------------------{\n", VCM_ANSI_RED, cell_id, VCM_ANSI_RESET);
        Fprintf(fp, "|        cell id : %d\n", get_cell_id(cell_id));
        Fprintf(fp, "|     snmp index : %d\n", get_snmp_index(cell_id));
        Fprintf(fp, "|      global id : %d\n", get_g_id(cell_id));
        Fprintf(fp, "|      cell type : %s\n", get_cell_type(cell_id)==CELL_VIRTUAL?"Virtual":"Simulate");
        Fprintf(fp, "|         unify? : %s\n", get_is_unify(cell_id)==UNIFY_ON?"Unify":"Not Unify");
        Fprintf(fp, "|     cell power : %d\n", get_cell_power(cell_id));
        Fprintf(fp, "|    user number : %d\n", get_cell_usernum(cell_id));
        Fprintf(fp, "| cell bandwidth : %d\n", get_cell_bandwidth(cell_id));
        Fprintf(fp, "|      cell mode : %s\n", vcm_mode_str[get_cell_mode(cell_id)]);
        Fprintf(fp, "|     cell state : %s\n", vcm_sm_state_str[get_cell_state(cell_id)]);

        /* RRU information */
        if(get_cell_type(cell_id)==CELL_VIRTUAL)
        {
            UINT8 rru_sn[VCM_RRU_SN_LEN], rru_ip[VCM_IP_LEN];
            get_rru_sn(cell_id, rru_sn);
            get_rru_ip(cell_id, rru_ip);
            Fprintf(fp, "|      rru infos : id(%#x), sn(%s), axc(speed:%d, index:%d)\n"\
                        "|         rru ip : addr(%s), port(%d)\n"\
                        "|     rru status : status(%s)\n", 
                        get_rru_id(cell_id), rru_sn, 
                        get_rru_axc_speed(cell_id), get_rru_axc_index(cell_id), 
                        rru_ip, get_rru_port(cell_id),
                        vcm_rru_status_str[get_rru_status(cell_id)]);
        }//RRU info
        
        /* MAC information */
        CHAR cfg_pth[VCM_STR_LEN], ether[VCM_IP_LEN], enb_ipaddr[VCM_IP_LEN], 
            gtpu_recv_addr[VCM_IP_LEN], vbs_eth[VCM_STR_LEN];
        get_mac_cfg_pth(cell_id, cfg_pth);
        get_mac_ether(cell_id, ether);
        get_mac_enb_ipaddr(cell_id, enb_ipaddr);
        get_mac_gtpu_recv_addr(cell_id, gtpu_recv_addr);
        get_mac_vbs_eth(cell_id, vbs_eth);
        Fprintf(fp, "|       mac info : snmp_index(%d), pid(%d), status(%s), sockfd(%d)\n"\
                    "|   mac cfg path : %s, \n"\
                    "|         mac ip : ether(%s), enb(%s), recv(%s), vbs(%s), \n",
                    get_mac_snmp_index(cell_id), get_mac_pid(cell_id), vcm_mac_status_str[get_mac_status(cell_id)],
                    get_mac_sockfd(cell_id), cfg_pth, ether, enb_ipaddr, gtpu_recv_addr, vbs_eth);

        /* PHY information */
        if(get_cell_type(cell_id)==CELL_VIRTUAL)
        {
            UINT8 FpgaMac[6];
            get_phy_FpgaMac(cell_id, FpgaMac);
            Fprintf(fp, "|      phy infos : index(%d), FpgaMac(%s), sockfd(%d)\n",
                        get_phy_index(cell_id), FpgaMac, get_phy_sockfd(cell_id));
        }
        else if(get_cell_type(cell_id)==CELL_SIMULATE)
        {
            Fprintf(fp, "|      phy infos : index(%d), pid(%d), port(%ld), sockfd(%d)\n",
                        get_simu_phy_index(cell_id), get_simu_phy_pid(cell_id), 
                        get_simu_phy_tcpServerPort(cell_id), get_phy_sockfd(cell_id));
        }
        
        Fprintf(fp, "+----------------------------------------------------------}\n");
    }

    __SEM_CELL_UNLOCK
    
    __vcm_set_errno(_VCM_ESUCCESS);
    
#endif /* _VCM_DISPLAY */
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_rrus_display(FILE *fp)
{
#ifdef _VCM_DISPLAY
    if(fp == NULL)
    {
        log_vcm_err("NULL pointer error. Don't display cell(s).\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    #if 0
    rru_info_t     Rrus[VCM_RRU_NUM];  /* rrus */
    UINT16          id;         // 0000 0000      0000       0000
                                    //  fpga_id     cpri-port  rru jilian
    UINT8           sn[VCM_RRU_SN_LEN];     // RRU sn
    RRU_STATUS      status;     // 0: added;  1: deleted 
    UINT8           ip[VCM_IP_LEN];     // RRU IP
    UINT16          port;       // RRU port
    UINT32          axc_speed;      // 1.4M, 3M, 5M, 10M, 15M, 20M
    UINT32          axc_index;      // AxC 0100010100101000010
    #endif
    Fprintf(fp, "+-%sShow Rrus%s---------------------------------------------{\n", VCM_ANSI_RED, VCM_ANSI_RESET);
    Fprintf(fp, "| rru -> id:sn:status:ip:port:axc_speed:axc_index   \n");
    UINT32 irru;
    for(irru=0; irru<VCM_RRU_NUM; irru++)
    {
        if(Rrus[irru].id != 0) 
        {

            Fprintf(fp, "|\t %x:%s:%s:%s:%d:%x:%x\n",
                            Rrus[irru].id, 
                            (CHAR*)Rrus[irru].sn, 
                            vcm_rru_status_str[Rrus[irru].status],
                            (CHAR*)Rrus[irru].ip,
                            Rrus[irru].port,
                            Rrus[irru].axc_speed,
                            Rrus[irru].axc_index
                            );
            Memdisplay(fp, (VOID*)&Rrus[irru], sizeof(rru_info_t), 20);
        }
    }
    Fprintf(fp, "+-----------------------------------------------------------}\n");


#endif /* _VCM_DISPLAY */
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_handle_rru_reg_req(CONST vcm_outer_msg_rrum_rru_reg_req *msg_p)
{
    log_vcm_debug();

    if (msg_p == NULL)
    {
        log_vcm_err("NULL pointer.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
    	return VCM_ERR;
    }
    
    vcm_outer_msg_rrum_rru_reg_req *msg = (vcm_outer_msg_rrum_rru_reg_req *)msg_p;
    
    // throw msg not complete error
    if (msg->hdr.len < sizeof(vcm_outer_msg_rrum_rru_reg_req))
    {
        log_vcm_err("msg not complete, msg[type(%d) code(%d) len(%d)<%d]\n",
                        msg->hdr.type, msg->hdr.code, msg->hdr.len,
                        sizeof(vcm_outer_msg_rrum_rru_reg_req));
        __vcm_set_errno(_VCM_EMLENERR);
        return VCM_ERR;
    }

    /**
     *  Decode rru register message
     */
    int ret;
    
    /* RruReg rru_reg = RRU_ADD, RRU_DEL */
    switch(msg->rru_reg)
    {
        case RRU_REGISTER:
            log_vcm_info("RRU register: RRU_ADD\n");
            ret = vcm_rru_register(msg);
            
            break;

        case RRU_UNREGISTER:
            log_vcm_info("RRU register: RRU_DEL\n");
            ret = vcm_rru_unregister(msg);

            break;

        default:
            log_vcm_err("unexpected msg code\n");
            ret = VCM_ERR;
    }
    return ret;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_rru_register(CONST vcm_outer_msg_rrum_rru_reg_req *rru)
{
    log_vcm_debug();
    
    typedef enum { RRU_EXIST, RRU_NOTEXIST, }RRU_STATE;

    if (rru == NULL)
    {
        log_vcm_err("NULL pointer.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
    	return VCM_ERR;
    }

    RRU_STATE   RRU_flag = RRU_NOTEXIST;
    UINT32      irru;
    UINT32      cell_id;
    
    for (irru = 0; irru < VCM_RRU_NUM; irru ++)
	{    
	    /**
	     *  if this rru is already an entry in global rru table
	     */
		if (Rrus[irru].status != RRU_STATUS_MIN)
		{
			log_vcm_info("irru=%d, msg->rru_sn == %s\n", irru, rru->rru_sn);
            
			/**
			 *  compare rru_sn is sufficient
			 */
			if (Memcmp(Rrus[irru].sn, rru->rru_sn, VCM_RRU_SN_LEN) == 0)
			{
				log_vcm_info("rru_sn already exist: %d %d %d %d\n",
						rru->rru_sn[0], rru->rru_sn[1], rru->rru_sn[2], rru->rru_sn[3]);

				/**
				 *  update its state into RRU_ADDED
				 */
				Rrus[irru].status = RRU_STATUS_ADDED;
                
				/**
				 *  fill rru_id if necessary in case it does not have it
				 */
				if (Rrus[irru].id == 0)
				{
					Rrus[irru].id = rru->rru_id;
				}
                
				RRU_flag = RRU_EXIST;
                
				break;
			}
		}
	}
    
    /**
     *  If rru already exist, then added == 1.
     *  If rru not exist in rru_info_table[], then execute following lines.
     */
	if (RRU_flag != RRU_EXIST)
	{   
	    /**
	     *  if this rru has not had an entry in rru_info_table,
		 *  then add an entry in the first entry with no rru status
		 */
		for (irru = 0; irru < VCM_RRU_NUM; irru ++)
		{
			if (Rrus[irru].status == RRU_STATUS_MIN)
			{
				log_vcm_info("add rru: Rrus[%d], %d%d%d%d%d\n",
						irru, 
						rru->rru_sn[0], 
						rru->rru_sn[1],
						rru->rru_sn[2],
						rru->rru_sn[3],
						rru->rru_sn[4]);
                
				Rrus[irru].id = rru->rru_id;

				Memcpy(Rrus[irru].sn, rru->rru_sn, VCM_RRU_SN_LEN);

				Rrus[irru].status = RRU_STATUS_ADDED;

				RRU_flag = RRU_EXIST;

				break;
			}
		}
	}

    /**
     *  If there is no room for new rrus.
     */
    if (RRU_flag != RRU_EXIST || irru == VCM_RRU_NUM)
	{
	    /**
	     *  If rru_info_table[] not room to place new rru info, then return 
	     *  VCM_ERR(-1). 
	     */
		log_vcm_err("there is no place for new RRU: %d%d%d%d%d\n", 
		        rru->rru_sn[0], 
				rru->rru_sn[1],
				rru->rru_sn[2],
				rru->rru_sn[3],
				rru->rru_sn[4]);
		/**
		 *  if there is no place for new RRU
		 */
		return VCM_ERR;
	}

    /**
     *  If there have some room for RRU and RRU sn is already exist in CELL entity which already
     *  setup but wait rru.
     */
    for (cell_id = VCM_MIN_CELL_NUM; cell_id <= VCM_MAX_CELL_NUM; cell_id ++)
	{
	    /**
	     *  If msg->rru_sn == cell_rru.rru_sn, that is to say if we already get
	     *  a rru info from CFIG_MGMT's cell setup request 
	     */
 		if (Memcmp(Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, rru->rru_sn, VCM_RRU_SN_LEN) == 0)
		{
			log_vcm_info("Cells[%d].cell_rru.rru_sn = <%s> and <%s>\n",
				    cell_id, Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, rru->rru_sn);

            /**
             *  some cell state may recv RRU register message
             */
            switch(get_cell_state(cell_id))
            {
                /**
                 *  If CELL_STOP, maybe because this cell don't have rru during it's running,
                 *  We turn this cell to CELL_STOP, when recv rru register we should recover
                 *  This cell to the STATE like before.
                 *  PS:
                 *      CELL_STOP use to deal with board clock sync and rru register
                 */
                case CELL_STOP:
                    cell_recover(cell_id);
                    break;
                case CELL_NULL:
                    set_rru_status(cell_id, RRU_STATUS_ADDED);
                    set_rru_id(cell_id, rru->rru_id);

    				log_vcm_info("RRU regeister successful!\n");
    				log_vcm_info("Cells[%d].cell_rru.rru_id = %d\n", cell_id-VCM_MIN_CELL_NUM, rru->rru_id);
    				log_vcm_info("Cells[%d].cell_rru.rru_sn = %s\n", cell_id-VCM_MIN_CELL_NUM, rru->rru_sn);

    				log_vcm_info("generate Cpri AxC config request.\n");
                    
    				/**
    				 *  config cpri AXC
    				 */
    				#if 0
                    //TODO
    				vcm_outer_msg_rrum_cpriaxc_cfg_req cpri_cfg_msg;

    				cpri_cfg_msg.hdr.type  = MODULE_CELL_MGMT;
    				cpri_cfg_msg.hdr.code  = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
    				cpri_cfg_msg.hdr.len   = sizeof(MsgRrumCpriAxcCfgReq);
                    cpri_cfg_msg.cell_id = cell_id;
    				cpri_cfg_msg.rru_id    = vc_entity_table[icell].cell_rru.rru_id;
    				cpri_cfg_msg.axc_speed = vc_entity_table[icell].cell_bandwidth;
    				cpri_cfg_msg.cfg_ind   = 1;

    				log_vcm_info("send MsgRrumCpriAxcCfgReq to RRUS_MGMT \n");

                    if (g_ne_sync_state != NE_SYNC_OK)
                    {
                        log_vcm_err("ne sync state is not OK, "\
                                "cannot set cpri-axc\n");
                        return FUNC_ERR;
                    }
                                                            
    				if((ret = send_msg_to_om(MODULE_RRUS_MGMT, &cpri_cfg_msg,
    						sizeof(MsgRrumCpriAxcCfgReq))) == FUNC_OK)
    				{
    					log_vcm_info("send cpri cfg msg to RRUM Success\n");
    				}
                    else
                    {
                        log_vcm_err("send cpri cfg msg to RRUM Error\n");
                    }

    				log_vcm_info("Cells[%d].state=WAIT_SETUP_CPRI_CFG \n", cell_id-VCM_MIN_CELL_NUM);
                    #endif //0
                    
    				/**
    				 *  change cell state into WAIT_SETUP_CPRI_CFG 
    				 */
    				set_cell_state(cell_id, WAIT_SETUP_CPRI_CFG);
                    
                    break;
                    
                case WAIT_SETUP_CPRI_CFG:
                case WAIT_SETUP_PHY_CFG:
                case WAIT_SETUP_MAC_INIT:
                case WAIT_SETUP_CELL_START:
                case CELL_NORMAL:
                case WAIT_DEL_CELL_CLOSE:
                case WAIT_DEL_MAC_DEL:
                case WAIT_DEL_PHY_DEL:
                case WAIT_DEL_CPRI_RESET:
                default:
                    log_vcm_err("This cell's rru already register. cell_id = %d\n", cell_id);
                    break;

            }
            /**
             *  run here means that we find the rru sn in cells
             */
			break;
		}
	}

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_rru_unregister(CONST vcm_outer_msg_rrum_rru_reg_req *rru)
{
    log_vcm_debug();
    
    typedef enum { RRU_DELETE, RRU_NODELETE, }RRU_STATE;

    if (rru == NULL)
    {
        log_vcm_err("NULL pointer.\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
    	return VCM_ERR;
    }

    RRU_STATE   RRU_flag = RRU_NODELETE;
    UINT32      irru;
    UINT32      cell_id;

    /**
     *  0921 add handle for RRU del msg
	 * STEP 1: update Rrus
	 */
	for (irru = 0; irru < VCM_RRU_NUM; irru ++)
	{
		/**
		 *  rru id must has been stored in registered rru
		 *  compare rru_id is sufficient
		 *  if no match (deleted != 1), return VCM_ERR OR ignore it and return VCM_OK
		 */
		if (Rrus[irru].id == rru->rru_id)
		{
			/**
			 *  throw sn error
			 */
			if (Memcmp(Rrus[irru].sn, rru->rru_sn, VCM_RRU_SN_LEN) != 0)
			{
                log_vcm_err("unexpected rru sn = %s, failed delete rru_id = %d\n",
                        rru->rru_sn, rru->rru_id);
                __vcm_set_errno(_VCM_EICRRUSN);
				return VCM_ERR;
			}

			/**
			 *  throw state error
			 */
			if (Rrus[irru].status == RRU_STATUS_MIN)
			{
                log_vcm_err("unexpected rru state = %d, failed delete rru_id = %d\n", 
                        Rrus[irru].status, rru->rru_id);
                __vcm_set_errno(_VCM_EICRRUSTATUS);
				return VCM_ERR;
			}

			/**
			 *  deal with duplicated request
			 */
			if (Rrus[irru].status == RRU_STATUS_DELETED)
			{
                log_vcm_info("duplicated RRU delete request, rru_id = %d\n", rru->rru_id);
				return VCM_OK;

			}
			/**
			 *  change rru_status into RRU_DELETED
			 */
			Rrus[irru].status = RRU_STATUS_DELETED;
            
			RRU_flag = RRU_DELETE;
            
			break;
		}
	}
    
    /**
     *  deal with no match
     */
	if (RRU_flag != RRU_DELETE)
	{
		log_vcm_err("no matching rru: rru_id = %d, rru_sn = %s\n", rru->rru_id, rru->rru_sn);
        __vcm_set_errno(_VCM_EICRRUID);
		return VCM_ERR;
	}

    /**
     *  STEP 2: change rru info in vc_entity_table, trigger rru_err_del process
     */
	for (cell_id = VCM_MIN_CELL_NUM; cell_id <= VCM_MAX_CELL_NUM; cell_id ++)
	{
		/**
		 *  compare rru_sn
		 */
 		if (Memcmp(Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, rru->rru_sn, VCM_RRU_SN_LEN) == 0)
		{
		    log_vcm_info("<%s>:<%s>\n", Cells[cell_id-VCM_MIN_CELL_NUM].rru.sn, rru->rru_sn);
            
			switch (get_cell_state(cell_id))
			{
				case CELL_NULL:
                case WAIT_SETUP_CPRI_CFG:
                case WAIT_SETUP_PHY_CFG:
                case WAIT_SETUP_MAC_INIT:
                case WAIT_SETUP_CELL_START:
                case CELL_NORMAL:
                case WAIT_DEL_CELL_CLOSE:
                case WAIT_DEL_MAC_DEL:
                case WAIT_DEL_PHY_DEL:
                case WAIT_DEL_CPRI_RESET:
					/**
					 *  change rru status
					 */
					log_vcm_warn("RRU delete, couldn't run this cell: cell_id = %d\n", cell_id);
					set_rru_status(cell_id, RRU_STATUS_DELETED);
					set_cell_state(cell_id, CELL_STOP);
                    cell_stop(cell_id);
					break;

                case CELL_STOP:
                    /**
					 *  change rru status
					 */
					set_rru_status(cell_id, RRU_STATUS_DELETED);
                    break;

				default:
					/**
					 *  invoke handle_rru_reg_err_del method, Now just log
					 */
					log_vcm_err("Can't unregister RRU: id = %d\n", rru->rru_id);

                    return VCM_ERR;
                    
					break;
			}
			break; // break loop
		}
	}
    
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description: clock synchronous handler
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_handle_bord_clock_sync()
{
    log_vcm_debug();



    
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description: clock synchronous handler
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_handle_bord_clock_async()
{
    log_vcm_debug();




    return VCM_OK;
}


