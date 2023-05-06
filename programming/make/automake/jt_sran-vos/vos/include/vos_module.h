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
 * @file vos_module.h
 * @brief vos 模块管理 头文件
 * @details 提供模块注册等函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/


#ifndef __VOS_MODULE_H__ 
#define __VOS_MODULE_H__

#include "vos_types.h"


/** 模块名大小 */
#define MODULE_NAME_LEN (48)

/** 模块管理类型 */
typedef struct vos_module_s{
    ULONG               moduleID;                     ///< 模块ID 
    CHAR                name[MODULE_NAME_LEN];        ///< 模块名 
    ULONG               queueID;                      ///< 模块消息队列，用于接收从其他模块发送过来的消息 
}vos_module_t;


/** 
 * 模块注册 API 
 * 向VOS 模块管理中注册模块 。
 * @param[in ]   name         模块名，大小为 MODULE_NAME_LEN
 * @param[in]    module       模块管理信息
 *                               传入值
 *                                      module.queueID  用于模块间通信
 * @param[out]   module       模块管理信息
 *                               传出值
 *                                      module.name
 *                                      module.moduleID
 * @return       VOS_OK - 成功，其他 - 失败
 */ 
LONG VOS_module_register(CHAR *name,vos_module_t *module);


/** 
 * 根据模块ID获得模块名 
 * @param[in ]   id         模块ID
 * @param[out]   name       模块名，大小为 MODULE_NAME_LEN
 * @return       VOS_OK - 成功，其他 - 失败
 */ 
LONG VOS_module_get_name(ULONG id,CHAR name[MODULE_NAME_LEN]);

/** 
 * 根据模块名获得模块ID 
 * @param[in ]   name       模块名，大小为 MODULE_NAME_LEN
 * @return       成功返回模块ID，失败返回 0
 */ 
ULONG VOS_module_get_Id(CHAR *name);


/** 
 * 向某个模块发送异步消息 
 * @param[in ]   dst_slot           目的槽位号
 * @param[in ]   dst_moduleName     目的模块名，大小为 MODULE_NAME_LEN
 * @param[in ]   src_moduleName     源模块名，大小为 MODULE_NAME_LEN
 * @param[in ]   msgCode            消息码
 * @param[in ]   msgData            消息数据
 * @param[in ]   msgDataLen         消息数据长度
 * @return       成功返回 VOS_OK，失败返回 其他
 */ 
LONG VOS_SendAsynMsg2Module(LONG dst_slot,CHAR dst_moduleName[MODULE_NAME_LEN],CHAR src_moduleName[MODULE_NAME_LEN],
                                    LONG msgCode,VOID *msgData,LONG msgDataLen);

/** 
 * 向某个模块发送同步消息 
 * @param[in ]   dst_slot           目的槽位号
 * @param[in ]   dst_moduleName     目的模块名，大小为 MODULE_NAME_LEN
 * @param[in ]   src_moduleName     源模块名，大小为 MODULE_NAME_LEN
 * @param[in ]   msgCode            消息码
 * @param[in ]   msgData            消息数据，如果没有则填 NULL
 * @param[in ]   msgDataLen         消息数据长度，如果msgData 为NULL 填0
 * @param[out]   ackData            接收返回数据的buf
 * @param[in ]   ackDataLen         输入：接收返回数据buf的长度
 * @param[out]   ackDataLen         输出：buf的有效长度
 * @param[in ]   timeout            等待接收的时间，单位毫秒,调用者自行判断需要等待时间，一般等待 5秒
 * @return       成功返回 VOS_OK，失败返回 其他
 */ 
LONG VOS_SendSynMsg2Module(LONG dst_slot,CHAR dst_moduleName[MODULE_NAME_LEN],CHAR src_moduleName[MODULE_NAME_LEN],
                                  LONG msgCode,VOID *msgData,LONG msgDataLen,VOID *ackData,LONG *ackDataLen,LONG timeout);


/** 
* 回复同步消息 
* @param[in ]   aulMsg             VOS_QueReceive 收到的原始消息
* @param[in ]   ackData            返回数据的buf
* @param[in ]   ackDataLen         返回数据buf的长度
* @return       成功返回 VOS_OK，失败返回 其他
*/ 
LONG VOS_SendSynAckMsg(ULONG aulMsg[4],VOID *ackData,LONG ackDataLen);


/** 从原始消息中获取消息类型 （同步/异步） */
#define VOS_MOD_MSG_GET_EVENT_TYPE(aulMsg)       aulMsg[0]

/** 从原始消息中获取消息类码 */
#define VOS_MOD_MSG_GET_CODE(aulMsg)         ((SYS_MSG_S *)(aulMsg[3]))->usMsgCode


/** 从原始消息中获取源模块号 */
#define VOS_MOD_MSG_GET_SRC_ID(aulMsg)       ((SYS_MSG_S *)(aulMsg[3]))->ulSrcModuleID


/** 从原始消息中获取目的模块号 */
#define VOS_MOD_MSG_GET_DST_ID(aulMsg)       ((SYS_MSG_S *)(aulMsg[3]))->ulDstModuleID


/** 从原始消息中获取源槽位号 */
#define VOS_MOD_MSG_GET_SRC_SLOT(aulMsg)     ((SYS_MSG_S *)(aulMsg[3]))->ulSrcSlotID


/** 从原始消息中获取目的槽位号 */
#define VOS_MOD_MSG_GET_DST_SLOT(aulMsg)     ((SYS_MSG_S *)(aulMsg[3]))->ulDstSlotID


/** 从原始消息中获取消息长度 */
#define VOS_MOD_MSG_GET_LEN(aulMsg)        ((SYS_MSG_S *)(aulMsg[3]))->usFrameLen

/** 从原始消息中获取消息buf 的指针 */
#define VOS_MOD_MSG_GET_DATAPTR(aulMsg)    ((SYS_MSG_S *)(aulMsg[3]))->ptrMsgBody

/** 从原始消息中拷贝消息到dst中 */
#define VOS_MOD_MSG_GET_DATA(dst,aulMsg)   if( NULL != VOS_MOD_MSG_GET_DATAPTR(aulMsg) )                                                 \
                                           {  VOS_MemCpy(dst,VOS_MOD_MSG_GET_DATAPTR(aulMsg),VOS_MOD_MSG_GET_LEN(aulMsg));  }           \
                                           else                                                                                         \
                                           {  VOS_ASSERT(0);   }

/** 释放原始消息中的sys msg */
#define VOS_MOD_MSG_FREE(aulMsg)           VOS_Free((VOID *)aulMsg[3])


/** msg handler的函数原型 */
typedef LONG(*VOS_msg_code_handler_ptr)(ULONG aulMsg[4]);


/** 用于模块间通信的code 和 handler的map */
typedef struct msg_code_handle_map_s
{
    LONG                     msg_code;   ///< 消息码 
    VOS_msg_code_handler_ptr handler;    ///< 消息码的处理函数
}VOS_msg_code_handle_map_t;



#endif /* __VOS_MODULE_H__ */
