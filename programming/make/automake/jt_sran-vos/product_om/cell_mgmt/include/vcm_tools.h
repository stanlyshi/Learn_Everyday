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
** File name: vcm_tools.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _VCM_TOOLS_H
#define _VCM_TOOLS_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <net/if.h>

#include <sys/types.h>
#include <sys/syscall.h>

/**
 *  Decode xml/xer file
 */
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>


#include "vcm_types.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#ifndef GET_TID
#define GET_TID() syscall(SYS_gettid)
#endif /*GET_TID*/


/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *      These function are defined in cell_mgmt_tools.h
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 remove_dir(CONST CHAR *dir);
    
/***********************************************************************************************************************
 * Description:
 *       These function are defined in cell_mgmt_tools.h
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 create_dir(CONST CHAR *dir, mode_t mode);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: 
 ```c
CHAR ipv4[256] = {"125.3.265.0"};
is_ipv4_valid(VCM_NETMASK, ipv4, VCM_Correct);
log_vcm_warn("%s\n", ipv4);
 ```
 OR
 ```c
int ret = is_ipv4_valid(VCM_NETMASK, "125.3.265.0", VCM_DoNothing);
log_vcm_warn("ret = %d\n", ret);

 ```
 **********************************************************************************************************************/
INT32 is_ipv4_valid(VCM_IPv4_TYPE type, CHAR *ipv4, VCM_OPERATOR_TYPE opr);


/***********************************************************************************************************************
 * Description: create a virtual ipv4 
 *
 * Input: cell_virt_id
 *
 * Output:  ipv4
 *          return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ***********************************************************************************************************************/
INT32 create_ipv4(UINT8 *ipv4);



/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 net_if_create(CONST CHAR *ifname, CONST CHAR *ip, CONST CHAR *netmask);

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 net_if_destroy(CONST CHAR *ifname);


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 *  Example:
 *
```c
CHAR hdaddr[VCM_HDADDR_LEN];
CHAR ifname[VCM_STR_LEN] = {"virbr0"};
get_if_hardware_addr(hdaddr, ifname);
log_vcm_info("#########%s\n", hdaddr);
```
 **********************************************************************************************************************/
INT32 get_if_hardware_addr(CONST CHAR *ifname, CHAR hdaddr[VCM_HDADDR_LEN]);

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 get_if_inet_addr(CONST CHAR *ifname, CHAR inet_addr[VCM_IP_LEN]);

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 set_if_inet_addr(CONST CHAR *ifname, CHAR inet_addr[VCM_IP_LEN]);

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 get_if_netmask(CONST CHAR *ifname, CHAR netmask[VCM_IP_LEN]);

/**********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *          
 * Output: return:  success: 0
 *                  error:   -1
 *         output: IPv4 address string
 **********************************************************************************************************************/
INT32 set_if_netmask(CONST CHAR *ifname, CHAR netmask[VCM_IP_LEN]);


/***********************************************************************************************************************
 * Description: reject character from string  
 *
 * Input:   
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_pid_alive(CONST pid_t pid);

/***********************************************************************************************************************
 * Description: 
 *
 * Input:   
 *
 * Output: 
 **********************************************************************************************************************/
INT32 is_pname_alive(CONST CHAR *pname);


/***********************************************************************************************************************
 * Description:
 *       These function are defined in cell_mgmt_tools.h
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 System(CONST CHAR *cmd, ...);

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 *
 * Example: 
 **********************************************************************************************************************/
INT32 Q_A(CONST CHAR* question);


/**********************************************************************************************************************
 * Description: reject character from string  
 *
 * Input:   str: string 
 *          ch: character you wanna reject from string
 * Output: return:  success: 0
 *                  error:   -1
 *         output: MAC address string
 **********************************************************************************************************************/
INT32 strstripc(CHAR *str, CONST CHAR ch);

/**********************************************************************************************************************
 * Description: joint some vars into a string  
 *
 * Input:   dst: destination string 
 *          fmt: arguments format string
 *          ...: arguments
 * Output: return:  success: dst address pointer
 *                  error:   NULL
 **********************************************************************************************************************/
char *strjoint(char *dst, const char *fmt, ...) __attribute__((__format__ (printf, 2, 3)));


/***********************************************************************************************************************
 * Description: show memory bytes on screen hex
 *
 * Input: p     :memory address
 *        nbytes:number of byte(uint8_t)
 *        col   :colonm on screen show
 *
 * Output: return:  success: VCM_OK
 *                  error:   VCM_ERR
 *
 *  Example:
 Memdisplay(STDOUT, &all_cell_set, VCM_CELL_NUM, 10);
 Memdisplaycmp(STDOUT, &all_cell_set, &all_cell_set, VCM_CELL_NUM, 10);
 **********************************************************************************************************************/
INT32 Memdisplay(FILE *fp, CONST VOID *  p, UINT32 nbytes, UINT8 col);

/***********************************************************************************************************************
 * Description: show comparation memory bytes on screen
 *
 * Input: p1     :memory address
 *        p2     :memory address
 *        nbytes :number of byte(uint8_t)
 *        col    :colonm on screen show
 *
 * Output: return:  success: VCM_OK
 *                  error:   VCM_ERR
 *
 *  Example:
 Memdisplay(STDOUT, &all_cell_set, VCM_CELL_NUM, 10);
 Memdisplaycmp(STDOUT, &all_cell_set, &all_cell_set, VCM_CELL_NUM, 10);
 **********************************************************************************************************************/
INT32 Memdisplaycmp(FILE *fp, CONST VOID *p1, CONST VOID *p2, UINT32 nbytes, UINT8 col);


/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
xmlDocPtr xml_doc(CHAR *szDocName);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
xmlXPathContextPtr xml_context(xmlDocPtr doc);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
INT32 alter_xml_node_value(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *content);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
INT32 alter_xml_node_value_next(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *content, INT32 index);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
xmlChar* get_xml_node_value(xmlXPathContextPtr context, CONST xmlChar *xpath);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
INT32 alter_xml_node_attr(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *attr);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
INT32 alter_xml_node_attr_next(xmlXPathContextPtr context, CONST xmlChar *xpath, CONST xmlChar *attr);

/***********************************************************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ***********************************************************************************************************************/
INT32 save_xml(CHAR *szDocName,xmlDocPtr doc,xmlXPathContextPtr context);



#endif /*<_VCM_TOOLS_H>*/

