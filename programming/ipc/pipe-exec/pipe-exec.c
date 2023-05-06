/**
 *  实现 PIPE 命令管道
 *  
 *  作者： 荣涛
 *  日期： 2020年10月13日
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "list.h"

#include "pipe-exec.h"

#define PBUFFER_LEN     1024


static int _read_pipe(struct pipe_exec *pe, const char *prompt)
{
    char ch;
    char line[PBUFFER_LEN] = {0};
    int line_pos = 0;
	ssize_t n;

	while ((n = read(pe->pout[0], &ch, 1)) > 0) {
        line[line_pos++] = ch;
        if(ch == '\n') { //一行结束
            line_pos = 0;
            if(strncmp(line, prompt, strlen(prompt)) == 0) {
                break;
            }
            printf("%s", line);
            memset(line, 0, sizeof(line));
        }
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


/* 初始化 */
int pipe_exec_create(struct pipe_exec *pe, const char *prompt, int argc, char *argv[])
{
    if(!pe || !prompt) {
        return PIPE_EXEC_INVALIDE_PARMS;
    }

    int ret = -1;
    
    memset(pe, 0, sizeof(struct pipe_exec));

    /* 初始化管道 */
    if (pipe(pe->pin) < 0)
        return -1;
    if (pipe(pe->pout) < 0)
        return -1;

    /* 提示字符串 */
    strcpy(pe->prompt, prompt);

    /* fork 执行子进程 */
    if((ret = fork()) < 0) {
        return -1;
    } else if (ret == 0) { //child
        close(pe->pout[0]);
        dup2(pe->pout[1], STDOUT_FILENO);
        close(pe->pout[1]);

        close(pe->pin[1]);
        dup2(pe->pin[0], STDIN_FILENO);
        close(pe->pin[0]);
        
		execvp(argv[0], (char **) argv);
        
        exit(0);
    } else { //parent
        printf("child %d\n", ret);
    	close(pe->pout[1]);
    	close(pe->pin[0]);
        
    	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    		printf("ignoring pipe signal failed\n");

    	_read_pipe(pe, pe->prompt);
    }
}


int pipe_exec(struct pipe_exec *pe, const char *cmd)
{
    if(!pe || !cmd) {
        return PIPE_EXEC_INVALIDE_PARMS;
    }
    _write_pipe(pe, cmd);
    _read_pipe(pe, pe->prompt);

    return PIPE_EXEC_OK;
}

