/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
$ ./a.out &
Child PID is 32360
[1] 32359
$ kill -STOP 32360
stopped by signal 19
$ kill -CONT 32360
continued
$ kill -TERM 32360
killed by signal 15
[1]+  Done                    ./a.out
$

*/

int main(int argc, char *argv[])
{
    pid_t cpid, w;
    int status;

    cpid = fork();
    if (cpid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) 
    {/* Code executed by child */
        printf("Child PID is %ld\n", (long) getpid());
        if (argc == 1)
            pause();/* Wait for signals */
        _exit(atoi(argv[1]));

    } 
    else 
    {/* Code executed by parent */
        do 
        {
            w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) 
            {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WIFEXITED(status)) 
            {
                printf("exited, status=%d\n", WEXITSTATUS(status));
            } 
            else if (WIFSIGNALED(status)) 
            {
                printf("killed by signal %d\n", WTERMSIG(status));
            } 
            else if (WIFSTOPPED(status)) 
            {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } 
            else if (WIFCONTINUED(status)) 
            {
                printf("continued\n");
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        exit(EXIT_SUCCESS);
    }
}
