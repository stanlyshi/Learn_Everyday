/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:45 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * mnt_mgmt_msg_handle.c
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "sbs_timer.h"
#include "sbs_msgq.h"
#include "sbs_task.h"
#include "sbs_log.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "mnt_mgmt_api.h"
#include "om_common_h.h"
#include "fpga_msg_receive_fn.h"
#include "inet_mgmt.h"
#include "msg_receive_fn.h"

Cpu_Time cpu_time_start;
Cpu_Time cpu_time_end;
/*************************************************************************************************

 Description :get cpu server and BMC asset info
 input: NULL
 output: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 get_cpu_server_asset_info()
{
	INT8 buf[100];
	INT8 tmp[50];
	FILE *fp = NULL;
	UINT32 index;
/****************************************get CPU name********************************************/
   if((fp = popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 16","r")) == NULL)
   //if((fp=popen("ipmitool fru print 16","r"))==NULL) //HP
   {
	 // printf("popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 16'ERROR!\n");
	  log_msg(LOG_ERR, BORD_MGMT, "open 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 16'ERROR!\n");
	  pclose(fp);
	  return FUNC_ERR;
   }
   bzero(buf,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp))
   {
	  bzero(tmp,sizeof(tmp));
	  sscanf(buf,"%[^:]",tmp);
	  buf[strlen(buf)] = '$';
	  if(strncmp(tmp," Product Name",13) == 0)
	  {
		 sscanf(buf,"%*[^:]: %[^$]",savecpuassetinfo[0].cpu_name);

	  }
	  bzero(buf,sizeof(buf));
   }
   pclose(fp);

   for(index = 1;index < CpuNum;index++)
  {
   memcpy(savecpuassetinfo[index].cpu_name,savecpuassetinfo[0].cpu_name,48);
  }

/**************************************get BIOS version*******************************************/
   if((fp=popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 239","r")) == NULL)
	   //if((fp=popen("ipmitool fru print 239","r"))==NULL)
   {
	   //printf("popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 239'ERROR!\n");
	   log_msg(LOG_ERR, BORD_MGMT, "open 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 239'ERROR!\n");
	   pclose(fp);
	   return FUNC_ERR;
   }
   bzero(buf,sizeof(buf));
   while(fgets(buf,sizeof(buf),fp))
   {
	   bzero(tmp,sizeof(tmp));
	   sscanf(buf,"%[^:]",tmp);
	   if(strncmp(tmp," Product Version",16) == 0)
	   {
		   buf[strlen(buf)] = '$';
		   sscanf(buf,"%*[^:]: %[^$]",savecpuassetinfo[0].bios_version);
	   }
	   bzero(buf,sizeof(buf));
	  }
	  pclose(fp);

	  for(index = 1;index < CpuNum;index++)
	  {
		  memcpy(savecpuassetinfo[index].bios_version,savecpuassetinfo[0].bios_version,48);
	  }
	  for(index = 0;index < CpuNum;index++)
	  {
	  savecpuassetinfo[index].cpu_id = savecpuid[index].cpu_id;
	  }

/******************************************get bmc name*****************************************/
	if((fp = popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 238","r")) == NULL)
	//if((fp =popen("ipmitool fru print 238","r"))== NULL) HP
	{
		//printf("popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 239'ERROR!\n");
		log_msg(LOG_ERR, BORD_MGMT, "popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng fru print 239'ERROR!\n");
		pclose(fp);
		return FUNC_ERR;
	}
	bzero(buf, sizeof(buf));
	while (fgets(buf, sizeof(buf), fp))
	{
		bzero(tmp, sizeof(tmp));
		sscanf(buf, "%[^:]", tmp);
		if (strncmp(tmp, " Product Name", 13) == 0)
		{
			buf[strlen(buf)] = '$';
			sscanf(buf, "%*[^:]: %[^$]", savebmcassetinfo.bmc_name);
		}
		bzero(buf, sizeof(buf));
	}
	pclose(fp);

/**************************************get bmc ip**********************************************/
	if((fp = popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng lan print 2","r")) == NULL)
	//if((fp=popen("ipmitool lan print 2","r"))==NULL) //HP
	{
		// printf("popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng lan print 2'ERROR!\n");
		log_msg(LOG_ERR, BORD_MGMT, "popen 'ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng lan print 2'ERROR!\n");
		pclose(fp);
		return FUNC_ERR;
	}
	bzero(buf,sizeof(buf));
    while(fgets(buf,sizeof(buf),fp))
    {
    	bzero(tmp,sizeof(tmp));
		sscanf(buf,"%[^:]",tmp);
		if(strncmp(tmp,"IP Address",10) == 0)
		{
			buf[strlen(buf)] = '$';
			sscanf(buf,"%*[^:]: %[^$]",savebmcassetinfo.bmc_addr);
		}
		bzero(buf,sizeof(buf));
    }
    pclose(fp);
    savebmcassetinfo.bmc_id = savecpuserverid.bmc_id;

#ifdef Debug
    for(index = 0;index < CpuNum;index++)
    {
    	log_msg(LOG_INFO, BORD_MGMT, "cpu_name=%s\n",savecpuassetinfo[index].cpu_name);
        log_msg(LOG_INFO, BORD_MGMT, "bios_version=%s\n",savecpuassetinfo[index].bios_version);
    }
    log_msg(LOG_INFO, BORD_MGMT, "bmc_name=%s\n",savebmcassetinfo.bmc_name);
    log_msg(LOG_INFO, BORD_MGMT, "bmc_ip=%s\n",savebmcassetinfo.bmc_addr);
#endif
    return FUNC_OK;
}

/**********************************************************************************************

 Description :get cpu server healthy info
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ************************************************************************************************/
INT32 get_cpu_server_heal_info()
{
	char buf[256];
	FILE *fp = NULL;
	int i = 0;
	UINT32 index;
	bzero(&savecpuhealinfo,0);
	//if((fp=popen("ipmitool sensor list|grep 'CPU'|awk '{print $4,$19}'","r"))==NULL)//HP
	if((fp = popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng sensor list|grep 'CPU'|awk '{print $4,$19}'","r")) == NULL)//HP
	{
		log_msg(LOG_ERR, BORD_MGMT, "open  ipmitool command failed in function get_cpu_server_heal_info()!\n");
		return FUNC_ERR;
	}
	memset(buf,0,sizeof(buf));
	while(fgets(buf,sizeof(buf),fp))
	{
		i++;
		if(i == 1)
		{
		sscanf(buf,"%f %f",&savecpuhealinfo[0].cpu_temp,&savecpuhealinfo[0].curr_cpu_temp_thres);//CPU1
		break;
		}
		/*if(i == 2)
		{
			sscanf(buf,"%f %f",&cpuhealinfo.cpu2_temp,&cpuhealinfo.cpu2_temp_thres);
			break;
		}*/
		memset(buf,0,sizeof(buf));
	}
	pclose(fp);

	for(index = 1;index < CpuNum;index++)
	{
		savecpuhealinfo[index].cpu_temp = savecpuhealinfo[0].cpu_temp;
		savecpuhealinfo[index].curr_cpu_temp_thres = savecpuhealinfo[0].curr_cpu_temp_thres;
	}

	for(index = 0;index < CpuNum;index++)
	{
		savecpuhealinfo[index].cpu_id = savecpuid[index].cpu_id;
	}
	    /********get cpu server fan info********/
	memset(&savecpuserverhealinfo,0,sizeof(SaveCpuServerHealInfo));
	//if((fp=popen("ipmitool sensor list|grep 'Fan 1 DutyCycle'|awk '{print $5}'","r"))==NULL)
	if((fp = popen("ipmitool -I lanplus -H 10.21.2.121 -U login -P wangpeng sensor list|grep 'Fan 1 DutyCycle'|awk '{print $5}'","r"))==NULL)//HP
	{
		log_msg(LOG_ERR, BORD_MGMT, "open  ipmitool command failed in function get_cpu_server_heal_info()!\n");
		return FUNC_ERR;
	}
	memset(buf,0,sizeof(buf));
	while(fgets(buf,sizeof(buf),fp))
	{
		sscanf(buf,"%f",&savecpuserverhealinfo.cpu_server_fan);
		memset(buf,0,sizeof(buf));
	}
	pclose(fp);
	savecpuserverhealinfo.cpu_server_id = savecpuserverid.cpu_server_id;
#ifdef Debug
	for(index = 0;index < CpuNum;index++)
	{
		log_msg(LOG_INFO, BORD_MGMT, "cpu_temp=%f\n",savecpuhealinfo[index].cpu_temp);
	    log_msg(LOG_INFO, BORD_MGMT, "cpu_temp_thres=%f\n",savecpuhealinfo[index].curr_cpu_temp_thres);
    }
	log_msg(LOG_INFO, BORD_MGMT, "cpu_server_fan=%f\n",savecpuserverhealinfo.cpu_server_fan);
#endif
	return FUNC_OK;
}
/**********************************************************************************************

 Description :get memory utilization
 input: NULL
 output: memory utilization


 **********************************************************************************************/
INT32 get_mem_info()
{
	float mem_usage = 0;
	UINT32 free_mem = 0;
	INT8 buff[100];
	INT32 i = 0;
	Mem_Info mem_info;
	memset(&mem_info,0,sizeof(Mem_Info));
	FILE *fp2=NULL;
	INT8 mem_name[30] = "0";
	INT8 Temp_name[30] = "0";
	bzero(buff, sizeof(buff));
	fp2 = fopen("/proc/meminfo", "r");
	if (fp2 == NULL)
	{
		log_msg(LOG_ERR, BORD_MGMT, "open /proc/meminfo failed in function get_mem_info()!\n");
		fclose(fp2);
		return FUNC_ERR;
	}
	while (fgets(buff, sizeof(buff), fp2))
	{
		for (i = 0; buff[i] != ':'; i++)
		{
			Temp_name[i] = buff[i];
		}

		if (strcmp(Temp_name, "MemTotal") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.MemTotal));
		}
		else if (strcmp(Temp_name, "MemFree") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.MemFree));
		}
		else if (strcmp(Temp_name, "Buffers") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.Buffers));
		}
		else if (strcmp(Temp_name, "Cached") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.Cached));
		}
		else if (strcmp(Temp_name, "SwapTotal") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.SwapTotal));
		}
		else if (strcmp(Temp_name, "SwapFree") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.SwapFree));
		}
		else if (strcmp(Temp_name, "Shmem") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.Shmem));
		}
		else if(strcmp(Temp_name, "HugePages_Free") == 0)
		{
			sscanf(buff, "%s%u", mem_name, &(mem_info.HugePages_Free));
		}
		else if(strcmp(Temp_name, "Hugepagesize") == 0)
		{
		  	sscanf(buff, "%s%u", mem_name, &(mem_info.Hugepagesize));
		}
		bzero(Temp_name, sizeof(Temp_name));
		bzero(mem_name, sizeof(mem_name));
		bzero(buff, sizeof(buff));
	}
	fclose(fp2);
	free_mem = mem_info.MemFree+mem_info.Buffers + mem_info.Cached + mem_info.HugePages_Free*mem_info.Hugepagesize;
	mem_usage = (float)(mem_info.MemTotal-free_mem) / mem_info.MemTotal;
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "meminfo_MemTotal = %u,%u,%u,%u,%u,%u,%u\n",mem_info.MemTotal,mem_info.MemFree,mem_info.Buffers,mem_info.Cached,mem_info.SwapTotal,mem_info.SwapFree,mem_info.Shmem);
    log_msg(LOG_INFO, BORD_MGMT, "HugePages_Free = %d,Hugepagesize = %d\n",mem_info.HugePages_Free,mem_info.Hugepagesize);
    log_msg(LOG_INFO, BORD_MGMT, "mem_usage = %f\n",mem_usage);
#endif
    return mem_usage;
}
/***********************************************************************************************

 Description :calculating CPU utilization
 input: NULL
 output:NULL
 return:CPU utilization

 **********************************************************************************************/
float cal_cpuoccupy()
{
	UINT32 od,nd;
	float cpu_use = 0;
	od = (cpu_time_start.user + cpu_time_start.nice + cpu_time_start.system + cpu_time_start.idle + cpu_time_start.iowait + cpu_time_start.irq + cpu_time_start.softirg);
	nd = (cpu_time_end.user + cpu_time_end.nice + cpu_time_end.system + cpu_time_end.idle + cpu_time_end.iowait + cpu_time_end.irq + cpu_time_end.softirg);
	if((nd-od) != 0)
	{
		cpu_use = ((nd - od - (cpu_time_end.idle - cpu_time_start.idle + cpu_time_end.iowait - cpu_time_start.iowait + cpu_time_end.softirg - cpu_time_start.softirg)) / (float)(nd - od));
	}
	else {
		cpu_use = 0;
	}

	return cpu_use;
}
/***********************************************************************************************

 Description :get CPU server computing resource usage:cpu_usage,memory_usage
 input :NULL
 output:NULL
 return:success FUNC_OK
        false FUNC_ERR

 ***********************************************************************************************/
void get_cpu_server_res_usage()
{
	float ret = 0;
	if(get_cpu_end_time() == FUNC_ERR)
	{
	   log_msg(LOG_ERR, BORD_MGMT, "Open /proc/stat failed in function get_cpu_server_res_usage()!\n");
	   return ;
	}
	savecpuservercomputesourceinfo.cpu_usg = cal_cpuoccupy();
	memcpy(&cpu_time_start,&cpu_time_end,sizeof(Cpu_Time));
 //get memory  usage
	ret = get_mem_info();
	if(ret != -1)
	 {
		 savecpuservercomputesourceinfo.mem_usg = ret;
	 }
	savecpuservercomputesourceinfo.cpu_server_id = savecpuserverid.cpu_server_id;
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "cpu_usg=%f\n",savecpuservercomputesourceinfo.cpu_usg);
#endif
}

/************************************************************************************************

 Description :get cpu used time from /proc/stat,at first time.
 input: NULL
 output: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 get_cpu_start_time()
{
	INT8 buf[256];
	FILE *fp = NULL;

	if((fp = fopen("/proc/stat","r")) == NULL)
	{
		log_msg(LOG_ERR, BORD_MGMT, "Open /proc/stat failed in function get_cpu_start_time()!\n");
		fclose(fp);
		return FUNC_ERR;
	}
	   memset(buf,0,sizeof(buf));
	   memset(&cpu_time_start,0,sizeof(cpu_time_start));
	   fgets(buf,sizeof(buf),fp);
	   sscanf(buf,"%s %lu %lu %lu %lu %lu %lu %lu",cpu_time_start.cpu_name,&(cpu_time_start.user),&(cpu_time_start.nice),&(cpu_time_start.system),&(cpu_time_start.idle),&(cpu_time_start.iowait),&(cpu_time_start.irq),&(cpu_time_start.softirg));
#ifdef Debug
	   log_msg(LOG_INFO, BORD_MGMT, "cpu_time_start.cpu_name=%s,%lu,%lu,%lu,%lu\n",cpu_time_start.cpu_name,cpu_time_start.user,cpu_time_start.nice,cpu_time_start.system,cpu_time_start.idle);
#endif
	   fclose(fp);
	   return FUNC_OK;
}
/**********************************************************************************************

 Description :get cpu used time from /proc/stat,at second time.
 input : NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **********************************************************************************************/
INT32 get_cpu_end_time()
{
	INT8 buf[256];
	FILE *fp = NULL;
	if((fp = fopen("/proc/stat","r")) == NULL)
	{
		log_msg(LOG_ERR, BORD_MGMT, "open /proc/stat failed in function get_cpu_end_time()!\n");
		fclose(fp);
		return FUNC_ERR;
	}
	memset(buf,0,sizeof(buf));
	memset(&cpu_time_end,0,sizeof(cpu_time_end));
    fgets(buf,sizeof(buf),fp);
    sscanf(buf,"%s %lu %lu %lu %lu %lu %lu %lu",cpu_time_end.cpu_name,&(cpu_time_end.user),&(cpu_time_end.nice),&(cpu_time_end.system),&(cpu_time_end.idle),&(cpu_time_end.iowait),&(cpu_time_end.irq),&(cpu_time_end.softirg));
#ifdef Debug
    log_msg(LOG_INFO, BORD_MGMT, "cpu time:%s,%u,%u,%u,%u\n",cpu_time_end.cpu_name,cpu_time_end.user,cpu_time_end.nice,cpu_time_end.system,cpu_time_end.idle);
#endif
    fclose(fp);
    return FUNC_OK;
}

/************************************************************************************************

 Description :converts the character type to an integer
 input: a character
 output:NULL
 return:success integer
         false FUNC_ERR

 ************************************************************************************************/
INT32 char_trans_int(char ch)
{
	if(ch >= '0' && ch <= '9')
	{
		return ch - '0';
	}
	if(ch >= 'A' && ch < 'F')
	{
		return (ch-'A') + 10;
	}
	if(ch >= 'a' && ch <= 'f')
	{
		return (ch-'a') + 10;
	}
	return FUNC_ERR;
}
