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
   2019年 03月 19日 星期二 14:31:25 CST. 
*/
/**
 * @file vos_cli.h
 * @brief vos cli 头文件
 * @details 提供命令行相关函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/

#ifndef __VOS_CLI_H__
#define __VOS_CLI_H__
#include "vos_types.h"
#include "vos/vospubh/vos_syslog.h"

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */



#define VTY_BUFSIZ 512
#define VTY_MAXHIST 20

enum node_type
{
    VIEW_NODE,              /* View node. Default mode of vty interface. */
    CONFIG_NODE,            /* Config node. Default mode of config file. */
    DEBUG_HIDDEN_NODE,      /* Debug hidden node */
    CONFIRM_ACTION_NODE,    /* Confirm action */
    MAX_NODE
};

/*add by lxh 2004-01-13 begin*/
struct exit_use_node
{
    enum node_type node;
    void * index;
    CHAR prompt[ 64 ];/*01-14*/
    int    node_in_use;
};
/*add by lxh 2004-01-13 end*/


/** VTY struct. */
struct vty
{
    /* File descripter of this vty. */
    int fd;
    char *cur_file_content;
    int cur_file_maxlength;
    /*
    int SessionID;  
    */  
    ULONG msg_que_id;
    ULONG msg_local_addr;
    
    /* Is this vty connect to file or not */
    enum { VTY_TERM, VTY_FILE, VTY_SHELL, VTY_SHELL_SERV, VTY_ONU_OAM }type;

    /* Node status of this vty */
    enum node_type node;

    /* What address is this vty comming from. */
    char *address;

    /* Privilege level of this vty. */
    int privilege;

    /* Failure count */
    int fail;

    /* Output buffer. */
    struct buffer *obuf;

    LONG  Sem_obuf ;

    /* Command input buffer */
    char *buf;

    /* Command cursor point */
    int cp;

    /* Command length */
    int length;

    /* Command max length. */
    int max;

    /* Histry of command */
    char *hist[VTY_MAXHIST];

    /* History lookup current point */
    int hp;

    /* History insert end point */
    int hindex;

    /* For current referencing point of interface, route-map,
    access-list etc... */
    void *index;
    struct exit_use_node node_his_save[8];/*add by lxh  2004-01-13*/

    /* For multiple level index treatment such as key chain and key. */
    void *index_sub;

    /* For escape character. */
    unsigned char escape;

    /* Current vty status. */
    enum { VTY_NORMAL, VTY_CLOSE, VTY_MORE, VTY_START, VTY_CONTINUE, VTY_IOREDIRECT }status;

    /* IAC handling */
    unsigned char iac;

    /* IAC SB handling */
    unsigned char iac_sb_in_progress;
    struct buffer *sb_buffer;

    /* Window width/height. */
    int width;
    int height;

    int scroll_one;

    /* Configure lines. */
    int lines;

    /* Current executing function pointer. */
    int (*func) (struct vty *, void *arg);

    /* Terminal monitor. */
    int monitor;

    /* In configure mode. */
    int config;

    /* B--added by liwei056@2012-5-18 for Cli-Resume-Cfg */
    /* In resume mode. */
    unsigned char resume;
    unsigned char resume_flags;
    unsigned char resume_slot;
    unsigned char resume_port;
    /* E--added by liwei056@2012-5-18 for Cli-Resume-Cfg */

    /* Read and write thread. */
    struct cl_lib_thread *t_read;
    struct cl_lib_thread *t_write;

    /* Timeout seconds and thread. */
    unsigned long v_timeout; /*Seconds number*/
    struct cl_lib_thread *t_timeout;
    unsigned long cl_vty_timeout_val ;/*Seconds number*/

    unsigned short keep_alive_status;   
    /* when status is 0 then when keep alive timeout ,send a tel option
    when status is 1 then when keep alive timeout ,close vty */
    unsigned long v_keep_alive_timeout;
    struct cl_lib_thread *t_keep_alive_timeout;

    unsigned short cl_vty_exit;

    /* Thread output function. */
    struct cl_lib_thread *t_output;
    struct cl_lib_thread *t_event;

    /* Output data pointer. */
    int (*output_func) (struct vty *, int);
    void (*output_clean) (struct vty *);

    struct route_node *output_rn;
    unsigned long output_count;
    int output_type;
    void *output_arg;

    /* vty's connection types , sock, ttys or http */
    int conn_type;
    /* vty's connection's user name */
    char *user_name;

    /*  enum cli_user_role_type user_role; user role should get it when needed.*/

    /* user login failed times ,in authnode , 3 fail disconnect , 
    in auth_enable 3 failed back to VIEW_NODE */

    int failed_times;

    /* Below this is for set pass */
    enum node_type prev_node;  /* preview node before set pass */ 
    enum node_type prev_prev_node;  /* pre-preview node before set pass added by suipl 2007/01/18 for pon slot/port/onuid*/ 
    char *setpass_username;
    char *setpass_first_passwd;
    int setpass_type;  /* 0 for set login pass , 1 for set enable pass */
    /* End of setpass struct */

    VOS_HANDLE ping_taskid;
    int ping_task_ready;
        /*为了告诉vty的使用者ping任务已经开始发送报文，
        可以正常关闭了，如果不等于1，则暂时不要关闭挂带的ping任务*/
    VOS_HANDLE v_shell_taskId;
	VOS_HANDLE v_bcm_cli_taskId;
	VOS_HANDLE v_cdp_pty_taskId;

    long           cl_vty_relay_type;  /* add by suxq 2008-06-19*/
	unsigned long  cl_vty_relay_tout;  /* add by suxq 2008-06-19*/
    unsigned short cl_vty_relay_exit;
    unsigned short cl_vty_in_relay;
    int cl_relay_fd;
    unsigned long cl_relay_fd_sem;
    unsigned long cl_relay_fd_close_sync_sem;
    /* add by liuyanjun,for ^Q close the socket ,2002/09/12 */
    int cl_relay_sock;
    VOS_HANDLE cl_telnet_taskid;
    int cl_relay_console_epon;
    int cl_relay_console_have_handshake;
    int relay_keep_alive_status;
    /* master of the threads. */
    struct cl_lib_thread_master *cl_relay_thread_master;
    /* added by ouzhigang 2002/06/05 */
    struct cl_lib_thread_master *sub_thread_master;
    VOS_HANDLE sub_task_id;
    /* end */
    struct cl_lib_thread *t_relay_read;
    struct cl_lib_thread *t_relay_keep_alive_timeout;

    /* IAC handling */
    unsigned char cl_relay_iac;

    /* IAC SB handling */
    unsigned char cl_relay_iac_sb_in_progress;
    struct buffer *cl_relay_sb_buffer;


    /* debug monitor  0 no monitor 1 monitor*/
    unsigned short debug_monitor;  

    unsigned long monitor_taskid;
    unsigned short monitor_type;
    int monitor_page_no;

    /************************************
    frozen vty in and(or) out 
    used for if a command can't return immediately
    then call frozen to freeze vty's in and(or) out
    call unfrozen when finished.
    1: freezed
    0: unfreezed
    ***********************************/

    unsigned short frozen;

    /* function that called by confirm action */
    int (*action_func) (struct vty *vty);

    /* B--added by liwei056@2009-3-24 for Cli-BroadCast */
    /* function that called by refuse action */
    int (*no_action_func) (struct vty *vty);
    
    int iSysCbFunArg1;
    int iSysCbFunArg2;
    int (*action_SysCBfunc) (struct vty *vty);
    /* function that called by vty's confirm for system handle */
    
    int iUsrCbFunArg1;
    int iUsrCbFunArg2;
    int (*action_UsrCBfunc) (struct vty *vty);
    /* function that called by vty's confirm for user action */

    /* Pre-Command input buffer */
    char *prev_clistr;

    /* Pre-Command length */
    int prev_length;
    /* E--added by liwei056@2009-3-24 for Cli-BroadCast */
    /* B--added by liwei056@2011-9-19 for Cli-BroadCast-User */
    unsigned char aBroadCastCtrls[4];
    unsigned char aBroadCastIDBuf[32];
    /* E--added by liwei056@2011-9-19 for Cli-BroadCast-User */

    /*For the glink module*/
    int hlink_sub_devno;
    char hlink_user_name[20];
    unsigned int hlink_client_type;
    /*end for the glink module*/
    /* add for the INTERFACE_NODE,VLAN_NODE,SUPER_VLAN_NODE,TRUNK_NODE,TUNNEL_NODE,LSP_NODE,
    LOOPBACK_NODE,POS_NODE prompt when two or more user in the switch by liuyanjun 2002/10/14 */
    char vty_cmd_prompt[64];

    VOS_SYSLOG_MONITOR_CONF  monitor_conf;

    /*add by jiangchao, 11/9 afternoon*/
	int nExactIndex;

    /*add by jiangchao, 10/29 afternoon*/
    ULONG g_ulSendMessageSequenceNo;

    /* add by duanbin 2003/1/22 */
    /* vty使用的引用计数 */
    int ncount ;
    /* add by liuyanjun 2003/01/11 */
        ULONG peerLic;
        ULONG rpuvtyShell;

    /*add by duanbin 2003/3/23*/
    int veryBigOutLineCount;    
    int veryBigOutNoFirst;
    
   /*记录认证用户类型*/
   LONG lUserRoleType;

		/*****add buy luj 2006-05-09***begin*****
		**init--0xff
		*0~9---batfileID
		****************************************/
		short batfile_editflag;

	/*onu config file point*/
	void *onuconfptr;
	/*onu config file point stack for profile edit*/
	void *orig_profileptr;

	/*AAA remote  authentication success or fail flag */
	BOOL  AaaAuthResult;
};


/* struct for cl_vector */
struct _cl_vector 
{
    unsigned int max;       /* max number of used slot */
    unsigned int alloced;   /* number of allocated slot */
    void **index;           /* index to data */
};
typedef struct _cl_vector *cl_vector;


/* Structure of command element. */
struct cmd_element
{
    char *string;		/* Command specification by string. */
    int (*func) (struct cmd_element *, struct vty *, int, char **);
    char *doc;			/* Documentation of this command. */
    int msg_flag;       /* 标识是DEFUN、QDEFUN等消息 */
    ULONG p_que_id;     /* 可保存指针和整型值 */
    int daemon;			/* Daemon to which this command belong. */
    cl_vector strvec;	/* Pointing out each description vector. */
    int cmdsize;		/* Command index count. */
    char *config;		/* Configuration string */
    cl_vector subconfig;/* Sub configuration string */
};


#define DEFUN_FLAG     0


#define DEFUN(funcname, cmdname, cmdstr, helpstr)   \
	int funcname (struct cmd_element *, struct vty *, int, char **);\
	struct cmd_element cmdname =   {cmdstr,     funcname,     helpstr , DEFUN_FLAG};\
	int funcname  (struct cmd_element *self, struct vty *vty, int argc, char **argv)


/* Return value of the commands. */
typedef enum cmd_ret_s{
    CMD_SUCCESS             =  0,
    CMD_WARNING             =  1,
    CMD_ERR_NO_MATCH        =  2,
    CMD_ERR_AMBIGUOUS       =  3,
    CMD_ERR_INCOMPLETE      =  4,
    CMD_ERR_EXEED_ARGC_MAX  =  5,
    CMD_ERR_NOTHING_TODO    =  6,
    CMD_COMPLETE_FULL_MATCH =  7,
    CMD_COMPLETE_MATCH      =  8,
    CMD_COMPLETE_LIST_MATCH =  9,
    CMD_SUCCESS_DAEMON      = 10,
}cmd_ret_t;

#define SHOW_STR "Show running system information\n"
#define CONFIG_STR "Config system's setting\n"
#define NO_STR "Negate a command or set its defaults\n"


int vty_out (struct vty *, const char *, ...);

void install_element (enum node_type, struct cmd_element *);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __VOS_TIMER_H__ */
