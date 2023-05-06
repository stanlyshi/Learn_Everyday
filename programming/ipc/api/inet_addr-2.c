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
#include<stdio.h>
#include<arpa/inet.h>

int main(){
    const char* addr1 = "192.168.8.254";
    const char* addr2 = "192.168.8.255";//

    unsigned int convAddr = inet_addr(addr1);
    if(convAddr == INADDR_NONE){
        puts("Error occured!");
    }
    else{
        printf("Network ordered integer addr: %#x \n", convAddr);
    }

    convAddr = inet_addr(addr2);
    
    //"192.168.8.256" == INADDR_NONE
    if(convAddr == INADDR_NONE){
        printf("Error occured! \n");
    }
    else
        printf("Network ordered integer addr: %#x \n", convAddr);

    return 0;
}