/*************************************************************************
	> File Name: test_writer.c
	> Author: wk
	> Mail: 18402927708@163.com
	> Created Time: Tue 26 Jul 2016 06:04:54 PM CST
 ************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "shm_queue.h"
#include "test_common.h"
 
int main(int argc, char *argv[])
{   
    sq_t sq = shmqueue_create(SHM_FILE, 124, 1024);
    printf("shmqueue_create: %s\n", shmqueue_errorstr());
    
	shmqueue_set_sigparam(sq, SIGUSR1, 1, 2);
    printf("shmqueue_set_sigparam: %s\n", shmqueue_errorstr());
    
	char data[64] = {0};
    
    int i = 0;
    while(1)
    {
        snprintf(data, sizeof(data), "I am Writer. %d", i++);
        if(shmqueue_put(sq, data, strlen(data))<0)
        {
        	printf("shmqueue_put: %s\n", shmqueue_errorstr());
            break;
        }
        sleep(1);
    }
	
    shmqueue_destroy(sq);
    printf("shmqueue_destroy: %s\n", shmqueue_errorstr());
    
	return 0;
}

