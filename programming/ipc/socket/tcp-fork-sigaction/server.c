/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:17 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/wait.h>

#define BUFFSIZE 64

void errorHandling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void removeChild(int signo)
{
    int status = 0;

    pid_t pid = waitpid(-1, &status, WNOHANG);
    
    printf("remove child process: pid = %d \n", pid);
}

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    struct sigaction act;
    act.sa_handler = removeChild;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGCHLD, &act, 0);

    int sockServ, sockClnt;

    struct sockaddr_in addrServ, addrClnt;

    memset(&addrServ, 0, sizeof(addrServ));
    addrServ.sin_family         = AF_INET;
    addrServ.sin_addr.s_addr    = htonl(INADDR_ANY);
    addrServ.sin_port           = htons(atoi(argv[1]));         

    sockServ = socket(AF_INET, SOCK_STREAM, 0);

    if(0 != bind(sockServ, (struct sockaddr*)&addrServ, sizeof(addrServ)))
    {
        errorHandling("bind() error!");
    }

    if(0 != listen(sockServ, 5))
    {
        errorHandling("listen() error!");
    }

    socklen_t addrClntLen = sizeof(addrClnt);
    
    while(1)
    {
        sockClnt = accept(sockServ, (struct sockaddr*)&addrClnt, &addrClntLen);
        
        if(sockClnt == -1)
        {
            continue;
        }
        else{
            puts("new client connected...");
        }

        pid_t pid = fork();

        if(pid == -1)
        {
            close(sockClnt);
            continue;
        }

        if(pid > 0)
        {
            close(sockClnt);
        }
        else
        {
            close(sockServ);
            
            char buf[BUFFSIZE];
            int receivedByte = 0;
            while((receivedByte = read(sockClnt, buf, sizeof(buf)))>0)
            {
                printf("Server recv: %s\n", buf);
                write(sockClnt, buf, receivedByte);
            }

            close(sockClnt);

            puts("client disconnected...");

            return 0;
        }
    }

    close(sockServ);
    return 0;
}