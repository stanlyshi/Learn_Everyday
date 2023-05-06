/*************************************************************************
	> File Name: test_reader_2.c
	> Author: wk
	> Mail: 18402927708@163.com
	> Created Time: Tue 26 Jul 2016 07:40:55 PM CST
 ************************************************************************/
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include"shm_queue.h"
#include<signal.h>
#include <sys/time.h>

 void siguser1(int signo) // dummy signal handler
 {
	   (void)signo;
	   printf("acept a sig\n");
 }

int main()
{
	// 打开共享内存队列
	sq_t sq = shmqueue_open(0x1234); //shmkey=0x1234

	// 如果需要signal通知，需要向系统注册一个signal handler
	signal(SIGUSR1, siguser1);
	// 向队列注册我们的pid以便接收通知
	// 如果你的进程需要fork多个进程，一定好保证在sq_register_signal()调用之前进行
	int sigindex = shmqueue_register_signal(sq);

	// 进入读循环
	while(1)
	{
		char buffer[1024];
         struct timeval write_time;
		int len = shmqueue_get(sq, buffer, sizeof(buffer),&write_time);
		if(len<0) // 读失败
		{
		}
		else if(len==0) // 没有数据，继续做其它操作，然后等待，这里可以进入select/epoll_wait等待
		{
			shmqueue_sigon(sq, sigindex); // 打开signal通知
			sleep(10);
			shmqueue_sigoff(sq, sigindex); // 关闭signal通知
		}
		else // 收到数据了
		{
		   printf("sec %lds,usec %ldus I am reader: %s\n",write_time.tv_sec,write_time.tv_usec,buffer);
		}
	}
    return 0;
}
