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
** File name: vcm_init.h
** Description: some of CELL_MGMT module's initialized function
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 2019.02
**
***********************************************************************************************************************/

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
vos_module_t    vcm_module;     /* cell mgmt module */
ULONG           vcm_msgqid;     /* cell mgmt msgq id */

TaskCreateRet   vcm_task1;      /* main ctrl task thread handle */
TaskCreateRet   vcm_task2;      /* performance statistics thread */
TaskCreateRet   vcm_task3;      /* module register thread */

extern vcm_set  all_cell_set, virt_cell_set, simu_cell_set, normal_cell_set;

extern Sem_t    sem_lock_cells;     /* lock of cells, For set and get api */

extern INT      L1_listen_sockfd, L2L3_listen_sockfd;

extern ipv4_t   IPv4_table[VCM_IF_IP_NUM];
extern ipv4_t   ipv4_used[VCM_IF_IP_NUM];

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
INT32 vcm_module_init()
{
    __called_once();

    INT ret = VCM_ERR;

    /**
     *  Create cell_mgmt message receive message queue.
     */
    vcm_msgqid = MsgqCreate();
    if(vcm_msgqid == 0)
    {
        log_vcm_err("%s create message queue error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EIPCMSGQERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s create message queue success.\n", CELL_MGMT_NAME);


    /**
     *  PHY UNIX domain socket register/initial
     */
    ret = unsocket_register(vcm_msgqid, (pCallBackFun)vcm_L1_socket_callback, 1, L1_UNIX_SOCK_PATH, 1);
    if(ret == VCM_ERR)
    {
        log_vcm_err("%s PHY unix domain socket register error(ret = %d).\n", CELL_MGMT_NAME, ret);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s PHY unix domain socket register success.\n", CELL_MGMT_NAME);
    L1_listen_sockfd = ret;

    /**
     *  MAC UNIX domain socket register/initial
     */
    ret = unsocket_register(vcm_msgqid, (pCallBackFun)vcm_L2L3_socket_callback, 1, L2L3_UNIX_SOCK_PATH, 1);
    if(ret == VCM_ERR)
    {
        log_vcm_err("%s MAC unix domain socket register error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_ECSOCKETERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s MAC unix domain socket register success.\n", CELL_MGMT_NAME);
    L2L3_listen_sockfd = ret;
    
    /**
     *  This is one of input of "VOS_module_register"
     */
    vcm_module.queueID = vcm_msgqid;

    /**
     *  Register cell_mgmt module
     */
    ret = VOS_module_register(CELL_MGMT_NAME, &vcm_module);
    if(ret != VCM_OK)
    {
        log_vcm_err("%s module register error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s module register success.\n", CELL_MGMT_NAME);

    /**
     *  Initial cell_mgmt state machine
     */
    ret = vcm_sm_init();
    if(ret != VCM_OK)
    {
        log_vcm_err("%s state machine initial error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s state machine initial success.\n", CELL_MGMT_NAME);

    /**
     *  Initial cell_mgmt cell entity
     */
    ret = vcm_cell_entity_init();
    if(ret != VCM_OK)
    {
        log_vcm_err("%s cell entity initial error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s cell entity initial success.\n", CELL_MGMT_NAME);

    /**
     *  Create semaphore & initialized
     */
    sem_lock_cells = Sem_init();
    if(sem_lock_cells == 0)
    {
        log_vcm_warn("%s create cells semaphore lock error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EIPCSEMCERR);
    }
    log_vcm_succ("%s create cells semaphore lock success.\n", CELL_MGMT_NAME);
    
    
    /**
     *  Check parameters is properate
     */
    ret = vcm_check_paras_valid();
    if(ret != VCM_OK)
    {
        log_vcm_err("%s check parameters error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s check parameters success.\n", CELL_MGMT_NAME);

    /**
     *  create Main ctrl task, 
     */
    ret = vcm_task_init();
    if(ret != VCM_OK)
    {
        log_vcm_err("%s main ctrl task create error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s main ctrl task create success.\n", CELL_MGMT_NAME);


    /**
     *  Module register initial
     */
    ret = vcm_module_register_init();
    if(ret != VCM_OK)
    {
        log_vcm_err("%s module register task create error.\n", CELL_MGMT_NAME);
        __vcm_set_errno(_VCM_EINITERR);
        return VCM_ERR;
    }
    log_vcm_succ("%s module register task create success.\n", CELL_MGMT_NAME);


    
    return VCM_OK;
}
/**
 *  We don't except others module know "vcm_module_init" interface but "cell_mgmt_module_init",
 *  So, I just alias a new name for API.    Rong Tao: 2019.02
 */
weak_alias(vcm_module_init, cell_mgmt_module_init);



/***********************************************************************************************************************
 * Description: cell management module initialized function
 *
 * Input: void
 *
 * Output: void
 *
 * Return: Success: VCM_OK, Failed: VCM_ERR
 **********************************************************************************************************************/
INT32 vcm_clean_up()
{
    __called_once();
    
    log_vcm_debug();

    INT32 ret = VCM_ERR;

    /**
     *  unix socket unregister
     */
    if(0 != socket_unregister(L1_listen_sockfd))
    {
        log_vcm_err("unix socket unregister error.\n");
    }
    if(0 != socket_unregister(L2L3_listen_sockfd))
    {
        log_vcm_err("unix socket unregister error.\n");
    }

    /** 
     *  clean L1(PHY):
     *  Maybe is simulate or virtual
     */
    if(VCM_OK != (ret = vcm_L1_clean_up()))
    {
        log_vcm_err("Clean L1(Phy layer) error.\n");
    }

    /** 
     *  clean L2L3(MAC):
     *  Maybe is simulate or virtual
     */
    if(VCM_OK != (ret = vcm_L2L3_clean_up()))
    {
        log_vcm_err("Clean L2L3(Mac layer) error.\n");
    }

    /**
     *  I add VOS to OM project, however, when i terminate the jt_sran execution,
     *  linux terminal is not display, which not show the command that input from
     *  keyboard until press ENTER. So i add this command to deal with the issue.
     *  Maybe it's because VOS console change the stdout and the stdin that we don't
     *  know, and it must be solved further. Rong Tao
     */
    System("stty sane");

    return VCM_OK;
}
weak_alias(vcm_clean_up, cell_mgmt_clean_up);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_check_paras_valid()
{
    INT32 ret = VCM_ERR;
    
    /**
     *  Paras
     */
    UINT32 if_index = 0;

    /**
     *  Check cell's folder
     */
    CHECK_F_EXIST(VCM_CELLS_FOLDER_PATH);
    
    /**
     * check mac's sources folder is exist or not 
     */
    CHECK_F_EXIST(MAC_SRC_DEFAULT_PATH);
    
    /**
     *  Check system network interface exist or not
     */
    if_index = if_nametoindex(VCM_IF_NAME);
    if(if_index == 0)
    {
        log_vcm_err("[%s] is not exist.\n", VCM_IF_NAME);
        return VCM_ERR;
    }

    /**
     *  Check IPv4 valiable or not for MAC
     */
    ret = is_ipv4_valid(VCM_REGULAR_IP, VCM_IF_IP_BASE, VCM_DoNothing);
    if(ret != VCM_OK)
    {
        log_vcm_err("[%s] is not valid.\n", VCM_IF_IP_BASE);
        return VCM_ERR;
    }

    /**
     *  Check netmask valiable or not for MAC
     */
    ret = is_ipv4_valid(VCM_NETMASK, VCM_IF_NETMASK, VCM_DoNothing);
    if(ret != VCM_OK)
    {
        log_vcm_err("[%s] is not valid.\n", VCM_IF_NETMASK);
        return VCM_ERR;
    }

    /**
     *  Make sure ether/networkInterface is exist.
     */
    CHAR cmd[VCM_STR_LEN], line[VCM_STR_LEN];
    Memset(cmd, 0, VCM_STR_LEN);
    Sprintf(cmd, "ifconfig | grep %s | awk '{print $1}'", VCM_IF_NAME);
    log_vcm_debug();
    FILE *fp = Popen(cmd, "r");
    Fgets(line, Strlen(VCM_IF_NAME)+1, fp);
    if(Strncmp(line, VCM_IF_NAME, sizeof(VCM_IF_NAME)) != 0)
    {
        log_vcm_err("<%s> is not exist.\n", VCM_IF_NAME);
        __vcm_set_errno(_VCM_EIINETIF);
        Pclose(fp);
        return VCM_ERR;
    }
    Pclose(fp);

    
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_cell_entity_init()
{    
    __called_once();    

    /**
     *  constant var 
     */
    UINT32  cell_id = 0;
    UINT32  rru_id = 0;
    UINT32  phy_id = 0;
    
    /**
     *  add cell mode to "vcm_mode_str[]"
     */
    add_mode(GSM);
    add_mode(GMR);
    add_mode(WCDMA);
    add_mode(TD_SCDMA);
    add_mode(TDD_LTE);
    add_mode(FDD_LTE);

    /**
     *  add rru status string to "vcm_rru_status_str[]"
     */
    add_rru_status(RRU_STATUS_ADDED);
    add_rru_status(RRU_STATUS_DELETED);

    /**
     *  add mac status string to "vcm_mac_status_str[]"
     */
    add_mac_status(MAC_STATUS_NULL);
    add_mac_status(MAC_STATUS_OK);
    add_mac_status(MAC_STATUS_ERR);

    
    /**
     *  Set VCM_SET to ZERO
     */
    VCM_ZERO(&all_cell_set);
    VCM_ZERO(&virt_cell_set);
    VCM_ZERO(&simu_cell_set);
    VCM_ZERO(&normal_cell_set);
    VCM_ZERO(&all_phy_index_set);
    VCM_ZERO(&real_phy_index_set);
    VCM_ZERO(&simu_phy_index_set);
    VCM_ZERO(&rrus_set);

    
    /**
     *  Set cell entity to zero
     */
    for(cell_id=VCM_MIN_CELL_NUM; cell_id<=VCM_MAX_CELL_NUM; cell_id++)
    {
        if(NULL == Memset(&Cells[cell_id-VCM_MIN_CELL_NUM], 0, sizeof(cell_node_t)))
        {
            log_vcm_err("cells Memset Error\n");
            __vcm_set_errno(_VCM_EINITERR);
            vcm_perror("ERROR");
            return VCM_ERR;
        }
    }

    /**
     *  Set rrus table to zero
     */
    for(rru_id=1; rru_id<=VCM_RRU_NUM; rru_id++)
    {
        if(NULL == Memset(&Rrus[rru_id-1], 0, sizeof(rru_info_t)))
        {
            log_vcm_err("rrus table Memset Error\n");
            __vcm_set_errno(_VCM_EINITERR);
            vcm_perror("ERROR");
            return VCM_ERR;
        }
        Rrus[rru_id-1].status  = RRU_STATUS_MIN;
        Rrus[rru_id-1].ip[0]   = '\0';
    }

    /**
     *  Set real PHY status to zero
     */
    for(phy_id=1; phy_id<=REAL_PHY_INDEX_NUM; phy_id++)
    {
        Real_phy_states[phy_id-1] = 0;
    }

    /**
     *  Set simulate PHY status to zero
     */
    for(phy_id = 1; phy_id <= SIMU_PHY_INDEX_NUM; phy_id++)
    {
        Simu_phy_states[phy_id-1] = 0;
    }

    /**
     *  Set IPv4 table to zero, this global var make sure ipv4 not reuse for MAC configuration
     */
    Memset(ipv4_used, 0, VCM_IF_IP_NUM*sizeof(ipv4_t));

    /**
     *  Turn pointer to NULL
     */
    for(cell_id=VCM_MIN_CELL_NUM; cell_id<=VCM_MAX_CELL_NUM; cell_id++)
    {
        Cells[cell_id-VCM_MIN_CELL_NUM].recover.arg = NULL;
        //set_rru_sn(cell_id, (CONST UINT8*)"\0");
        //set_phy_FpgaMac(cell_id, (CONST UINT8*)"\0");
        Cells[cell_id-VCM_MIN_CELL_NUM].phy.real.FpgaMac[0] = '\0';
        /* Maybe more */
    }

    log_vcm_debug();
    
    __vcm_set_errno(_VCM_EINITSUCCESS);
    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_task_init()
{
    /**
     *  Create VOS cell_mgmt main ctrl thread
     */
    vcm_task1 = TaskCreate(CELL_MGMT_MAIN_TASK_NAME, vcm_task_main_ctrl, NULL);
    if(vcm_task1 == NULL)
    {
        log_vcm_err("Can't create main ctrl thread [TaskCreate] \n");
        __vcm_set_errno(_VCM_EIPCTASKERR);
        MsgqDelete(vcm_msgqid);
        return VCM_ERR;
    }
    log_vcm_succ("Create main ctrl thread Success [TaskCreate] \n");

    
    //TODO: some other thread creation

    /**
     *  Performance statistics thread task
     *  not coding yet
     */
#if 0
    vcm_task2 = TaskCreate(CELL_MGMT_PERS_TASK_NAME, vcm_task_performance_statistics, NULL);
    if(vcm_task2 == NULL)
    {
        log_vcm_err("Can't create Performance statistics thread [TaskCreate] \n");
        __vcm_set_errno(_VCM_EIPCTASKERR);
        MsgqDelete(vcm_msgqid);
        return VCM_ERR;
    }
    log_vcm_succ("Create Performance statistics thread Success [TaskCreate] \n");
#endif 

    /**
     *  create CELL_MGMT module register thread 
     */
    vcm_task3 = TaskCreate(CELL_MGMT_REGISTER_TASK_NAME, vcm_task_module_register_ctrl, NULL);
    if ( NULL == vcm_task3)
	{
	    log_vcm_err("create module register thread error.\n");
        __vcm_set_errno(_VCM_EIPCTASKERR);
        return VCM_ERR;
    }
    log_vcm_succ("create module register thread success.\n");

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_task_main_ctrl(VOID *arg)
{
    log_vcm_info("%s main ctrl ... pid(%d), tid(%d)\n", CELL_MGMT_NAME, getpid(), GET_TID());
    
#define TEST
#ifdef TEST

    #if 1
    UINT32  id = 3;
    
    set_cell_id(id);
    set_cell_mode(id, FDD_LTE);
    set_cell_bandwidth(id, 20);
    set_cell_type(id, CELL_SIMULATE);
    set_cell_type(id+1, CELL_SIMULATE);
    set_cell_state(id, CELL_NORMAL);
    set_mac_cfg_pth(id, "/home/rongtao/mac/cfg");
    set_mac_enb_ipaddr(id, "127.0.0.1");
    set_mac_ether(id, "10.21.3.242");
    set_mac_pid(id, 123);
    //set_simu_phy_index(id, 9);
    set_simu_phy_pid(id, 4567);
    set_simu_phy_sockfd(id, 2145);
    set_simu_phy_status(id, SIMU_PHY_STATUS_OK);
    set_rru_status(id, RRU_STATUS_ADDED);


    VCM_SET(3, &all_cell_set);
    VCM_SET(5, &all_cell_set);
    VCM_SET(8, &all_cell_set);
    VCM_SET(12, &all_cell_set);
    VCM_SET(12, &virt_cell_set);
    VCM_SET(8, &simu_cell_set);
    VCM_SET(3, &normal_cell_set);
    
    cell_info_display(STDOUT, VCM_ALL_CELLS);
    cell_info_display(STDOUT, id);
    
    System("mkdir %s/%d", VCM_CELLS_FOLDER_PATH, id);
    System("cp -a %s/* %s/%d", MAC_SRC_DEFAULT_PATH, VCM_CELLS_FOLDER_PATH, id);

    vcm_L2L3_init(id);
    vcm_L1_init(id);

    /* TEST: EVENT thing happend */
    vcm_register_thing_happend(id, CELLOK);


    Sleep(20);

    vcm_register_thing_happend(id, CELLNULL);
    
    #endif

    /**
     *  test net interface
     */
    #if 0
    /* TEST: network interface (ip, netmask, hardware address, etc.) */
    set_if_netmask("ens33:1", "255.255.0.0");
    
    char str[16];
    get_if_inet_addr("ens33:1", str);
    log_vcm_info("ens33: ip:%s\n", str);
    
    sleep(2);
    set_if_inet_addr("ens33:1", "10.21.33.23");

    
    get_if_netmask("ens33:1", str);
    
    log_vcm_info("ens33: netmask:%s\n", str);

    UINT8 ipv4[16];
    INT32 iii;
    for(iii=0; iii<3; iii++) 
    {
        create_ipv4(ipv4);
        log_vcm_warn("ipv4:%d-- %s\n", iii, ipv4);
    }

    net_if_create("ens33:2", "10.21.21.231", "255.255.255.0");
    #endif


    /**
     *  test rru register
     */
    #if 0
    Fprintf(STDOUT, "##############################################\n");
    Fprintf(STDOUT, "##############################################\n");
    Fprintf(STDOUT, "##############################################\n");
    Fprintf(STDOUT, "##############################################\n");
    set_rru_id(5, 123);
    set_rru_port(5, 456);
    CONST UINT8 ip[] = {"10.21.3.242"};
    set_rru_ip(5, ip);
    set_rru_axc_index(5, 789);
    set_rru_axc_speed(5, 101112);
    CONST UINT8 sn[] = {"15962"};
    set_rru_sn(5, sn);
    //set_rru_status(5, RRU_STATUS_MIN);
    cell_info_display(STDOUT, 5);
    set_cell_state(5, CELL_NULL);

    vcm_outer_msg_rrum_rru_reg_req rru_reg = {
        .hdr.type = 1,
        .hdr.code = 2,
        .hdr.len = sizeof(vcm_outer_msg_rrum_rru_reg_req),
        .rru_id = 123,
        .rru_sn = "15962",
        .rru_reg = RRU_REGISTER,
    };

    vcm_handle_rru_reg_req(&rru_reg);

    cell_info_display(STDOUT, 5);
    vcm_rrus_display(STDOUT);

    rru_reg.rru_reg = RRU_UNREGISTER;
    
    vcm_handle_rru_reg_req(&rru_reg);

    cell_info_display(STDOUT, 5);
    vcm_rrus_display(STDOUT);

    
    #endif
    
#endif /*<TEST>*/
    
    
    while(1)
    {
        log_vcm_info("Recv message ...\n");

        
        

        Sleep(2);
    }
    
    return VCM_OK;
}


