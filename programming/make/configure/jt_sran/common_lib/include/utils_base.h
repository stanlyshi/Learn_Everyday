/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/*******************************************************************************
 ** 
 ** Copyright (c) 2018 Sylincom.
 ** All rights reserved.
 **
 ** File name: utils_base.h
 ** Description: Header file for base utils function.
 **
 ** Current Version: 0.1
 ** Author: wangcheng (wangcheng2017@ict.ac.cn)
 ** Date: 2018.09.12
 **
 ******************************************************************************/
#ifndef UTILS_BASE_H
#define UTILS_BASE_H

/* Includes********************************************************************/
#include "sbs_type.h"

/* Macros**********************************************************************/

/* Constants*******************************************************************/

/* Variables*******************************************************************/

/* Declarations****************************************************************/

/* Functions*******************************************************************/

/*******************************************************************************
** make ipv4 DDN to char array.
**	like "10.21.100.142" -> "10 21 100 142"
** Input:
** 		ip string(DDN), char buf[4].
** Output:
** 		NULL
** Return:
**      success 0
**      error	-1
**
********************************************************************************/
extern INT32 utils_ipv4_DDN_to_char_array(const char* ip, unsigned char buf[4]);



/*******************************************************************************
** make ipv4 char array to DDN.
**	like "10 21 100 142" -> "10.21.100.142"
** Input:
** 		ip string[4], char buf[16].
** Output:
** 		NULL
** Return:
**      success 0
**      error	-1
**
********************************************************************************/
extern INT32 utils_ipv4_char_array_to_DDN(unsigned char ip[4], char buf[16]);



/*******************************************************************************
** print memory by Hexadecimal.
**	like int i = 1 -> The memory "test" len is: 4
**					  01 00 00 00
** Input:
** 		msg_p, ,msg_len, msg_name.
** Output:
** 		HexaDecimal memory info.
** Return:
**      NULL
**      
**
********************************************************************************/
extern inline void utils_show_memory(UINT8 *msg_p, INT32 len, char *name);



/*******************************************************************************
 ** Show the system time.
 **	like Wed Sep 12 15:44:21 2018
 **	Input:
 **	
 ** Output:  
 ** 
 ** Return: time string.         
 ********************************************************************************/
extern char* utils_show_system_time();



/*******************************************************************************
 ** make mac address format to printable and readable.
 **	like ("B82A72D0067E") -> "B8:2A:72:D0:06:7E"
 **	Input: mac adresss string, char buf.
 **	
 ** Output:  
 ** 	 
 ** Return: 
 ** 		success 0
 **			error	-1         
 ********************************************************************************/
extern INT32 utils_mac_format_print(const char* mac, char buf[18]);


/*******************************************************************************
 ** make mac address format to HexaDecimal char buf[6].
 **	like "B8:2A:72:D0:06:7E" -> "B8 2A 72 D0 06 7E"
 **	Input: mac adresss string, char buf, flag(1, UPPER, 0, ).
 **	
 ** Output:  
 ** 
 ** Return: 
 ** 		success 0
 **			error	-1         
 ********************************************************************************/
extern INT32 utils_mac18_to_mac6(const char mac[18], unsigned char buf[6]);

/*******************************************************************************
 ** make mac address mac[6] format to HexaDecimal char buf[6].
 **	like "B8 2A 72 D0 06 7E"(hexdecimal char[6]) -> "B8:2A:72:D0:06:7E"
 **	Input: mac adresss string, char buf, flag(1, UPPER, 0, ).
 **	
 ** Output:  
 ** 
 ** Return: 
 ** 		success 0
 **			error	-1         
 ********************************************************************************/
extern INT32 utils_mac6_to_mac18(const unsigned char macsrc[6], unsigned char macdst[18]);


#endif
 
