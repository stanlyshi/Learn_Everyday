#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "external.h"

#define PBUFFER_LEN 512

typedef struct pipe_command {
    int pin[2];    
    int pout[2];
    pthread_t exe_task;

}pcmd_t;

typedef struct pipe_command_args {
    int argc;
    char **argv;
}pcmd_args_t;

static int create_pipe(pcmd_t *pcmd)
{
    pipe(pcmd->pin);
    pipe(pcmd->pout);
}

static int destroy_pipe(pcmd_t *pcmd)
{
    close(pcmd->pin[0]);
    close(pcmd->pin[1]);
    close(pcmd->pout[0]);
    close(pcmd->pout[1]);
}

/* 执行命令 */
static int execute_command(pcmd_t *pcmd)
{
    
}

static void *thread_routine(void*arg) 
{
    pcmd_t *_pcmd = (pcmd_t *)arg;
    int nread = 0;

    char buffer[PBUFFER_LEN] = {0};

    nread = read(_pcmd->pin[0], buffer, PBUFFER_LEN);
    if(nread <= 0) {
        fprintf(stderr, "read from pin[0] error.\n");
    }
    printf("SLAVE: %s\n", buffer);

    /* 执行命令 */
    execute_command(_pcmd);
    buffer[0] = 'A';

    write(_pcmd->pout[1], buffer, strlen(buffer));
}

static int create_thread(pcmd_t *pcmd)
{
    pthread_create(&pcmd->exe_task, NULL, thread_routine, pcmd);
}

int pexec(const char *cmdfmt, ...)
{
    pcmd_t pcmd;

    
    create_pipe(&pcmd);
    create_thread(&pcmd);
    
    char buffer[PBUFFER_LEN] = {"I am You."};

    write(pcmd.pin[1], buffer, strlen(buffer));

    read(pcmd.pout[0], buffer, PBUFFER_LEN);

    printf("MASTER: %s\n", buffer);

    destroy_pipe(&pcmd);
}

int test_main()
{
    
    while(1) {
        
        pexec("ls %s", "-la");
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    test_main();
}

