/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:18 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:06 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include "common.h"

sem_t sem;
void* pthread_main(void* param)
{
    int str_len;
    char buf[BUF_SIZE];
    int clnt_sock=*((int*)param);
    while(1)
    {
        str_len=read(clnt_sock,buf,BUF_SIZE);
        if(str_len<=0)
            break;
        write(clnt_sock,buf,str_len);
    }
    sem_post(&sem);
    close(clnt_sock);
}

int main(int argc,char* argv[])
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    int clnt_addr_sz;
    pthread_t thread_id;

    if(argc!=2)
    {
        printf("Usage %s <port>\n",argv[0]);
        exit(1);
    }
    serv_sock=socket(AF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    listen(serv_sock,10);
    
    sem_init(&sem,0,5);
    while(1)
    {
        sem_wait(&sem);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
        pthread_create(&thread_id,NULL,(void*)pthread_main,&clnt_sock);
    }
    //pthread_join(thread_id,NULL);
    sem_destroy(&sem);
    return 0;
}
