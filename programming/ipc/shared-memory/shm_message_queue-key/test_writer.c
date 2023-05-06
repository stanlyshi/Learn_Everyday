/*************************************************************************
	> File Name: test_writer.c
	> Author: wk
	> Mail: 18402927708@163.com
	> Created Time: Tue 26 Jul 2016 06:04:54 PM CST
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
 
int main(int argc, char *argv[])
{
    // 创建或打开队列
	long shmkey = 0x1234;
	int element_size = 124; // 基本块的大小，如果数据小于1个块，就按1个块存储，否则，存储到连续多个块中，只有第一个块有块头部信息
	int element_count = 1024; // 队列的长度，总共有多少个块

    if(shmqueue_exist(shmkey))
    {
        printf("shmqueue_exist: %ld\n", shmkey);
    }

    sq_t sq = shmqueue_create(shmkey, element_size, element_count);
    printf("shmqueue_create: %s\n", shmqueue_errorstr());
    
	// 如果需要开启signal通知功能，设置一下通知的参数
	shmqueue_set_sigparam(sq, SIGUSR1, 1, 2);

	// 现在可以开始写数据了
	char *data = "--> I am writer";
	if(shmqueue_put(sq, data, strlen(data))<0)
	{
		// 队列满了。。。
	}
    shmqueue_destroy(sq);
    
	return 0;
}

