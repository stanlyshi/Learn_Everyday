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
/*
 * inet_mgmt.c
 *
 *  Created on: 2018å¹?8æœ?29æ—?
 *      Author: root
 */


/*
 ============================================================================
 Name        : Netport.c
 Author      : yeguanwen
 Version     : V1.0
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
//#include <netinet/ether.h> /*raw socket head file*/
//#include <sys/un.h>  /*unsocket head file*/
//#include <net/if.h> /*struct ifreq , define SIOCGIFINDEX*/
//#include <sys/ioctl.h> /*ioctl SIOCGIFADDR*/
#include <netpacket/packet.h> /*struct sockaddr_ll*/
#include <sys/un.h>  /*unsocket head file*/
#include <sys/types.h>
#include <unistd.h>  /*socket close will use  */
#include <netinet/in.h>   /*htons/htonl*/
#include <arpa/inet.h>  /*inet_ntoa*/
#include <sbs_msgq.h>
#include "sbs_type.h"
#include "inet_mgmt.h"
#include "sbs_task.h"
#include "sbs_log.h"

/*global variable*/
extern TcpInfo tcp_info[MAX_SOCK_NUM];/*store tcp info*/
extern SockPara sock_para[MAX_SOCK_NUM];/*store udp,unsocket,rawsocket info*/
extern ListenFdInfo listen_fd_info[MAX_SOCK_NUM];
InetMsg inet_msg[MAX_MSGQ_MSG_NUM];

fd_set readset;/*store all socket fd*/
INT32 maxfd;/*maxfd is current max fd value*/

INT32 g_InetDebug = 1;

INT32 inet_mgmt_module_init()
{
	/*init data structure */
	memset(tcp_info,0,sizeof(tcp_info));
	memset(sock_para,0,sizeof(sock_para));
	memset(inet_msg,0,sizeof(inet_msg));
	memset(&readset,0,sizeof(readset));

	/*create inet_module main task */
	THREAD_MAIN_CTRL=create_pthread(1, SCHED_RR, TASK_JOIN, inet_mgmt_start, NULL);
	if(-1==THREAD_MAIN_CTRL)
	{
		log_msg(LOG_ERR,INET_MGMT,"create message send pthread \n");
		return -1;
	}

	return 0;
}

/*inet_module task start*/
INT32 inet_mgmt_start()
{

	TaskType pthread_id_msg_send;
	TaskType pthread_id_msg_receive;

	
	/*create msgque,save inter module message*/
	MSGQ_ID_INET_MGMT=open_msgq(INET_MGMT_MSGQ_NAME,MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);
	if (MSGQ_ID_INET_MGMT==-1)
	{
		log_msg(LOG_ERR,INET_MGMT,"create message message queue fail\n");
		return -1;
	}
    /*create message send pthread, send message to FPGA/RRU/CPRI*/
	pthread_id_msg_send=create_pthread(1, SCHED_RR,	TASK_JOIN, msg_send_fn, NULL);
	if(-1==pthread_id_msg_send)
	{
		log_msg(LOG_ERR,INET_MGMT,"create message send pthread fail\n");
		return -1;
	}
	
	/*create message receive pthread,receive message from FPGA/RRU/CPRI*/
	pthread_id_msg_receive=create_pthread(1, SCHED_RR,	TASK_JOIN, msg_receive_fn, NULL);
	if(-1==pthread_id_msg_receive)
	{
		log_msg(LOG_ERR,INET_MGMT,"create message receive pthread\n");
		return -1;
	}
	
	if (g_InetDebug)
	{
		log_msg(LOG_INFO,INET_MGMT,"create INET_MGMT message queue success,msgq id =%d\n",MSGQ_ID_INET_MGMT);
	}
	
	return 0;
}


/*--------read inet msg message_queue ,and invoke socket sending message to rru/fpga-----------*/

void *msg_send_fn(void*arg)
{
	char buff[MAX_INET_MSGQ_MSG_LEN]; /*save message */
	UINT32 i;
	INT32 len_receive_msgq ;    /* receive message's length */
	InetMsg *p_vomsg=NULL; /*the message pointer about inet_mgmt*/
	struct sockaddr_in clientaddr;
	/*receive every module message,and send out*/
	struct sockaddr_ll clientaddr_mac;
	struct sockaddr_un clientaddr_mac_un;
	INT32 len=0;

	if (g_InetDebug)
	{
		log_msg(LOG_INFO,INET_MGMT,"Waiting messages from Other modules\n");
	}
	
	
	while(1)
	{

		/*receive messages from the message queue*/
		len_receive_msgq = receive_msgq(MSGQ_ID_INET_MGMT, buff,MAX_INET_MSGQ_MSG_LEN, WAIT_FOREVER);
		if(len_receive_msgq < 0)
		{
			usleep(250000);/*delay 25000 microsecond*/
			continue;
		}
		else
		{
			p_vomsg=(InetMsg*)buff;

			if (g_InetDebug)
			{
				log_msg(LOG_INFO,INET_MGMT,"msg_send_fn, msg fd=%d, ip=0x%x\n", p_vomsg->fd,p_vomsg->ip);
			}
			
			/*judge socket type*/
			if (p_vomsg->flag==1) /* tcp message*/
			{
				/*find conn_fd through ip*/
				if (0 != p_vomsg->fd)
				{
					if(-1==send(p_vomsg->fd,p_vomsg->msg_p,p_vomsg->msg_len,0))
					{
						log_msg(LOG_WARNING,INET_MGMT,"send tcp message fail, ip=0x%x, errno:%s\n",p_vomsg->ip,strerror(errno));
					}
				}
				else
				{
					for(i=0;i<MAX_SOCK_NUM;i++)
					{
						if(p_vomsg->ip == tcp_info[i].ip)
						{
							if(-1==send(tcp_info[i].conn_fd,p_vomsg->msg_p,p_vomsg->msg_len,0))
							{
								log_msg(LOG_WARNING,INET_MGMT,"send tcp message fail, ip=0x%x, errno:%s\n",p_vomsg->ip,strerror(errno));
							}
							break;
						}
					}
				}
				free(p_vomsg->msg_p);
				
			} 
			else if(p_vomsg->flag==0) /*send udp/unsocket/rawsocket*/
			{
				for(i=0;i<MAX_SOCK_NUM;i++)/*find client socket address structure from sock_para*/
				{
					if(p_vomsg->fd==sock_para[i].sock_fd)
					{
						if (sock_para[i].socket_type==UDP)
						{
							
							log_msg(LOG_INFO,INET_MGMT,"msg_send_fn, msg type is udp\n");
							len=sizeof(struct sockaddr_in);
						}
						else if (sock_para[i].socket_type==RAWSOCKET)
						{
							log_msg(LOG_INFO,INET_MGMT,"msg_send_fn, msg type is raw\n");
							len=sizeof(struct sockaddr_ll);
						}
						else if (sock_para[i].socket_type==UNSOCKET)
						{
							log_msg(LOG_INFO,INET_MGMT,"msg_send_fn, msg type is un_sock\n");
							len=sizeof(struct sockaddr_un);
						}
						if (sock_para[i].socket_type==RAWSOCKET)
						{
							if(-1==sendto(p_vomsg->fd,p_vomsg->msg_p,p_vomsg->msg_len,0,0,0/*&p_vomsg->clientaddr, len*/))
							{
								log_msg(LOG_WARNING,INET_MGMT,"send socket message fail, ip=0x%x, errno:%s\n",p_vomsg->ip,strerror(errno));
							}

						}
						else
						{
							//if(-1==sendto(p_vomsg->fd,p_vomsg->msg_p,p_vomsg->msg_len,0,&sock_para[i].clientaddr, sizeof(sock_para[i].clientaddr)))
							if(-1==sendto(p_vomsg->fd,p_vomsg->msg_p,p_vomsg->msg_len,0,&p_vomsg->clientaddr, len))
							{
								log_msg(LOG_WARNING,INET_MGMT,"send socket message fail, ip=0x%x, errno:%s\n",p_vomsg->ip,strerror(errno));
							}
						}
						free(p_vomsg->msg_p);
						break;

					}
				}
			}
		}
	}
	
	return NULL;
	
}

/*receive message from FPGA/RRU,and send to every module*/

void *msg_receive_fn(void*arg)
{
	fd_set readyfd_set;
	//UINT32 maxfd;/*maxfd is current max fd value*/
	UINT32 i,j;

	INT32 value;// just for debug

	UINT32 ret;/*store the value returned by recvfrom/recv*/
	socklen_t addrlen=0 ;       /*the length of socket's address struct.*/
	INT32 connect_fd;
	UINT32 select_num;
	char buff[MAX_RECV_MSG_LEN]; /*store message form netport*/
	struct timeval timeout={1,250000};
	//struct timeval timeout;
	struct sockaddr_in clientaddr;
	struct sockaddr_ll clientaddr_mac;
	struct sockaddr_un clientaddr_mac_un;

	memset(&timeout,0,sizeof(struct timeval));

	/*initial the masker*/
    FD_ZERO(&readyfd_set);
	
	if (g_InetDebug)
	{
		log_msg(LOG_INFO,INET_MGMT,"Socket: waiting messages from socket\n");
	}

	while(1)
	{
		if (SYS_RUNNING == g_om_state)
		{
			break;
		}
		sleep(5);
	}
		
	while(1)
	{
		readyfd_set=readset;/*readset is global variable to store all socket fd*/
		select_num=select(maxfd+1,&readyfd_set,NULL,NULL,&timeout);
		if (select_num <= 0)
		{
			usleep(500000);/*delay 25000 microsecond*/
			//sleep(1);
			//log_msg(LOG_INFO,INET_MGMT,"Socket: seclet is timeout\n");
			continue;
		}

		if (g_InetDebug)
		{
			log_msg(LOG_INFO,INET_MGMT,"Socket: seclet is active, active fd num is %d\n",select_num);
		}
			
		for(i=0;i<MAX_SOCK_NUM;i++)
		{
			if(FD_ISSET(listen_fd_info[i].listenfd,&readyfd_set))
			{
				/*if the activate socked is listen socket.*/
				addrlen=sizeof(clientaddr);
				connect_fd=accept(listen_fd_info[i].listenfd,(struct sockaddr*)&clientaddr,&addrlen);
				FD_SET(connect_fd,&readset);

				for(j = 0; j < MAX_SOCK_NUM; j++)
				{
					if(tcp_info[j].conn_fd==0)
					{
						tcp_info[j].conn_fd = connect_fd;
						tcp_info[j].socket_type = listen_fd_info[i].socket_type;
						if (TCP == tcp_info[j].socket_type)
						{
							tcp_info[j].ip = clientaddr.sin_addr.s_addr;
						}
						
						tcp_info[j].msgq = listen_fd_info[i].msgq;
						tcp_info[j].pFun = listen_fd_info[i].pFun;
						
						if (g_InetDebug)
						{
							log_msg(LOG_INFO, INET_MGMT,"TCP: new tcp connection from %s on socket %ld\n",\
							inet_ntoa(clientaddr.sin_addr), connect_fd);
						}
						break;
					}
				}
				
				/*update max fd value*/
				if (connect_fd>maxfd)
				{
					maxfd=connect_fd;
				}
			}
		}

		/*receive tcp socket*/
		for(i=0;i<MAX_SOCK_NUM;i++) 
		{
			if(tcp_info[i].conn_fd!=0 && FD_ISSET(tcp_info[i].conn_fd,&readyfd_set))
			{
				/*------------receive tcp--------------*/
				memset(buff,0,MAX_RECV_MSG_LEN);
				
				ret=recv(tcp_info[i].conn_fd,buff,MAX_RECV_MSG_LEN,0);
				if(ret==-1 || ret==0)
				{
					log_msg(LOG_WARNING,INET_MGMT,"TCP: recv tcp message fail, conn_fd=%d, ip=0x%x, errno:%s, ret=%d\n",\
					tcp_info[i].conn_fd,tcp_info[i].ip,strerror(errno), ret);
					
					socket_unregister(tcp_info[i].conn_fd);
					continue;
				}

				#if 0
				if (g_InetTest)
				{
					/*_----------------for test----------------------*/
					if (send_msg_to_module(MODULE_BORD_MGMT,TCP,buff,ret)!=0)
					{
						log_msg(LOG_WARNING,INET_MGMT,"TCP: send message to OM_RRU msgq fail,errno:%s\n",strerror(errno));
						break;
					}
					else
					{
						//printf("send msg to %d success\n",MODULE_RRUS_MGMT);
						log_msg(LOG_INFO,INET_MGMT,"TCP: send message to OM_RRU msgq success\n");
					}
				}
				#else
				if (tcp_info[i].pFun)
				{
					tcp_info[i].pFun(tcp_info[i].conn_fd, 0, buff, ret);
				}
				else
				{
					if (send_msg_to_module(tcp_info[i].msgq,TCP,buff,ret)!=0)
					{
						log_msg(LOG_WARNING,INET_MGMT,"TCP: send message to msgq %ld,errno:%s\n",tcp_info[i].msgq, strerror(errno));
					}
				}
				
				#endif
				
				
			}
		}

		/*-------receive udp/unsocket/rawsocket message -------*/
		for(j=0;j<MAX_SOCK_NUM;j++)
		{
			if(sock_para[j].sock_fd !=0 && FD_ISSET(sock_para[j].sock_fd,&readyfd_set))
			{
				/*----------receive udp  message-------*/
				if(sock_para[j].socket_type==UDP)
				{
					memset(buff,0,MAX_RECV_MSG_LEN);
					addrlen=sizeof(clientaddr);
					memset(&clientaddr,0,addrlen);
					
					ret=recvfrom(sock_para[j].sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr,&addrlen);
					if(ret==-1)
					{
						log_msg(LOG_WARNING,INET_MGMT,"UDP: receive socket message fail,socket fd=%d,errno:%s\n",sock_para[j].sock_fd,strerror(errno));
						continue;
					}

					if (g_InetDebug)
					{
						log_msg(LOG_INFO,INET_MGMT,"UDP: receive UDP message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
					}
						

					//if(sock_para[j].addrUsed==0) /*add new connect socket address structure*/
					{
						memcpy(&sock_para[j].clientaddr,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
						//sock_para[j].addrUsed=1;

					}
					
					/*send message to every module queue*/
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, &clientaddr, buff,ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq,sock_para[j].socket_type,buff,ret);	
					}
					
				}/*----------receive mac message--------------*/
				else if(sock_para[j].socket_type==RAWSOCKET)
				{
					ret=0;
					addrlen=0;
					memset(buff,0,MAX_RECV_MSG_LEN);
					addrlen=sizeof(clientaddr_mac);
					memset(&clientaddr_mac,0,addrlen);
					
					ret=recvfrom(sock_para[j].sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr_mac,&addrlen);
					if (ret<0)
					{
						log_msg(LOG_WARNING,INET_MGMT,"Receive raw message fail,socket fd=%d,errno:%s\n",sock_para[j].sock_fd,strerror(errno));
						continue;
					}
					
					if (g_InetDebug)
					{
						log_msg(LOG_INFO,INET_MGMT,"RAW: receive raw message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
					}
						

					//if(sock_para[j].addrUsed==0) /*add new connect socket address structure*/
					{
						memcpy(&sock_para[j].clientaddr,(struct sockaddr*)&clientaddr_mac,sizeof(clientaddr_mac));
						//sock_para[j].addrUsed=1;
					}
					/*send message to every module queue*/
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, &clientaddr_mac, buff,ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq,sock_para[j].socket_type,buff,ret);	
					}
					
				}
				else if(sock_para[j].socket_type==UNSOCKET)
				{
					ret=0;
					addrlen=0;
					memset(buff,0,MAX_RECV_MSG_LEN);
					addrlen=sizeof(clientaddr_mac_un);
					memset(&clientaddr_mac_un,0,addrlen);
					
					ret=recvfrom(sock_para[j].sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr_mac_un,&addrlen);
					if (ret<0)
					{
						log_msg(LOG_WARNING,INET_MGMT,"Receive MAC message fail,socket fd=%d,errno:%s\n",sock_para[j].sock_fd,strerror(errno));
						continue;
					}
					
					if (g_InetDebug)
					{
						log_msg(LOG_INFO,INET_MGMT,"RAW: receive MAC message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
					}
						

					//if(sock_para[j].addrUsed==0) /*add new connect socket address structure*/
					{
						memcpy(&sock_para[j].clientaddr,(struct sockaddr*)&clientaddr_mac_un,sizeof(clientaddr_mac_un));
						//sock_para[j].addrUsed=1;
					}
					/*send message to every module queue*/
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, &clientaddr_mac_un, buff,ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq,sock_para[j].socket_type,buff,ret);	
					}
				}
			}
		}

	}

	return NULL;
}


/*send message to RRUS_MGMT,BORD_MGMT,CELL_MGMT*/
INT32 send_msg_to_module(MsgqType msgq/*UINT16 module_id*/,UINT32 socktype,char *msg_p,UINT32 msg_len)
{
#if 0
	switch(module_id)
	{
		case MODULE_BORD_MGMT:
			if (0 != send_msgq(MSGQ_ID_BORD_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
			{
				log_msg(LOG_WARNING,INET_MGMT,"send message error to BRDM\n");
				return -1;
			}
		break;
		case MODULE_CELL_MGMT:
			if (0 != send_msgq(MSGQ_ID_CELL_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
			{
				log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
				return -1;
			}
		break;
		case MODULE_RRUS_MGMT:
			if (0 != send_msgq(MSGQ_ID_RRUS_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
			{
				log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
				return -1;
			}
		break;
		default:
			log_msg(LOG_WARNING,INET_MGMT,"wrong module type!\n");
			return -1;
		break;
	}
#else
	if (0 != send_msgq(msgq,msg_p,msg_len, WAIT_FOREVER, 1))
	{
		log_msg(LOG_WARNING,INET_MGMT,"send message error to msgq %ld\n", msgq);
		return -1;
	}
#endif
	return 0;
}

/*
·µ»ØÖµ: -1Ê§°Ü£¬0³É¹¦£»
Èë²Î£º
	module_id£ºµ÷ÓÃÕßÄ£¿éºÅ
	streamflag:·¢ËÍÊý¾Ý×Ö½ÚÁ÷±êÖ¾£¬1Îª×Ö½ÚÁ÷£¬0ÎªÆäËü
	fd:×Ö½ÚÁ÷Ê±£¬Èç¹ûÊÇtcp£¬fdºÍipÑ¡Ò»£¬unsockÖ»Ñ¡fd£»ÆäËüÎª0
	ip£ºtcpÊ±fdºÍipÑ¡Ò»£¬ÆäËüÎª0
	clientaddr£º×Ö½ÚÁ÷Ê±Îª¿Õ£¬ÆäËüÁ÷ÐèÒªÌîÐ´ÕæÊµµÄsockaddr½á¹¹
	msg_p£º·¢ËÍÊý¾ÝÖ¸Õë£¬º¯Êý²»¸ºÔðÊÍ·ÅÄÚ´æ
	msg_len:·¢ËÍÊý¾Ý³¤¶È
*/

INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,char *msg_p,UINT32 msg_len)
{
	InetMsg stInetMsg;
	
  	UINT32 InetMsg_len=sizeof(InetMsg);/*msg_len+32;*/// message structure Inetmsg length
	InetMsg *p_InetMsg = &stInetMsg;//malloc(InetMsg_len);
#if 0
	if (flag==1) /*send tcp*/
	{ 
		/*find conn_fd through ip*/
		p_InetMsg->flag=flag;
		p_InetMsg->ip = ip;
		p_InetMsg->module_id=module_id;
		p_InetMsg->fd=-1; /*now fd is no use*/
		p_InetMsg->msg_len=msg_len;
		p_InetMsg->msg_p = malloc(msg_len);
		if (!p_InetMsg->msg_p)
		{
			return -1;
		}
		
		memcpy(p_InetMsg->msg_p,msg_p,msg_len);
		if(-1==send_msgq(MSGQ_ID_INET_MGMT,p_InetMsg,InetMsg_len,WAIT_FOREVER,1))
		{
			log_msg(LOG_WARNING,INET_MGMT,"send tcp message to inet_mgmt msgq error,ERRNO:%s\n",strerror(errno));
			return -1;
		}
		
		/*---------------------log debug----------------*/
		if (g_InetDebug)
			log_msg(LOG_INFO,INET_MGMT,"send tcp message to inet_mgmt ok ^^^^^^^^^^^^^^\n");
	}
	else if(flag==0) /*send udp/unsocket/rawsocket*/
	{
		p_InetMsg->flag=flag;
		p_InetMsg->ip = ip;
		p_InetMsg->module_id=module_id;
		p_InetMsg->fd=fd;
		p_InetMsg->msg_len=msg_len;
		p_InetMsg->msg_p = malloc(msg_len);
		
		if (!p_InetMsg->msg_p)
		{
			return -1;
		}
		
		memcpy(p_InetMsg->msg_p,msg_p,msg_len);
		if(-1==send_msgq(MSGQ_ID_INET_MGMT,p_InetMsg,InetMsg_len,WAIT_FOREVER,1))
		{
			log_msg(LOG_WARNING,INET_MGMT,"send message to inet_mgmt msgq error,ERRNO:%s\n",strerror(errno));
			return -1;
		}
	}
#else

	/*find conn_fd through ip*/
	p_InetMsg->flag = streamflag;
	p_InetMsg->ip = ip;
	/*p_InetMsg->module_id = module_id;*/
	if (clientaddr)
	{
		memcpy(&p_InetMsg->clientaddr, clientaddr, sizeof(p_InetMsg->clientaddr));
	}
	
	p_InetMsg->fd = fd; /*now fd is no use*/
	p_InetMsg->msg_len = msg_len;
	
	p_InetMsg->msg_p = malloc(msg_len);
	if (!p_InetMsg->msg_p)
	{
		return -1;
	}
	
	memcpy(p_InetMsg->msg_p,msg_p,msg_len);
	if(-1==send_msgq(MSGQ_ID_INET_MGMT,p_InetMsg,InetMsg_len,WAIT_FOREVER,1))
	{
		log_msg(LOG_WARNING,INET_MGMT,"INET_SEND_MSG: send message to inet_mgmt msgq error,ERRNO:%s\n",strerror(errno));
		return -1;
	}
	
	if (g_InetDebug)
	{
		log_msg(LOG_INFO,INET_MGMT,"INET_SEND_MSG: send message to inet_mgmt ok\n");
	}
		
#endif

	return 0;
}







