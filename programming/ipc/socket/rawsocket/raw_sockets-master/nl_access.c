#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>   //Provides declarations for icmp header
#include <netinet/udp.h>  
#include <arpa/inet.h> 

int total=0;
int sock_raw;
int tcp=0, icmp=0, igmp=0,others=0,udp=0;
int main()
{
	 int  data_size, saddr_size;
     unsigned char buffer[65536];
	 struct sockaddr saddr;
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if(sock_raw <0){
		perror("socket error !\n");
		exit(EXIT_FAILURE);
	}

    while(1){
    	     saddr_size = sizeof(saddr);
             data_size = recvfrom(sock_raw,buffer,sizeof(buffer),0,&saddr,&saddr_size);
	        if(data_size<0){
	        	perror("failed to get packets");
	        	exit(EXIT_FAILURE);
	        }
		    struct ip *ipH = (struct ip*)buffer;
           
            switch(ipH->ip_p)
            {
        	    case 1:
        	    ++icmp;
        	    printf("ICMP packet recieved\n"); break;
        	    case 2:
        	    ++igmp;
        	    printf("IGMP packet recieved\n"); break;
        	    case 6:
        	    ++tcp;
        	    printf("TCP packet recieved\n"); break;
        	    case 17:
        	    ++udp;
        	    printf("UDP packet recieved\n"); break;
        	    default: //some other protocols
        	    ++others;
        	    printf("other packet recieved\n");
        	    break;
            }
            ++total;
           printf("TCP: %d   ICMP: %d   IGMP: %d    UDP: %d\n",tcp,icmp,igmp,udp);
	    
    }
    close(sock_raw);
}