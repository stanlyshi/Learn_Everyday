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
 * @file vos_timer.h
 * @brief vos timer 头文件
 * @details 提供定时器操作函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/

#ifndef __VOS_TIMER_H__
#define __VOS_TIMER_H__
#include "vos_types.h"

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */


/** VOS Timer 类型枚举值 */
typedef enum
{
    VOS_TIMER_LOOP,             ///< 循环执行的timer 
    VOS_TIMER_NO_LOOP,          ///< 执行一次的timer 
}VOS_TIMER_TYPE_EN;


/** 
 * 创建定时器
 * @param[in]   ulModuleId         模块ID
 * @param[in]   msgQId             如果pfTimerFunc非空则无效；如果pfTimerFunc为NULL，则向msgQId队列发送定时器超时消息
 * @param[in]   lMillSec           定时器的时间，毫秒
 * @param[in]   pfTimerFunc        超时回调函数
 * @param[in]   pArg               回调函数参数
 * @param[in]   enType             定时器类型
 * @return      返回Timer ID，失败则返回VOS_ERROR
 */
LONG VOS_TimerCreate(ULONG ulModuleId, ULONG msgQId, LONG lMillSec,
    VOID  (*pfTimerFunc)(VOID *), VOID  *pArg, VOS_TIMER_TYPE_EN enType);


/** 
* 改变定时器时间,不能在超时回调函数中调用
* @param[in]   ulModuleId         模块ID
* @param[in]   ulTimerId          定时器Id
* @param[in]   lMillSec           定时器的时间，毫秒
* @return      成功返回VOS_OK，失败则返回VOS_ERROR
*/
LONG VOS_TimerChange(ULONG ulModuleId, ULONG ulTimerId, LONG lMillSec);


/** 
* 删除定时器
* @param[in]   ulModuleId         模块ID
* @param[in]   ulTimerId          定时器Id
* @return      成功VOS_OK，失败则返回VOS_ERROR
*/
LONG VOS_TimerDelete(ULONG ulModuleId, ULONG ulTimerId);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __VOS_TIMER_H__ */
