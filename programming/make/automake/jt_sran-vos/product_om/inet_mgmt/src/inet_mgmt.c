/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: inet_mgmt.c
** Description:   
**
** Current Version: 
** $Revision$
** Author: first    : Ye Guanwen    2018��8��29��
**         modify   : Rong Tao      2019.02
** Date: 2018.08 - 2019.02 - 
** 
***********************************************************************************************************************/
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
vos_module_t    inet_module;                /* inet mgmt module */
ULONG           inet_msgqid;                /* inet mgmt msgq id */
TaskCreateRet   inet_task_main;             /* main ctrl task thread handle */
TaskCreateRet   inet_task_recv;             /* recv thread */
TaskCreateRet   inet_task_send;             /* send thread */

fd_set          readset;                    /*store all socket fd*/
INT32           maxfd;                      /*maxfd is current max fd value*/


extern TcpInfo      tcp_info[MAX_SOCK_NUM];         /*store tcp info*/
extern SockPara     sock_para[MAX_SOCK_NUM];        /*store udp,unsocket,rawsocket info*/
extern ListenFdInfo listen_fd_info[MAX_SOCK_NUM];   /*store listen fd info*/


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
/*function declaration*/
INT32 inet_mgmt_module_init()
{
    log_inet_debug();

    INT ret = INET_ERR;

    /**
     *  init data structure 
     */
	Memset(tcp_info, 0, sizeof(tcp_info));
	Memset(sock_para, 0, sizeof(sock_para));
	Memset(&readset, 0, sizeof(readset));

    /**
     *  Create inet_mgmt message queue.
     */
    inet_msgqid = MsgqCreate();
    if(inet_msgqid == 0)
    {
        log_inet_err("%s create message queue error.\n", INET_MGMT_NAME);
        return INET_ERR;
    }
    log_inet_succ("%s create message queue success.\n", INET_MGMT_NAME);

    /**
     *  This is one of input of "VOS_module_register"
     */
    inet_module.queueID = inet_msgqid;

    /**
     *  Register inet_mgmt module
     */
    ret = VOS_module_register(INET_MGMT_NAME, &inet_module);
    if(ret != INET_OK)
    {
        log_inet_err("%s module register error.\n", INET_MGMT_NAME);
        
        return INET_ERR;
    }
    log_inet_succ("%s module register success.\n", INET_MGMT_NAME);

    
    /**
     *  Create VOS cell_mgmt main ctrl thread
     */
    inet_task_main = TaskCreate(INET_MGMT_MAIN_TASK_NAME, inet_mgmt_start, NULL);
    if(inet_task_main == NULL)
    {
        log_inet_err("Can't create main ctrl thread [TaskCreate] \n");
        MsgqDelete(inet_msgqid);
        return INET_ERR;
    }
    log_inet_succ("Create main ctrl thread Success [TaskCreate] \n");


    return INET_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 inet_mgmt_clean_up()
{
    log_inet_debug();

    return INET_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 inet_mgmt_start()
{
    log_inet_debug();

    log_inet_info("pid(%d), tid(%d)\n", getpid(), GET_TID());

    /**
     *  Create receive thread
     */
    inet_task_recv = TaskCreate(INET_MGMT_RECV_TASK_NAME, msg_receive_fn, NULL);
    if(inet_task_recv == NULL)
    {
        log_inet_err("Can't create recv thread [TaskCreate] \n");
        return INET_ERR;
    }
    log_inet_succ("Create recv thread Success [TaskCreate] \n");

    /**
     *  Create send thread
     */
    inet_task_send = TaskCreate(INET_MGMT_SEND_TASK_NAME, msg_send_fn, NULL);
    if(inet_task_send == NULL)
    {
        log_inet_err("Can't create send thread [TaskCreate] \n");
        return INET_ERR;
    }
    log_inet_succ("Create send thread Success [TaskCreate] \n");

    


    return INET_OK;
}



/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
void *msg_send_fn(void*arg)
{
    log_inet_debug();
    log_inet_info("pid(%d), tid(%d)\n", getpid(), GET_TID());

    ULONG aulMsg[4];
    UINT32 isock;
	INT32 len_recv_msgq ;    /* receive message's length */
	InetMsg *p_vomsg = NULL; /*the message pointer about inet_mgmt*/
	INT32 len = 0;

    /**
     *  Variables initial
     */
    p_vomsg = (InetMsg*)Malloc(sizeof(InetMsg));
    Memset(p_vomsg, 0, sizeof(InetMsg));

    while(1)
	{
        /**
         *  receive messages from the message queue 
         */
        Memset(aulMsg, 0, sizeof(aulMsg));
		len_recv_msgq = MsgqRecv(inet_module.moduleID, aulMsg);
		if(len_recv_msgq < 0)
		{
			usleep(250000);/* delay 25000 microsecond */
			continue;
		}
        /**
         *  In INET_MGMT module, we determine qmueue message as follows:
         *  ULONG aulMsg[4]:
         *      aulMsg[0]: msg type
         *      aulMsg[1]: msg code
         *      aulMsg[2]: msg length
         *      aulMsg[3]: msg body pointer(64bit memory address)
         */
        /**
         *  Get p_vomsg
         */
        if(aulMsg[2] < sizeof(InetMsg))
        {
            log_inet_err("receive msgqueue msg len too small. need(%d) but is(%ld)\n", sizeof(InetMsg), aulMsg[2]);
            continue;
        }
        Memcpy(p_vomsg, (void*)aulMsg[3], aulMsg[2]);

        log_inet_info("msgfd = %d, ip = 0x%x\n", p_vomsg->fd,p_vomsg->ip);

        switch(p_vomsg->flag)
        {
            /**
             *  Tcp message
             */
            case 1: 
                /**
                 *  find conn_fd through ip
                 */
				if (0 != p_vomsg->fd)
				{
					if(-1==send(p_vomsg->fd, p_vomsg->msg_p, p_vomsg->msg_len, 0))
					{
						log_inet_warn("send tcp message fail, ip=0x%x\n",p_vomsg->ip);
					}
				}
				else
				{
					for(isock=0; isock<MAX_SOCK_NUM; isock++)
					{
						if(p_vomsg->ip == tcp_info[isock].ip)
						{
							if(-1==send(tcp_info[isock].conn_fd,p_vomsg->msg_p,p_vomsg->msg_len,0))
							{
								log_inet_warn("send tcp message fail, ip=0x%x\n",p_vomsg->ip);
							}
							break;
						}
					}
				}
				Free(p_vomsg->msg_p);
                break;
            /**
             *  udp/unsocket/rawsocket
             */
            case 0:
                /**
                 *  find client socket address structure from sock_para
                 */
                for(isock=0;isock<MAX_SOCK_NUM;isock++)
				{
					if(p_vomsg->fd==sock_para[isock].sock_fd)
					{
						if (sock_para[isock].socket_type==UDP)
						{
							log_inet_info("msg_send_fn, msg type is udp\n");
							len = sizeof(struct sockaddr_in);
						}
						else if (sock_para[isock].socket_type==RAWSOCKET)
						{
							log_inet_info("msg_send_fn, msg type is raw\n");
							len = sizeof(struct sockaddr_ll);
						}
						else if (sock_para[isock].socket_type==UNSOCKET)
						{
							log_inet_info("msg_send_fn, msg type is un_sock\n");
							len = sizeof(struct sockaddr_un);
						}
						if (sock_para[isock].socket_type==RAWSOCKET)
						{
							if(-1 == sendto(p_vomsg->fd, p_vomsg->msg_p, p_vomsg->msg_len, 0, 0, 0))
							{
								log_inet_warn("send socket message fail, ip=0x%x\n",p_vomsg->ip);
							}
						}
						else
						{
							if(-1 == sendto(p_vomsg->fd,p_vomsg->msg_p,p_vomsg->msg_len,0,&p_vomsg->clientaddr, len))
							{
								log_inet_warn("send socket message fail, ip=0x%x\n",p_vomsg->ip);
							}
						}
						Free(p_vomsg->msg_p);
						break;
					}
				}
                break;

            /**
             *  No this kind of protocol
             */
            default:
                log_inet_err("No this kind of protocol\n");
                break;
        }
    }

    return NULL;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
void *msg_receive_fn(void*arg)
{
    log_inet_debug();
    log_inet_info("pid(%d), tid(%d)\n", getpid(), GET_TID());

    fd_set readyfd_set;
	UINT32 i,j;
	UINT32 ret;                 /*store the value returned by recvfrom/recv*/
	socklen_t addrlen = 0;      /*the length of socket's address struct.*/
	INT32 connect_fd;
	UINT32 select_num;
	UINT8 buff[MAX_RECV_MSG_LEN]; /*store message form netport*/
	struct timeval timeout = {1,250000};

	/**
	 *  receive every module message,and send out
	 */
    struct sockaddr_in clientaddr;
	struct sockaddr_ll clientaddr_mac;
	struct sockaddr_un clientaddr_mac_un;

    /**
     *  initial the paras
     */
	Memset(&timeout,0,sizeof(struct timeval));
    FD_ZERO(&readyfd_set);

    #if 0
    while(1)
	{
		if (SYS_RUNNING == g_om_state)
		{
			break;
		}
		sleep(5);
	}
    #endif
    
    while(1)
	{
	    /**
	     *  readset is global variable to store all socket fd
         */
	    readyfd_set = readset; 
        
		timeout.tv_sec = 1;
		timeout.tv_usec = 250000;
		select_num = select(maxfd+1, &readyfd_set, NULL, NULL, &timeout);
        
		if (select_num <= 0)
		{
			usleep(500000); /*delay 25000 microsecond*/
			continue;
		}

        for(i=0; i<MAX_SOCK_NUM; i++)
		{
			if(FD_ISSET(listen_fd_info[i].listenfd, &readyfd_set))
			{
				/**
				 *  if the activate socked is listen socket.
				 */
				addrlen = sizeof(clientaddr);
				connect_fd = accept(listen_fd_info[i].listenfd, (struct sockaddr*)&clientaddr, &addrlen);
				FD_SET(connect_fd, &readset);

				for(j = 0; j < MAX_SOCK_NUM; j++)
				{
					if(tcp_info[j].conn_fd == 0)
					{
						tcp_info[j].conn_fd = connect_fd;
						tcp_info[j].socket_type = listen_fd_info[i].socket_type;
                        
						if (TCP == tcp_info[j].socket_type)
						{
							tcp_info[j].ip = clientaddr.sin_addr.s_addr;
						}
						
						tcp_info[j].msgq = listen_fd_info[i].msgq;
						tcp_info[j].pFun = listen_fd_info[i].pFun;
						
						log_inet_info("TCP: new tcp connection from %s on socket %ld\n", 
                                        inet_ntoa(clientaddr.sin_addr), connect_fd);
						break;
					}
				}
				
				/* update max fd value */
				if (connect_fd > maxfd)
				{
					maxfd = connect_fd;
				}
			}
		}

        /** 
         *  receive tcp socket
         */
		for(i=0; i<MAX_SOCK_NUM; i++) 
		{
			if(tcp_info[i].conn_fd != 0 && FD_ISSET(tcp_info[i].conn_fd, &readyfd_set))
			{
				/**
				 *  receive tcp
				 */
				Memset(buff, 0, MAX_RECV_MSG_LEN);
				
				ret = recv(tcp_info[i].conn_fd, buff, MAX_RECV_MSG_LEN,0);
				if(ret==-1 || ret==0)
				{
					log_inet_warn("TCP: recv tcp message fail, conn_fd=%d, ip=0x%x, ret=%d\n",\
					                tcp_info[i].conn_fd,tcp_info[i].ip, ret);
					
					socket_unregister(tcp_info[i].conn_fd);
                    
					continue;
				}
				
                log_inet_info("TCP: receive TCP message ok,fd=%d,length=%d\n",tcp_info[i].conn_fd,ret);
				
				if (tcp_info[i].pFun)
				{
					tcp_info[i].pFun(tcp_info[i].conn_fd, (struct sockaddr_storage *)0, buff, ret);
				}
				else
				{
					if (send_msg_to_module(tcp_info[i].msgq, TCP, buff, ret)!=0)
					{
						log_inet_warn("TCP: send message to msgq %ld\n",tcp_info[i].msgq);
					}
				}
			}
		}


        /**
         *  receive udp/unsocket/rawsocket message 
         */
		for(j=0; j<MAX_SOCK_NUM; j++)
		{
			if(sock_para[j].sock_fd != 0 && FD_ISSET(sock_para[j].sock_fd, &readyfd_set))
			{
				/**
				 *  receive udp  message
				 */
				if(sock_para[j].socket_type == UDP)
				{
					Memset(buff,0,MAX_RECV_MSG_LEN);
					addrlen = sizeof(clientaddr);
					Memset(&clientaddr,0,addrlen);
					
					ret=recvfrom(sock_para[j].sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr,&addrlen);
					if(ret==-1)
					{
						log_inet_warn("UDP: receive socket message fail,socket fd=%d\n",sock_para[j].sock_fd);
						continue;
					}

					log_inet_info("UDP: receive UDP message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
						
					/*send message to every module queue*/
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, (struct sockaddr_storage *)&clientaddr, buff,ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq, sock_para[j].socket_type, buff,ret);	
					}
					
				}
                /**
                 *  Receive raw socket message
                 */
				else if(sock_para[j].socket_type == RAWSOCKET)
				{
					ret=0;
					addrlen=0;
					Memset(buff, 0, MAX_RECV_MSG_LEN);
					addrlen=sizeof(clientaddr_mac);
					Memset(&clientaddr_mac,0,addrlen);
					
					ret = recvfrom(sock_para[j].sock_fd, buff, sizeof(buff), 0, (struct sockaddr*)&clientaddr_mac, &addrlen);
					if (ret < 0)
					{
						log_inet_warn("Receive raw message fail,socket fd=%d\n",sock_para[j].sock_fd);
						continue;
					}
					
					log_inet_info("RAW: receive raw message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
						
					/**
					 *  send message to every module queue
					 */
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, (struct sockaddr_storage *)&clientaddr_mac, buff, ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq, sock_para[j].socket_type, buff, ret);	
					}
					
				}
                /**
                 *  Receive UNIX domain socket message
                 */
				else if(sock_para[j].socket_type == UNSOCKET)
				{
					ret = 0;
					addrlen = 0;
					Memset(buff, 0, MAX_RECV_MSG_LEN);
					addrlen = sizeof(clientaddr_mac_un);
					Memset(&clientaddr_mac_un, 0, addrlen);
					
					ret = recvfrom(sock_para[j].sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&clientaddr_mac_un,&addrlen);
					if (ret<0)
					{
						log_inet_warn("Receive MAC message fail,socket fd=%d\n",sock_para[j].sock_fd);
						continue;
					}
					
					log_inet_info("RAW: receive MAC message ok,fd=%d,length=%d\n",sock_para[j].sock_fd,ret);
						
					/*send message to every module queue*/
					if (sock_para[j].pFun)
					{
						sock_para[j].pFun(sock_para[j].sock_fd, (struct sockaddr_storage *)&clientaddr_mac_un, buff,ret);
					}
					else
					{
						send_msg_to_module(sock_para[j].msgq, sock_para[j].socket_type, buff, ret);	
					}
				}
			}
		}
    }

    return NULL;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 send_msg_to_module(MsgqType msgq/*UINT16 module_id*/,UINT32 socktype,UINT8 *msg_p,UINT32 msg_len)
{
    log_inet_debug();

    ULONG ulMsg[4];

    /**
     *  In INET_MGMT module, we determine qmueue message as follows:
     *  ULONG aulMsg[4]:
     *      aulMsg[0]: msg type
     *      aulMsg[1]: msg code
     *      aulMsg[2]: msg length
     *      aulMsg[3]: msg body pointer(64bit memory address)
     */
    ulMsg[0] = 0;
    ulMsg[1] = 0;
    ulMsg[2] = msg_len;
    ulMsg[3] = (ULONG)msg_p;

    if (0 != MsgqSend(msgq, ulMsg))
	{
		log_inet_err("send message error to msgq %ld\n", msgq);
		return INET_ERR;
	}

    return INET_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,
                        UINT8 *msg_p,UINT32 msg_len)
{
    log_inet_debug();

    InetMsg stInetMsg;
	
  	//UINT32 InetMsg_len = sizeof(InetMsg);   /*msg_len+32;*/
	InetMsg *p_InetMsg = &stInetMsg;//Malloc(InetMsg_len);

    ULONG ulMsg[4];

    /**
     *  find conn_fd through ip
     */
	p_InetMsg->flag     = streamflag;
	p_InetMsg->ip       = ip;
	/*p_InetMsg->module_id = module_id;*/
    
	if (clientaddr)
	{
		Memcpy(&p_InetMsg->clientaddr, clientaddr, sizeof(p_InetMsg->clientaddr));
	}
	
	p_InetMsg->fd       = fd; /*now fd is no use*/
	p_InetMsg->msg_len  = msg_len;
	p_InetMsg->msg_p    = Malloc(msg_len);
    
	if (!p_InetMsg->msg_p)
	{
	    log_inet_err("NULL pointer error.\n");
		return INET_ERR;
	}
	
	Memcpy(p_InetMsg->msg_p, msg_p, msg_len);

    /**
     *  In INET_MGMT module, we determine qmueue message as follows:
     *  ULONG aulMsg[4]:
     *      aulMsg[0]: msg type
     *      aulMsg[1]: msg code
     *      aulMsg[2]: msg length
     *      aulMsg[3]: msg body pointer(64bit memory address)
     */
    ulMsg[0] = 0;
    ulMsg[1] = 0;
    ulMsg[2] = msg_len;
    ulMsg[3] = (ULONG)p_InetMsg;
    
	if(-1 == MsgqSend(inet_module.moduleID, ulMsg))
	{
		log_inet_err("INET_SEND_MSG: send message to inet_mgmt msgq error\n");
		return INET_ERR;
	}
	
	log_inet_info("INET_SEND_MSG: send message to inet_mgmt ok\n");

    return INET_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 log_inet(FILE *fp, LOG_INET_TYPE type, CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...)
{
    /**
     *  Check log type valiable
     */
    if(type <= INET_LOG_TYPE_MIN || type >= INET_LOG_TYPE_MAX)
    {
        va_list arg1;
        va_start(arg1, fmt);
        
        Vfprintf(fp, fmt, arg1);
        
        va_end(arg1);
        
        return INET_WARN;
    }

    /**
     *  Switch the log type
     */
    switch(type)
    {
        case INET_LOG_ERROR:
            Fprintf(fp, "  %s[Error][INET]%s %s:%s:%d ", "\x1b[1;31m", "\x1b[0m", __file, __func, __line);
            break;
        
        case INET_LOG_INFO:
            Fprintf(fp, "   %s[Info][INET]%s %s:%s:%d ", "\x1b[1;34m", "\x1b[0m", __file, __func, __line);
            break;
            
        case INET_LOG_SUCCESS:
            Fprintf(fp, "%s[Success][INET]%s %s:%s:%d ", "\x1b[1;32m", "\x1b[0m", __file, __func, __line);
            break;
        
        case INET_LOG_WARNING:
            Fprintf(fp, "%s[Warning][INET]%s %s:%s:%d ", "\x1b[1;35m", "\x1b[0m", __file, __func, __line);
            break;
        
        case INET_LOG_DEBUG:
            Fprintf(fp, "  %s[Debug][INET]%s %s:%s:%d ", "\x1b[1;37;40m", "\x1b[0m", __file, __func, __line);
            break;

        case INET_LOG_SIMPLE:
            //Do nothing
            break;
        default:
            //TODO
            break;
    }

    /**
     *  log
     */
    va_list arg;
    va_start(arg, fmt);

    Vfprintf(fp, fmt, arg);
    
    va_end(arg);

    return INET_OK;
}


