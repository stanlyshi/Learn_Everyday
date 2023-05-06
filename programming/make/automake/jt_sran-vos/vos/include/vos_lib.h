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
 * @file vos_lib.h
 * @brief vos lib 头文�?
 * @details 包含格式化输出，内存申请释放，打印断�?等函�?
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.09
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/

#ifndef _VOS_LIB_H_
#define _VOS_LIB_H_

#ifdef __cplusplus
    extern "C" {
#endif

#include "vos_types.h"
#include "vos_module.h"
#include <stdarg.h>

/** 
 * VOS 平台初始�?
 * @param[in]   appPath      APP文件路径
 * @param[in]   cfgPath      vos config 文件路径
 * @param[in]   mods         定义已存在模块的数组
 * @param[in]   num          mods数组大小
 * @return      成功时返回申请内存的地址，失败则返回NULL�?
 */ 
LONG VOS_Init(const CHAR *appPath,const CHAR *cfgPath,vos_module_t * mods,INT num);


/** 
 * 分配内存
 * @param[in]   ulSize      申请内存的大�?
 * @param[in]   ulModuleId  申请模块的模块ID
 * @return      成功时返回申请内存的地址，失败则返回NULL�?
 */ 
#define VOS_Malloc(ulSize,ulModuleId) VOS_MallocWithDebug(ulSize,ulModuleId,(UCHAR *)__FILE__,__LINE__)


/** 
 * 释放内存
 * @param[in]   ptr      释放内存的指�?
 */ 
VOID VOS_Free (const VOID *ptr);


/** 
 * 根据bExp判断是否打印调用�?
 * @param[in]   exp        当bExp为假时打印调用栈
 */ 
#define VOS_ASSERT(exp)   VOS_ASSERT_X( (BOOL)(exp), __FILE__, __LINE__ , 0)


/** 
 * 格式化输出到默认输出
 * @param[in]   format    格式字符�?
 * @param[in]   ...       要输出的变量
 * @return      返回输出的字节数
 */ 
ULONG VOS_Printf(const CHAR *format ,...);


/** 
 * 格式化输出到指定字符串中
 * @param[in]   buf       输出位置
 * @param[in]   fmt       格式字符�?
 * @param[in]   ...       要输出的变量
 * @return      返回输出的字节数
 */ 
ULONG VOS_Sprintf(CHAR *buf ,CHAR *fmt, ...);


/** 
 * 格式化输出到指定字符串中
 * @param[in]   buf       输出位置
 * @param[in]   size      buf的大�?
 * @param[in]   fmt       格式字符�?
 * @param[in]   ...       要输出的变量
 * @return      返回输出的字节数
 */ 
ULONG VOS_Snprintf(CHAR *buf, ULONG size, CHAR *fmt, ...);


/** 
 * 读取格式化的字符串中的数�?
 * @param[in]   buf       要读取的字符�?
 * @param[in]   fmt       读取格式
 * @param[in]   ...       存储数据的指�?
 * @return      返回成功读取的字段个�?
 */ 
ULONG VOS_Sscanf(const CHAR * buf, const CHAR * fmt,...);


/** 同qsort */
VOID VOS_qsort(VOID *base, INT nmemb, INT size,
                  INT (*compar)(const VOID *, const VOID *));

/** �? vcm_vsnprintf */
ULONG VOS_Vsnprintf(CHAR * buf, ULONG size, const CHAR * fmt, va_list args);

/** �? Vsprintf */
ULONG VOS_Vsprintf(CHAR * buf, const CHAR * fmt, va_list args);


/** VOS 消息类型 */
typedef enum VOS_MOD_EVENT_TYPE_S
{
    VOS_MOD_EVENT_TYPE_ASYN = 1,          ///< 异步消息
    VOS_MOD_EVENT_TYPE_SYN  = 2,           ///< 同步消息
    VOS_MOD_EVENT_TYPE_MAX  = 0x100,       ///< VOS 占用�?大消息类型ID
}VOS_MOD_EVENT_TYPE_T;



/*=========================================================================================================================*/
/* ###### 下面的函数不要直接调用，使用上面提供的宏 ###### */
/** 
 * 分配内存,不要直接调用该API，�?�过�? VOS_Malloc 调用
 * @param[in]   size      申请内存的大�?
 * @param[in]   moduleId  申请内存的模块ID
 * @param[in]   file      申请内存处的文件�?
 * @param[in]   line      申请内存处的行号
 * @return      成功时返回申请内存的地址，失败则返回NULL�?
 */ 
void* VOS_MallocWithDebug (unsigned long size, unsigned long moduleId, unsigned char *file, unsigned long line);


/** 
 * 打印断言，根据bExp判断是否打印调用栈，不要直接调用该API，�?�过�? VOS_ASSERT 调用
 * @param[in]   bExp        当bExp为假时打印调用栈
 * @param[in]   szFileName  打印处的文件�?
 * @param[in]   iLine       打印处的行号
 * @param[in]   bOutConsole 输出fd
 */ 
VOID VOS_ASSERT_X( BOOL bExp, CHAR *szFileName, INT iLine , INT bOutConsole);


#ifdef __cplusplus
    }
#endif

#endif /*_VOS_LIB_H_*/
