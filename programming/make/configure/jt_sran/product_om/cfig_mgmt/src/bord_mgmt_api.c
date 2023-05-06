/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */

#if 0
#include <sbs_type.h>
INT32 CTRL_FPGA_Device_Get_produce_type(UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Device_Get_board_type(UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Device_Get_fpga_server_name(char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_Device_Get_fpga_server_sn(char *valne,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_Device_Get_fpga_server_version(char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_Device_Get_fpga_server_manuf_data(char *value,UINT32 ulength)
{
	return 0;
}

/*fpga ----asset information get api*/
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_id(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_DeviveInfo_Get_fpga_slot(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_name(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_version(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_sn(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_FPGA_DeviceInfo_Get_fpga_fw_version(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
/*fpga server----heath_thres information get api*/
INT32 CTRL_FPGA_Healthres_Get_fpga_server_id(UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_fpga_server_fan_thres(UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_old_fpga_server_fan_thres(UINT32 *value)
{
	return 0;
}
/*fpga server----heath information get api*/
INT32 CTRL_FPGA_Health_Get_fpga_server_id(UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Health_Get_fpga_server_fan(UINT32 *value)
{
	return 0;
}
/*fpga----heath_thres information get api*/
INT32 CTRL_FPGA_Healthres_Get_fpga_id(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_fpga_temp_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_old_fpga_temp_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_fpga_vol_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Healthres_Get_old_fpga_vol_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
/*fpga----heath information get api*/
INT32 CTRL_FPGA_HealthInfo_Get_fpga_id(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_HealthInfo_Get_fpga_vol(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_HealthInfo_Get_fpga_temp(UINT32 index,UINT32 *value)
{
	return 0;
}
/*fpga server----clock module information get api*/

INT32 CTRL_FPGA_Clock_Get_PosModeCfg(char *value)
{
	return 0;
}
INT32 CTRL_FPGA_Clock_Get_old_PosModeCfg(char *value)
{
	return 0;
}
INT32 CTRL_FPGA_Clock_Get_CloSreCfg(char *value)
{
	return 0;
}
INT32 CTRL_FPGA_Clock_Get_old_CloSreCfg(char *value)
{
	return 0;
}
/*fpga server----power state get api*/

INT32 CTRL_FPGA_Power_Get_fpgaServerPowerCtl(char *value)
{
	return 0;
}
INT32 CTRL_FPGA_Power_Get_old_fpgaServerPowerCtl(char *value)
{
	return 0;
}
INT32 CTRL_FPGA_Power_Get_fpgaPowerCtl(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_FPGA_Power_Get_old_fpgaPowerCtl(UINT32 index,UINT32 *value)
{
	return 0;
}
/*cpu  server---asset information get api*/

INT32 CTRL_CPU_Device_Get_cpu_id(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_cpu_name(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_bios_version(UINT32 index,char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_bmc_id(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_bmc_name(char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_bmc_fw_version(char *value,UINT32 ulength)
{
	return 0;
}
INT32 CTRL_CPU_Device_Get_bmc_addr(char *addr,UINT32 ulength)
{
	return 0;
}
/*cpu  server---health information get api*/

INT32 CTRL_CPU_Health_Get_cpu_server_id(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Health_Get_cpu_server_fan_thres(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Health_Get_old_cpu_server_fan_thres(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Health_Get_cpu_server_fan(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_cpu_id(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_cpu_temp_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_old_cpu_temp_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_cpu_temp(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_cpu_vol_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_old_cpu_vol_thres(UINT32 index,UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_HealthInfo_Get_cpu_vol(UINT32 index,UINT32 *value)
{
	return 0;
}
/*cpu  server---computing information get api*/

INT32 CTRL_CPU_Cal_Get_cpu_server_id(UINT32 *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_cpu_usg_thres(float *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_old_cpu_usg_thres(float *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_cpu_usg(float *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_mem_usg_thres(float *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_old_mem_usg_thres(float *value)
{
	return 0;
}
INT32 CTRL_CPU_Cal_Get_mem_usg(float *value)
{
	return 0;
}

#endif




