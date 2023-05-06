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
** File name: perfs_tools.c
** Description:  performance statistics module's  tools
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/

/* Globals -----------------------------------------------------------------------------------------------------------*/
extern PerfCell        PerfCells[PERF_CELL_NUM];
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 log_perf(FILE *fp, LOG_PERF_TYPE type, 
                CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...)
{
    /**
     *  Check log type valiable
     */
    if(type <= PERF_LOG_TYPE_MIN || type >= PERF_LOG_TYPE_MAX)
    {
        va_list arg1;
        va_start(arg1, fmt);
        
        Vfprintf(fp, fmt, arg1);
        
        va_end(arg1);
        
        return PERF_WARN;
    }

    /**
     *  Switch the log type
     */
    switch(type)
    {
        case PERF_LOG_ERROR:
            Fprintf(fp, "  %s[Error][perf]%s %s:%s:%d ", PERF_ANSI_RED, PERF_ANSI_RESET, __file, __func, __line);
            break;
        
        case PERF_LOG_INFO:
            Fprintf(fp, "   %s[Info][perf]%s %s:%s:%d ", PERF_ANSI_BLUE, PERF_ANSI_RESET, __file, __func, __line);
            break;
            
        case PERF_LOG_SUCCESS:
            Fprintf(fp, "%s[Success][perf]%s %s:%s:%d ", PERF_ANSI_GREEN, PERF_ANSI_RESET, __file, __func, __line);
            break;
        
        case PERF_LOG_WARNING:
            Fprintf(fp, "%s[Warning][perf]%s %s:%s:%d ", PERF_ANSI_PURPLE, PERF_ANSI_RESET, __file, __func, __line);
            break;
        
        case PERF_LOG_DEBUG:
            Fprintf(fp, "  %s[Debug][perf]%s %s:%s:%d ", PERF_ANSI_GRAY, PERF_ANSI_RESET, __file, __func, __line);
            break;

        case PERF_LOG_SIMPLE:
            //Do nothing
            break;
        default:
            //TODO
            break;
    }

    /**
     *  log
     */
    va_list arg;
    va_start(arg, fmt);

    Vfprintf(fp, fmt, arg);
    
    va_end(arg);

    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 get_cell_perf(UINT32 cell_id, cell_perf_t *cell_perf)
{
    log_perf_debug();

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    /** No need mutex
    static sem_t* cell_sem_lock = Malloc(sizeof(sem_t));;
    if(sem_init(cell_sem_lock, 0, 1) != 0)
    {
        log_perf_err("sem_init error\n");
    }*/
    
    if(cell_perf == NULL)
    {
        /**
         *  Never use malloc to allocate new memmory for cell_perf
         *  Just return ERROR
         */
        log_perf_err("NULL pointer error.\n");
        return PERF_ERR;
    }

    if(cell_id < MIN_CELL_ID || cell_id > MAX_CELL_ID)
    {
        log_perf_err("cell id out of range (%d)\n", cell_id);
        Memset(cell_perf, 0, sizeof(cell_perf_t));
        return PERF_ERR;
    }

    /**
     *  Fill in cell_perf
     */
    cell_perf->cell_id = cell_id;
    Memcpy(&(cell_perf->mac), &(PerfCells[cell_id-MIN_CELL_ID].mac), sizeof(perf_mac_t));
    
    if(PerfCells[cell_id-MIN_CELL_ID].cell_type == CELL_VIRTUAL)
        Memcpy(&(cell_perf->realphy), &(PerfCells[cell_id-MIN_CELL_ID].realphy), sizeof(perf_realphy_t));
    if(PerfCells[cell_id-MIN_CELL_ID].cell_type == CELL_SIMULATE)
        Memcpy(&(cell_perf->simuphy), &(PerfCells[cell_id-MIN_CELL_ID].simuphy), sizeof(perf_simuphy_t));
    else
    {
        log_perf_err("No this kind of perf cell entity cell type.\n");
        return PERF_ERR;
    }
    
    return PERF_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 display_cell_performance(FILE *fp, UINT32 cell_id)
{
    log_perf_debug();

    if(cell_id < MIN_CELL_ID || cell_id > MAX_CELL_ID)
    {
        log_perf_err("cell id out of range (%d)\n", cell_id);
        return PERF_ERR;
    }

    if(PerfCells[cell_id-MIN_CELL_ID].state != PERF_VALIDE_STATE)
    {
        log_perf_err("cell is not ready yet\n");
        return PERF_ERR;
    }

    if(fp == NULL) fp = STDOUT;
    
#define __MAC       PerfCells[cell_id-MIN_CELL_ID].mac
#define __MACEBR    PerfCells[cell_id-MIN_CELL_ID].mac.ebr
#define __MACLR     PerfCells[cell_id-MIN_CELL_ID].mac.lr
#define __MACBW     PerfCells[cell_id-MIN_CELL_ID].mac.bw_occupy
#define __MACACCESS PerfCells[cell_id-MIN_CELL_ID].mac.ue_asr 

#define __SHOW_PHY  (PerfCells[cell_id-MIN_CELL_ID].cell_type==0)
    
#define __PHY       PerfCells[cell_id-MIN_CELL_ID].realphy
#define __PHYTO_D0  PerfCells[cell_id-MIN_CELL_ID].realphy.dl_through[0]
#define __PHYTO_D1  PerfCells[cell_id-MIN_CELL_ID].realphy.dl_through[1]
#define __PHYTO_D2  PerfCells[cell_id-MIN_CELL_ID].realphy.dl_through[2]
#define __PHYTO_U   PerfCells[cell_id-MIN_CELL_ID].realphy.ul_through
#define __PHYSN0    PerfCells[cell_id-MIN_CELL_ID].realphy.sn[0]
#define __PHYSN1    PerfCells[cell_id-MIN_CELL_ID].realphy.sn[1]

    Fprintf(fp, "+-Cell Performance--------------------------------------------------+\n");
    Fprintf(fp, "|                                                                    \n");
    Fprintf(fp, "|  +-Mac------------------------------------------------+\n");
    Fprintf(fp, "|  | error Block rate       : %d.%d                      \n", __MACEBR.i,__MACEBR.d);
    Fprintf(fp, "|  | PDCP rate              : uplink(%ld), downlink(%ld) \n", __MACLR.pdcp_ul,__MACLR.pdcp_dl);
    Fprintf(fp, "|  | RLC  rate              : uplink(%ld), downlink(%ld) \n", __MACLR.rlc_ul,__MACLR.rlc_dl);
    Fprintf(fp, "|  | MAC  rate              : uplink(%ld), downlink(%ld) \n", __MACLR.mac_ul,__MACLR.mac_dl);
    Fprintf(fp, "|  | RLC buffer             : %d                         \n", __MAC.rlc_buf);
    Fprintf(fp, "|  | Bandwidth Occupancy    : %d.%d                      \n", __MACBW.i, __MACBW.d);
    Fprintf(fp, "|  | UE on/off              : %d/%d                      \n", __MAC.ue_ons, __MAC.ue_offs);
    Fprintf(fp, "|  | UE access success rate : %d.%d                      \n", __MACACCESS.i, __MACACCESS.d);
    Fprintf(fp, "|  +----------------------------------------------------+\n");
    Fprintf(fp, "|                                                        \n");
  if(__SHOW_PHY){
    Fprintf(fp, "|  +-Phy------------------------------------------------+\n");
    Fprintf(fp, "|  | Throught Out:  downlink Code 0: %d.%d               \n", __PHYTO_D0.i, __PHYTO_D0.d);
    Fprintf(fp, "|  |                downlink Code 1: %d.%d               \n", __PHYTO_D1.i, __PHYTO_D1.d);
    Fprintf(fp, "|  |                downlink Total : %d.%d               \n", __PHYTO_D2.i, __PHYTO_D2.d);
    Fprintf(fp, "|  |                  uplink Total : %d.%d               \n", __PHYTO_U.i, __PHYTO_U.d);
    Fprintf(fp, "|  | S/N         :  %d.%d ~ %d.%d                        \n", __PHYSN0.i,__PHYSN0.d, __PHYSN1.i,__PHYSN1.d);
    Fprintf(fp, "|  | Data        :  uplink(%ld), downlink(%ld)           \n", __PHY.ul_vol, __PHY.dl_vol);
    Fprintf(fp, "|  +----------------------------------------------------+\n");
  }
    Fprintf(fp, "+-------------------------------------------------------------------+\n");

    return PERF_OK;
}



