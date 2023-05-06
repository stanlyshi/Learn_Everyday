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
 * @file vos_sys_diff.h
 * @brief vos 系统差异变量 头文件
 * @details 由于系统不同而导致的差异参数，不要直接包含此文件.
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.09
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/



#ifndef __VOS_SYS_DIFF_H__
#define __VOS_SYS_DIFF_H__

#include "vos_types.h"

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if OS_LINUX
#define __TASK_PRIORITY_TIME_CRITICAL           90     /* tTimer */
#define __TASK_PRIORITY_HIGHEST                 80     /* tDevice */
#define __TASK_PRIORITY_HIGHER                  70     /* consloe telnet */
#define __TASK_PRIORITY_HIGH                    60     /* cdp等  *//*hot */
#define __TASK_PRIORITY_ABOVE_NORMAL            50     /* stp *//* IP Receive *//*backup*/
#define __TASK_PRIORITY_NORMAL                  40     /* protocols *//* FDB FIB *//* radius */
#define __TASK_PRIORITY_BELOW_NORMAL            30     /* syslog */
#define __TASK_PRIORITY_LOWEST                  10
#define __TASK_PRIORITY_IDLE                     1
#define __VOS_DEFAULT_STACKSIZE                 1024*1024*8  /* 任务的缺省栈大小. */
#endif

#if OS_VXWORKS
#define __TASK_PRIORITY_TIME_CRITICAL           40      /* tTimer */
#define __TASK_PRIORITY_HIGHEST                 60      /* tDevice */
#define __TASK_PRIORITY_HIGHER                  80      /* consloe telnet */
#define __TASK_PRIORITY_HIGH                    100     /* cdp等  *//*hot */
#define __TASK_PRIORITY_ABOVE_NORMAL            120     /* stp *//* IP Receive *//*backup*/
#define __TASK_PRIORITY_NORMAL                  150     /* protocols *//* FDB FIB *//* radius */
#define __TASK_PRIORITY_BELOW_NORMAL            180     /* syslog */
#define __TASK_PRIORITY_LOWEST                  200
#define __TASK_PRIORITY_IDLE                    255
#define __VOS_DEFAULT_STACKSIZE                 1024 * 64  /* 任务的缺省栈大小. */
#endif



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VOS_SYS_DIFF_H__ */
