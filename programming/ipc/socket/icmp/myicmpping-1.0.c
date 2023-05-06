

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#ifndef offsetof
#define offsetof(type, member)    ( (int) & ((type*)0) -> member )
#endif

#define ICMP_BUF_SIZE           256
#define ICMP_RECVBUF_SIZE       (50 * 1024)
#define ICMP_MAGIC_ID           0xf1f2
#define ICMP_PROTO_NAME         "icmp"
#define ICMP_DATA               "Rong Tao's ICMP Ping"

#define ICMP_PING_SUCC          __ICMP_PING_SUCC
#define ICMP_PING_FAIL          __ICMP_PING_FAIL

#define ICMP_LOG(fmt...)        //printf(fmt)

#define ICMP_LOG_ERR(fmt...)    fprintf(stderr, fmt);

typedef enum {
    __ICMP_PING_FAIL = 0,
    __ICMP_PING_SUCC,
}icmp_ping_rlst_t;

/* type of icmp ping */
typedef struct __icmp_ping_s {
    int sockfd;
    struct sockaddr_in dst_addr, from_addr;
    int ntx, nrx, ntry;   //number of send pkg and recv pkg
    int tx, rx;
    int timeout_s;  //time out second
    int (*log)(const char *fmt, ...);
    int rslt; //__ICMP_PING_XXXX
}icmp_ping_t;


/* functions */
icmp_ping_t* icmp_ping_init(const char *ipv4, int timeout_sec, int ntry, int (*log)(const char *fmt, ...));
int icmp_ping(const char *ipv4, unsigned int timeout, int ntry, int (*log)(const char *fmt, ...));
void icmp_ping_finish (icmp_ping_t *ping);
int icmp_ping_destroy(icmp_ping_t* ping);


int icmp_socket();
int icmp_dst_addr(const char *addrHost, struct sockaddr_in * dst_addr);
unsigned short icmp_gen_chksum(unsigned short * data, int len);
int icmp_pkg_pack(void *buffer, int pack_no, const void *data, int data_size);
int icmp_send_pkg(icmp_ping_t *ping, const void *data, int size);
int icmp_recv_pkg(icmp_ping_t *ping, void *recvbuf, int size);
int icmp_pkg_unpack(icmp_ping_t *ping, char * buf, int len);

static void __icmp_timeval_sub(struct timeval * out, struct timeval * in);


/* alogrithm of checksum */
unsigned short icmp_gen_chksum(unsigned short * data, int len)
{
    int             nleft   = len;
    int             sum     = 0;
    unsigned short  *w      = data;
    unsigned short  answer  = 0;

    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1)
    {
        * (unsigned char *) (&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum     = (sum >> 16) + (sum & 0xffff);
    sum     += (sum >> 16);
    answer  = ~sum;
    
    return answer;
}


/* create a socke to icmp */
int icmp_socket()
{
    int sockfd;
    int size = ICMP_RECVBUF_SIZE;
    
    struct protoent * protocol  = NULL;
    
    if ((protocol = getprotobyname(ICMP_PROTO_NAME)) == NULL)
    {
        ICMP_LOG_ERR("getprotobyname error.\n\r");
        return -1;
    }
    if ((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
    {
        ICMP_LOG_ERR("socket error.\n\r");
        return -1;
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size))!=0)
    {
        ICMP_LOG_ERR("setsockopt SO_RCVBUF error.\n\r");
        close(sockfd);
        return -1;
    }
#if 1    
    struct timeval timeout = {1,0}; //sec
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval))!=0)
    {
        ICMP_LOG_ERR("setsockopt SO_RCVTIMEO error.\n\r");
        close(sockfd);
        return -1;
    }
#endif    
    return sockfd;
}

/* get dst address */
int icmp_dst_addr(const char *addrHost, struct sockaddr_in * dst_addr)
{
    struct hostent  * host      = NULL;
    unsigned long   inaddr      = 0;

    bzero(dst_addr, sizeof(struct sockaddr_in));
    dst_addr->sin_family = AF_INET;

    if ((inaddr = inet_addr(addrHost)) == INADDR_NONE)
    {
        if ((host = gethostbyname(addrHost)) == NULL)
        {
            ICMP_LOG_ERR("gethostbyname error.\n\r");
            return (-1);
        }
        memcpy((char *) &dst_addr->sin_addr, host->h_addr, host->h_length);
    }
    else 
    {
        memcpy((char *) &dst_addr->sin_addr, (char *) &inaddr, sizeof(dst_addr->sin_addr));
    }
    
    return 0;
}


/* set icmp hdr */
int icmp_pkg_pack(void *buffer, int pack_no, const void *data, int data_size)
{
    int i, packsize = 0;

    struct icmp * icmp  = malloc(sizeof(struct icmp));
    icmp->icmp_type     = ICMP_ECHO;
    icmp->icmp_code     = 0;
    icmp->icmp_cksum    = 0;
    icmp->icmp_seq      = htons(pack_no);
    icmp->icmp_id       = htons(ICMP_MAGIC_ID);

    gettimeofday((struct timeval *) &icmp->icmp_data, NULL);

    memcpy(buffer, icmp, sizeof(struct icmp));
    packsize += sizeof(struct icmp);

    if(data && data_size)
    {
        memcpy(buffer+packsize, data, data_size);
        packsize += data_size;
        
    }

    return packsize;
}



/* send icmp package */
int icmp_send_pkg(icmp_ping_t *ping, const void *data, int size)
{
    int             packetsize;
    unsigned short  checksum = 0;
    int             n = 0;
    
    char pkg_buffer[ICMP_BUF_SIZE];
    
    packetsize  = icmp_pkg_pack(pkg_buffer, ping->ntx, data, size);
    checksum    = icmp_gen_chksum((unsigned short *)pkg_buffer, packetsize);
    
#define ICMP_PKG_CHKSUM_OFFSET  offsetof(struct icmp, icmp_cksum)
#define ICMP_PKG_CHKSUM_SIZE    2

    memcpy(pkg_buffer + ICMP_PKG_CHKSUM_OFFSET, &checksum, ICMP_PKG_CHKSUM_SIZE);

    if ((n = sendto(ping->sockfd, pkg_buffer, packetsize, 0, 
                     (struct sockaddr *) &ping->dst_addr, sizeof(struct sockaddr_in)))< 0)
    {
        ICMP_LOG_ERR("sendto error. n = %d\n\r", n);
        return 0;
    }
    ping->ntx++;
                     
    return n;
}


/* send icmp package */
int icmp_recv_pkg(icmp_ping_t *ping, void *recvbuf, int size)
{
    int n, fromlen;
    
    fromlen = sizeof(struct sockaddr_in);

    if ((n = recvfrom(ping->sockfd, recvbuf, size, 0, 
                      (struct sockaddr *) &ping->from_addr, &fromlen)) < 0)
    {
        ICMP_LOG_ERR("recvfrom error.n = %d\n\r", n);
        return 0;
    }

    ping->nrx++;
    
    return n;
}


/* unpack icmp pkg */
int icmp_pkg_unpack(icmp_ping_t *ping, char * buf, int len)
{
    int     i, iphdrlen;

    struct  ip * ip = NULL;
    struct  icmp * icmp = NULL;
    struct  timeval * tvsend = NULL;
    double  rtt;

    ip          = (struct ip *)buf;
    iphdrlen    = ip->ip_hl << 2;
    icmp        = (struct icmp *) (buf + iphdrlen);
    len        -= iphdrlen;

    if (len < 8)
    {
        ICMP_LOG_ERR("ICMP packet\'s length is less than 8\n\r");
        return - 1;
    }

    /* ensure icmp reply is mine response msg */
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id = ntohs(ICMP_MAGIC_ID)))
    {
        tvsend  = (struct timeval *)icmp->icmp_data;

        struct timeval tvrecv;
        gettimeofday(&tvrecv, NULL);
        __icmp_timeval_sub(&tvrecv, tvsend);
        rtt = tvrecv.tv_sec * 1000 + tvrecv.tv_usec / 1000;
        if(ping->log)
            ping->log("%d byte from %s: icmp_req=%u ttl=%d rtt=%.3f ms\n\r", 
                        len, inet_ntoa(ping->from_addr.sin_addr), ntohs(icmp->icmp_seq), ip->ip_ttl, rtt);
    }
    else 
    {
        return - 1;
    }
    return 0;
}


static void __icmp_timeval_sub(struct timeval * out, struct timeval * in)
{
    if ((out->tv_usec -= in->tv_usec) < 0)
    {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}


/**********************************************************************************************************************/
/* icmp ping with timeout */
void icmp_ping_finish (icmp_ping_t *ping)
{
    if(ping->log)
    {
        ping->log("\n--- ping statistics ---\n\r");
        ping->log("%d packets transmitted, %d received, %d%% lost\n\r", 
                ping->ntx, ping->nrx, 
                (ping->ntx - ping->nrx) / ping->ntx * 100);
    }
    
    if(ping->ntx == ping->ntry)
        ping->rslt = __ICMP_PING_SUCC;
    
    return;
}

icmp_ping_t* icmp_ping_init(const char *ipv4, int timeout_sec, int ntry, int (*log)(const char *fmt, ...))
{
    icmp_ping_t *ping = (icmp_ping_t *)malloc(sizeof(icmp_ping_t));
    if(!ping) return NULL;

    memset(ping, 0x00, sizeof(icmp_ping_t));
    
    ping->sockfd    = icmp_socket();
    ping->timeout_s = timeout_sec;
    ping->ntry      = ntry;
    ping->log       = log;
    
    icmp_dst_addr(ipv4, &ping->dst_addr);
    
    if(ping->log)
        ping->log("PING %s(%s): %d bytes data in ICMP packets.\n\r", 
                       ipv4, inet_ntoa(ping->dst_addr.sin_addr), sizeof(ICMP_DATA));
    
    return ping;
}

int icmp_ping_destroy(icmp_ping_t* ping)
{
    if(ping)
    {
        close(ping->sockfd);
        free(ping);
    }
    ping = NULL;
}

/* PING */
int icmp_ping(const char *ipv4, unsigned int timeout, int ntry, int (*log)(const char *fmt, ...))
{
    icmp_ping_t *ping = icmp_ping_init(ipv4, timeout, ntry, log);

    char pkg_buffer[ICMP_BUF_SIZE];

    ping->rslt = __ICMP_PING_FAIL;

    while(ping->ntx < ping->ntry)
    {
        ping->tx = icmp_send_pkg(ping, ICMP_DATA, sizeof(ICMP_DATA));
        if(ping->tx < 8) 
        {
            ping->rslt = __ICMP_PING_FAIL;
            break;
        }
        
        ping->rx = icmp_recv_pkg(ping, pkg_buffer, ping->tx);
        if(ping->rx<8) 
        {
            ping->rslt = __ICMP_PING_FAIL;
            break;
        }
        if (icmp_pkg_unpack(ping, pkg_buffer, ping->rx) == -1)
            continue;
        //sleep(1);
    }
    
    icmp_ping_finish(ping);
    icmp_ping_destroy(ping);
    
    return ping->rslt;
}


int main(int argc, char * argv[])
{

    if (argc < 2)
    {
        ICMP_LOG_ERR("Usage: %s hostname/IP address\n\r", argv[0]);
        return (-1);
    }
#if 0
    int u1,u2,u3,u4;
	char ip[16] = {0};

	u1 = 10;
	u2 = 170;
	for(u3=6;u3<7;u3++)
	{
		for(u4=50;u4<90;u4++)
		{
			memset(ip, 0, 16);
			sprintf(ip, "%d.%d.%d.%d", u1, u2, u3, u4);
            int rslt = icmp_ping(ip, 1, 3);
            if(rslt == ICMP_PING_SUCC)
                printf("ping \033[1;32m%s\033[m is OK.\n", ip);
            else
                printf("ping \033[1;31m%s\033[m is not OK.\n", ip);
        }
    }
#else
    int rslt = icmp_ping(argv[1], 1, 3, NULL);
    if(rslt == ICMP_PING_SUCC)
        printf("ping \033[1;32m%s\033[m is OK.\n", argv[1]);
    else
        printf("ping \033[1;31m%s\033[m is not OK.\n", argv[1]);

#endif
    
    return 0;
}


