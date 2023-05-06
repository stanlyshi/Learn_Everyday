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
 * @file vos_linklist.h
 * @brief vos 链表 头文件
 * @details 提供链表操作函数
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/



#ifndef _VOS_LINKLIST_H
#define _VOS_LINKLIST_H

#ifdef __cplusplus
extern"C"{
#endif

/** 链表节点 */
typedef struct cl_lib_listnode
{
    struct cl_lib_listnode *next;
    struct cl_lib_listnode *prev;
    void *data;
}*plistnode;

/** 链表名长度 */
#define LIST_NAME_LEN (48)

/** 链表节 */
typedef struct cl_lib_list
{
    struct cl_lib_listnode *head;
    struct cl_lib_listnode *tail;
    char name[LIST_NAME_LEN];
    unsigned long moduleID;
    unsigned int count;
    int ( *cmp ) ( void *val1, void *val2 );
    void ( *del ) ( void *val );
}*plist;


/** 获得链表头 */
#define cl_lib_listhead(X) ((X)->head)

/** 获得链表尾 */
#define cl_lib_listtail(X) ((X)->tail)

/** 获得下个节点 */
#define cl_lib_nextnode(X) ((X) = (X)->next)

/** 获得上个节点 */
#define cl_lib_prevnode(X) ((X) = (X)->prev)

/** 获得链表中节点数量 */
#define cl_lib_listcount(X) ((X)->count)

/** 链表为空 */
#define cl_lib_list_isempty(X) ((X)->head == NULL && (X)->tail == NULL)

/** 获得节点中的数据 */
#define cl_lib_getdata(X) ((X)->data)

/** 
 * 创建一个linklist
 * @param[in]   name      list名
 * @param[in]   moduleID  创建list的模块ID
 * @return      成功时返回新list地址，失败则返回NULL。
 */ 
struct cl_lib_list *cl_lib_list_new (char *name,unsigned long moduleID);



/** 
 * free并删除linklist中的所有节点，然后free该list
 * ！！！注意，如果data也需要free，则需要实现list的del成员
 * @param[in]   plist1      要删除的list
 * @return      成功时返回新list地址，失败则返回NULL。
 */
void cl_lib_list_delete ( struct cl_lib_list * plist1 );

/** 
 * 将数据添加到链表尾，直接传入要添加的数据，该API会自动创建节点
 * @param[in]   plist1      待添加的list
 * @param[in]   val         待添加的数据
 */
void cl_lib_listnode_add ( struct cl_lib_list * plist1, void * val );


/** 
 * 将数据添加到链表头，直接传入要添加的数据，该API会自动创建节点
 * @param[in]   plist1      待添加的list
 * @param[in]   val         待添加的数据
 */
void cl_lib_listnode_add_toHead ( struct cl_lib_list * plist1, void * val );

/** 
 * 删除数据等于val的节点,！！！不会free 数据
 * @param[in]   plist1      待删除的list
 * @param[in]   val         待删除的数据
 */
void cl_lib_listnode_delete ( struct cl_lib_list * plist1, void * val );


#ifdef __cplusplus
}
#endif

#endif /* _VOS_LINKLIST_H */
