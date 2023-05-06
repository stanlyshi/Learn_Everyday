/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:20 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:07 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:47 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string>
#include<string.h>
#include<iostream>
using namespace std;

class InetAddress{
public:
    InetAddress(std::string port, const std::string ip = ""){
        memset(&addr_, 0, sizeof(addr_));
        addr_.sin_family = AF_INET;
        addr_.sin_port   = htons(atoi(port.c_str()));

        if(ip == "")
            addr_.sin_addr.s_addr = htonl(INADDR_ANY);
        else
            addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~InetAddress(){}

    struct sockaddr_in addr_;
};

class Socket{
public:
    Socket(int type){
        fd_ = socket(PF_INET, type, 0);
    }
    ~Socket(){
        close(fd_);
    }

    ssize_t sendto(InetAddress& addr, void* pData, size_t nBytes){
        sockaddr* toAddr = (sockaddr*)&(addr.addr_);
        socklen_t addrLen = sizeof(addr.addr_); 

        return ::sendto(fd_, pData, nBytes, 0, toAddr, addrLen);
    }

    ssize_t recvfrom(InetAddress& addr, void* buf, size_t nBytes){
        return ::recvfrom(fd_, buf, nBytes, 0, 0, 0);
    }

private:
    int fd_;
};

template <class type>
type func(type t0)
{
    type t;
    
    return t;
}

void errorHandler(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Usage: %s <ip> <port> \n", argv[0]);
        exit(1);
    }

    Socket sock(SOCK_DGRAM);

    InetAddress addr(argv[2], argv[1]);

    char buf[256];
    while(1){
        fgets(buf, sizeof(buf), stdin);
        int sendLen = sock.sendto(addr, buf, sizeof(buf));
        int recvLen = sock.recvfrom(addr, buf, sizeof(buf)-1);

        buf[recvLen] = 0;
        printf("Message from server: %s", buf);
    }

    return 0;
}