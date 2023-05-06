// C program to Implement Ping

// compile as -o ping
// run as sudo ./ping <hostname>

//ICMP(internet control management protocol) 是四层的协议。 
//根据 Understanding LINUX NETWORK INTERNALS Chapter 25 中的介绍，ICMP 的主要作用是交换控制信息：
//
//The Internet Control Message Protocol (ICMP) is a transport protocol used by Internet hosts to 
//exchange control messages, notably error notifications and information requests.
//
//Linux 内核的协议栈中包含了 ICMP， 不过这个协议比较有意思， 它的实现是一半在内核态一半在用户态。 
//我们经常使用 ping 指令来测试某个节点是否在线， ping 指令用的便是 ICMP 协议， 它向目标机器发送 
//ICMP echo request 报文，并等待目标机器发回的 ICMP echo response 报文， 这些操作都是在用户态下完成的。 
//目标节点接收到 ICMP echo request 报文后会自动发送 ICMP echo response 报文， 基本上没人听说过 
//ping server 应用，因为接收报文和回应报文的操作是内核中的 ICMP 协议自动完成的。
//
//ping 指令先构造好 ICMP 请求报文，然后利用 raw socket 发送封装好的 ICMP 报文。 如果同时运行两个 ping 程序， 
//内核如何将收到的 ICMP 报文传到正确的进程上？ 目前主要是通过 ICMP 报文中的 identifier 字段， 
//该字段的作用与 TCP 协议中的端口字段类似：
//
//How can the two ping instances distinguish the replies so that they are not confused by the traffic 
//meant for the other instance? The L4 protocol must include the information needed to distinguish the 
//applications in its header or payload. For example, the ICMPECHO REQUEST messages sent by the ping 
//command get their ICMP header’s identifier field initialized to the sender’s process ID (pid). This 
//field is what will allow the ping application to recognize the input ECHO REPLY ICMP messages that 
//will be sent back by the recipient(from Understanding LINUX NETWORK INTERNALS Chapter 24).


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

// Define the Packet Constants
// ping packet size
#define PING_PKT_S 64

// Automatic port number
#define PORT_NO 0

// Automatic port number
// #define PING_SLEEP_RATE 100000
#define PING_SLEEP_RATE 10

// Gives the timeout delay for receiving packets
// in seconds
#define RECV_TIMEOUT 1

// Define the Ping Loop
int pingloop=1;


// ping packet structure
struct ping_pkt
{
    struct icmphdr hdr;
    char msg[PING_PKT_S-sizeof(struct icmphdr)];
};

// Calculating the Check Sum
unsigned short checksum(void *b, int len)
{ unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;

    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}


// Interrupt handler
void intHandler(int dummy)
{
    pingloop=0;
}

// Performs a DNS lookup
char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
    printf("\nResolving DNS..\n");
    struct hostent *host_entity;
    char *ip=(char*)malloc(NI_MAXHOST*sizeof(char));
    int i;

    if ((host_entity = gethostbyname(addr_host)) == NULL)
    {
        // No ip found for hostname
        return NULL;
    }

    //filling up address structure
    strcpy(ip, inet_ntoa(*(struct in_addr *)
                        host_entity->h_addr));

    (*addr_con).sin_family = host_entity->h_addrtype;
    (*addr_con).sin_port = htons (PORT_NO);
    (*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;

    return ip;

}

// Resolves the reverse lookup of the hostname
char* reverse_dns_lookup(char *ip_addr)
{
    struct sockaddr_in temp_addr;
    socklen_t len;
    char buf[NI_MAXHOST], *ret_buf;

    temp_addr.sin_family = AF_INET;
    temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *) &temp_addr, len, buf,
                    sizeof(buf), NULL, 0, NI_NAMEREQD))
    {
        printf("Could not resolve reverse lookup of hostname\n");
        return NULL;
    }
    ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) );
    strcpy(ret_buf, buf);
    return ret_buf;
}

// make a ping request
void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr,
                char *ping_dom, char *ping_ip, char *rev_host)
{
    int ttl_val=64, msg_count=0, i, addr_len, flag=1,
            msg_received_count=0;

    struct ping_pkt pckt;
    struct sockaddr_in r_addr;
    struct timespec time_start, time_end, tfs, tfe;
    long double rtt_msec=0, total_msec=0;
    struct timeval tv_out;
    tv_out.tv_sec = RECV_TIMEOUT;
    tv_out.tv_usec = 0;

    clock_gettime(CLOCK_MONOTONIC, &tfs);


    // set socket options at ip to TTL and value to 64,
    // change to what you want by setting ttl_val
    if (setsockopt(ping_sockfd, SOL_IP, IP_TTL,
            &ttl_val, sizeof(ttl_val)) != 0)
    {
        printf("\nSetting socket options to TTL failed!\n");
        return;
    }

    else
    {
        printf("\nSocket set to TTL..\n");
    }

    // setting timeout of recv setting
    setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO,
                (const char*)&tv_out, sizeof tv_out);

    // send icmp packet in an infinite loop
    while(pingloop)
    {
        // flag is whether packet was sent or not
        flag=1;

        //filling packet
        bzero(&pckt, sizeof(pckt));

        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = getpid();

        for ( i = 0; i < sizeof(pckt.msg)-1; i++ )
            pckt.msg[i] = i+'0';

        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = msg_count++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));


        usleep(PING_SLEEP_RATE);

        //send packet
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        if ( sendto(ping_sockfd, &pckt, sizeof(pckt), 0,
        (struct sockaddr*) ping_addr,
            sizeof(*ping_addr)) <= 0)
        {
            printf("\nPacket Sending Failed!\n");
            flag=0;
        }

        //receive packet
        addr_len=sizeof(r_addr);

        if ( recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0,
            (struct sockaddr*)&r_addr, &addr_len) <= 0
            && msg_count>1)
        {
            printf("\nPacket receive failed!\n");
        }

        else
        {
            clock_gettime(CLOCK_MONOTONIC, &time_end);

            double timeElapsed = ((double)(time_end.tv_nsec -
                                time_start.tv_nsec))/1000000.0;
            rtt_msec = (time_end.tv_sec-
                        time_start.tv_sec) * 1000.0
                        + timeElapsed;

            // if packet was not sent, don't receive
            if(flag)
            {
                if(!(pckt.hdr.type ==69 && pckt.hdr.code==0))
                {
                    printf("Error..Packet received with ICMP type %d code %d\n",
                        pckt.hdr.type, pckt.hdr.code);
                }
                else
                {
                    printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt=%Lf ms.\n",
                        PING_PKT_S, ping_dom, rev_host,
                        ping_ip, msg_count,
                        ttl_val, rtt_msec);

                    msg_received_count++;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &tfe);
    double timeElapsed = ((double)(tfe.tv_nsec -
                        tfs.tv_nsec))/1000000.0;

    total_msec = (tfe.tv_sec-tfs.tv_sec)*1000.0+
                        timeElapsed;

    printf("\n===%s ping statistics===\n", ping_ip);
    printf("\n%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n",
        msg_count, msg_received_count,
        ((msg_count - msg_received_count)/msg_count) * 100.0,
        total_msec);
}

// Driver Code
int main(int argc, char *argv[])
{
    int sockfd;
    char *ip_addr, *reverse_hostname;
    struct sockaddr_in addr_con;
    int addrlen = sizeof(addr_con);
    char net_buf[NI_MAXHOST];

    if(argc!=2)
    {
        printf("\nFormat %s <address>\n", argv[0]);
        return 0;
    }

    ip_addr = dns_lookup(argv[1], &addr_con);
    if(ip_addr==NULL)
    {
        printf("\nDNS lookup failed! Could not resolve hostname!\n");
        return 0;
    }

    reverse_hostname = reverse_dns_lookup(ip_addr);
    printf("\nTrying to connect to '%s' IP: %s\n",
                                    argv[1], ip_addr);
    printf("\nReverse Lookup domain: %s",
                        reverse_hostname);

    //socket()
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sockfd<0)
    {
        printf("\nSocket file descriptor not received!!\n");
        return 0;
    }
    else
        printf("\nSocket file descriptor %d received\n", sockfd);

    signal(SIGINT, intHandler);//catching interrupt

    //send pings continuously
    send_ping(sockfd, &addr_con, reverse_hostname,
                                ip_addr, argv[1]);

    return 0;
}

