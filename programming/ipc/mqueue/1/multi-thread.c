#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <time.h>

struct dbg_msg {
    unsigned long msg_seq;
    unsigned long msg_ack;
};

#define MQUEUE_NAME "/_rtoax_mq_"


pthread_t task1, task2;

mqd_t mqd;




void *task1_fn(void*arg) 
{
    int ret;
    ssize_t n;
    struct dbg_msg dmsg;
    struct timespec timeout = {2,0};
    
    char buffer[1024] ={0};

    dmsg.msg_seq = 11;
    dmsg.msg_ack = 0;
    
    while(1) {
        
        ret = mq_send(mqd, (char*)&dmsg, sizeof(struct dbg_msg), 0);
        if(ret != 0) {
            continue;
        } else {
//            printf("T1: send seq(%ld), ack(%ld)\n", dmsg.msg_seq, dmsg.msg_ack);

            //从 1970.1.1计时
            timeout.tv_sec = time(NULL)+2;
            n= mq_timedreceive(mqd, buffer, 1024, 0, &timeout);
//            n= mq_receive(mqd, buffer, 1024, 0);
            if(n>0){
                struct dbg_msg *pdmsg = (struct dbg_msg*)buffer;
                printf("T1: recv ack seq(%ld), ack(%ld)\n", pdmsg->msg_seq, pdmsg->msg_ack);
                dmsg.msg_seq = pdmsg->msg_seq;
                dmsg.msg_ack = pdmsg->msg_ack;
            }
        }
        sleep(1);
    }
}


void *task2_fn(void*arg) 
{
    int ret;
    ssize_t n;
    char buffer[1024] ={0};
    
    struct dbg_msg *pdmsg;

    while(1) {
        n= mq_receive(mqd, buffer, 1024, 0);
        if(ret != 0) {
            continue;
        } else {
            pdmsg = (struct dbg_msg*)buffer;
//            printf("T2: recv seq(%ld), ack(%ld)\n", pdmsg->msg_seq, pdmsg->msg_ack);

            pdmsg->msg_seq++;
            pdmsg->msg_ack++;
            
            ret = mq_send(mqd, (char*)pdmsg, sizeof(struct dbg_msg), 0);            
        }
    }
    
}

void mqueue_init()
{
    int ret;
    struct mq_attr attr;
    
    ret = mq_unlink(MQUEUE_NAME);
    perror("mq_unlink: ");

    attr.mq_flags = 0;
    attr.mq_msgsize = 1024;

    attr.mq_maxmsg = 256;
    mqd = mq_open(MQUEUE_NAME, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO, &attr);
    printf("Init mqeue, mqd =  %d, %s\n", mqd, strerror(errno));
}

void task_init()
{
    pthread_create(&task1, NULL, task1_fn, NULL);
    pthread_create(&task2, NULL, task2_fn, NULL);
}

void task_main()
{
    while(1){
        sleep(2);
    }
}

int main()
{
    mqueue_init();

    
    task_init();

    

    task_main();
}

