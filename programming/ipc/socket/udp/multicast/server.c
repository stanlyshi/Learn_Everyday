/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年06月03日 星期天 12时56分19秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>

#define SERVER_PORT 8000
#define CLIENT_PORT 9000
#define MAXLINE 1500

//分类
//组播组可以是永久的也可以是临时的。组播组地址中，有一部分由官方分配的，称为永久组播组。
//永久组播组保持不变的是它的ip地址，组中的成员构成可以发生变化。永久组播组中成员的数量
//都可以是任意的，甚至可以为零。那些没有保留下来供永久组播组使用的ip组播地址，可以被临时组播组利用。
//
//224.0.0.0～224.0.0.255为预留的组播地址（永久组地址），地址224.0.0.
//0保留不做分配，其它地址供路由协议使用；
//
//224.0.1.0～224.0.1.255是公用组播地址，可以用于Internet；
//224.0.2.0～238.255.255.255为用户可用的组播地址（临时组地址），全网范围内有效；
//239.0.0.0～239.255.255.255为本地管理组播地址，仅在特定的本地范围内有效。
//组播地址列表
//列表如下：
//224.0.0.0 基准地址（保留）
//224.0.0.1 所有主机的地址 （包括所有路由器地址）
//224.0.0.2 所有组播路由器的地址
//224.0.0.3 不分配
//224.0.0.4 dvmrp路由器
//224.0.0.5 所有ospf路由器
//224.0.0.6 ospf DR/BDR
//224.0.0.7 st路由器
//224.0.0.8 st主机
//224.0.0.9 rip-2路由器
//224.0.0.10 Eigrp路由器
//224.0.0.11 活动代理
//224.0.0.12 dhcp 服务器/中继代理
//224.0.0.13 所有pim路由器
//224.0.0.14 rsvp封装
//224.0.0.15 所有cbt路由器
//224.0.0.16 指定sbm
//224.0.0.17 所有sbms
//224.0.0.18 vrrp
//224.0.0.22 IGMPv3
//以太网传输单播ip报文的时候，目的mac地址使用的是接收者的mac地址。但是在传输组播报文
//时，传输目的不再是一个具体的接收者，而是一个成员不确定的组，所以使用的是组播mac地址。
//组播mac地址是和组播ip地址对应的。iana（internet assigned number 
//authority）规定，组播mac地址的高24bit为0x01005e，mac 地址的低23bit为组播ip地址的低23bit。
//
//由于ip组播地址的后28位中只有23位被映射到mac地址，这样就会有32个ip组播地址映射到同一m
//ac地址上。


#define GROUP "239.0.0.2"

int main(void)
{
    int sockfd;
    struct sockaddr_in serveraddr, clientaddr;
    char buf[MAXLINE] = "itcast\n";
    struct ip_mreqn group;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);                /* 构造用于UDP通信的套接字 */
    
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                        /* IPv4 */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);         /* 本地任意IP INADDR_ANY = 0 */
    serveraddr.sin_port = htons(SERVER_PORT);

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);        /* 设置组地址 */
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);      /* 本地任意IP */
    group.imr_ifindex = if_nametoindex("eth0");             /* 给出网卡名,转换为对应编号: eth0 --> 编号  命令:ip ad */

    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));  /* 组播权限 */

    bzero(&clientaddr, sizeof(clientaddr));                 /* 构造 client 地址 IP+端口 */
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, GROUP, &clientaddr.sin_addr.s_addr); /* IPv4  239.0.0.2+9000 */
    clientaddr.sin_port = htons(CLIENT_PORT);

    int i = 0;
    while (1) {
        sprintf(buf, "itcast %d\n", i++);
        //fgets(buf, sizeof(buf), stdin);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        sleep(1);
    }

    close(sockfd);

    return 0;
}

