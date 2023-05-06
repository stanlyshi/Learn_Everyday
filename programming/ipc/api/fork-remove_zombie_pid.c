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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void child_handler(int signo)
{
    if(signo == SIGCHLD)
    {
        int status = 0;
        pid_t pid = waitpid(-1, &status, WNOHANG);

        if(WIFEXITED(status))
        {
            printf("child process pid: %d \n", pid);
            printf("child send: %d \n", WEXITSTATUS(status));
        }
    }
}

int main()
{
    struct sigaction act;
    act.sa_handler = child_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);

    pid_t pid = fork();

    if(pid == 0)
    {
        puts("I'm child process");

        sleep(20);

        puts("child process will exit");
        exit(3);
    }
    else
    {
        printf("fork() child pid: %d \n", pid);
        puts("wait...");
        sleep(500);
    }

    return 0;
}
