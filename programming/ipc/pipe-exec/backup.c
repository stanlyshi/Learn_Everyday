#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "pipe-exec.h"

#define PBUFFER_LEN 512
#define PEXEC_PROMPT    "exec>"

struct pipe_exec {
#define PI_WR   0x0001
#define PI_RD   0x0002
#define PO_WR   0x0004
#define PO_RD   0x0008
    int pin[2];
    int pout[2];
    
    char prompt[32];
};



static int _pipe_init(struct pipe_exec *pe)
{
    if (pipe(pe->pin) < 0)
        return -1;
    if (pipe(pe->pout) < 0)
        return -1;

    return 0;
}

static int _pipe_close(struct pipe_exec *pe, int mask)
{
    if(mask|PI_RD) close(pe->pin[0]);
    if(mask|PI_WR) close(pe->pin[1]);
    if(mask|PO_RD) close(pe->pout[0]);
    if(mask|PO_WR) close(pe->pout[1]);
}


static int _read_pipe(struct pipe_exec *pe)
{
	char buffer[PBUFFER_LEN];
	ssize_t rxed;

	while ((rxed = read(pe->pout[0], buffer, sizeof(buffer))) > 0) {
        if(strncmp(buffer, PEXEC_PROMPT, strlen(PEXEC_PROMPT)) == 0) {
            break;
        }
        printf("%s\n", buffer);
	}
    
	return 0;

}

static void _write_pipe(struct pipe_exec *pe, const char *msg)
{
	char *value = NULL;
	if(asprintf(&value, "%s\n", msg) == -1) return;
	if (write(pe->pin[1], value, strlen(value)) == -1) {
		printf("error occured when trying to write to the pipe\n");
	}
    
	free(value);
}


static int _pipe_exec(struct pipe_exec *pe)
{
    int ret;
    char buffer[PBUFFER_LEN];
    
    if(0 != _pipe_init(pe)) {
        printf("create pipe error.\n");
        return -1;
    }

    if((ret = fork()) < 0) {
        return -1;
    } else if (ret == 0) { //child
        close(pe->pout[0]);
        dup2(pe->pout[1], STDOUT_FILENO);
        close(pe->pout[1]);

        close(pe->pin[1]);
        dup2(pe->pin[0], STDIN_FILENO);
        close(pe->pin[0]);


        int i=0;
		const char *argv[2];
		argv[i++] = "./mycmd.sh";
		argv[i++] = NULL;
        
		execvp(argv[0], (char **) argv);
        
        exit(0);
    } else { //parent
    	close(pe->pout[1]);
    	close(pe->pin[0]);

    	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    		printf("ignoring pipe signal failed\n");

    	_read_pipe(pe);

        while(1) {
            printf("INPUT CMD>>");
            scanf("%s", buffer);
            _write_pipe(pe, buffer);
            _read_pipe(pe);
        }
    }

}



int pipe_exec()
{
    struct pipe_exec pe;
    _pipe_exec(&pe);

    
}



int main()
{
    pipe_exec();
}

