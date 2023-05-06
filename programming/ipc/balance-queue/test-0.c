/**
 *  test-0.c
 *  
 *  简单的发送接收
 *  
 *  荣涛  2021年1月13日
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include "bqueue.h"

void *enqueue_task(void*arg){
    int i =0;
    while(1) {
        
    }
    pthread_exit(NULL);
}

void *dequeue_task(void*arg){

    int i =0;
    void *pmsg;
    while(1) {
        
    }
    pthread_exit(NULL);
}



int main()
{
    pthread_t enqueue_taskid, dequeue_taskid;

    pthread_create(&enqueue_taskid, NULL, enqueue_task, NULL);
    pthread_create(&dequeue_taskid, NULL, dequeue_task, NULL);

    pthread_join(enqueue_taskid, NULL);
    pthread_join(dequeue_taskid, NULL);

    return EXIT_SUCCESS;
}


