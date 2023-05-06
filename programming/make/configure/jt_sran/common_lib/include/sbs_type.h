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
 ** Copyright (c) 2006-2010 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: lte_type.h
 ** Description: Header file for types definition.
 **
 ** Current Version: 1.0
 ** Author: Jihua Zhou (jhzhou@ict.ac.cn)
 ** Date: 2006.01.04
 **
 ******************************************************************************/

#ifndef SBS_TYPE_H
#define SBS_TYPE_H

#include <stdio.h>
#include <stdint.h>
#include "err.h"




/* Dependencies ------------------------------------------------------------- */
//#define	ERR_NO_MEM		-2000
//#define	ERR_FUNC_PARAM	-1999
//#define	ERR_SYS_PARAM	-1998
//#define	ERR_BIT_PACK	-1997

#define	NUM_ZERO		0
#define	NUM_ONE			1

/* Constants ---------------------------------------------------------------- */
#define FALSE 0
#define TRUE 1


/* Types -------------------------------------------------------------------- */
typedef int8_t    INT8;
typedef int16_t   INT16;
typedef int32_t   INT32;
typedef int64_t   INT64;
typedef uint8_t   UINT8;
typedef uint16_t  UINT16;
typedef uint32_t  UINT32;
typedef uint64_t  UINT64;
typedef UINT8     BOOL;

typedef void *(* FUNCPTR)(void *args);

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */

static inline void show_memory(UINT8 *msg_p, INT32 len, char *name)
{
	int i;

	if (msg_p == NULL) {
        printf("The pointer is NULL in show_memory()\n");
		return;
	}
	
	if (name != NULL) {
        printf("The memory \"%s\" len is: %d\n", name, len);
	} else {
        printf("The memory len is %d:\n", len);
	}

	for (i = 0; i < len; i++) {
        printf("%02x ", *(UINT8 *)(msg_p + i));
		if ((i + 1) % 24 == 0) {
            printf("\n");
		}
	}
    printf("\n");
}


#endif  /* SBS_TYPE_H */
