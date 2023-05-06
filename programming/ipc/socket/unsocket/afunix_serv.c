/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:22 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:08 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/un.h>
#define BUF_SIZE 1024

void error_handling(const char* message);
int main()
{
    int serv_sock,clnt_sock;
    struct sockaddr_un serv_addr,clnt_addr;
    int  clnt_addr_sz,str_len;
    char buf[BUF_SIZE];
    char * path="sock";

    serv_sock=socket(AF_UNIX,SOCK_STREAM,0);
    

    serv_addr.sun_family=AF_UNIX;
    strncpy(serv_addr.sun_path,path,strlen(path));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,1)==-1)
        error_handling("listen() error");
    
    clnt_addr_sz=sizeof(clnt_addr);
    if((clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz))==-1)
        error_handling("accept() error");
    
    while(1)
    {
        str_len = read(clnt_sock,buf,BUF_SIZE);
        if(str_len<=0)
            break;
        write(clnt_sock,buf,str_len);
    }

    close(clnt_sock);
    close(serv_sock);   
    return 0;
}
void error_handling(const char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
