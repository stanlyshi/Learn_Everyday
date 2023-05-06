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
 * @file vos_que.h
 * @brief vos 消息队列 头文件
 * @details 提供消息队列操作函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/


#ifndef __VOS_QUE_H__
#define __VOS_QUE_H__

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** 队列中无消息 */
#define VOS_NO_MSG          0

/** 队列中有消息 */
#define VOS_HAVE_MSG        (4 * sizeof(ULONG))

/** 队列类型，linux中只支持 VOS_MSG_Q_FIFO 和 VOS_MSG_Q_PRIORITY 类型 */
typedef enum {
    VOS_MSG_Q_FIFO           = 0, ///< first in first out queue
    VOS_MSG_Q_PRIORITY       = 1, ///< priority sorted queue
    VOS_MSG_Q_PIPE           = 2, ///< pipe queue
    VOS_MSG_Q_ISOCK			 = 3, ///< ISOCK queue
}VOS_msgq_type_t;

/** 消息优先级 */
typedef enum {
    MSG_PRI_NORMAL      = 0,  ///< 普通优先级
    MSG_PRI_URGENT      = 1,  ///< 高优先级
}VOS_msgq_pri_t;


/** 
 * 创建消息队列
 * @param[in]   lMaxMsgs     消息队列中最大消息数目
 * @param[in]   lOptions     选项，VOS_msgq_type_t 类型，linux中支持MSG_Q_FIFO和MSG_Q_PRIORITY
 * @return      成功返回队列ID，失败则返回 0
 */ 
ULONG VOS_QueCreate(LONG lMaxMsgs, LONG lOptions);


/** 
 * 删除队列
 * @param[in]   msgQId     消息队列ID
 * @return      成功返回队列ID，失败则返回 0
 */ 
LONG VOS_QueDelete(ULONG msgQId);


/** 
 * 向消息队列发消息
 * @param[in]   msgQId     消息队列ID
 * @param[in]   aulMsg     发送的消息，含有4个ULONG的数组，其中第一个为消息类型,
 *                         第二个为消息码, 第三和第四个用户可以自行使用
 * @param[in]   lMsec      当队列满时，发送任务等待的时间，单位是毫秒。
 *                         VOS_WAIT_FOREVER 表示永久等待。VOS_NO_WAIT 表示不等待。
 * @param[in]   lPriority  消息的优先级。分为MSG_PRI_NORMAL和MSG_PRI_URGENT两种。
 * @return      VOS_OK - 成功，其他 - 失败
 * @note WINDOWS中存在Bug，队列满了后，不能等待，只能返回失败
 */ 
LONG VOS_QueSend(ULONG msgQId, ULONG aulMsg[4], LONG lMsec, LONG lPriority);


/** 
 * 从消息队列接受
 * @param[in]   msgQId     消息队列ID
 * @param[in]   aulMsg     接收的消息，含有4个ULONG的数组，其中第一个为消息类型,
 *                         第二个为消息码, 第三和第四个用户可以自行使用
 * @param[in]   lMsec      当队列满时，发送任务等待的时间，单位是毫秒。
 *                         VOS_WAIT_FOREVER 表示永久等待。VOS_NO_WAIT 表示不等待。
 * @return      失败为VOS_ERROR；成功时如果消息队列中消息数为0 则返回VOS_NO_MSG，
 *              如果消息队列中消息数不为0 ，则返回VOS_HAVE_MSG。
 * @note 一般任务的入口函数就阻塞在VOS_QueReceive。
 */ 
LONG VOS_QueReceive(ULONG msgQId, ULONG aulMsg[4], LONG lMsec);


/** 
 * 将队列和任务绑定，用于队列管理,一般创建完队列和任务后调用
 * @param[in]   hTask     任务句柄
 * @param[in]   ulQId     消息队列ID
 */
VOID VOS_QueBindTask(VOS_HANDLE hTask, ULONG ulQId);


/** 
 * 获得队列中消息的数目
 * @param[in]   msgQId    消息队列ID
 * @return      成功则返回消息队列中消息的数目，失败则VOS_ERROR。
 */
LONG VOS_QueNum(ULONG msgQId);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VOS_QUE_H__ */
