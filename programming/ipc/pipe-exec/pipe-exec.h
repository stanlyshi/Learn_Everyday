/**
 *  实现 PIPE 命令管道
 *  
 *  作者： 荣涛
 *  日期： 2020年10月13日
 */
#ifndef __PIPE_EXEC_H
#define __PIPE_EXEC_H 1

#define PIPE_EXEC_OK                0       /* 正确无误 */
#define PIPE_EXEC_INVALIDE_PARMS    (-1)    /* 参数错误 */
#define PIPE_EXEC_SYS_ERR           (-2)    /* 系统函数调用失败 */


/**
 *      master task                      slave task
 *          |                               |
 *          |                               |
 *          | pin[1]  __________    pin[0]  |
 *          |------->()_________)---------->|
 *          |                               |
 *          |                       +-------^-------+
 *          |                       | pin[0]->stdin |
 *          |                       | pin[0]->stdout|
 *          |                       |     execvp    |
 *          |                       |     exit(0)   |
 *          |                       +-------^-------+
 *          |                               |
 *          | pout[0] __________    pout[1] |
 *          |<-------()_________)<----------|
 *          |                               |
 *          |                               |
 *  
 */
struct pipe_exec {
    int pin[2];
    int pout[2];
    
    char prompt[32];
};

/**
 *  创建
 */
int pipe_exec_create(struct pipe_exec *pe, const char *prompt, int argc, char *argv[]);

/**
 *  执行
 */
int pipe_exec(struct pipe_exec *pe, const char *cmd);


#endif /*<__PIPE_EXEC_H>*/
