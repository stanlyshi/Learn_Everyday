#include <stdio.h>

#include "shmq.h"

#include "test_common.h"

int main()
{
    shmq_t shmq = shmq_slave_open(MY_SHMQ_NAME, MY_SHMQ_SIZE);

    char buf[sizeof(test_context)] = {0};
    int n;
    while(1)
    {
        n = shmq_read(shmq, buf, sizeof(buf), sizeof(test_context));
        if(n<= 0)
        {
            printf("Read error.\n");
            break;
        }
        printf("shmq read: %s\n", buf);
    } 

    return 0;
}

