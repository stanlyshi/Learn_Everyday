/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/*******************************************************************************
 ** 
 ** Copyright (c) 2018 Sylincom.
 ** All rights reserved.
 **
 ** File name: utils_base.c
 ** Description: Header file for base utils function.
 **
 ** Current Version: 0.1
 ** Author: wangcheng (wangcheng2017@ict.ac.cn)
 ** Date: 2018.09.12
 **
 ******************************************************************************/


/* Includes********************************************************************/
#include "sbs_type.h"
#include "utils_base.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
INT32 utils_ipv4_DDN_to_char_array(const char* ip, unsigned char buf[4])
{
	char point = '.';
	char tmp[4];
	int i = 0;
	int len = strlen(ip);
	int index_buf = 0;
	int index_tmp = 0;
	for (i=0; i < len; i++)
	{
		if (ip[i] == point)
		{
			tmp[index_tmp] = '\0';
			buf[index_buf++] = atoi(tmp);
			memset(tmp, 0, 4);
			index_tmp = 0;
		}
		else
			tmp[index_tmp++] = ip[i];
	}
	tmp[index_tmp] = '\0';
	buf[index_buf++] = atoi(tmp);
	
	return 0;
}



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
INT32 utils_ipv4_char_array_to_DDN(unsigned char ip[4], char buf[16])
{
	int i = 0, j = 0;
	for (i=0; i < 4; i++)
	{
		char ret;
		if ((ret = ip[i] / 100) != 0)
		{
			buf[j++] = ret + 48;
		}
		
		int tmp = ip[i] - 100 * ret;
		ret = tmp / 10;
		buf[j++] = ret + 48;
		
		tmp = tmp - 10 * ret;
		ret = tmp;
		buf[j++] = ret + 48;
		if (i != 3)
			buf[j++] = '.';
	}
	buf[j] = '\0';
	
	return 0;
}



/*******************************************************************************
** print memory by Hexadecimal.
**	like int i = 1 -> The memory "test" len is: 4
**					  01 00 00 00(little endian)
** Input:
** 		msg_p, ,msg_len, msg_name.
** Output:
** 		HexaDecimal memory info.
** Return:
**      NULL
**      
**
********************************************************************************/
inline void utils_show_memory(UINT8 *msg_p, INT32 len, char *name)
{
    int i;

    if (msg_p == NULL) 
    {
        printf("The pointer is NULL in show_memory()\n");
        return;
    }   
    
    if (name != NULL) 
    {
        printf("The memory \"%s\" len is: %d\n", name, len);
    } 
    else 
    {
        printf("The memory len is %d:\n", len);
    }   

    for (i = 0; i < len; i++) 
    {
        printf("%02x ", *(UINT8 *)(msg_p + i));
        if ((i + 1) % 24 == 0) 
        {
            printf("\n");
        }   
    }   
    printf("\n");
}



/*******************************************************************************
 ** Show the system time.
 **	like Wed Sep 12 15:44:21 2018
 **	Input:
 **	
 ** Output:  
 ** 
 ** Return: time string.         
 ********************************************************************************/
char* utils_show_system_time()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	return asctime(timeinfo);
}



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
INT32 utils_mac_format_print(const char* mac, char buf[18])
{
	int i = 0, count = 0;
	for (i=0; i < 12; i++)
	{
		buf[count++] = mac[i];
		if ((i + 1) % 2 == 0 && i + 1 < 12)
			buf[count++] = ':';
	}
	buf[count] = '\0';
	
	return 0;
}


/*******************************************************************************
 ** make mac address format to HexaDecimal char buf[6].
 **	like "B8:2A:72:D0:06:7E" -> "B8 2A 72 D0 06 7E"(HexaDecimal)
 **	Input: mac adresss string, char buf.
 **	
 ** Output:  
 ** 
 ** Return: 
 ** 		success 0
 **			error	-1         
 ********************************************************************************/
 unsigned char utils_Hexadecimal_to_Decimal(char c)
 {
 	if ('0' <= c && c <= '9')
 		return c - 48;
 	else if (c == 'A' || c == 'a')
 		return 10;
 	else if (c == 'B' || c == 'b')
 		return 11;
 	else if (c == 'C' || c == 'c')
 		return 12;
 	else if (c == 'D' || c == 'd')
 		return 13;
 	else if (c == 'E' || c == 'e')
 		return 14;
 	else if (c == 'F' || c == 'f')
 		return 15;
 	else 
 	{
 		printf("char c is invalid.....");
 		return 0;
 	}
 }
 
INT32 utils_mac18_to_mac6(const char mac[18], unsigned char buf[6])
{
	int i = 0, count = 0, tmp = -1;
	unsigned char c1 = 0, c2 = 0;
	for (i=0; i < 17; i++)
	{
		if (mac[i] == ':')
		{
			tmp = i;
			buf[count++] = (c1 << 4) | (c2);
			c1 = 0;
			c2 = 0; 
		}
		else if ((i - tmp - 1) % 2 == 0)
			c1 = utils_Hexadecimal_to_Decimal(mac[i]);
		else
			c2 = utils_Hexadecimal_to_Decimal(mac[i]);
	}
	buf[count] = (c1 << 4) | (c2);
	
	return 0;
}

/*******************************************************************************
 ** make mac address mac[6] format to char buf[18].
 **	like "B8 2A 72 D0 06 7E"(hexdecimal char[6]) -> "B8:2A:72:D0:06:7E"
 **	Input: mac adresss string, char buf.
 **	
 ** Output:  
 ** 
 ** Return: 
 ** 		success 0
 **			error	-1         
 ********************************************************************************/
INT32 utils_mac6_to_mac18(const unsigned char macsrc[6], unsigned char macdst[18])
{
	snprintf((char*)macdst, 18, "%02x:%02x:%02x:%02x:%02x:%02x", macsrc[0], macsrc[1], macsrc[2], macsrc[3], macsrc[4], macsrc[5]);
	macdst[17] = '\0';
	
	return 0;
}
