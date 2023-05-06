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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * cfig_common.h
 *
 *  Created on: Sep 3, 2018
 *      Author: root
 */

#ifndef PRODUCT_OM_CFIG_MGMT_CFIG_H_CFIG_COMMON_H_
#define PRODUCT_OM_CFIG_MGMT_CFIG_H_CFIG_COMMON_H_

//#define REC_MSG_BUFFER_SIZE   1024
#define MSG_HEAD_LEN          64

#define MAX_RRU_ID 				65535
#define MAX_CPRI_INDEX 			65535

#define setbit(x,y) (x |= (1 << y))
#define clrbit(x,y) (x &= ~(1 << y))
#endif /* PRODUCT_OM_CFIG_MGMT_CFIG_H_CFIG_COMMON_H_ */
