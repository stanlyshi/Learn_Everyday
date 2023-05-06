/*************************************************************************
	> File Name: test_reader.c
	> Author: wk
	> Mail: 18402927708@163.com
	> Created Time: Tue 26 Jul 2016 06:03:34 PM CST
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
#include <sys/time.h>
#include <unistd.h>

#include "shm_queue.h"
#include "test_common.h"


int main()
{
	sq_t sq = shmqueue_open(SHM_FILE);
    
	signal(SIGUSR1, SHNQ_SIGUSR1);
    
	int sigindex = shmqueue_register_signal(sq);

	// 进入读循环
	while(1)
	{
		char buffer[1024];
         struct timeval write_time;
		int len = shmqueue_get(sq, buffer, sizeof(buffer),&write_time);
		if(len<0) 
		{
		}
		else if(len==0)
		{
			shmqueue_sigon(sq, sigindex); // 打开signal通知
			sleep(10);  //可以进入select 或者 epoll_wait 等待
			shmqueue_sigoff(sq, sigindex); // 关闭signal通知
		}
		else // 收到数据了
		{
			printf("I am reader: %s\n", buffer);
		}
	}
    return 0;
}

