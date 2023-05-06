/* Copyright (C) Rong Tao @Sylincom Beijing, 2019å¹´ 06æœˆ 14æ—¥ æ˜ŸæœŸäº” 09:06:33 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled â€˜â€˜GNU
 Free Documentation Licenseâ€™â€™.
   2019å¹´ 03æœˆ 14æ—¥ æ˜ŸæœŸå›› 19:24:46 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019å¹´ 03æœˆ 08æ—¥ æ˜ŸæœŸäº” 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019å¹´ 03æœˆ 07æ—¥ æ˜ŸæœŸå›› 20:28:04 CST. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ether.h> /*raw socket head file*/
#include <sys/un.h>  /*unsocket head file*/
#include <net/if.h> /*struct ifreq , define SIOCGIFINDEX*/
#include <sys/ioctl.h> /*ioctl SIOCGIFADDR*/
#include <netpacket/packet.h> /*struct sockaddr_ll*/
#include <sys/types.h>
#include <unistd.h>  /*socket close will use  */
#include <netinet/in.h>  /*htons/htonl*/
#include <arpa/inet.h>  /*inet_ntoa*/
//#include <sbs_msgq.h>
#include "sbs_type.h"
#include "inet_mgmt.h"
//#include "sbs_task.h"
#include "sbs_log.h"
#include "om_common_h.h"

TcpInfo tcp_info[MAX_SOCK_NUM];
SockPara sock_para[MAX_SOCK_NUM];/*store udp,unsocket,rawsocket info*/
extern fd_set readset;/*store all socket fd*/
extern INT32 maxfd;/*maxfd is current max fd value*/

ListenFdInfo listen_fd_info[MAX_SOCK_NUM];/*store listen fd info*/

//#define RAW_SOCKET_IF_NAME "ens33"


/*
·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
Èë²Î£º
	Msgq£ºµ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
	p:µ÷ÓÃÕß»Øµ÷º¯Êý
	flag:1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0Î»ÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
	Port£ºÐèÒª¼àÌýµÄ¶Ë¿ÚºÅ
	Path£ºÂ·¾¶Ãû
	Protocol£ºÐ­ÒéÀàÐÍ
*/


/*
·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
Èë²Î£º
	Msgq£ºµ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
	p:µ÷ÓÃÕß»Øµ÷º¯Êý
	callback_flag:1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
	Port£ºÐèÒª¼àÌýµÄ¶Ë¿ÚºÅ
*/


INT32 socket_tcp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port)
{
	INT32 option=1;
	INT32 i;
	struct sockaddr_in serveraddr;
	INT32 listen_fd;
	struct timeval timeout={2,0};//timeout

	if (callback_flag)
	{
		if (!p)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	
	listen_fd=socket(AF_INET,SOCK_STREAM,0);
	if (listen_fd<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"create tcp fail\n");
	}

	//option = (SO_KEEPALIVE>SO_REUSEADDR)? SO_KEEPALIVE:SO_REUSEADDR;
	//setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR|SO_KEEPALIVE, &option, sizeof(option));
	setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &timeout, sizeof(timeout));
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &timeout, sizeof(timeout));
	setsockopt(listen_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));/*set receive message timeout */
	setsockopt(listen_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));/*set send message timeout*/

	memset(&serveraddr,0,sizeof(struct sockaddr_in));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons(port);
	if (bind(listen_fd,(struct sockaddr*)(&serveraddr),sizeof(serveraddr))<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"bind tcp port fail\n");
		close(listen_fd);
		return -1;

	}

	if (listen(listen_fd,LISTEN_LEN)<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"call listen fail\n");
		close(listen_fd);
		return -1;
	}

	/*add listen fd to listen_fd_info*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(listen_fd_info[i].listenfd==0)
		{
			listen_fd_info[i].socket_type = TCP;
			listen_fd_info[i].listenfd=listen_fd;
			if (callback_flag)
			{
				listen_fd_info[i].pFun = p;
			}
			else
			{
				listen_fd_info[i].msgq = msgq;
			}
			break;
		}
	}
	if (MAX_SOCK_NUM == i)
	{
		close(listen_fd);
		return -1;
	}

	FD_SET(listen_fd,&readset);
	/*update max fd value*/
	if (listen_fd>maxfd)
	{
		maxfd=listen_fd;
	}
	return listen_fd;
}


/*
·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
Èë²Î£º
	Msgq£ºµ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
	p:µ÷ÓÃÕß»Øµ÷º¯Êý
	callback_flag:1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
	Port£ºÐèÒª¼àÌýµÄ¶Ë¿ÚºÅ

*/

INT32 socket_udp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port)
{
	INT32 sock_fd;
	UINT32 i;

	//INT32 value; //add for degbug

	INT32 option = 1;
	struct sockaddr_in serveraddr;

	if (callback_flag)
	{
		if (!p)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
	}

	sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	if (sock_fd<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"create udp socket fail\n");
		close(sock_fd);
		return -1;
	}
    /*socket option */
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option));
    setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &option, sizeof(option));
	//setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR|SO_BROADCAST|SO_KEEPALIVE, &option, sizeof(option));
	/*----------debug output------------*/
	//log_msg(LOG_INFO,INET_MGMT,"\n");

	memset(&serveraddr,0,sizeof(struct sockaddr_in));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons(port);
	if (bind(sock_fd,(struct sockaddr*)(&serveraddr),sizeof(serveraddr))<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"bing udp socket port fail\n");
		close(sock_fd);
		return -1;
	}
	
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(sock_para[i].sock_fd==0)
		{
			sock_para[i].sock_fd=sock_fd;
			sock_para[i].socket_type=UDP;
			if (callback_flag)
			{
				sock_para[i].pFun = p;
			}
			else
			{
				sock_para[i].msgq = msgq;
			}
			//sock_para[i].addrUsed=0;

			break;
		}
	}

	if (MAX_SOCK_NUM == i)
	{
		close(sock_fd);
		return -1;
	}
	
	FD_SET(sock_fd,&readset);/*add sock fd to readable socket set*/

	/*-----update max fd value--------------*/
	if (sock_fd>maxfd)
	{
		maxfd=sock_fd;
	}
	return sock_fd;
}


/*
·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
Èë²Î£º
	Msgq£ºµ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
	p:µ÷ÓÃÕß»Øµ÷º¯Êý
	callback_flag:1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
	Path£ºÂ·¾¶Ãû
	stream_flag£º1Îª×Ö½ÚÁ÷£¬0ÎªÊý¾Ý±¨
*/


INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag)
{
	INT32 sock_fd;
	UINT32 i;
	struct sockaddr_un addr;

	if (callback_flag)
	{
		if (!p)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
	}
	if (stream_flag)
	{	
		sock_fd=socket(AF_LOCAL,SOCK_STREAM,0);
		if (sock_fd<0)
		{
			log_msg(LOG_ERR,INET_MGMT,"open unix socket fail,errno:%s\n",strerror(errno));
			return -1;
		}

		unlink(path);//reset path
		memset(&addr,0,sizeof(struct sockaddr_un));
		addr.sun_family=AF_LOCAL;
		strcpy(addr.sun_path,path);
		
		if (bind(sock_fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
		{
			log_msg(LOG_ERR,INET_MGMT,"bind  unix socket port fail,errno:%s\n",strerror(errno));
			close(sock_fd);
			return -1;
		}

		if (listen(sock_fd,LISTEN_LEN)<0)
		{
			log_msg(LOG_ERR,INET_MGMT,"call listen fail\n");
			close(sock_fd);
			return -1;
		}

		/*add listen fd to listen_fd_info*/
		for(i=0;i<MAX_SOCK_NUM;i++)
		{
			if(listen_fd_info[i].listenfd==0)
			{
				listen_fd_info[i].socket_type = UNSOCKET;
				listen_fd_info[i].listenfd=sock_fd;
				if (callback_flag)
				{
					listen_fd_info[i].pFun = p;
				}
				else
				{
					listen_fd_info[i].msgq = msgq;
				}
				break;
			}
		}		
	}
	else
	{	
		sock_fd=socket(AF_LOCAL,SOCK_DGRAM,0);
		if (sock_fd<0)
		{
			log_msg(LOG_ERR,INET_MGMT,"open unix socket fail,errno:%s\n",strerror(errno));
			return -1;
		}

		unlink(path);//reset path
		memset(&addr,0,sizeof(struct sockaddr_un));
		addr.sun_family=AF_LOCAL;
		strcpy(addr.sun_path,path);
		
		if (bind(sock_fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
		{
			log_msg(LOG_ERR,INET_MGMT,"bind  unix socket port fail,errno:%s\n",strerror(errno));
			close(sock_fd);
			return -1;
		}

		for(i=0;i<MAX_SOCK_NUM;i++)
		{
			if(sock_para[i].sock_fd==0)
			{
				sock_para[i].sock_fd=sock_fd;
				sock_para[i].socket_type=UNSOCKET;
				sock_para[i].msgq = msgq;
				if (callback_flag)
				{
					sock_para[i].pFun = p;
				}
				else
				{
					sock_para[i].msgq = msgq;
				}
				//memcpy(&sock_para[i].clientaddr,(struct sockaddr*)(&addr),sizeof(struct sockaddr));
				break;
			}
		}
	}

	if (MAX_SOCK_NUM == i)
	{
		close(sock_fd);
		return -1;
	}
	
	FD_SET(sock_fd,&readset);/*add sock fd to readable socket set*/
	/*update max fd value*/
	if (sock_fd>maxfd)
	{
		maxfd=sock_fd;
	}
	
	return sock_fd;
}


/*
·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
Èë²Î£º
	Msgq£ºµ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
	p:µ÷ÓÃÕß»Øµ÷º¯Êý
	callback_flag:1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
	Protocol£ºÐ­ÒéÀàÐÍ
*/


INT32 raw_socket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag,UINT32 protocol)
{
	INT32 raw_fd;
	UINT32 i;
	unsigned char localmac[6]={0x00,0x0c,0x29,0xba,0x92,0x96};
	struct sockaddr_ll raw_sockaddr; /*raw socket structure*/

	struct ifreq ethreq;/*Interface request structure used for socket ioctl's.*/

	if (callback_flag)
	{
		if (!p)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_msg(LOG_ERR,INET_MGMT,"parameter invalid\n");
			return -1;
		}
		
	}
	
	raw_fd=socket(PF_PACKET,SOCK_RAW,htons(protocol));
	if (raw_fd<0)
	{
		printf("create raw fail\n");
		return -1;
	}
	
	bzero(&raw_sockaddr,sizeof(raw_sockaddr));
	bzero(&ethreq,sizeof(ethreq));

	strncpy(ethreq.ifr_name,g_om_port_name,strlen(g_om_port_name));/*assign a interface,and save interface name in struct ethreq*/

	/*----------set promiscuous model----------*/
	/*if(-1==ioctl(raw_fd,SIOCGIFFLAGS,&ethreq))  //get current interface flag ,and save in ethreq.ifr_flags ,
	{
	perror("ioctl");
	close(raw_fd);
	return -1;
	}*/

	/*reserve old interface flag ,and add new flag*/
	/*ethreq.ifr_flags |= IFF_PROMISC; //promiscuous model,receive every packet
	if(-1==ioctl(raw_fd,SIOCSIFFLAGS,&ethreq))  //set new interface flags ,
	{
	perror("ioctl");
	close(raw_fd);
	return -1;
	}*/

	if(-1==ioctl(raw_fd,SIOCGIFINDEX,&ethreq)) /* get interface id through ethreq.ifr_name,and store in ethreq.ifr_ifindex*/
	{
		log_msg(LOG_ERR,INET_MGMT,"ioctl fail,errno:%s\n",strerror(errno));
		close(raw_fd);
		return -1;
	}

	/*---------bind local addr structure---------*/
	raw_sockaddr.sll_family=PF_PACKET;
	raw_sockaddr.sll_protocol=htons(protocol);
	raw_sockaddr.sll_ifindex=ethreq.ifr_ifindex;
	raw_sockaddr.sll_halen = htons(6);
	if(bind(raw_fd,(struct sockaddr*)&raw_sockaddr,sizeof(raw_sockaddr))<0)
	{
		log_msg(LOG_ERR,INET_MGMT,"bind raw_fd addr structure fail,errno:%s\n",strerror(errno));
		close(raw_fd);
		return -1;
	}
	
	/* store struct_ll in global variable*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(sock_para[i].sock_fd==0)
		{
			sock_para[i].sock_fd=raw_fd;
			sock_para[i].socket_type=RAWSOCKET;
			if (callback_flag)
			{
				sock_para[i].pFun = p;
			}
			else
			{
				sock_para[i].msgq = msgq;
			}
			break;
		}
	}
	if (MAX_SOCK_NUM == i)
	{
		close(raw_fd);
		return -1;
	}
	FD_SET(raw_fd,&readset);/*add sock fd to readable socket set*/
	/*-----update max fd value--------------*/
	if (raw_fd>maxfd)
	{
		maxfd=raw_fd;
	}
#if 0
//	switch(protocol){
//	   case ETH_P_ALL:/*receive all Ethernet packet*/
//
//		    strncpy(ethreq.ifr_name,"eth0",IFNAMSIZ);/*assign a interface,and save interface name in struct ethreq*/
//		    if(-1==ioctl(raw_fd,SIOCGIFINDEX,&ethreq))  /*get interface id through ethreq.ifr_name,and store in ethreq.ifr_ifindex*/
//		     {
//		       perror("ioctl");
//		       close(raw_fd);
//		       exit(-1);
//		     }
//
//		    if(-1==ioctl(raw_fd,SIOCGIFFLAGS,&ethreq))  /*get current interface flag ,and save in ethreq.ifr_flags ,*/
//		    {
//		   	 perror("ioctl");
//		   	 close(raw_fd);
//		   	 exit(-1);
//		    }
//
//		    /*reserve old interface flag ,and add new flag*/
//		    ethreq.ifr_flags |= IFF_PROMISC; /*promiscuous model,receive every packet*/
//		    if(-1==ioctl(raw_fd,SIOCSIFFLAGS,&ethreq))  /*set new interface flags ,*/
//		    {
//		    	 perror("ioctl");
//		    	 close(raw_fd);
//		       exit(-1);
//		    }
//
//	      bzero(&originaddr,sizeof(originaddr));
//         originaddr.sll_ifindex=ethreq.ifr_ifindex;/*add interface id to sockaddr_ll */
//         originaddr.sll_protocol=htons(protocol); /*add protocol to sockaddr_ll */
//
//         /*store struct_ll in global variable*/
//         for(i=0;i<MAX_SOCK_NUM;i++){
//   			if(sock_para[i].sock_fd==0)
//   			      {
//   				sock_para[i].sock_fd=raw_fd;
//   				sock_para[i].socket_type=RAWSOCKET;
//   				sock_para[i].module_id=module_id;
//   				memcpy(&sock_para[i].clientaddr,&originaddr,sizeof(struct sockaddr));
//   			      }
//            }break;
//       /*receive other Ethernet packet*/
//
//       //need to add
//	   case 0x10DB: /* CTRL_FPGA--BRDM  mac protocol*/
//
//	       strncpy(ethreq.ifr_name,"eth0",IFNAMSIZ);/*assign a interface,and save interface name in struct ethreq*/
//	       if(-1==ioctl(raw_fd,SIOCGIFINDEX,&ethreq))  /*get interface id through ethreq.ifr_name,and store in ethreq.ifr_ifindex*/
//	   		 {
//	   		    perror("ioctl");
//	   		    close(raw_fd);
//	   		    exit(-1);
//	   		 }
//	       bzero(&originaddr,sizeof(originaddr));
//	       originaddr.sll_ifindex=ethreq.ifr_ifindex;/*add interface id to sockaddr_ll */
//	       originaddr.sll_protocol=htons(protocol); /*add protocol to sockaddr_ll */
//
//	         /*store struct_ll in global variable*/
//	       for(i=0;i<MAX_SOCK_NUM;i++){
//	   		  if(sock_para[i].sock_fd==0)
//	   			      {
//	   				sock_para[i].sock_fd=raw_fd;
//	   				sock_para[i].socket_type=RAWSOCKET;
//	   				sock_para[i].module_id=module_id;
//	   				memcpy(&sock_para[i].clientaddr,&originaddr,sizeof(struct sockaddr));
//	   			      }
//	            }break;
//
//	   default:
//	       strncpy(ethreq.ifr_name,"eth0",IFNAMSIZ);/*assign a interface,and save interface name in struct ethreq*/
//	       if(-1==ioctl(raw_fd,SIOCGIFINDEX,&ethreq)) /* get interface id through ethreq.ifr_name,and store in ethreq.ifr_ifindex*/
//	   		 {
//	   		    perror("ioctl");
//	   		    close(raw_fd);
//	   		    exit(-1);
//	   		 }
//	       bzero(&originaddr,sizeof(originaddr));
//	       originaddr.sll_ifindex=ethreq.ifr_ifindex;/*add interface id to sockaddr_ll */
//	       originaddr.sll_protocol=htons(protocol); /*add protocol to sockaddr_ll */
//
//	        /* save struct_ll in global variable*/
//	       for(i=0;i<MAX_SOCK_NUM;i++){
//	   		  if(sock_para[i].sock_fd==0)
//	   			      {
//	   				sock_para[i].sock_fd=raw_fd;
//	   				sock_para[i].socket_type=RAWSOCKET;
//	   				sock_para[i].module_id=module_id;
//	   				memcpy(&sock_para[i].clientaddr,&originaddr,sizeof(struct sockaddr));
//	   			      }
//	            }break;
#endif

	return raw_fd;
}


INT32 socket_unregister(INT32 sockfd)
{
	/*close socket and reset global table*/
	INT32 i;
	/*unregister listen fd*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if (sockfd==listen_fd_info[i].listenfd)
		{
			#if 0
			listen_fd_info[i].listenfd=0;
			listen_fd_info[i].msgq=0;
			listen_fd_info[i].pFun = 0;
			#else
			memset(&listen_fd_info[i],0,sizeof(listen_fd_info[i]));
			#endif
			close(sockfd);
			FD_CLR(sockfd,&readset);
			return 0;
		}
	}

	/*unregister tcp fd*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(sockfd==tcp_info[i].conn_fd)
		{
			#if 0
			tcp_info[i].conn_fd=0;
			tcp_info[i].ip = 0;
			tcp_info[i].msgq = 0;
			tcp_info[i].pFun = 0;
			#else
			memset(&tcp_info[i],0,sizeof(tcp_info[i]));
			#endif
			close(sockfd);
			FD_CLR(sockfd,&readset);
			return 0;
		}
	}

	/**unregister other fd*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if (sockfd==sock_para[i].sock_fd)
		{
			memset(&sock_para[i],0,sizeof(sock_para[i]));
			close(sockfd);
			FD_CLR(sockfd,&readset);
			return 0;
		}
	}

	return -1;

}
