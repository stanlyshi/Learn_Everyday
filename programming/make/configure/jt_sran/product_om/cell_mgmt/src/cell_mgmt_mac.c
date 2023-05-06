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
** File name: cell_mgmt_mac.c
** Description: some of interface for MAC execution
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sbs_type.h"
#include "sbs_socket.h"

#include "util_dir.h"
#include "util_printer.h"
#include "util_exec.h"
#include "util_xml.h"
#include "util_systemer.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: copy mac files and execute MAC executeable file 
 *
 * Input: cell_virt_id
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_mac_init(INT32 cell_virt_id)
{
    debug();
    
    /*cell virtual id valiable*/
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }

    char path_cell[MAX_STR_LEN];
    sprintf(path_cell, "%s/%d", PATH_CELLS, cell_virt_id);
    
    /*cells folder exist or not*/
    if(-1 == is_exist(PATH_CELLS) || -1 == is_exist(path_cell))
    {
        /** One of cell's folder not exist, in this folder include 1 2 3 4...
         *  cell's folders, and in these folders have mac execution and mac 
         *  configure xml/xer files's folder.
         *  So, if this folder not exist, return FUNC_ERR(-1).
         */
        error("%s:%d %s or %s doesn't Exist.\n", 
            __func__, __LINE__, PATH_CELLS, path_cell);

        return FUNC_ERR;
    }

    sh_cmd("cd %s/%d; ls", PATH_CELLS, cell_virt_id);

    /* Copy PHYConfig.xer from /etc/jt_product/jt_sran/l2_l3/default/cfgfile
     * to /etc/jt_product/jt_sran/l2_l3/cells/1/cfgfile. Cfig Mgmt module
     * modify "PHYConfig.xer" file, then Cell Mgmt module copy this file to 
     * cell folder for each one of cell. information from: Bai Jinlong
     */
    char str_PHYConfig_xer[MAX_STR_LEN];
    sprintf(str_PHYConfig_xer, 
            "%s/%s/%s", MAC_SRC_PATH_DEFAULT, "cfgfile", "PHYConfig.xer");
    if(-1 == is_exist(str_PHYConfig_xer))
    {
        error("%s:%d %s or %s doesn't Exist.\n", 
            __func__, __LINE__, str_PHYConfig_xer);
        return FUNC_ERR;
    }
    
    sh_cmd("cp %s %s/%d/%s", 
        str_PHYConfig_xer, PATH_CELLS, cell_virt_id, "cfgfile/PHYConfig.xer");

    
    /* configure the mac xml/xer cfgfile */
    if(FUNC_OK == cfg_Macconfig_value(cell_virt_id))
    {
        success("%s:%d alter mac cfgfile Success\n", __func__, __LINE__);
    }
    else
    {
        error("%s:%d alter mac cfgfile Error\n", __func__, __LINE__);
    }
    
    
    /* create virtual network card */
    char virt_ipv4[16];
    create_virtual_ipv4(cell_virt_id, virt_ipv4);
    
    if(FUNC_ERR == create_virtual_network_card(cell_virt_id, virt_ipv4, IFCONFIG_INTERFACE_NAME))
    {
        error("%s:%d <%s> not exist.\n", __func__, __LINE__, IFCONFIG_INTERFACE_NAME);
        return FUNC_ERR;
    }
    
    /* execute mac */
    sh_cmd("cd %s/%d; sh start.sh %s %s:%d %s &",
            PATH_CELLS, cell_virt_id, 
            MAC_MEMORY_SIZE, IFCONFIG_INTERFACE_NAME, cell_virt_id, MAC_SUN_PATH);
    
    success("%s:%d Mac running...\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: kill MAC process with pid_t 
 *
 * Input: cell_virt_id
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_mac_kill(INT32 cell_virt_id)
{
    debug();
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }

    if(FUNC_OK == delete_cell_folder(cell_virt_id))
    {
        success("%s:%d delete %s Success\n", __func__, __LINE__, 
                vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth);
    }
    else
    {
        warning("%s:%d delete %s Failed\n", __func__, __LINE__, 
                vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth);
    }
    
    delete_virtual_network_card(cell_virt_id, IFCONFIG_INTERFACE_NAME);
    
    pid_t mac_pid = vc_entity_table[cell_virt_id-1].cell_mac.mac_pid;

    info("%s:%d mac_pid = %d to kill.\n", __func__, __LINE__, mac_pid);
    
    FILE *fp = popen("ps -e | grep %d | awk '{print $1}'", "r");
    char str_pid[CMD_SIZE];
    
    if(NULL != fgets(str_pid, 20, fp))
    {
        if(mac_pid == atoi(str_pid))
        {
            sh_cmd("kill -9 %d", mac_pid);
            success("%s:%d kill mac successful: pid_t: %d\n", 
                    __func__, __LINE__, mac_pid);
            
            return FUNC_OK;
        }
    }
    else
    {
        warning("%s:%d mac pid not exit. %d\n", __func__, __LINE__, mac_pid);
        return FUNC_ERR;
    }
}

/******************************************************************************
 * Description: alter MAC configure file(don't generate new xml file)
 *
 * Input: cell_virt_id
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cfg_Macconfig_value(const UINT32 cell_virt_id)
{
    debug();
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d [MAC]cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    /* go in cell folder */
    char path_cell[MAX_STR_LEN];
    sprintf(path_cell, "%s/%d", PATH_CELLS, cell_virt_id);
    
    if(0 == is_exist(path_cell))
    {
        info("%s:%d cell folder: %s\n", __func__, __LINE__, path_cell);
    }
    else
    {
        error("%s:%d Cell folder not exist: %s\n", __func__, __LINE__, path_cell);
        return FUNC_ERR;
    }
    
    /* alter mac cfg xer file value */
    char mac_cfg_file[MAX_STR_LEN];
    
    /* src mac cfg xml/xer file */
    sprintf(mac_cfg_file, "%s/%s", path_cell, "cfgfile/cfg.xer");
                          
    if(-1 == is_exist(mac_cfg_file))
    {
        error("%s:%d %s not exist\n", __func__, __LINE__, mac_cfg_file);
        return FUNC_ERR;
    }
    
    xmlDocPtr doc = cfg_parse(mac_cfg_file);
    xmlXPathContextPtr context = cfg_context(doc);
    
/*******************************************************************************
**  Start alter mac cfg xml file
*******************************************************************************/
    char xml_value[MAX_STR_LEN], xml_path[MAX_STR_LEN];
    
    /*alter cell virtual id in mac cfg xml file */
    memset(xml_path, 0, MAX_STR_LEN);
    memset(xml_value, 0, MAX_STR_LEN);
    
    /* vbs-id - cell_virt_id */
    sprintf(xml_path, "/Cfg/vbs-id");
    sprintf(xml_value, "%d", vc_entity_table[cell_virt_id-1].cell_virt_id==0?
                         cell_virt_id:
                         vc_entity_table[cell_virt_id-1].cell_virt_id);
    if(0 == alter_cfg_node_value(context, (xmlChar *)xml_path, (xmlChar *)xml_value))
    {
        success("%s:%d alter %s: %s/%s\n", 
                __func__, __LINE__, mac_cfg_file, xml_path, xml_value);
    }
    else
    {
        error("%s:%d alter %s: %s/%s\n", 
                __func__, __LINE__, mac_cfg_file, xml_path, xml_value);
        return FUNC_ERR;
    }
    
    /*alter cell mode in mac cfg xml file */
    memset(xml_path, 0, MAX_STR_LEN);
    memset(xml_value, 0, MAX_STR_LEN);
    
    sprintf(xml_path, "/Cfg/vbs-mode");
    
    switch(vc_entity_table[cell_virt_id-1].cell_mode)
    {
        case VC_MODE_MIN:   sprintf(xml_value, "NULL");     break;
        case GSM:           sprintf(xml_value, "GSM");      break;
        case GMR:           sprintf(xml_value, "GMR");      break;
        case WCDMA:         sprintf(xml_value, "WCDMA");    break;
        case TD_SCDMA:      sprintf(xml_value, "TD_SCDMA"); break;
        case TDD_LTE:       sprintf(xml_value, "TDD_LTE");  break;
        case FDD_LTE:       sprintf(xml_value, "FDD_LTE");  break;
        default:            sprintf(xml_value, "NULL");     break;
    }
    
    if(0 == alter_cfg_node_value(context, (xmlChar *)xml_path, (xmlChar *)xml_value))
    {
        success("%s:%d alter %s: %s/%s\n", 
                __func__, __LINE__, mac_cfg_file, xml_path, xml_value);
    }
    else
    {
        error("%s:%d alter %s: %s/%s\n", 
                __func__, __LINE__, mac_cfg_file, xml_path, xml_value);
        return FUNC_ERR;
    }
    
/*******************************************************************************
** End  alter mac cfg xml file
*******************************************************************************/
    
    /*save mac cfg xml file */
    if(0 == save_cfg(mac_cfg_file, doc, context))
    {
        success("%s:%d [MAC]success save mac cfg %s\n", \
                __func__, __LINE__, mac_cfg_file);
        return FUNC_OK;
    }
    else
    {
        error("%s:%d [MAC]error save mac cfg %s\n", \
                __func__, __LINE__, mac_cfg_file);
        return FUNC_ERR;
    }
}

/******************************************************************************
 * Description: delete cell's folder(for example: 1 2 3 4)
 *
 * Input: cell_virt_id
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 delete_cell_folder(const UINT32 cell_virt_id)
{
    debug();
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    char path_cell[MAX_STR_LEN];
    memset(path_cell, 0, MAX_STR_LEN);
    
    sprintf(path_cell, "%s/%d", PATH_CELLS, cell_virt_id);
    
    if(0 == is_exist(path_cell))
    {
        sh_cmd("cd %s; ls", path_cell);
        delete_file(path_cell);
        success("%s:%d delete cell folder %s\n", __func__, __LINE__, path_cell);
        
        return FUNC_OK;
    }
    else
    {
        warning("%s:%d cell folder not exist to delete %s\n", \
                __func__, __LINE__, path_cell);
        
        return FUNC_ERR;
    }
}

/******************************************************************************
 * Description: MAC unsocket callback function
 *
 * Input: sock_fd: accept() return's sockfd
 *
 * Output: void
 ******************************************************************************/
void callback_mac_inet(INT32 sock_fd, struct sockaddr *clientaddr, 
                        char *msg_p, UINT32 msg_len)
{
    debug();

    if(msg_p == NULL || msg_len <= 0)
    {
        error("%s:%d msg is NULL pointer. msg_len = %d\n", 
                __func__, __LINE__, msg_len);
        return;
    }
    
    if(clientaddr == NULL)
    {
        warning("%s:%d clientaddr is NULL pointer\n", __func__, __LINE__);
    }
    
    UINT32 MODULE_ID;
    
    UINT32 cell_virt_id = ((MsgMacInitRsp *)msg_p)->cell_virt_id;
    vc_entity_table[cell_virt_id - 1].cell_mac.unsockfd = sock_fd;
    
    INT32   mac_pid = ((MsgMacInitRsp *)msg_p)->pid;
    vc_entity_table[cell_virt_id - 1].cell_mac.mac_pid = mac_pid;

    UINT32  msg_type = ((MsgMacInitRsp *)msg_p)->msghead.msg_type;
    
    switch(msg_type)
    {
        case MSG_TYPE_MAC_CELL_MGMT:
            MODULE_ID = MODULE_CELL_MGMT;
            break;
        case MSG_TYPE_MAC_RRUS_MGMT:
            MODULE_ID = MODULE_RRUS_MGMT;
            break;
        case MSG_TYPE_MAC_CFIG_MGMT:
            MODULE_ID = MODULE_CFIG_MGMT;
            break;
        /* case MSG_TYPE_MAC_ALAR_MGMT:
            MODULE_ID = MODULE_ALAR_MGMT;
            break; */
        default:
            break;
    }
    
    if (send_msg_to_om(MODULE_ID, (char*)msg_p, msg_len) == FUNC_ERR)
    {
        error("%s:%d send msg to MSGQ error!\n", __func__, __LINE__);
    }
    else
    {
        success("%s:%d send msg to MSGQ success!\n", __func__, __LINE__);
    }
    
/** if you want to send msg to MAC by interface 'send_msg_to_inet',
    please follow this interface value format.

    MsgMacInitRsp msg = {
        .msghead.msg_type   = MSG_TYPE_PHY_CELL_MGMT,
        .msghead.msg_code   = 1,
        .msghead.msg_len    = sizeof(MsgMacInitRsp),
        .cell_virt_id       = 1,
        .pid                = getpid(),
        .rslt               = 0,
    };
    send_msg_to_inet(MSG_TYPE_PHY_CELL_MGMT, 
                     SOCK_STREAM_VALID, 
                     sock_fd, 
                     IP_INVALID, 
                     SOCKADDR_NULL, 
                     &msg, 
                     msg_len);
*/
    return;
}

/******************************************************************************
 * Description: create a virtual ipv4 
 *
 * Input: cell_virt_id
 *
 * Output:  ipv4
 *          return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 create_virtual_ipv4(const UINT32 cell_virt_id, UINT8 *ipv4)
{
    struct in_addr addr1;
    int l1;
    l1 = inet_addr(IP_BASE) + htonl(cell_virt_id);
    memcpy(&addr1, &l1, 4);
    strcpy(ipv4 , inet_ntoa(addr1));

    return FUNC_OK;
}

/******************************************************************************
 * Description: create a virtual network card
 *
 * Input:   cell_virt_id
 *          ipv4    :10.21.2.242(for example)
 *          ethname :eth0/ virbr0(for example)
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 create_virtual_network_card(const UINT32 cell_virt_id, const UINT8 *ipv4,
                                    const UINT8 *ethname)
{
    if(ethname == NULL || ipv4 == NULL)
    {
        error("%s:%d NULL pointer Error.\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    char cmd[MAX_STR_LEN], str[MAX_STR_LEN];

    sh_cmd("ifconfig %s | grep %s | awk '{print $1}' ", ethname, ethname);
    sprintf(cmd, "ifconfig %s | grep %s | awk '{print $1}' ", ethname, ethname);
    
    FILE *fp = popen(cmd, "r");

    fgets(str, strlen(ethname)+1, fp);
    
    if(strncmp(str, ethname, sizeof(ethname)) == 0)
    {
        success("%s:%d <%s> is exist.\n", __func__, __LINE__, ethname);
        sh_cmd("ifconfig %s:%d %s", ethname, cell_virt_id, ipv4);
    }
    else
    {
        error("%s:%d <%s> is not exist.\n", __func__, __LINE__, ethname);
        pclose(fp);
        return FUNC_ERR;
    }
    pclose(fp);
    return FUNC_OK;
}

/******************************************************************************
 * Description: delete virtual network card
 *
 * Input:   cell_virt_id
 *          ethname :eth0/ virbr0(for example)
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 delete_virtual_network_card(const UINT32 cell_virt_id, const UINT8 *ethname)
{
    if(ethname == NULL)
    {
        error("%s:%d NULL pointer Error.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    char cmd[MAX_STR_LEN], str[MAX_STR_LEN], ethtmp[MAX_STR_LEN];

    sprintf(ethtmp, "%s:%d", ethname, cell_virt_id);
    sh_cmd("ifconfig %s | grep %s | awk '{print $1}' ", ethtmp, ethtmp);
    sprintf(cmd, "ifconfig %s | grep %s | awk '{print $1}' ", ethtmp, ethtmp);
    
    
    FILE *fp = popen(cmd, "r");
    
    fgets(str, strlen(ethtmp)+1, fp);
    
    if(strncmp(str, ethtmp, sizeof(ethtmp)) == 0)
    {
        success("%s:%d <%s> is exist.\n", __func__, __LINE__, ethtmp);
        sh_cmd("ifconfig %s down", ethtmp);
    }
    else
    {
        warning("%s:%d <%s> is not exist.\n", __func__, __LINE__, ethtmp);
        pclose(fp);
        return FUNC_ERR;
    }
    pclose(fp);
    return FUNC_OK;
}