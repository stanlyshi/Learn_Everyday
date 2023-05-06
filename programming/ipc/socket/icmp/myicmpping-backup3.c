

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
#define ICMP_PKG_SIZE           86
#define ICMP_RECVBUF_SIZE       (50 * 1024)
#define ICMP_PKG_CHKSUM_OFFSET  offsetof(struct icmp, icmp_cksum)
#define ICMP_PKG_CHKSUM_SIZE    2
#define ICMP_MAGIC_ID           0xf1f2
#define ICMP_PROTO_NAME         "icmp"
#define ICMP_MAX_WAIT_TIME      1
#define ICMP_MAX_N_PKG          1

#define ICMP_LOG(fmt...)        printf(fmt)

#define IPFMT                   "%d.%d.%d.%d"
#define ipfmt(ip)               (ip) & 0xff, ((ip) >> 8) & 0xff, ((ip) >> 16) & 0xff, ((ip) >> 24) & 0xff
#define ICMP_START_FMT          "PING %s(%s): %d bytes data in ICMP packets."
#define ICMP_PING_FMT           "%d byte from %s: icmp_req=%u ttl=%d rtt=%.3f ms"
#define ICMP_END1_FMT           "\n--- ping statistics ---"
#define ICMP_END2_FMT           "%d packets transmitted, %d received, %d%% lost"

/* type of icmp ping */
typedef struct __icmp_ping_s {
    int sockfd;
    int timerfd;
    pthread_t pthreadID;
    pthread_mutex_t mutex;
    pthread_rwlock_t rwlock;
#define ICMP_PING_MUTEX_LOCK(ping)      pthread_mutex_lock(&ping->mutex)    
#define ICMP_PING_MUTEX_UNLOCK(ping)    pthread_mutex_unlock(&ping->mutex)
#define ICMP_PING_RWLOCK_RDLOCK(ping)   pthread_rwlock_rdlock(&ping->rwlock)
#define ICMP_PING_RWLOCK_WRLOCK(ping)   pthread_rwlock_wrlock(&ping->rwlock)
#define ICMP_PING_RWLOCK_UNLOCK(ping)   pthread_rwlock_unlock(&ping->rwlock)
    pthread_cond_t  cond;
    struct sockaddr_in dst_addr, from_addr;
    int ntx, nrx, ntry;   //number of send pkg and recv pkg
    int tx, rx;
    void *recv_buf, *send_buf;
    void*(*timeout_handler)(void*arg);
    int timeout_s;  //time out second
}icmp_ping_t;


/* functions */
icmp_ping_t* icmp_ping_init(const char *ipv4, int timeout_sec);
int icmp_ping_destroy(icmp_ping_t* ping);


int icmp_socket();
int icmp_dst_addr(const char *addrHost, struct sockaddr_in * dst_addr);
unsigned short icmp_gen_chksum(unsigned short * addr, int len);
int icmp_pkg_pack(void *buffer, int pack_no, const void *data, int data_size);
int icmp_send_pkg(icmp_ping_t *ping, const void *data, int size);
int icmp_recv_pkg(icmp_ping_t *ping, void *recvbuf, int size);
int icmp_pkg_unpack(struct sockaddr_in * addr, char * buf, int len);

static void __icmp_timeval_sub(struct timeval * out, struct timeval * in);


/* alogrithm of checksum */
unsigned short icmp_gen_chksum(unsigned short * addr, int len)
{
    int             nleft   = len;
    int             sum     = 0;
    unsigned short  *w      = addr;
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
        ICMP_LOG("getprotobyname error.\n\r");
        return -1;
    }
    if ((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
    {
        ICMP_LOG("socket error.\n\r");
        return -1;
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size))!=0)
    {
        ICMP_LOG("setsockopt SO_RCVBUF error.\n\r");
        close(sockfd);
        return -1;
    }
#if 1    
    struct timeval timeout = {1,0}; //sec
    if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval))!=0)
    {
        ICMP_LOG("setsockopt SO_RCVTIMEO error.\n\r");
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
            ICMP_LOG("gethostbyname error.\n\r");
            return (-1);
        }
        memcpy((char *) &dst_addr->sin_addr, host->h_addr, host->h_length);
    }
    else 
    {
        memcpy((char *) &dst_addr->sin_addr, (char *) &inaddr, sizeof(dst_addr->sin_addr));
    }

    ICMP_LOG(ICMP_START_FMT"\n\r", addrHost, inet_ntoa(dst_addr->sin_addr), ICMP_PKG_SIZE);
    
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
    
    memcpy(pkg_buffer + ICMP_PKG_CHKSUM_OFFSET, &checksum, ICMP_PKG_CHKSUM_SIZE);

    if ((n = sendto(ping->sockfd, pkg_buffer, packetsize, 0, 
                     (struct sockaddr *) &ping->dst_addr, sizeof(struct sockaddr_in)))< 0)
    {
        ICMP_LOG("sendto error. n = %d\n\r", n);
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
        ICMP_LOG("recvfrom error.n = %d\n\r", n);
        return 0;
    }

    ping->nrx++;
    
    return n;
}


/* unpack icmp pkg */
int icmp_pkg_unpack(struct sockaddr_in * addr, char * buf, int len)
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
        ICMP_LOG("ICMP packet\'s length is less than 8\n\r");
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
        ICMP_LOG(ICMP_PING_FMT"\n\r", len, inet_ntoa(addr->sin_addr), ntohs(icmp->icmp_seq), ip->ip_ttl, rtt);
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
static void * __icmpping_timeout_cb(void *arg)
{
    icmp_ping_t *_icmp_ping = (icmp_ping_t*)arg;
    unsigned int timeout_s = _icmp_ping->timeout_s;
    uint64_t exp = 0;
    
	struct itimerspec t1 = {{timeout_s,0},{timeout_s,0}}, t2;
    
    int ret = timerfd_settime(_icmp_ping->timerfd, 0, &t1, &t2);
    while(1)
	{
		int n = read(_icmp_ping->timerfd, &exp, sizeof(exp));	
		
		ICMP_LOG(ICMP_END1_FMT"\n\r");
        ICMP_LOG(ICMP_END2_FMT"\n\r", 
                _icmp_ping->ntx, _icmp_ping->nrx, 
                (_icmp_ping->ntx - _icmp_ping->nrx) / _icmp_ping->ntx * 100);
        
        break;
	}
    close(_icmp_ping->timerfd);
    close(_icmp_ping->sockfd);
    
    return NULL;
}

icmp_ping_t* icmp_ping_init(const char *ipv4, int timeout_sec)
{
    icmp_ping_t *icmp_ping = (icmp_ping_t *)malloc(sizeof(icmp_ping_t));
    if(!icmp_ping) return NULL;

    memset(icmp_ping, 0x00, sizeof(icmp_ping_t));
    
    icmp_ping->sockfd    = icmp_socket();
    icmp_ping->timeout_s = timeout_sec;
    icmp_ping->timerfd   = timerfd_create(CLOCK_REALTIME, TFD_CLOEXEC);
    icmp_ping->ntry      = ICMP_MAX_N_PKG;
    
    pthread_mutex_init(&icmp_ping->mutex, NULL);
    pthread_cond_init(&icmp_ping->cond, NULL);
    pthread_rwlock_init(&icmp_ping->rwlock, NULL);
    
    
pthread_create(&icmp_ping->pthreadID, NULL, __icmpping_timeout_cb, icmp_ping);
    
    icmp_dst_addr(ipv4, &icmp_ping->dst_addr);
    
    return icmp_ping;
}

int icmp_ping_destroy(icmp_ping_t* ping)
{
    if(ping)
    {
        pthread_mutex_destroy(&ping->mutex);
        pthread_cond_destroy(&ping->cond);
        pthread_rwlock_destroy(&ping->rwlock);
        free(ping);
    }
        
    ping = NULL;
}

/* PING */
int icmpping(const char *ipv4, unsigned int timeout)
{
    icmp_ping_t *icmp_ping = icmp_ping_init(ipv4, timeout);

    char pkg_buffer[ICMP_BUF_SIZE];
    char __data[] = {"Rong Tao's ICMP Ping"};

    while(icmp_ping->ntx < ICMP_MAX_N_PKG)
    {
        icmp_ping->tx = icmp_send_pkg(icmp_ping, __data, sizeof(__data));
        if(icmp_ping->tx < 8) 
        {
            break;
        }
        
        icmp_ping->rx = icmp_recv_pkg(icmp_ping, pkg_buffer, icmp_ping->tx);
        if(icmp_ping->rx<8) 
        {
            break;
        }
        if (icmp_pkg_unpack(&icmp_ping->from_addr, pkg_buffer, icmp_ping->rx) == -1)
            continue;
        //sleep(1);
    }
    
    icmp_ping_destroy(icmp_ping);
    return 0;
}


int main(int argc, char * argv[])
{

    if (argc < 2)
    {
        ICMP_LOG("Usage: %s hostname/IP address\n\r", argv[0]);
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
            icmpping(ip, 1);
        }
    }
#else
    icmpping(argv[1], 1);
#endif
    
    return 0;
}


