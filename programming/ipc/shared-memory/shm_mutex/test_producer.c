#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "shmq.h"

#include "test_common.h"

shmq_t shmq = NULL;


void sig_handler(int signum)
{
    printf("Catch crtl-C.\n");

    shmq_close(shmq);

    exit(1);
}

int main()
{
    signal(SIGINT, sig_handler);
    
    shmq = shmq_master_open(MY_SHMQ_NAME, MY_SHMQ_SIZE);
    
    int i=0;
    int ret;
    while(1)
    {
        sleep(1);
        ret = shmq_write(shmq, test_context, sizeof(test_context), sizeof(test_context)+i++);
        if(ret<=0)
        {
            continue;
        }
        printf("shmq write: %s(%d)\n", test_context, ret);
    }

    printf("exit.\n");

    return 0;
}


