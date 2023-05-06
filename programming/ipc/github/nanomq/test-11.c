#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include <nmq2.h>


#ifndef TEST_NUM
#define TEST_NUM   (1UL<<20)
#endif

/**
 *  获取tick
 */
#ifndef RDTSC
#define RDTSC() ({\
    register uint32_t a,d; \
    __asm__ __volatile__( "rdtsc" : "=a"(a), "=d"(d)); \
    (((uint64_t)a)+(((uint64_t)d)<<32)); \
    })
#endif

typedef struct  {
#define TEST_MSG_MAGIC 0x123123ff    
    int magic;
    unsigned long value;
    uint64_t latency;
}__attribute__((aligned(64))) test_msgs_t;

test_msgs_t *test_msgs;
uint64_t latency_total = 0;
uint64_t total_msgs = 0;
uint64_t error_msgs = 0;


void *enqueue_task(void*arg){
    node_t *node = (node_t *)arg;
    int i =0;
    test_msgs_t *pmsg;
    while(1) {
//        usleep(1000);
        pmsg = &test_msgs[i++%TEST_NUM];
        pmsg->latency = RDTSC();
        unsigned long addr = (unsigned long)pmsg;
//        node_send(node, 1, &addr, sizeof(unsigned long));
        ctx_sendto(node->context_, node->node_, 1, &addr, sizeof(unsigned long));
    }
    pthread_exit(NULL);
}

void *dequeue_task(void*arg){
    node_t *node = (node_t *)arg;

    size_t sz = sizeof(unsigned long);
    test_msgs_t *pmsg;
    unsigned long addr;
    while(1) {
//        node_recv(node, 0, &addr, &sz);
        ctx_recvfrom(node->context_, 0, node->node_, &addr, &sz);
        pmsg = (test_msgs_t *)addr;
//        printf("%p, %lx\n", pmsg, addr);
    
        latency_total += RDTSC() - pmsg->latency;
        pmsg->latency = 0;
        if(pmsg->magic != TEST_MSG_MAGIC) {
            error_msgs++;
        }
        
        total_msgs++;

        if(total_msgs % 1000000 == 0) {
            printf("dequeue. per msgs \033[1;31m%lf ns\033[m, msgs (total %ld, err %ld).\n", 
                    latency_total*1.0/total_msgs/3000000000*1000000000,
                    total_msgs, error_msgs);
        }
    }
    pthread_exit(NULL);
}



int main()
{
    pthread_t enqueue_taskid, dequeue_taskid;

    context_t ctx1;
    char *fname = tempnam(NULL, "rtoax-nmq-"); // UGLY
    printf("fname = %s\n", fname);
    
    ctx_init(&ctx1, fname);
    ctx_create(&ctx1, 4, 8, 1024);

    node_t node0, node1;

    node_init(&node0, &ctx1, 0);
    node_init(&node1, &ctx1, 1);


    unsigned int i =0;
    test_msgs = (test_msgs_t *)malloc(sizeof(test_msgs_t)*TEST_NUM);
    for(i=0;i<TEST_NUM;i++) {
//        test_msgs[i].magic = TEST_MSG_MAGIC + (i%10000==0?1:0); //有错误的消息
        test_msgs[i].magic = TEST_MSG_MAGIC; //有错误的消息
        test_msgs[i].value = i+1;
    }


    pthread_create(&enqueue_taskid, NULL, enqueue_task, &node0);
    pthread_create(&dequeue_taskid, NULL, dequeue_task, &node1);

    pthread_join(enqueue_taskid, NULL);
    pthread_join(dequeue_taskid, NULL);

    return EXIT_SUCCESS;
}



