/* Copyright (C) Rong Tao @Sylincom Beijing, 2019å¹´ 06æœˆ 14æ—¥ æ˜ŸæœŸäº” 09:06:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019å¹´ 03æœˆ 19æ—¥ æ˜ŸæœŸäºŒ 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled â€˜â€˜GNU
 Free Documentation Licenseâ€™â€™.
   2019å¹´ 03æœˆ 19æ—¥ æ˜ŸæœŸäºŒ 14:31:24 CST. 
*/
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: inet_mgmt_socket_register.c
** Description:  
**
** Current Version: 
** $Revision$
** Author: first    : Ye Guanwen    2018Äê8ÔÂ29ÈÕ
**         modify   : Rong Tao      2019.02
** Date: 2018.08 - 2019.02 - 
**
***********************************************************************************************************************/
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ether.h>      /*raw socket head file*/
#include <net/if.h>             /*struct ifreq , define SIOCGIFINDEX*/
#include <sys/ioctl.h>          /*ioctl SIOCGIFADDR*/
#include <netpacket/packet.h>   /* struct sockaddr_ll */
#include <sys/un.h>             /* unsocket head file */
#include <sys/types.h>
#include <unistd.h>             /* socket close will use */
#include <netinet/in.h>         /* htons/htonl*/
#include <arpa/inet.h>          /* inet_ntoa */
#include <sys/select.h>

#include "inet_mgmt.h"
#include "inet_mgmt_config.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
TcpInfo         tcp_info[MAX_SOCK_NUM];
SockPara        sock_para[MAX_SOCK_NUM];        /*store udp,unsocket,rawsocket info*/
ListenFdInfo    listen_fd_info[MAX_SOCK_NUM];   /*store listen fd info*/


extern  fd_set  readset;    /*store all socket fd*/
extern  INT32   maxfd;      /*maxfd is current max fd value*/


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *      ·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
 *      Èë²Î£º
 *          Msgq£º           µ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
 *          p:              µ÷ÓÃÕß»Øµ÷º¯Êý
 *          callback_flag:  1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
 *          Port£º           ÐèÒª¼àÌýµÄ¶Ë¿ÚºÅ
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
/*register function declaration*/
INT32 socket_tcp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port)
{
    log_inet_debug();

	INT32   i;
	struct  sockaddr_in serveraddr;
	INT32   listen_fd;
	struct  timeval timeout={2,0};//timeout

	if (callback_flag)
	{
		if (!p)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
		
	}
	
	listen_fd = Socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		log_inet_err("create tcp fail\n");
	}

	//option = (SO_KEEPALIVE>SO_REUSEADDR)? SO_KEEPALIVE:SO_REUSEADDR;
	//setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR|SO_KEEPALIVE, &option, sizeof(option));
	setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &timeout, sizeof(timeout));
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &timeout, sizeof(timeout));
	setsockopt(listen_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));/*set receive message timeout */
	setsockopt(listen_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));/*set send message timeout*/

	Memset(&serveraddr,0,sizeof(struct sockaddr_in));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);
	if (bind(listen_fd,(struct sockaddr*)(&serveraddr),sizeof(serveraddr))<0)
	{
		log_inet_err("bind tcp port fail\n");
		Close(listen_fd);
		return INET_ERR;

	}

	if (listen(listen_fd,LISTEN_LEN)<0)
	{
		log_inet_err("call listen fail\n");
		Close(listen_fd);
		return INET_ERR;
	}

	/*add listen fd to listen_fd_info*/
	for(i=0; i<MAX_SOCK_NUM; i++)
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
		Close(listen_fd);
		return INET_ERR;
	}

	FD_SET(listen_fd,&readset);
    
	/**
	 *  update max fd value
	 */
	if (listen_fd > maxfd)
	{
		maxfd = listen_fd;
	}
	return listen_fd;
}


/***********************************************************************************************************************
 * Description:
 *      ·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
 *      Èë²Î£º
 *          Msgq£º           µ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
 *          p:              µ÷ÓÃÕß»Øµ÷º¯Êý
 *          callback_flag:  1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
 *          Port£º           ÐèÒª¼àÌýµÄ¶Ë¿ÚºÅ
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 socket_udp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port)
{
    log_inet_debug();

    INT32 sock_fd;
	UINT32 i;

	//INT32 value; //add for degbug

	INT32 option = 1;
	struct sockaddr_in serveraddr;

	if (callback_flag)
	{
		if (!p)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
	}

	sock_fd = Socket(AF_INET,SOCK_DGRAM,0);
	if (sock_fd<0)
	{
		log_inet_err("create udp socket fail\n");
		Close(sock_fd);
		return INET_ERR;
	}
    
    /*socket option */
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &option, sizeof(option));
    setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, &option, sizeof(option));
	//setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR|SO_BROADCAST|SO_KEEPALIVE, &option, sizeof(option));


	Memset(&serveraddr,0,sizeof(struct sockaddr_in));
	serveraddr.sin_family       =AF_INET;
	serveraddr.sin_addr.s_addr  =htonl(INADDR_ANY);
	serveraddr.sin_port         =htons(port);
    
	if (bind(sock_fd,(struct sockaddr*)(&serveraddr),sizeof(serveraddr))<0)
	{
		log_inet_err("bing udp socket port fail\n");
		Close(sock_fd);
		return INET_ERR;
	}
	
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(sock_para[i].sock_fd==0)
		{
			sock_para[i].sock_fd = sock_fd;
			sock_para[i].socket_type = UDP;
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
		Close(sock_fd);
		return INET_ERR;
	}
	/**
	 *  add sock fd to readable socket set
	 */
	FD_SET(sock_fd,&readset);

	/**
	 * update max fd value
	 */
	if (sock_fd>maxfd)
	{
		maxfd=sock_fd;
	}
	return sock_fd;
}


/***********************************************************************************************************************
 * Description:
 *      ·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
 *      Èë²Î£º
 *          Msgq£º           µ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
 *          p:              µ÷ÓÃÕß»Øµ÷º¯Êý
 *          callback_flag:  1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
 *          path:           UNUX domain path
 *          stream_flag:    use SOCK_STREAM or not 
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag)
{
    log_inet_debug();

    INT32 sock_fd;
	UINT32 i;
	struct sockaddr_un addr;

	if (callback_flag)
	{
		if (!p)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
		
	}
	else
	{
		if (0 == msgq)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
	}
	if (stream_flag)
	{	
		sock_fd = Socket(AF_LOCAL,SOCK_STREAM,0);
		if (sock_fd<0)
		{
			log_inet_err("open unix socket fail\n");
			return INET_ERR;
		}

		unlink(path);//reset path
		Memset(&addr,0,sizeof(struct sockaddr_un));
		addr.sun_family = AF_LOCAL;
		Strcpy(addr.sun_path,path);
		
		if (bind(sock_fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
		{
			log_inet_err("bind  unix socket port fail\n");
			Close(sock_fd);
			return INET_ERR;
		}

		if (listen(sock_fd,LISTEN_LEN)<0)
		{
			log_inet_err("call listen fail\n");
			Close(sock_fd);
			return INET_ERR;
		}

		/*add listen fd to listen_fd_info*/
		for(i=0;i<MAX_SOCK_NUM;i++)
		{
			if(listen_fd_info[i].listenfd==0)
			{
				listen_fd_info[i].socket_type = UNSOCKET;
				listen_fd_info[i].listenfd = sock_fd;
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
		sock_fd = Socket(AF_LOCAL,SOCK_DGRAM,0);
		if (sock_fd<0)
		{
			log_inet_err("open unix socket fail\n");
			return INET_ERR;
		}

		unlink(path);//reset path
		Memset(&addr,0,sizeof(struct sockaddr_un));
		addr.sun_family = AF_LOCAL;
		Strcpy(addr.sun_path,path);
		
		if (bind(sock_fd,(struct sockaddr*)(&addr),sizeof(addr))<0)
		{
			log_inet_err("bind  unix socket port fail\n");
			Close(sock_fd);
			return INET_ERR;
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
				//Memcpy(&sock_para[i].clientaddr,(struct sockaddr*)(&addr),sizeof(struct sockaddr));
				break;
			}
		}
	}

	if (MAX_SOCK_NUM == i)
	{
		Close(sock_fd);
		return INET_ERR;
	}
	
	FD_SET(sock_fd,&readset);/*add sock fd to readable socket set*/
    
	/**
	 *  update max fd value
	 */
	if (sock_fd>maxfd)
	{
		maxfd = sock_fd;
	}
	
	return sock_fd;
}


/***********************************************************************************************************************
 * Description:
 *      ·µ»ØÖµ: ·µ»Øsock fd £¬-1±íÊ¾´´½¨Ê§°Ü£¬ÆäËüÖµ±íÊ¾´´½¨³É¹¦£»
 *      Èë²Î£º
 *          Msgq£º           µ÷ÓÃÕßÏûÏ¢¶ÓÁÐ
 *          p:              µ÷ÓÃÕß»Øµ÷º¯Êý
 *          callback_flag:  1Îª»Øµ÷º¯ÊýÓÐÐ§£¬0ÎªÏûÏ¢¶ÓÁÐÓÐÐ§£¬¶þÑ¡Ò»
 *          protocol      
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 raw_socket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag,UINT32 protocol)
{
    log_inet_debug();

    INT32 raw_fd;
	UINT32 i;
	struct sockaddr_ll raw_sockaddr; /*raw socket structure*/

	struct ifreq ethreq;/*Interface request structure used for socket ioctl's.*/

	if (callback_flag)
	{
		if (!p)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
	}
	else
	{
		if (0 == msgq)
		{
			log_inet_err("parameter invalid\n");
			return INET_ERR;
		}
		
	}
	
	raw_fd = Socket(PF_PACKET, SOCK_RAW, htons(protocol));
	if (raw_fd<0)
	{
		log_inet_err("create raw fail\n");
		return INET_ERR;
	}
	
	bzero(&raw_sockaddr,sizeof(raw_sockaddr));
	bzero(&ethreq,sizeof(ethreq));

    /**
     *  assign a interface,and save interface name in struct ethreq
     */
	Strncpy(ethreq.ifr_name, G_OM_PORT_NAME, Strlen(G_OM_PORT_NAME));
    /**
     *  get interface id through ethreq.ifr_name,and store in ethreq.ifr_ifindex
     */
	if(-1 == Ioctl(raw_fd,SIOCGIFINDEX,&ethreq)) 
	{
		log_inet_err("ioctl fail\n");
		Close(raw_fd);
		return INET_ERR;
	}

	/*---------bind local addr structure---------*/
	raw_sockaddr.sll_family     = PF_PACKET;
	raw_sockaddr.sll_protocol   = htons(protocol);
	raw_sockaddr.sll_ifindex    = ethreq.ifr_ifindex;
	raw_sockaddr.sll_halen      = htons(6);
    
	if(bind(raw_fd,(struct sockaddr*)&raw_sockaddr,sizeof(raw_sockaddr))<0)
	{
		log_inet_err("bind raw_fd addr structure fail\n");
		Close(raw_fd);
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
		Close(raw_fd);
		return -1;
	}
	FD_SET(raw_fd,&readset);/*add sock fd to readable socket set*/
	/*-----update max fd value--------------*/
	if (raw_fd>maxfd)
	{
		maxfd=raw_fd;
	}
	return raw_fd;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 socket_unregister(INT32 sockfd)
{
    log_inet_debug();

    /**
     *  close socket and reset global table
     */
	INT32 i;
	/*unregister listen fd*/
	for(i=0; i<MAX_SOCK_NUM; i++)
	{
		if (sockfd == listen_fd_info[i].listenfd)
		{
			Memset(&listen_fd_info[i],0,sizeof(listen_fd_info[i]));
			Close(sockfd);
			FD_CLR(sockfd,&readset);
			return INET_OK;
		}
	}

	/*unregister tcp fd*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if(sockfd==tcp_info[i].conn_fd)
		{
			Memset(&tcp_info[i],0,sizeof(tcp_info[i]));
			Close(sockfd);
			FD_CLR(sockfd,&readset);
			return INET_OK;
		}
	}

	/**unregister other fd*/
	for(i=0;i<MAX_SOCK_NUM;i++)
	{
		if (sockfd==sock_para[i].sock_fd)
		{
			Memset(&sock_para[i],0,sizeof(sock_para[i]));
			Close(sockfd);
			FD_CLR(sockfd,&readset);
			return INET_OK;
		}
	}

	return INET_ERR;
}


