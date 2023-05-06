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
 * @file vos_sem.h
 * @brief vos 信号量头文件
 * @details 提供信号量操作函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/



#ifndef __VOS_SEM_H__
#define __VOS_SEM_H__

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** 阻塞  */
#define VOS_WAIT_FOREVER   -1

/** 非阻塞  */
#define VOS_WAIT_NO_WAIT    0


/** 信号量状态 */
typedef enum
{
    VOS_SEM_EMPTY,			///<  0: semaphore not available
    VOS_SEM_FULL			///<  1: semaphore available
}VOS_SEM_B_STATE;

/** 信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO */
typedef enum{
    VOS_SEM_Q_FIFO           = 0x0,	///< first in first out queue
    VOS_SEM_Q_PRIORITY       = 0x1,	///< priority sorted queue
    VOS_SEM_DELETE_SAFE      = 0x4,	///< owner delete safe (mutex opt.)
    VOS_SEM_INVERSION_SAFE   = 0x8,	///< no priority inversion (mutex opt.)
}VOS_sem_option_t;


/** 
 * 创建Bianary类型的信号量，linux中 与Count类型 一致
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @param[in]   enInitialState      初始化状态，VOS_SEM_B_STATE 类型的枚举值
 * @return      返回SEM_ID，失败则返回 0
 */ 
#define VOS_SemBCreate(lOption, enInitialState)     __VOS_SemBCreate(lOption, enInitialState, __FUNCTION__, __LINE__)


/** 
 * 创建Count类型的信号量
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @param[in]   enInitialState      初始化状态，VOS_SEM_B_STATE 类型的枚举值
 * @return      返回SEM_ID，失败则返回 0
 */ 
#define VOS_SemCCreate(lOption, enInitialState)     __VOS_SemCCreate(lOption, enInitialState, __FUNCTION__, __LINE__)


/** 
 * 创建互斥锁
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @return      返回SEM_ID，失败则返回 0
 */ 
#define VOS_SemMCreate(lOption)                            __VOS_SemMCreate(lOption, __FUNCTION__, __LINE__)


/** 
 * 获取信号量
 * @param[in]   semID      信号量ID
 * @param[in]   lMsec      超时等待的时间，单位毫秒，也可以为VOS_WAIT_FOREVER或VOS_NO_WAIT
 * @return      VOS_OK - 成功，其他 - 失败/超时
 */ 
LONG VOS_SemTake(ULONG semID, LONG lMsec);


/** 
 * 释放信号量
 * @param[in]   semID      信号量ID
 * @return      VOS_OK - 成功，其他 - 失败
 */ 
LONG VOS_SemGive(ULONG semID);


/** 
 * 删除信号量
 * @param[in]   semID      信号量ID
 * @return      VOS_OK - 成功，其他 - 失败
 */ 
LONG VOS_SemDelete(ULONG semID);






/* ###### 下面的函数不要直接调用，使用上面提供的宏 ###### */


/** 
 * 不要直接调用该API，通过宏 VOS_SemBCreate 使用
 * 创建Bianary类型的信号量，linux中 与Count类型 一致
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @param[in]   enInitialState      初始化状态，VOS_SEM_B_STATE 类型
 * @param[in]   pFunc               调用该API的文件名
 * @param[in]   iLine               调用该API的行号
 * @return      返回SEM_ID，失败则返回 0
 */ 
ULONG __VOS_SemBCreate(LONG lOption, VOS_SEM_B_STATE enInitialState,const CHAR *pFunc, INT iLine);


/** 
 * 不要直接调用该API，通过宏 VOS_SemCCreate 使用
 * 创建Count类型的信号量，linux中 与Count类型 一致
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @param[in]   lInitialCount       初始值
 * @param[in]   pFunc               调用该API的文件名
 * @param[in]   iLine               调用该API的行号
 * @return      返回SEM_ID，失败则返回 0
 */ 
ULONG  __VOS_SemCCreate(LONG lOption, LONG lInitialCount,const CHAR *pFunc, INT iLine);


/** 
 * 不要直接调用该API，通过宏 VOS_SemMCreate 使用
 * 创建互斥锁
 * @param[in]   lOption             信号量选项，linux 中无效，使用 VOS_SEM_Q_FIFO
 * @param[in]   pFunc               调用该API的文件名
 * @param[in]   iLine               调用该API的行号
 * @return      返回SEM_ID，失败则返回 0
 */ 
ULONG  __VOS_SemMCreate(LONG lOption,const CHAR *pFunc, INT iLine);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __VOS_SEM_H__ */
