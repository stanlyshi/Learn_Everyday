/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <setjmp.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_SIZE		4096
#define MAX_WAIT_TIME	5
#define MAX_NO_PACKETS	3

#define debug() //printf("%s:%d\n", __func__, __LINE__)
#define IPFMT "%d.%d.%d.%d"
#define ipfmt(ip)\
	(ip) & 0xff, ((ip) >> 8) & 0xff, ((ip) >> 16) & 0xff, ((ip) >> 24) & 0xff
    
// struct icmphdr
// {
  // u_int8_t type;		/* message type */
  // u_int8_t code;		/* type sub-code */
  // u_int16_t checksum;
  // union
  // {
    // struct
    // {
      // u_int16_t	id;
      // u_int16_t	sequence;
    // } echo;			/* echo datagram */
    // u_int32_t	gateway;	/* gateway address */
    // struct
    // {
      // u_int16_t	__unused;
      // u_int16_t	mtu;
    // } frag;			/* path mtu discovery */
  // } un;
// };
    
char sendpacket[PACKET_SIZE];
char recvpacket[PACKET_SIZE];

int sockfd, datalen = 56;
int nsend = 0, nreceived = 0;
struct sockaddr_in dest_addr;
pid_t pid;
struct sockaddr_in from;
struct timeval tvrecv;

/**
 *	functions
 */
void statistics(int signo);
unsigned short cal_chksum(unsigned short *addr, int len);
int pack(int pack_no);
void send_packet(void);
void recv_packet(void);
int unpack(char *buf, int len);
void tv_sub(struct timeval *out, struct timeval *in);

/**
 *	signal callback
 */
void statistics(int signo)
{
	printf("\n-----------------PING statistics------------\n");
	printf("%d packets transmitted, %d received, %d%% lost\n", 
			nsend, nreceived, (nsend-nreceived)/nsend*100);
	close(sockfd);
	exit(1);
}

/**
 *	校验和算法
 */
unsigned short cal_chksum(unsigned short *addr, int len)
{
	int nleft = len;
	int sum = 0;
	unsigned short *w = addr;
	unsigned short answer = 0;

	/**
	 *	把icmp报头二进制数据以2字节为单位累加
	 */
	while(nleft>1)
	{
		sum += *w++;
		nleft -= 2;
	}
	if(nleft == 1)
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w;
		sum += answer;
	}
	sum = (sum>>16)+(sum&0xffff);
	sum += (sum>>16);
	answer = ~sum;
	return answer;
}

/**
 *	设置icmp报头
 */
int pack(int pack_no)
{
	int i, packsize;
	struct icmp *icmp = malloc(sizeof(struct icmp));
	struct timeval *tval;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_cksum = 0;
	icmp->icmp_seq = pack_no;
	icmp->icmp_id = pid;
	packsize = 8+datalen;
	tval = (struct timeval *)icmp->icmp_data;
	gettimeofday(tval, NULL);
	icmp->icmp_cksum = cal_chksum((unsigned short *)icmp, packsize);
	return packsize;
}

/**
 *发送三个icmp报文
 */
void send_packet()
{
	int packetsize;
	while(nsend < MAX_NO_PACKETS)
	{
		nsend++;
		packetsize = pack(nsend);
        printf("packetsize = %d, sockfd = %d\n", packetsize, sockfd);
		if(sendto(sockfd, sendpacket, packetsize, 0,
					(struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
		{
			perror("sendto error.");
			continue;
		}
		sleep(1);
	}
}

/**
 *	接收icmp报文
 */
void recv_packet()
{
	int n, fromlen;
	extern int errno;
	signal(SIGALRM, statistics);
	fromlen = sizeof(from);
	while(nreceived < nsend)
	{
		alarm(MAX_WAIT_TIME);
		if((n=recvfrom(sockfd, recvpacket, sizeof(recvpacket), 0, 
						(struct sockaddr *)&from, &fromlen)) < 0)
		{
			if(errno == EINTR) 
				continue;
			perror("recvfrom error.");
			continue;
		}
		gettimeofday(&tvrecv, NULL);
		if(unpack(recvpacket, n) == -1)
			continue;
		nreceived++;
	}
}

/**
 *	剥去icmp报头
 */
int unpack(char *buf, int len)
{
	int i, iphdrlen;
    
	struct ip *ip = NULL;
	struct icmp *icmp = NULL;
	struct timeval *tvsend = NULL;

	double rtt;

	ip = (struct ip *)buf;
	iphdrlen = ip->ip_hl << 2;
	icmp = (struct icmp*)(buf + iphdrlen);
	len -= iphdrlen;
    
	if(len < 8)
	{
		printf("ICMP packet\'s lengthis less than 8\n");
		return -1;
	}
	/*确保所接收的是我所发的ICMP回应*/
	if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id = pid))
	{
		tvsend = (struct timeval *)icmp->icmp_data;
		tv_sub(&tvrecv, tvsend);
		rtt = tvrecv.tv_sec*1000+tvrecv.tv_usec/1000;
		printf("%d byte from %s: icmp_req=%u ttl=%d rtt=%.3f ms\n",
				len,
				inet_ntoa(from.sin_addr),
				icmp->icmp_seq,
				ip->ip_ttl,
				rtt);
	}
	else
	{
		return -1;
	}
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if((out->tv_usec -= in->tv_usec) < 0)
	{
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}

int main(int argc, char *argv[])
{
	struct hostent *host = NULL;
	struct protoent *protocol = NULL;
	unsigned long inaddr = 0;
	int waittine = MAX_WAIT_TIME;
	int size = 50*1024;
    debug();
	if(argc < 2)
	{
		printf("Usage: %s hostname/IP address\n", argv[0]);
		exit(1);
	}
	if((protocol = getprotobyname("icmp")) == NULL)
	{
		perror("getprotobyname error.");
		exit(1);
	}
    debug();
	if((sockfd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
	{
		perror("socket error.");
		exit(1);
	}
    debug();
	setuid(getuid());
    debug();
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    perror("setsockopt.");
    
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
    debug();
	if((inaddr = inet_addr(argv[1])) == INADDR_NONE)
	{
        debug();
		if((host=gethostbyname(argv[1])) == NULL)
		{
			perror("gethostbyname error.");
			exit(1);
		}
		memcpy((char*)&dest_addr.sin_addr, host->h_addr, host->h_length);
	}
	else
	{
        printf("inaddr = %ld, INADDR_NONE = %ld\n", inaddr, INADDR_NONE);
        perror("inet_addr.");
        debug();
        printf(IPFMT"\n", ipfmt(inaddr));
        printf("%x\n", dest_addr);
		memcpy((char*)&dest_addr.sin_addr, (char*)&inaddr, sizeof(dest_addr.sin_addr));
	}
    debug();
	pid = getpid();
	printf("PING %s(%s): %d bytes data in ICMP packets.\n", 
			argv[1], inet_ntoa(dest_addr.sin_addr), datalen);
    perror("inet_ntoa.");
    debug();
	send_packet();
    debug();
	recv_packet();
    debug();
	statistics(SIGALRM);

	return 0;
}



















