/**********************************************************************************************************************\
*  文件： test-0.c
*  介绍： 低时延队列 轮询接口测试例
*  作者： 荣涛
*  日期：
*       2021年1月27日    
\**********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>

#include <fastq.h>

/* 测试消息总数 */
#ifndef TEST_NUM
#define TEST_NUM   (1UL<<20)
#endif

/* 队列发送 NODE， 将 fastq_create 接口参数说明 */
#define NODE_1 1
#define NODE_2 2


#ifndef RDTSC
#define RDTSC() ({\
    register uint32_t a,d; \
    __asm__ __volatile__( "rdtsc" : "=a"(a), "=d"(d)); \
    (((uint64_t)a)+(((uint64_t)d)<<32)); \
    })
#endif

/* 测试消息结构 */
typedef struct  {
#define TEST_MSG_MAGIC 0x123123ff    
    int magic;
    unsigned long value;
    uint64_t latency;
}__attribute__((aligned(64))) test_msgs_t;

/* 发送的消息指针指向的内存空间 */
test_msgs_t *test_msgs;

/* 统计时延和消息数 */
uint64_t latency_total = 0;
uint64_t total_msgs = 0;
uint64_t error_msgs = 0;

/* 队列句柄 */
struct fastq_context ctx1;

/* 发送消息主任务 */
void *enqueue_task(void*arg) {
    /* 获取句柄 */
    struct fastq_context *ctx = (struct fastq_context *)arg;
    int i =0;
    test_msgs_t *pmsg;

    /* 轮询发送（会造成CPU利用率 100%） */
    while(1) {
        pmsg = &test_msgs[i++%TEST_NUM];

        /* 获取发送时 ticks */
        pmsg->latency = RDTSC();
        unsigned long addr = (unsigned long)pmsg;

        /* 发送，从    NODE1 发送到 NODE2 */
        fastq_sendto(ctx, NODE_1, NODE_2, &addr, sizeof(unsigned long));
    }
    pthread_exit(NULL);
}

/* 消息处理函数 */
void handler_test_msg(test_msgs_t*msg)
{
    /* 处理消息流程 */
}

void *dequeue_task(void*arg) {
    struct fastq_context *ctx = (struct fastq_context *)arg;

    size_t sz = sizeof(unsigned long);
    test_msgs_t *pmsg;
    unsigned long addr;

    /* 主任务循环 */
    while(1) {

        /* 轮询接收（接收从 NODE1 发往 NODE2 的消息，与发送端保持一致） */
        fastq_recvfrom(ctx, NODE_1, NODE_2, &addr, &sz);
        pmsg = (test_msgs_t *)addr;

        /* 计算消息接收的时延 */
        latency_total += RDTSC() - pmsg->latency;
        pmsg->latency = 0;

        /* 统计错误的消息 */
        if(pmsg->magic != TEST_MSG_MAGIC) {
            error_msgs++;
        }
        
        total_msgs++;

        /* 处理消息 */
        handler_test_msg(pmsg);

        /* 每 1000000 消息输出统计信息 */
        if(total_msgs % 1000000 == 0) {
            printf("dequeue. per msgs \033[1;31m%lf ns\033[m, msgs (total %ld, err %ld).\n", 
                    latency_total*1.0/total_msgs/3000000000*1000000000,
                    total_msgs, error_msgs);
        }
    }
    pthread_exit(NULL);
}

int sig_handler(int signum) {
    fastq_ctx_print(stderr, &ctx1);
    exit(1);
}

int main()
{
    pthread_t enqueue_taskid, dequeue_taskid;

    signal(SIGINT, sig_handler);

    /* 初始化消息队列 */
    fastq_create(&ctx1, 4, 8, 1024);

    /* 初始化消息结构（理解为内存池） */
    unsigned int i =0;
    test_msgs = (test_msgs_t *)malloc(sizeof(test_msgs_t)*TEST_NUM);
    for(i=0;i<TEST_NUM;i++) {
        test_msgs[i].magic = TEST_MSG_MAGIC + (i%10000==0?1:0); //每10000个有一个错误消息
//        test_msgs[i].magic = TEST_MSG_MAGIC; //有错误的消息
        test_msgs[i].value = i+1;
    }

    /* 创建任务 */
    pthread_create(&enqueue_taskid, NULL, enqueue_task, &ctx1);
    pthread_create(&dequeue_taskid, NULL, dequeue_task, &ctx1);

	pthread_setname_np(enqueue_taskid, "enqueue");
	pthread_setname_np(dequeue_taskid, "dequeue");

	pthread_setname_np(pthread_self(), "test-0");

    pthread_join(enqueue_taskid, NULL);
    pthread_join(dequeue_taskid, NULL);

    return EXIT_SUCCESS;
}



