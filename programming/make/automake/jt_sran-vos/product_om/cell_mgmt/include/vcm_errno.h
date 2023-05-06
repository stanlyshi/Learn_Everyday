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
** File name: vcm_errno.h 
** Description: 
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.01 -  
**
***********************************************************************************************************************/
#ifndef _VCM_ERRNO_H
#define _VCM_ERRNO_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_types.h"
#include "vcm_sys_wrapper.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/

enum __vcm_error_t_codes 
{
/* base error number and some initialized function */
_VCM_E_SUB_SECTION11 = -1,
_VCM_ESUCCESS               =0x00000000,    /* success */
_VCM_EINITERR               =0x00000001,    /* Cell management initial error */
_VCM_EINITSUCCESS           =0x00000002,    /* Cell management initial success */
_VCM_E_SUB_SECTION12,

/* for cell entity error number */
_VCM_E_SUB_SECTION21        =0x10000000,
_VCM_EICN                   =0x10000001,    /* invalid cell number */
_VCM_EICID                  =0x10000002,    /* invalid cell id */
_VCM_EICMAC                 =0x10000003,    /* invalid cell mac */
_VCM_EICPHY                 =0x10000004,    /* invalid cell phy */
_VCM_EICMODE                =0x10000005,    /* invalid cell mode */
_VCM_EICSTATE               =0x10000006,    /* invalid cell state */
_VCM_EICTYPE                =0x10000007,    /* invalid cell type */
_VCM_EICRRUID               =0x10000008,    /* invalid cell rru's id */
_VCM_EICRRUSN               =0x10000009,    /* invalid cell rru's sn */
_VCM_EICRRUSTATUS           =0x1000000a,    /* invalid cell rru's status */
_VCM_EICRRUIP               =0x1000000b,    /* invalid cell rru's ip */
_VCM_EICRRUPORT             =0x1000000c,    /* invalid cell rru's port */
_VCM_EICAxCSPEED            =0x1000000d,    /* invalid cell rru's axc_speed */
_VCM_EICAxCINDEX            =0x1000000e,    /* invalid cell rru's axc_index */
_VCM_EICPHYINDEX            =0x1000000f,    /* invalid cell phy's index */
_VCM_EICPHYSTATUS           =0x10000010,    /* invalid cell phy's status */
_VCM_EICPHYPORT             =0x10000011,    /* invalid cell phy's port */
_VCM_EICPHYSOCKFD           =0x10000012,    /* invalid cell phy's sockfd */
_VCM_EICMACPATH             =0x10000013,    /* invalid cell mac's config path */
_VCM_EICMACETHER            =0x10000014,    /* invalid cell mac's ether interface */
_VCM_EICMACSOCKFD           =0x10000015,    /* invalid cell mac's sockfd */
_VCM_E_SUB_SECTION22,

/* for CELL_MGMT message error number */
_VCM_E_SUB_SECTION31        =0x20000000,
_VCM_EMNKOMSG               =0x20000001,    /* no this kind of message */
_VCM_EMSENDERR              =0x20000002,    /* send message error */
_VCM_EMRECVERR              =0x20000003,    /* receive message error */
_VCM_EMLENERR               =0x20000004,    /* message length error */
_VCM_E_SUB_SECTION32,

/* for CELL_MGMT pointer\string error number */
_VCM_E_SUB_SECTION41        =0x30000000,
_VCM_ENULLPOINTER           =0x30000001,    /* Cell's NULL pointer error */
_VCM_EOORSTRING             =0x30000002,    /* Cell's String out of range */
_VCM_EICHAR                 =0x30000003,    /* Invalid charactor */
_VCM_E_SUB_SECTION42,

/* for cell_mgmt network error */
_VCM_E_SUB_SECTION51        =0x40000000,
_VCM_ENIFNOEXIST            =0x40000001,    /* no inet interface error */
_VCM_EIINETIF               =0x40000002,    /* invalide inet interface error */
_VCM_ECSOCKETERR            =0x40000003,    /* Create socket error */
_VCM_ECIFCREATEERR          =0x40000004,    /* Create inet interface error */
_VCM_EIIPERR                =0x40000005,    /* invalide ip address error */
_VCM_EINETMASKERR           =0x40000006,    /* invalide netmask address error */
_VCM_E_SUB_SECTION52,

/* for cell mgmt IPC error */
_VCM_E_SUB_SECTION61        =0x50000000,
_VCM_EIPCSEMCERR            =0x50000001,    /* Semaphore create error */
_VCM_EIPCPIPEERR            =0x50000002,    /* Pipe create error */
_VCM_EIPCPIPERERR           =0x50000003,    /* Pipe read error */
_VCM_EIPCPIPEWERR           =0x50000004,    /* Pipe write error */
_VCM_EIPCTASKERR            =0x50000005,    /* Thread/Task create error */
_VCM_EIPCMSGQERR            =0x50000006,    /* Msgqueue create error */
_VCM_E_SUB_SECTION62,

/* for cell_mgmt file error */
_VCM_E_SUB_SECTION71        =0x60000000,
_VCM_EFDOPENERR             =0x60000001,    /* FD open error */
_VCM_EFDREADERR             =0x60000002,    /* FD read error */
_VCM_EFDWRITEERR            =0x60000003,    /* FD write error */
_VCM_EFDCLOSEERR            =0x60000004,    /* FD close error */
_VCM_EFDSETBLOCKERR         =0x60000005,    /* FD set block error */
_VCM_EFDSETNONBLOCKERR      =0x60000006,    /* FD set nonblock error */
_VCM_EFNOTEXIST             =0x60000007,    /* cell_mgmt: No such file or directory */
_VCM_EFNOTREADABLE          =0x60000008,    /* cell_mgmt: File or directory is not readable */
_VCM_EFNOTWRITEABLE         =0x60000009,    /* cell_mgmt: File or directory is not writeable */
_VCM_EFNOTEXECUTEABLE       =0x6000000a,    /* cell_mgmt: File or directory is not executeable */
_VCM_E_SUB_SECTION72,


/* configuration error */
_VCM_E_SUB_SECTION81        =0x70000000,
_VCM_EMACCFGERR             =0x70000001,    /* cell mac's configuration error */
_VCM_EPHYCFGERR             =0x70000002,    /* cell phy's configuration error */
_VCM_ESPHYCFGERR            =0x70000003,    /* cell simulate phy's configuration error */
_VCM_E_SUB_SECTION82,


}; /*<enum __vcm_error_t_codes>*/

#ifndef _STRING_VCM_E
#define _STRING_VCM_E


#define _STRING_VCM_ESUCCESS        "Vcm Success"  /* success */
#define _STRING_VCM_EINITERR        "Cell management module initial error"
#define _STRING_VCM_EINITSUCCESS    "Cell management module initial success"
#define _STRING_VCM_EICN            "Invalid cell number"
#define _STRING_VCM_EICID           "Invalid cell id"
#define _STRING_VCM_EICMAC          "Invalid cell mac"
#define _STRING_VCM_EICPHY          "Invalid cell phy"
#define _STRING_VCM_EICMODE         "Invalid cell mode"
#define _STRING_VCM_EICSTATE        "Invalid cell state"
#define _STRING_VCM_EICTYPE         "Invalid cell type"
#define _STRING_VCM_EICRRUID        "Invalid cell rru's id"
#define _STRING_VCM_EICRRUSN        "Invalid cell rru's sn"
#define _STRING_VCM_EICRRUSTATUS    "Invalid cell rru's status"
#define _STRING_VCM_EICRRUIP        "Invalid cell rru's ip"
#define _STRING_VCM_EICRRUPORT      "Invalid cell rru's port"
#define _STRING_VCM_EICAxCSPEED     "Invalid cell rru's axc speed"
#define _STRING_VCM_EICAxCINDEX     "Invalid cell rru's axc index"
#define _STRING_VCM_EICPHYINDEX     "Invalid cell phy's index"
#define _STRING_VCM_EICPHYSTATUS    "Invalid cell phy's status"
#define _STRING_VCM_EICPHYPORT      "Invalid cell phy's port"
#define _STRING_VCM_EICPHYSOCKFD    "Invalid cell phy's sockfd"
#define _STRING_VCM_EICMACPATH      "Invalid cell mac's path"
#define _STRING_VCM_EICMACETHER     "Invalid cell mac's ether interface"
#define _STRING_VCM_EICMACSOCKFD    "Invalid cell mac's sockfd"
#define _STRING_VCM_EMNKOMSG        "No this kind of message"
#define _STRING_VCM_EMSENDERR       "Send message by msgqueue error"
#define _STRING_VCM_EMRECVERR       "Receive message by msgqueue error"
#define _STRING_VCM_EMLENERR        "Message length error"
#define _STRING_VCM_ENULLPOINTER    "Cell's NULL pointer error"
#define _STRING_VCM_EOORSTRING      "Cell's String out of range"
#define _STRING_VCM_EICHAR          "Invalid charactor"
#define _STRING_VCM_ENIFNOEXIST     "No inet interface error"
#define _STRING_VCM_EIINETIF        "Invalide inet interface error"
#define _STRING_VCM_ECSOCKETERR     "Create socket error"
#define _STRING_VCM_ECIFCREATEERR   "Create inet interface error"
#define _STRING_VCM_EIIPERR         "invalide ip address error"
#define _STRING_VCM_EINETMASKERR    "invalide netmask address error"
#define _STRING_VCM_EIPCSEMCERR     "Semaphore create error"
#define _STRING_VCM_EIPCPIPEERR     "Pipe create error"
#define _STRING_VCM_EIPCPIPERERR    "Pipe read error"
#define _STRING_VCM_EIPCPIPEWERR    "Pipe write error"
#define _STRING_VCM_EIPCTASKERR     "Thread/Task create error" 
#define _STRING_VCM_EIPCMSGQERR     "Msgqueue create error" 
#define _STRING_VCM_EFDOPENERR          "FD open error"
#define _STRING_VCM_EFDREADERR          "FD read error"
#define _STRING_VCM_EFDWRITEERR         "FD write error"
#define _STRING_VCM_EFDCLOSEERR         "FD close error"
#define _STRING_VCM_EFDSETBLOCKERR      "FD set block error"
#define _STRING_VCM_EFDSETNONBLOCKERR   "FD set nonblock error" 
#define _STRING_VCM_EFNOTEXIST          "No such file or directory error" 
#define _STRING_VCM_EFNOTREADABLE       "File or directory is not readable error" 
#define _STRING_VCM_EFNOTWRITEABLE      "File or directory is not writeable error" 
#define _STRING_VCM_EFNOTEXECUTEABLE    "File or directory is not executeable error" 
#define _STRING_VCM_EMACCFGERR      "cell mac's configuration error"
#define _STRING_VCM_EPHYCFGERR      "cell phy's configuration error"
#define _STRING_VCM_ESPHYCFGERR     "cell simulate phy's configuration error"



#endif  /* <_STRING_VCM_E> */



/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/

/* Globals -----------------------------------------------------------------------------------------------------------*/
extern LONG     __vcm_errno;
extern CHAR     __vcm_errno_string[VCM_STR_LEN];
extern Sem_t    sem_lock_errno;

/* Functions ---------------------------------------------------------------------------------------------------------*/
#define __SEM_ERRNO_LOCK     do{Sem_wait(sem_lock_errno);/*log_vcm_debug();*/}while(0);
#define __SEM_ERRNO_UNLOCK   do{Sem_post(sem_lock_errno);/*log_vcm_debug();*/}while(0);

#define __vcm_set_errno(val)  \
    do{\
        __SEM_ERRNO_LOCK \
        __vcm_errno = (val);\
        __vcm_set_errno_string(__vcm_errno2errstring(val));\
        __SEM_ERRNO_UNLOCK \
    }while(0)
    
#define __vcm_set_errno_string(errstr) \
    Strcpy(__vcm_errno_string, (VOID*)errstr)
#define __vcm_errno2errstring(err) \
    _STRING##err



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
INT32 vcm_perror(CONST CHAR *script);




/* Check Macros ------------------------------------------------------------------------------------------------------*/



#endif /*<_VCM_ERRNO_H>*/
