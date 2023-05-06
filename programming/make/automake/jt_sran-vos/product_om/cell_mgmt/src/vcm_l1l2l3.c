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
** File name: vcm_l1l2l3.c
** Description:
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin
** Date: 2019.01 - 
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
INT8            Real_phy_states[REAL_PHY_INDEX_NUM]; /*PHY:0-no use;1-use*/
INT8            Simu_phy_states[SIMU_PHY_INDEX_NUM]; /*  */

vcm_set         all_phy_index_set, real_phy_index_set, simu_phy_index_set; /* for phy index recording */

INT             L1_listen_sockfd;
INT             L2L3_listen_sockfd;

extern ipv4_t   IPv4_table[VCM_IF_IP_NUM];



/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_L1_init(CONST UINT32 cell_id)
{
    log_vcm_debug();
    
    CHECK_ONE_CELL_ID(cell_id);

    INT32 ret = VCM_ERR;

    /**
     *  Firstly, should determine this cell is Virtual or Simulate
     */
    cell_type_t cell_type = get_cell_type(cell_id);

    switch(cell_type)
    {
        /**
         *  If it's virtual cell, use virtual phy
         */
        case CELL_VIRTUAL: /* Virtual Cell */
            
            //TODO

            if(!VCM_ISSET(get_phy_index(cell_id), &all_phy_index_set))
                VCM_SET(get_phy_index(cell_id), &all_phy_index_set);
            if(!VCM_ISSET(get_phy_index(cell_id), &real_phy_index_set))
                VCM_SET(get_phy_index(cell_id), &real_phy_index_set);
            break;

        /**
         *  If it's simulate cell, use simulate phy
         */
        case CELL_SIMULATE: /* Simulate Cell */
        {
            
            /**
             *  get cell's folder path 
             *  /home/Cells ==> /home/Cells/1
             */
            CHAR this_cell_path[VCM_STR_LEN];
            Sprintf(this_cell_path, "%s/%d", VCM_CELLS_FOLDER_PATH, cell_id);

            /* check cell's folder Create successfully or not */
            CHECK_F_EXIST(this_cell_path);

            /**
             *  get cell's folder path 
             *  /home/Cells ==> /home/Cells/1 
             */
            char this_cell_simu_phy_path[VCM_STR_LEN];
            Sprintf(this_cell_simu_phy_path, "%s/%s", this_cell_path, SIMU_PHY_FOLDER);

            /* check cell's folder Create successfully or not */
            CHECK_F_EXIST(this_cell_simu_phy_path);

            /* Change Directory: switch the folder into cell's folder */
            System("cd %s; ls", this_cell_simu_phy_path);

            // modify cfg file of simu phy
            if (VCM_ERR == vcm_L1_cfg(cell_id))
            {
                log_vcm_err("modify simu phy cfig file failed\n");
                __vcm_set_errno(_VCM_EPHYCFGERR);
                ret = VCM_ERR;
                break;
            }
            
            /**
             *  start simu_phy
             */
            ret = System("cd %s; sh %s &", this_cell_simu_phy_path, SIMU_PHY_EXECUTION_SCRIPT);

            if(!VCM_ISSET(get_simu_phy_index(cell_id), &all_phy_index_set))
                VCM_SET(get_simu_phy_index(cell_id), &all_phy_index_set);
            if(!VCM_ISSET(get_simu_phy_index(cell_id), &simu_phy_index_set))
                VCM_SET(get_simu_phy_index(cell_id), &simu_phy_index_set);

            
            break;
        }
        default: /* Cell type is error */
            log_vcm_err("No this kind of cell type(%d)\n", cell_type);
            ret = VCM_ERR;
            break;
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
INT32 vcm_L2L3_init(CONST UINT32 cell_id)
{
    log_vcm_debug();

    CHECK_ONE_CELL_ID(cell_id);

    /**
     *  Cell_phy_index is use to make MAC & simu_phy communicate with each other
     *  based on libsml, and simulate cell is init MAC first, but we need 
     *  cell_phy_index when we init MAC, so change set cell phy index location
     *  in set_mac_init().
     */
    if(CELL_SIMULATE == get_cell_type(cell_id))
    {
        log_vcm_info("Get simulate phy_index.\n");
        UINT8 iphy;
        for (iphy = SIMU_PHY_INDEX_START; iphy <= SIMU_PHY_INDEX_END; iphy ++)
        {
            if(1 != VCM_ISSET(iphy, &simu_phy_index_set))
            {
                VCM_SET(iphy, &simu_phy_index_set);
                VCM_SET(iphy, &all_phy_index_set);
                
                set_simu_phy_index(cell_id, iphy);

                Fprintf(STDOUT, "Simulate phy index bits: \n");
                vcm_set_display(STDOUT, all_phy_index_set, 0, PHY_INDEX_NUM, '1');
                Fprintf(STDOUT, "\n");
                vcm_set_display(STDOUT, simu_phy_index_set, 0, PHY_INDEX_NUM, '1');
                Fprintf(STDOUT, "\n");
                
                break;
            }
        }
    }

    /* check all cells's father folder is exist or not */
    CHECK_F_EXIST(VCM_CELLS_FOLDER_PATH);

    /**
     *  get cell's folder path 
     *  /home/Cells ==> /home/Cells/1
     */
    char this_cell_path[VCM_STR_LEN];
    Sprintf(this_cell_path, "%s/%d", VCM_CELLS_FOLDER_PATH, cell_id);

    /* check cell's folder Create successfully or not */
    CHECK_F_EXIST(this_cell_path);

    /* Change Directory: switch the folder into cell's folder */
    System("cd %s; ls", this_cell_path);

    /* check mac's sources folder is exist or not */
    CHECK_F_EXIST(MAC_SRC_DEFAULT_PATH);

    /* Copy PHYConfig.xer from /etc/jt_product/jt_sran/l2_l3/default/cfgfile
     * to /etc/jt_product/jt_sran/l2_l3/cells/1/cfgfile. Cfig Mgmt module
     * modify "PHYConfig.xer" file, then Cell Mgmt module copy this file to 
     * cell folder for each one of cell. information from: Bai Jinlong
     */
    char str_PHYConfig_xer[VCM_STR_LEN];
    Sprintf(str_PHYConfig_xer, "%s/%s/%s", MAC_SRC_DEFAULT_PATH, MAC_CFG_FOLDER, MAC_CFG_PHYConfig);

    CHECK_F_EXIST(str_PHYConfig_xer);
    
    System("cp %s %s/%s/%s", str_PHYConfig_xer, this_cell_path, MAC_CFG_FOLDER, MAC_CFG_PHYConfig);

    /**
     *  configure the mac xml/xer cfgfile 
     */
    if(VCM_OK == vcm_L2L3_cfg(cell_id))
    {
        log_vcm_info("mac configure Success\n");
    }
    else
    {
        log_vcm_err("mac configure Failed\n");
        __vcm_set_errno(_VCM_EMACCFGERR);
        return VCM_ERR;
    }

    /** 
     *  Execute mac 
     *  Use start.sh to execute the mac software
     */
    char mac_txt[VCM_STR_LEN];
    char ipcrm_shm[VCM_STR_LEN];

    Memset(mac_txt, 0, VCM_STR_LEN);
    Memset(ipcrm_shm, 0, VCM_STR_LEN);

    cell_type_t cell_type = get_cell_type(cell_id);
    
    switch(cell_type)
    {
        case CELL_VIRTUAL: /* Virtual Cell */
            Sprintf(mac_txt, "txt%d", get_phy_index(cell_id));
            Sprintf(ipcrm_shm, "%#010x", get_phy_index(cell_id)+1);
            break;
            
        case CELL_SIMULATE: /* Simulate Cell */
            Sprintf(mac_txt, "txt%d", get_simu_phy_index(cell_id));
            Sprintf(ipcrm_shm, "%#010x", get_simu_phy_index(cell_id)+1);
            break;
            
        default: /* Cell type is error */
            log_vcm_err("No this kind of cell type(%d)\n", cell_type);
            return VCM_ERR;
            break;
    }

    System("cd %s; sh %s %s %s %s:%d %s %s %s &",
            this_cell_path, 
            MAC_EXECUTION_SCRIPT,   //start.sh
            mac_txt,                //-P txt0 
            MAC_MEMORY_SIZE,        //
            VCM_IF_NAME, 
            cell_id, 
            L2L3_UNIX_SOCK_PATH,
            cell_type == CELL_VIRTUAL?"virtual":"simulate",
            ipcrm_shm);

    set_mac_status(cell_id, MAC_STATUS_OK);
            
    log_vcm_info("cell[%d]'s Mac is running...\n", cell_id-VCM_MIN_CELL_NUM);
    
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_L1_cfg(CONST UINT32 cell_id)
{
    log_vcm_debug();
    
    CHECK_ONE_CELL_ID(cell_id);


    /**
     *  Firstly, should determine this cell is Virtual or Simulate
     */
    cell_type_t cell_type = get_cell_type(cell_id);
    
    switch(cell_type)
    {
        /**
         *  If it's virtual cell, use virtual phy
         */
        case CELL_VIRTUAL: /* Virtual Cell */
            
            break;

        /**
         *  If it's simulate cell, use simulate phy
         */
        case CELL_SIMULATE: /* Simulate Cell */
        {
            
            CHAR    simu_phy_eth[VCM_IP_LEN]; 
            CHAR    simu_phy_ip[VCM_IP_LEN]; 	
            UINT64  tcp_server_port        = SIMU_PHY_BASE_PORT + get_simu_phy_index(cell_id); 
            CHAR    dsp_port[VCM_STR_LEN]  = SIMU_PHY_DSP_PORT;
            CHAR    gts_port[VCM_STR_LEN]  = SIMU_PHY_GTS_PORT;
            CHAR    mac_eth[VCM_STR_LEN];
            UINT8   phy_cell_id = get_simu_phy_index(cell_id);

            Memset(simu_phy_eth, 0, VCM_IP_LEN);
            Memset(simu_phy_ip, 0, VCM_IP_LEN);
            Memset(mac_eth, 0, VCM_STR_LEN);

            set_simu_phy_tcpServerPort(cell_id, tcp_server_port);

            get_mac_ether(cell_id, simu_phy_eth);

            get_mac_gtpu_recv_addr(cell_id, simu_phy_ip);

            /* Not ens33:1, but ens33 */
            Sprintf(mac_eth, "%s", VCM_IF_NAME);

            /**
             *  get cell's folder path 
             *  /home/Cells ==> /home/Cells/1
             */
            char this_cell_path[VCM_STR_LEN];
            Memset(this_cell_path, 0, VCM_STR_LEN);
            Sprintf(this_cell_path, "%s/%d", VCM_CELLS_FOLDER_PATH, cell_id);

            /* check cell's folder Create successfully or not */
            CHECK_F_EXIST(this_cell_path);

            /**
             *  get cell's folder path 
             *  /home/Cells ==> /home/Cells/1
             */
            char this_cell_simuphy_cfg[VCM_STR_LEN];
            Memset(this_cell_simuphy_cfg, 0, VCM_STR_LEN);
            Sprintf(this_cell_simuphy_cfg, "%s/%s/%s", this_cell_path, SIMU_PHY_FOLDER, SIMU_PHY_CFIG);

            /* check cell's folder Create successfully or not */
            CHECK_F_EXIST(this_cell_simuphy_cfg);

            FILE *cfg_fp = fopen(this_cell_simuphy_cfg, "w");

#define L1_cfg_Fprintf(fmt, val)                                    \
    if (0 > Fprintf(cfg_fp, fmt, val)) {                            \
        log_vcm_err("failed to write simu phy cfg: %s\n", #val);    \
        __vcm_set_errno(_VCM_EFDWRITEERR);                          \
        return VCM_ERR;                                             \
    }

            L1_cfg_Fprintf("dspEthAddr:%s\n",     simu_phy_eth);
            L1_cfg_Fprintf("gtsEthAddr:%s\n",     simu_phy_eth);
            L1_cfg_Fprintf("ueeEthAddr:%s\n",     simu_phy_eth);
            L1_cfg_Fprintf("dspIPAddr:%s\n",      simu_phy_ip);
            L1_cfg_Fprintf("gtsIPAddr:%s\n",      simu_phy_ip);
            L1_cfg_Fprintf("tcpServerPort:%ld\n", tcp_server_port);
            L1_cfg_Fprintf("unsockPath:%s\n",     L2L3_UNIX_SOCK_PATH);
            L1_cfg_Fprintf("dspPort:%s\n",        dsp_port);
            L1_cfg_Fprintf("gtsPort:%s\n",        gts_port);
            L1_cfg_Fprintf("mac_eth:%s\n",        mac_eth);
            L1_cfg_Fprintf("vbsId:%d\n",          cell_id);
            L1_cfg_Fprintf("phyCellIndex:%u\n",   phy_cell_id);

            Fflush(cfg_fp);
            Fclose(cfg_fp);
    
            break;
        }
        default: /* Cell type is error */
            log_vcm_err("No this kind of cell type(%d)\n", cell_type);
            return VCM_ERR;
            break;
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
INT32 vcm_L2L3_cfg(CONST UINT32 cell_id)
{
    log_vcm_debug();
    
    CHECK_ONE_CELL_ID(cell_id);

    /**
     *  get cell's folder path 
     *  /home/Cells ==> /home/Cells/1
     */
    char this_cell_path[VCM_STR_LEN];
    Sprintf(this_cell_path, "%s/%d", VCM_CELLS_FOLDER_PATH, cell_id);

    /* check cell's folder Create successfully or not */
    CHECK_F_EXIST(this_cell_path);

    /* alter mac cfg xer file value */
    char VbsMainConfig[VCM_STR_LEN];
    
    /* src mac cfg xml/xer file */
    Sprintf(VbsMainConfig, "%s/%s/%s", this_cell_path, MAC_CFG_FOLDER, MAC_CFG_VbsMainConfig);

    CHECK_F_EXIST(VbsMainConfig);

    /**
     *  Now alter VbsMainConfig.xer configuration file with libxml2
     */
    xmlDocPtr VbsMainConfig_doc = xml_doc(VbsMainConfig);
    xmlXPathContextPtr VbsMainConfig_context = xml_context(VbsMainConfig_doc);

    /**
     *  declare
     */
    CHAR virt_ipv4[VCM_IP_LEN];
    CHAR vbs_id[VCM_STR_LEN];
    CHAR phy_cell_id[VCM_STR_LEN];
    CHAR vbs_mode[VCM_STR_LEN];
    CHAR cfg_cellid[VCM_STR_LEN];
    CHAR vbs_bandwidth[VCM_STR_LEN];
    CHAR vbs_eth[VCM_STR_LEN];
    CHAR hdw_addr[VCM_STR_LEN];

    /* set memory to zero */
    Memset(virt_ipv4,       0, 16);
    Memset(vbs_id,          0, VCM_STR_LEN);
    Memset(phy_cell_id,     0, VCM_STR_LEN);
    Memset(vbs_mode,        0, VCM_STR_LEN);
    Memset(cfg_cellid,      0, VCM_STR_LEN);
    Memset(vbs_bandwidth,   0, VCM_STR_LEN);
    Memset(vbs_eth,         0, VCM_STR_LEN);
    Memset(hdw_addr,        0, VCM_STR_LEN);

    /* initial value */
    //Sprintf(virt_ipv4, "%s", VCM_IF_IP_BASE);
    create_ipv4((UINT8 *)virt_ipv4);
    
    Sprintf(vbs_id, "%d", cell_id);

    switch(get_cell_mode(cell_id))
    {
        case VC_MODE_MIN:   Sprintf(vbs_mode, "fdd-lte");      break;
        case GSM:           Sprintf(vbs_mode, "gsm");          break;
        case GMR:           Sprintf(vbs_mode, "gmr");          break;
        case WCDMA:         Sprintf(vbs_mode, "wcdma");        break;
        case TD_SCDMA:      Sprintf(vbs_mode, "td-scdma");     break;
        case TDD_LTE:       Sprintf(vbs_mode, "tdd-lte");      break;
        case FDD_LTE:       Sprintf(vbs_mode, "fdd-lte");      break;
        default:            Sprintf(vbs_mode, "fdd-lte");      break;
    }

    Sprintf(vbs_bandwidth, "%d", get_cell_bandwidth(cell_id));

    Sprintf(vbs_eth, "%s:%d", VCM_IF_NAME, cell_id);

    set_mac_vbs_eth(cell_id, vbs_eth);
    set_mac_gtpu_recv_addr(cell_id, (CHAR*)virt_ipv4);


#define CFG_PATH_vbs_id     "/Cfg/vbs-id"
#define CFG_PATH_vbs_eth    "/Cfg/vbs-eth"
#define CFG_PATH_vbs_mode   "/Cfg/vbs-mode"
#define CFG_PATH_vbs_bw     "/Cfg/vbs-bandwidth"
#define CFG_PATH_enb_ip     "/Cfg/enb-ipaddr"
#define CFG_PATH_gtpu_addr  "/Cfg/gtpu-recv-addr"

#define ALTER_VbsMainConfig(valPath, valStr)                                \
    if(VCM_OK == alter_xml_node_value(VbsMainConfig_context,                \
                    (CONST xmlChar *)valPath, (CONST xmlChar *)valStr)) {   \
        log_vcm_info("alter %s: %s/%s\n", VbsMainConfig, valPath, valStr);  \
    } else {                                                                \
        log_vcm_err("alter %s: %s/%s\n", VbsMainConfig, valPath, valStr);   \
    }

    ALTER_VbsMainConfig(CFG_PATH_vbs_id,    vbs_id);
    ALTER_VbsMainConfig(CFG_PATH_vbs_eth,   VCM_IF_NAME); /* why isn't "vbs_eth" */
    ALTER_VbsMainConfig(CFG_PATH_vbs_mode,  vbs_mode);
    ALTER_VbsMainConfig(CFG_PATH_vbs_bw,    vbs_bandwidth);
    ALTER_VbsMainConfig(CFG_PATH_enb_ip,    virt_ipv4);
    ALTER_VbsMainConfig(CFG_PATH_gtpu_addr, virt_ipv4);

    
    if(VCM_OK != net_if_create(vbs_eth, virt_ipv4, VCM_IF_NETMASK))
    {
        log_vcm_err("Mac initial -> create inet interface error.\n");
        return VCM_ERR;
    }
    
    if(VCM_OK != get_if_hardware_addr(vbs_eth, hdw_addr))
    {
        log_vcm_err("Mac initial -> get hardware address(MAC address) error.\n");
        return VCM_ERR;
    }

    set_mac_ether(cell_id, hdw_addr);

    /**
     *  Maybe here alter VbsMainConfig more
     */
    //TODO

    save_xml(VbsMainConfig, VbsMainConfig_doc, VbsMainConfig_context);
    
    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_L1_kill(CONST UINT32 cell_id)
{
    log_vcm_debug();
    
    CHECK_ONE_CELL_ID(cell_id);

    INT32 ret = VCM_OK;

    /**
     *  Firstly, should determine this cell is Virtual or Simulate
     */
    cell_type_t cell_type = get_cell_type(cell_id);
    
    switch(cell_type)
    {
        case CELL_VIRTUAL: /* Virtual Cell */
            
            VCM_CLR(get_phy_index(cell_id), &all_phy_index_set);
            VCM_CLR(get_phy_index(cell_id), &real_phy_index_set);
            //TODO
            break;
            
        case CELL_SIMULATE: /* Simulate Cell */
            log_vcm_warn("Ready to kill %d\n", get_simu_phy_pid(cell_id));
            
            VCM_CLR(get_simu_phy_index(cell_id), &all_phy_index_set);
            VCM_CLR(get_simu_phy_index(cell_id), &simu_phy_index_set);
        
            if(VCM_OK == is_pid_alive(get_simu_phy_pid(cell_id)))
            {
                log_vcm_debug();
                /**
                 *  I find kill() interface can't terminate process
                 *      Rong Tao: 2019.02
                 */
                //if(0 != Kill(get_simu_phy_pid(cell_id), SIGINT))
                    System("kill %d\n", get_simu_phy_pid(cell_id));
            }

            /**
             *  get cell's simulate phy folder path 
             *  /home/Cells/1/simulatePhy
             */
            char this_cell_simu_phy_path[VCM_STR_LEN];
            Memset(this_cell_simu_phy_path, 0, VCM_STR_LEN);
            Sprintf(this_cell_simu_phy_path, "%s/%d/%s", VCM_CELLS_FOLDER_PATH, cell_id, SIMU_PHY_FOLDER);

            /* check cell's simulate phy folder Create successfully or not */
            if(0 == ACCESS_IS_EXIST(this_cell_simu_phy_path))
            {
                if(VCM_OK != remove_dir(this_cell_simu_phy_path))
                {
                    log_vcm_err("SimulatePHY delete -> delete folder error. %s\n", this_cell_simu_phy_path);
                }
            }

            /**
             *  Memset cell's simulate PHY to 0
             */
            VOID *p = MEM_ADDR_OF_CELL(cell_id)+offsetof(cell_node_t, phy.simu);
            //log_vcm_err("!!!!!!!!!!%p : %p\n", &Cells[cell_id-VCM_MIN_CELL_NUM].phy.simu, p);

            if(p == NULL) ret = VCM_ERR;
            
            Memset(p, 0, sizeof(simu_phy_info_t));

            set_simu_phy_status(cell_id, SIMU_PHY_STATUS_NULL);
            
            break;
            
        default: /* Cell type is error */
            log_vcm_err("No this kind of cell type(%d)\n", cell_type);
            return VCM_ERR;
            break;
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
INT32 vcm_L2L3_kill(CONST UINT32 cell_id)
{
    log_vcm_debug();

    INT32 ret = VCM_OK;

    /* valid cell_id */
    CHECK_ONE_CELL_ID(cell_id);

    /* if mac pid alive, kill it */
    log_vcm_warn("Ready to kill %d\n", get_mac_pid(cell_id));
    
    if(VCM_OK == is_pid_alive(get_mac_pid(cell_id)))
    {
        log_vcm_debug();
        /**
         *  I find kill() interface can't terminate process
         *      Rong Tao: 2019.02
         */
        //if(0 != Kill(get_mac_pid(cell_id), SIGINT))
            System("kill %d\n", get_mac_pid(cell_id));
    }

    /**
     *  get cell's folder path 
     *  /home/Cells ==> /home/Cells/1
     */
    char this_cell_path[VCM_STR_LEN];
    Memset(this_cell_path, 0, VCM_STR_LEN);
    Sprintf(this_cell_path, "%s/%d", VCM_CELLS_FOLDER_PATH, cell_id);

    /* check cell's folder Create successfully or not */
    if(0 == ACCESS_IS_EXIST(this_cell_path))
    {
        if(VCM_OK != remove_dir(this_cell_path))
        {
            log_vcm_err("Mac delete -> delete cell folder error. %s\n", this_cell_path);
        }
    }

    /**
     *  Delete inet interface
     */
    CHAR vbs_eth[VCM_STR_LEN];
    Memset(vbs_eth, 0, VCM_STR_LEN);

    Sprintf(vbs_eth, "%s:%d", VCM_IF_NAME, cell_id);
    
    if(VCM_OK != net_if_destroy(vbs_eth))
    {
        log_vcm_err("Mac delete -> inet interface delete error. %s\n", vbs_eth);
        ret = VCM_ERR;
    }

    /**
     *  Memset cell's mac to 0
     */
    VOID *p = MEM_ADDR_OF_CELL(cell_id)+offsetof(cell_node_t, mac);
    //log_vcm_err("%p : %p\n", &Cells[cell_id-VCM_MIN_CELL_NUM].mac, p);

    if(p == NULL) ret = VCM_ERR;
    
    Memset(p, 0, sizeof(mac_info_t));

    set_mac_status(cell_id, MAC_STATUS_NULL);
    
    return ret==VCM_OK?VCM_OK:ret;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_L1_clean_up()
{
    log_vcm_debug();

    UINT32 cell_id;
    
    for(cell_id=VCM_MIN_CELL_NUM; cell_id<=VCM_MAX_CELL_NUM; cell_id++)
    {
        /**
         *  Firstly, should determine this cell is Virtual or Simulate
         */
        cell_type_t cell_type = get_cell_type(cell_id);
        
        /**
         *  Send SIGINT to every l1app(phy), TODO
         *  Maybe use kill(pid_t pid, int signum) interface.
         */
        switch(cell_type)
        {
            case CELL_VIRTUAL: /* Virtual Cell */
                //TODO
                break;
                
            case CELL_SIMULATE: /* Simulate Cell */
                if(get_simu_phy_status(cell_id) == SIMU_PHY_STATUS_OK || 
                   get_simu_phy_status(cell_id) == SIMU_PHY_STATUS_ERR)
                {
                    vcm_L1_kill(cell_id);
                }
                break;
                
            default: /* Cell type is error */
                log_vcm_err("No this kind of cell type(%d)\n", cell_type);
                return VCM_ERR;
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
INT32 vcm_L2L3_clean_up()
{
    log_vcm_debug();

    UINT32 cell_id;
    for(cell_id=VCM_MIN_CELL_NUM; cell_id<=VCM_MAX_CELL_NUM; cell_id++)
    {
        if(get_mac_status(cell_id) == MAC_STATUS_OK || get_mac_status(cell_id) == MAC_STATUS_ERR)
        {
            vcm_L2L3_kill(cell_id);
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
VOID  vcm_L1_socket_callback(INT32 sock_fd, struct sockaddr_storage *clientaddr, UINT8 *msg_p, UINT32 msg_len)
{
    log_vcm_debug();

    log_vcm_info("sockfd: %d\n", sock_fd);
    Fprintf(STDOUT, "<<<<<<<<<<<<<<<<<<<<<<<<\n");
    Memdisplay(STDOUT, msg_p, msg_len, 10);
    Fprintf(STDOUT, ">>>>>>>>>>>>>>>>>>>>>>>>\n");

    if(msg_p == NULL || msg_len <= 0)
    {
        log_vcm_err("msg is NULL pointer. msg_len = %d\n", msg_len);
        return;
    }
    
    if(clientaddr == NULL)
        log_vcm_warn("clientaddr is NULL pointer\n");

    UINT16  msg_type;
    UINT16  msg_code _unused; 
    UINT32  msg_len_decode _unused; 
    UINT32  cell_id _unused; 

    vcm_outer_msg_l1_hdr *msghdr = (vcm_outer_msg_l1_hdr*)msg_p;

    msg_type = msghdr->hdr.type;
    msg_code = msghdr->hdr.code;
    msg_len_decode  = msghdr->hdr.len;
    cell_id  = msghdr->cell_id;

    switch(msg_type)
    {
        case L1_MSG_TYPE_PHY_CELL_MGMT:
            log_vcm_info("Got PHY cell setup/del rsp.\n");
            set_phy_sockfd(cell_id, sock_fd);
            break;
            
        case L1_MSG_TYPE_PHY_ALAR_MGMT:
            log_vcm_warn("Got PHY alarm message.\n");
            //TODO
            break;
        
        case L1_MSG_TYPE_PHY_MAIN_CTRL:
            log_vcm_info("Got PHY load rsp.\n");
            //TODO
            break;
        
        case L1_MSG_TYPE_PHY_PRFM_MGMT:
            log_vcm_info("Got PHY performance statistics message.\n");
            //TODO
            break;
        
        default:
            log_vcm_err("Got unknown msg from PHY.\n");
            break;
    }

    /**
     *  Send to Other MODULEs, use sync/ansync interface
     */
    //TODO

    return;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
VOID  vcm_L2L3_socket_callback(INT32 sock_fd, struct sockaddr_storage *clientaddr, UINT8 *msg_p, UINT32 msg_len)
{
    log_vcm_debug();

    log_vcm_info("sockfd: %d\n", sock_fd);
    Fprintf(STDOUT, "<<<<<<<<<<<<<<<<<<<<<<<<\n");
    Memdisplay(STDOUT, msg_p, msg_len, 10);
    Fprintf(STDOUT, ">>>>>>>>>>>>>>>>>>>>>>>>\n");

    if(msg_p == NULL || msg_len <= 0)
    {
        log_vcm_err("msg is NULL pointer. msg_len = %d\n", msg_len);
        return;
    }
    
    if(clientaddr == NULL)
        log_vcm_warn("clientaddr is NULL pointer\n");

    UINT16  msg_type;
    UINT16  msg_code;
    UINT32  msg_len_decode;
    UINT32  cell_id;
    INT32   mac_pid, simu_phy_pid;
   

    vcm_outer_msg_l2l3_hdr *msghdr = (vcm_outer_msg_l2l3_hdr*)msg_p;
    
    msg_type = msghdr->hdr.type;
    msg_code = msghdr->hdr.code;
    msg_len_decode  = msghdr->hdr.len;
    cell_id  = msghdr->cell_id;

    if(msg_len_decode != msg_len)
        log_vcm_warn("Mac's msg length not correct. recvlen(%d), decode(%d)\n", msg_len, msg_len_decode);

    log_vcm_info("Recv mac/simuphy msg: [type(%x), code(%x), len(%d), cell_id(%d)]\n", 
                    msg_type, msg_code, msg_len_decode, cell_id);


    switch(msg_type)
    {
        /**
         *  MAC message with CELL_MGMT
         */
        case L2L3_MSG_TYPE_MAC_CELL_MGMT:
            
            switch(msg_code)
            {
                /**
                 *  MAC initial response message
                 */
                case L2L3_MSG_CODE_MAC_VCM_MAC_INIT_RSP:
                {
                    log_vcm_info("Recv MAC initial response message.\n");
                    INT32   ret;
                
                    if(VCM_OK != is_cell_id_available(cell_id))
                    {
                        log_vcm_err("Get mac's invalid cell_id(%d)\n", cell_id);
                        break;
                    }
                        
                    mac_pid = ((vcm_outer_msg_l2l3_init_rsp*)msg_p)->pid;
                    ret     = ((vcm_outer_msg_l2l3_init_rsp*)msg_p)->rslt;

                    if(ret != 0) {
                        log_vcm_err("Mac's initial response result is error(%d)\n", ret);
                        set_mac_status(cell_id, MAC_STATUS_ERR);
                    }
                    else 
                        set_mac_status(cell_id, MAC_STATUS_OK);
                    
                    set_mac_sockfd(cell_id, sock_fd);
                    set_mac_pid(cell_id, mac_pid);
                    
                    break;
                }
                /**
                 *  MAC cell start response message
                 */
                case L2L3_MSG_CODE_MAC_VCM_CELL_START_RSP:
                    //TODO
                    break;
                
                /**
                 *  MAC cell delete response message
                 */
                case L2L3_MSG_CODE_MAC_VCM_CELL_DELETE_RSP:
                    //TODO
                    break;
                    
                default:
                    log_vcm_err("No this kind of or Not support this message code(%x)\n", msg_code);
                    break;
            }
            break;

        /**
         *  MAC message with RRUS_MGMT
         */
        case L2L3_MSG_TYPE_MAC_RRUS_MGMT:
            //TODO
            break;

        /**
         *  MAC message with CFIG_MGMT
         */
        case L2L3_MSG_TYPE_MAC_CFIG_MGMT:
            //TODO
            break;

        /**
         *  MAC message with ALAR_MGMT
         */
        case L2L3_MSG_TYPE_MAC_ALAR_MGMT:
            //TODO
            break;

        /**
         *  MAC message with PRFM_MGMT
         */
        case L2L3_MSG_TYPE_MAC_PRFM_MGMT:
            //TODO
            break;

        /**
         *  simulate PHY message with CELL_MGMT
         */
        case L1_MSG_TYPE_SIMU_PHY_CELL_MGMT:
            if(msg_code == L1_MSG_CODE_SIMU_PHY_INIT_RSP)
            {
                log_vcm_info("Recv simulate PHY initial response message.\n");
                
                INT32   ret;
                
                if(VCM_OK != is_cell_id_available(cell_id))
                {
                    log_vcm_err("Get mac's invalid cell_id(%d)\n", cell_id);
                    break;
                }
                    
                simu_phy_pid = ((vcm_outer_msg_simu_l1_init_rsp*)msg_p)->pid;
                ret          = ((vcm_outer_msg_simu_l1_init_rsp*)msg_p)->rslt;

                if(ret != 0) {
                    log_vcm_err("simulate PHY's initial response result is error(%d)\n", ret);
                    set_simu_phy_status(cell_id, SIMU_PHY_STATUS_ERR);
                }
                else 
                    set_simu_phy_status(cell_id, SIMU_PHY_STATUS_OK);

                log_vcm_info("recv simupate PHY msg: sockfd = %d, pid = %d\n", sock_fd, simu_phy_pid);
                
                set_simu_phy_sockfd(cell_id, sock_fd);
                set_simu_phy_pid(cell_id, simu_phy_pid);
            }
            else
            {
                log_vcm_err("No this kind of or Not support this message code(%x)\n", msg_code);
            }
            break;
            
        /**
         *  simulate PHY message with ALAR_MGMT
         */
        case L1_MSG_TYPE_SIMU_PHY_ALAR_MGMT:
            //TODO
            //break;
            
        /**
         *  simulate PHY message with PRFM_MGMT
         */
        case L1_MSG_TYPE_SIMU_PHY_PRFM_MGMT:
            //TODO
            //break;

        /**
         *  MAC or simulate PHY message error
         */
        default:
            log_vcm_err("No this kind of or Not support this message type(%x)\n", msg_type);
            break;
    }

    return;
}

