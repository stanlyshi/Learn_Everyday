/**
 *  PIPE 命令管道 测试
 *  
 *  作者： 荣涛
 *  日期： 2020年10月13日
 */

#include <stdio.h>
#include "pipe-exec.h"


int main()
{

    struct pipe_exec pe2;

    char *argv2[] = {
        "./mycmd.sh", NULL
    };
    pipe_exec_create(&pe2, "pipe-exec>", 1, argv2);

    char buffer[512];
    
    while(1) {
        printf("INPUT CMD>>");
        scanf("%s", buffer);
        printf("buffer <%s>\n", buffer);
        pipe_exec(&pe2, buffer);
    }
    
}

