/**********************************************************************************************************************\
*  文件： fastq.h
*  介绍： 低时延队列
*  作者： 荣涛
*  日期：
*       2021年1月25日    创建与开发轮询功能
*       2021年1月27日 添加 通知+轮询 功能接口，消除零消息时的 CPU100% 问题
*       2021年1月28日 调整代码格式，添加必要的注释
*       2021年2月1日 添加多入单出队列功能 ： 采用 epoll 实现
\**********************************************************************************************************************/

#ifndef __fAStMQ_H
#define __fAStMQ_H 1

#include <stdbool.h>

/* 强制inline，尽可能减少执行的指令数
    若需要调试，请编译时 定义 */
#ifndef always_inline
#define always_inline __attribute__ ((__always_inline__))
#endif

/**
 * FastQ 接口概览
 *
 *	fastq_create        初始化 FastQ 句柄 fastq_context    
 *	fastq_ctx_print     Dump FastQ 信息
 *
 *	fastq_sendto        发送接口：轮询方式（直至发送成功为止返回）
 *	fastq_sendnb        发送接口：非阻塞方式
 *	fastq_recvfrom      接收接口：轮询方式（直至接收成功为止返回）
 *	fastq_recvnb        接收接口：非阻塞方式
 *
 *	fastq_sendto_main   发送接口：轮询发送+通知（直至发送成功为止返回）
 *  fastq_sendtry_main  发送接口：尝试发送+通知（可能由于队列满等问题发送失败）
 *	fastq_recv_main     接收接口：接收通知+轮询（直至接收完队列中所有消息）
 *  
 *  具体使用请参见文件末尾的实例代码
 */

/**
 *  不需要关注的结构
 */
struct fastq_header;
struct fastq_ring;

/**
 *  fastq_context - fastq 队列上下文
 *  
 *  fastq_context 结构中的字段不允许用户修改
 *  
 *  param[*]    p  根据 fastq_create 参数申请的内存空间
 *  param[*]    header 保存参数信息
 *  param[*]    ring 轮询ring
 *  param[*]    pepfd 监听所有接收队列的epoll fd
 *  param[*]    data ring结构数据地址
 */
struct fastq_context {
    struct fastq_header *hdr;
    struct fastq_ring *ring;
    int *pepfd;
    void *ptr;   
    char *data;
};

/**
 *  msg_handler_t - fastq_recv_main 接收函数
 *  
 *  param[in]   msg 接收消息地址
 *  param[in]   sz 接收消息大小，与 fastq_create (..., msg_size) 保持一致
 */
typedef void (*msg_handler_t)(void*msg, size_t sz);


/**
 *  fastq_create - fastq_context 实体句柄的初始化
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   nodes 使用 fastq_context 交互的线程数，参见 发送和接收 API 的 from 和 to 字段
 *                  大小不可大于 FASTQ_MAX_NODE
 *  param[in]   size ring队列中的节点数
 *  param[in]   msg_size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_create(struct fastq_context *self, unsigned int nodes, unsigned int size, unsigned int msg_size);


/**
 *  fastq_ctx_print - fastq_context 信息 dump
 *  
 *  param[in]   fp 文件指针
 *  param[in]   self 见 fastq_context
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline void 
fastq_ctx_print(FILE*fp, struct fastq_context *self);


/**
 *  fastq_sendto - 轮询方式发送消息（该接口将占满 CPU-100%）
 *  
 *  注意： *** 使用该接口请查看示例代码 ***
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_sendto(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size);



/**
 *  fastq_sendnb - 非阻塞方式发送消息
 *  
 *  注意： *** 注意判断返回值 ***
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool
fastq_sendnb(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size);



/**
 *  fastq_recvfrom - 轮询方式接收消息（该接口将占满 CPU-100%）
 *  
 *  注意： *** 使用该接口请查看示例代码 ***
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_recvfrom(struct fastq_context *self, unsigned int from, unsigned int to, void *msg, size_t *size);



/**
 *  fastq_recvnb - 非阻塞方式接收消息
 *  
 *  注意： *** 注意判断返回值 ***
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_recvnb(struct fastq_context *self, unsigned int from, unsigned int to, void *msg, size_t *size);



/**
 *  fastq_sendto_main - 通知 + 轮询方式发送消息
 *  
 *  接口步骤：
 *      1. 轮询入队
 *      2. 通知
 *  
 *  注意： *** 推荐使用该接口 *** 必须使用 fastq_recv_main 接口 接收
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_sendto_main(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size);


/**
 *  fastq_sendtry_main - 通知 + 尝试方式发送消息（可能由于队列满发送失败）
 *  
 *  接口步骤：
 *      1. 轮询入队
 *      2. 通知
 *  
 *  注意： *** 推荐使用该接口 *** 必须使用 fastq_recv_main 接口 接收
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   from 发送使用 的 ring， 参见 fastq_create 的 nodes 参数
 *  param[in]   to 向 to ring 发送， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline bool 
fastq_sendtry_main(struct fastq_context *self, unsigned int from, unsigned int to, const void *msg, size_t size);


/**
 *  fastq_recv_main - 通知 + 轮询方式接收消息
 *  
 *  接口步骤：
 *      1. 等待通知
 *      2. 轮询出队
 *  
 *  注意： *** 推荐使用该接口 *** 必须使用 fastq_sendto_main 接口 发送
 *  
 *  param[in]   self 见 fastq_context
 *  param[in]   node 接收该 node 上 所有环形队列的消息， 参见 fastq_create 的 nodes 参数
 *  param[in]   msg 消息体指针
 *  param[in]   size ring队列中的节点大小，若传递指针，填写 sizeof(unsigned long)
 *
 *  return 成功 返回 true ， 失败 返回 false
 */
always_inline  bool 
fastq_recv_main(struct fastq_context *self, unsigned int node, msg_handler_t handler);


#endif /*<__fAStMQ_H>*/

