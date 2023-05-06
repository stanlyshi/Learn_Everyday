#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h> 
#include <stdlib.h> 
#include <netpacket/packet.h>
#include <net/ethernet.h> //dll protocols

#define PROTOCOL    0x16ad

unsigned long long int total =0; 

int main()
{
    int sockfd,  len, ret, recvlen;
    char buffer[2048];
    const int on = 1;
    
    struct sockaddr_ll pla;
    sockfd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));//ETH_P_ALL
    if(sockfd<0)
    {
        perror("packet_socket");
        //exit(0);
    }
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
		perror("setsockopt error");
	}
    printf("Types of the captured packets are...\n");
    
    struct sockaddr_ll dstaddr;
    dstaddr.sll_family = htons(AF_PACKET);
    dstaddr.sll_ifindex = 1;
    dstaddr.sll_protocol = htons(PROTOCOL);
    
    #if 1
    //a4:bf:01:04:1d:ea
    //a4:bf:01:04:1d:eb
    dstaddr.sll_addr[0] = 0xa4;
    dstaddr.sll_addr[1] = 0xbf;
    dstaddr.sll_addr[2] = 0x01;
    dstaddr.sll_addr[3] = 0x04;
    dstaddr.sll_addr[4] = 0x1d;
    dstaddr.sll_addr[5] = 0xea;
    #else
    //a4:bf:01:21:ab:23
    //28:6e:d4:88:c7:9a
    dstaddr.sll_addr[0] = 0x28;
    dstaddr.sll_addr[1] = 0x6e;
    dstaddr.sll_addr[2] = 0xd4;
    dstaddr.sll_addr[3] = 0x88;
    dstaddr.sll_addr[4] = 0xc7;
    dstaddr.sll_addr[5] = 0x9a;    
    #endif
	dstaddr.sll_halen = htons(ETH_ALEN);
    
    printf("htons (ETH_ALEN) = %d\n", htons (ETH_ALEN));
    //return 0;
    
	while(1)
	{
        sleep(2);
		len = sizeof(struct sockaddr_ll);
		recvlen = recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)&pla,&len);
        ret = sendto(sockfd, buffer, recvlen, 0, (struct sockaddr *) &dstaddr, sizeof (dstaddr));
                
        printf("Ind(%d) %02x:%02x:%02x:%02x:%02x:%02x(%4d,%04x) > %02x:%02x:%02x:%02x:%02x:%02x(%4d,%04x), ", 
                pla.sll_ifindex,
                pla.sll_addr[0],pla.sll_addr[1],pla.sll_addr[2],
                pla.sll_addr[3],pla.sll_addr[4],pla.sll_addr[5],
                recvlen, pla.sll_protocol,
                dstaddr.sll_addr[0],dstaddr.sll_addr[1],dstaddr.sll_addr[2],
                dstaddr.sll_addr[3],dstaddr.sll_addr[4],dstaddr.sll_addr[5],
                ret, dstaddr.sll_protocol);
                
                
		switch(pla.sll_pkttype)
		{
				// these constant values are taken from linux/if_packet.h
			case 0://PACKET_HOST
			printf("%s \t","Host"); 
			break;
			case 1://PACKET_BROADCAST
			printf("%s \t","Broadcast"); 
			break;
			case 2://PACKET_MULTICAST
			printf("%s \t","Multicast"); 
			break;
			case 3://PACKET_OTHERHOST
			printf("%s \t","Oth_Host"); 
			break;
			case 4://PACKET_OUTGOING
			printf("%s \t","Outgoing");
			break;
			case 5://PACKET_LOOPBACK
			printf("%s \t","Loop_Back");
			break;
			case 6://PACKET_FASTROUTE
			printf("%s \t","Fast_Route");
			break;
		}
        printf("\n");
		//++total;
		//printf("total packets:%llu \n",total);
	}
}

