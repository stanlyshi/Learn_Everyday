/**
 *	异步管道
 *	rongtao@sylincom.com
 *	2020年8月4日11:11:03
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <pthread.h>
#include <sys/epoll.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

/* epoll最大event数 */
#define MAX_EVENTS 64

/* 管道两头的线程 */
static pthread_t tpipe_in1;
static pthread_t tpipe_in2;
static pthread_t tpipe_in3;
static pthread_t tpipe_in4;
static pthread_t tpipe_in5;
static pthread_t tpipe_out;

/* 管道两头的FD */
static int pipe_fd[2] = {-1};

/* 管道两头的打开方式 */
static FILE *pipe_in_fp = NULL;
static FILE *pipe_out_fp = NULL;

/* epoll fd */
static int epoll_fd = -1;

/* epoll events */
static struct epoll_event epoll_evs[MAX_EVENTS];

static volatile size_t pkg_size = 10;

FILE *record_rate_fp = NULL;

int nr_thread_write = 1;
int max_pkg_size = 10000;
int pkg_size_interval = 20;

/* 向管道发送数据，数据将从另一端输出 */
inline int mysendto(void *buf, size_t size) {
    int n = write(fileno(pipe_in_fp), buf, size);
//    printf("<< pipe << %s\n", buf);
}

/* 管道进口任务1 */
void * thread_pipe_in(void*arg) {

    char threadname[16];
    unsigned long int tid = pthread_self();
    pthread_getname_np(tid, threadname, 16);
    
	while(1) {
//		usleep(tid%521233);
        static char buf[1500] = {0};
//        sprintf(buf, "I'm thread %s-%lu.", threadname, tid);
        mysendto(buf, pkg_size);
	}
}

/* 管道出口任务主循环 */
int pipe_out_loop(void*arg) {

    static char buffer[4096] = {0};
    int nr_events = 0, iev;
    
    static long int npkg = 0;
    static long int nbyte = 0;
    static struct timeval timestart = {0,0};
    static struct timeval timeend = {0,0};
    
    static int start_flag = 0;
    
    nr_events = epoll_wait(epoll_fd, epoll_evs, MAX_EVENTS, -1);
    
    for(iev = 0; iev < nr_events; iev ++) {
        if(epoll_evs[iev].data.fd == fileno(pipe_out_fp) 
            && epoll_evs[iev].events&EPOLLIN) {
            
            int n = read(fileno(pipe_out_fp), buffer, 4096);
            if(start_flag == 0) {
                gettimeval(&timestart);
            }
            start_flag = 1;
            npkg++;
            nbyte+=n;
//            printf(">> pipe >> %s\n", buffer);
            
        }
    }
    
    gettimeval(&timeend);

    if(npkg%100000==0) {
        static char description[256] = {0};
        static double Mbps = 0.0;
    
        sprintf(description, "PIPE PKG Size %4ld", pkg_size);
        Mbps = statistic_throughput(description, &timestart, &timeend, nbyte, npkg);

        fprintf(record_rate_fp, "%4ld        %10.2lf    \n", pkg_size, Mbps);
        fflush(record_rate_fp);
        
        memset(&timestart, 0, sizeof(struct timeval));
        memset(&timeend, 0, sizeof(struct timeval));
        start_flag = 0;
        npkg = nbyte = 0;
        
        pkg_size += pkg_size_interval;

        if(pkg_size > max_pkg_size) {
            exit(1);
        }
    }
}

/* 管道出口任务 */
void * thread_pipe_out(void*arg) {
    
	while(1) {
        /* 该loop类比于fstack的ff_run(loop, NULL) */
        pipe_out_loop(NULL);
	}
}

/* 初始化管道 */
void pipe_initial() {

	pipe(pipe_fd);
    
	/* 打开管道两端 */
    pipe_in_fp = fdopen(pipe_fd[1], "w");
    pipe_out_fp = fdopen(pipe_fd[0], "r");

}

/* 启动任务 */
void thread_execute() {

    if(nr_thread_write>=1)
	pthread_create(&tpipe_in1, NULL, thread_pipe_in, NULL);
    if(nr_thread_write>=2)
	pthread_create(&tpipe_in2, NULL, thread_pipe_in, NULL);
    if(nr_thread_write>=3)
	pthread_create(&tpipe_in3, NULL, thread_pipe_in, NULL);
    if(nr_thread_write>=4)
	pthread_create(&tpipe_in4, NULL, thread_pipe_in, NULL);
    if(nr_thread_write>=5)
	pthread_create(&tpipe_in5, NULL, thread_pipe_in, NULL);

	pthread_create(&tpipe_out, NULL, thread_pipe_out, NULL);
    
    if(nr_thread_write>=1)
    pthread_setname_np(tpipe_in1, "pipe-in1");
    if(nr_thread_write>=2)
	pthread_setname_np(tpipe_in2, "pipe-in2");
    if(nr_thread_write>=3)
	pthread_setname_np(tpipe_in3, "pipe-in3");
    if(nr_thread_write>=4)
	pthread_setname_np(tpipe_in4, "pipe-in4");
    if(nr_thread_write>=5)
	pthread_setname_np(tpipe_in5, "pipe-in5");
    
	pthread_setname_np(tpipe_out, "pipe-out");
    
}

/* 主进程loop */
void main_loop() {

	while(1) {
		sleep(1);
        /* do nothing */
	}
}

/* 初始化epoll */
void epoll_initial() {
	epoll_fd = epoll_create(1);
	struct epoll_event pipe_ev;
	pipe_ev.data.fd = fileno(pipe_out_fp);
	pipe_ev.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fileno(pipe_out_fp), &pipe_ev);
}

/* 主函数 */
int main (int argc, char *argv[]) {

    if(argc<5) {
        printf("%s [nthread] [max-pkg-size] [pkg-size-interval] [record-file-name] .\n", argv[0]);
        exit(1);
    
    } else {
        
        nr_thread_write = atoi(argv[1]);
        max_pkg_size = atoi(argv[2]);
        pkg_size_interval = atoi(argv[3]);
    }
    

    record_rate_fp = fopen(argv[4], "w");

	pipe_initial();/* 初始化管道 */
	
	epoll_initial();/* 初始化epoll */

	thread_execute();/* 开始执行任务 */

    main_loop();/*主循环，由主进程执行 */
}
