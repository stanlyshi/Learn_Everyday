/**********************************************************************************************************************\
*  文件： test-4.c
*  介绍： 低时延队列 多入单出队列 通知+轮询接口测试例
*  作者： 荣涛
*  日期：
*       2021年2月2日    
\**********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>

//#define _FASTQ_STATS //开启统计功能
#include <fastq.h>



/* 测试的消息总数 */
#ifndef TEST_NUM
#define TEST_NUM   (1UL<<20)
#endif

enum {
    NODE_1 = 1, 
    NODE_2,
    NODE_3,
    NODE_4,

};

#ifndef RDTSC
#define RDTSC() ({\
    register uint32_t a,d; \
    __asm__ __volatile__( "rdtsc" : "=a"(a), "=d"(d)); \
    (((uint64_t)a)+(((uint64_t)d)<<32)); \
    })
#endif

typedef struct  {
#define TEST_MSG_MAGIC 0x123123ff    
    unsigned long value;
    int magic;
    uint64_t latency;
}__attribute__((aligned(64))) test_msgs_t;



uint64_t latency_total = 0;
uint64_t total_msgs = 0;
uint64_t error_msgs = 0;

struct enqueue_arg {
    int srcModuleId;
    test_msgs_t *msgs;
};

void *enqueue_task(void*arg){
    int i =0;
    struct enqueue_arg *parg = (struct enqueue_arg *)arg;
    test_msgs_t *ptest_msg = parg->msgs;
    test_msgs_t *pmsg;
    
    unsigned long send_cnt = 0;
    
    while(1) {
        usleep(1000000);
        pmsg = &ptest_msg[i++%TEST_NUM];
        pmsg->latency = RDTSC();
        printf("send %lx(%lx)\n", pmsg->value, *(unsigned long*)pmsg);
        VOS_FastQTrySend(parg->srcModuleId, NODE_1, pmsg, sizeof(test_msgs_t));

        send_cnt++;
        
        if(send_cnt % 10000000 == 0) {
            sleep(10);
        }
    }
    pthread_exit(NULL);
}

void handler_test_msg(void* msg, size_t size)
{
    test_msgs_t *pmsg = (test_msgs_t *)msg;
    
    printf("recv %lx\n", pmsg->value);
    
    latency_total += RDTSC() - pmsg->latency;
    pmsg->latency = 0;
    if(pmsg->magic != TEST_MSG_MAGIC) {
        error_msgs++;
    }
    
    total_msgs++;

    if(total_msgs % 1000 == 0) {
        printf("dequeue. per msgs \033[1;31m%lf ns\033[m, msgs (total %ld, err %ld).\n", 
                latency_total*1.0/total_msgs/3000000000*1000000000,
                total_msgs, error_msgs);
    }

}

void *dequeue_task(void*arg) {
    
    VOS_FastQRecv( NODE_1, handler_test_msg);
    pthread_exit(NULL);
}

int sig_handler(int signum) {

    VOS_FastQDump(NULL, NODE_1);
    exit(1);
}

int main()
{
    pthread_t enqueueTask1;
    pthread_t enqueueTask2;
    pthread_t enqueueTask3;
    pthread_t dequeueTask;
    
    test_msgs_t *test_msgs21;
    test_msgs_t *test_msgs31;
    test_msgs_t *test_msgs41;
    
    int max_msg = 16;
    
    signal(SIGINT, sig_handler);
    VOS_FastQCreateModule(NODE_1, max_msg, sizeof(test_msgs_t));
    VOS_FastQCreateModule(NODE_2, max_msg, sizeof(test_msgs_t));
    VOS_FastQCreateModule(NODE_3, max_msg, sizeof(test_msgs_t));
    VOS_FastQCreateModule(NODE_4, max_msg, sizeof(test_msgs_t));
    
    unsigned int i =0;
    test_msgs21 = (test_msgs_t *)malloc(sizeof(test_msgs_t)*TEST_NUM);
    for(i=0;i<TEST_NUM;i++) {
        test_msgs21[i].magic = TEST_MSG_MAGIC + (i%10000==0?1:0); //有错误的消息
//        test_msgs21[i].magic = TEST_MSG_MAGIC;
        test_msgs21[i].value = 0xff00000000000000 + i+1;
    }
    test_msgs31 = (test_msgs_t *)malloc(sizeof(test_msgs_t)*TEST_NUM);
    for(i=0;i<TEST_NUM;i++) {
        test_msgs31[i].magic = TEST_MSG_MAGIC + (i%10000==0?1:0); //有错误的消息
//        test_msgs21[i].magic = TEST_MSG_MAGIC;
        test_msgs31[i].value = 0xffff000000000000 + i+1;
    }
    test_msgs41 = (test_msgs_t *)malloc(sizeof(test_msgs_t)*TEST_NUM);
    for(i=0;i<TEST_NUM;i++) {
        test_msgs41[i].magic = TEST_MSG_MAGIC + (i%10000==0?1:0); //有错误的消息
//        test_msgs21[i].magic = TEST_MSG_MAGIC;
        test_msgs41[i].value = 0xffffff0000000000 + i+1;
    }
    struct enqueue_arg enqueueArg1;
    struct enqueue_arg enqueueArg2;
    struct enqueue_arg enqueueArg3;

    enqueueArg1.srcModuleId = NODE_2;
    enqueueArg1.msgs = test_msgs21;
    enqueueArg2.srcModuleId = NODE_3;
    enqueueArg2.msgs = test_msgs31;
    enqueueArg3.srcModuleId = NODE_4;
    enqueueArg3.msgs = test_msgs41;
    

    pthread_create(&enqueueTask1, NULL, enqueue_task, &enqueueArg1);
    pthread_create(&enqueueTask2, NULL, enqueue_task, &enqueueArg2);
    pthread_create(&enqueueTask3, NULL, enqueue_task, &enqueueArg3);
    pthread_create(&dequeueTask, NULL, dequeue_task, NULL);

	pthread_setname_np(enqueueTask1, "enqueue1");
	pthread_setname_np(enqueueTask2, "enqueue2");
	pthread_setname_np(enqueueTask3, "enqueue3");
	pthread_setname_np(dequeueTask,  "dequeue");

	pthread_setname_np(pthread_self(), "test-1");

    pthread_join(enqueueTask1, NULL);
    pthread_join(enqueueTask2, NULL);
    pthread_join(enqueueTask3, NULL);
    pthread_join(dequeueTask, NULL);

    return EXIT_SUCCESS;
}





