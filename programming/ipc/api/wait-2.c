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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if(pid == 0){
        exit(3);
    }
    else{
        printf("child pid: %d \n", pid);

        pid_t pid = fork();

        if (pid == 0){
            exit(7);
        }
        else{
            printf("child pid: %d \n", pid);

            int status = 0;
            wait(&status);
            //waitpid(-1, &status, 0); //same as "wait(&status);"

            if(WIFEXITED(status)){
                printf("child send1: %d \n", WEXITSTATUS(status));
            }

            wait(&status);

            if(WIFEXITED(status)){
                printf("child send2: %d \n", WEXITSTATUS(status));
            }

            sleep(3);
        }
    }

    return 0;
}   