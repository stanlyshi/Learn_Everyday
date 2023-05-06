/**
 *  获取系统RAM使用率
 *  
 *  日期：2020年10月16日
 *  作者：荣涛
 */
#include <linux/kernel.h>
#include <linux/sysinfo.h>
#include <stdio.h>
#include <sys/sysinfo.h>

/**
 *  struct ram_occupy - RAM使用情况结构体
 *  
 *  具体参数见参数说明
 */
struct ram_occupy {
    unsigned long totalRamMemMb;    /* 总的可用主内存大小 */
    unsigned long freeRamMemMb;     /* 可用的内存大小 */
    unsigned long totalSwapMb;      /* 总交换空间大小 */
    unsigned long freeSwapMb;       /* 可用的交换空间大小 */
    struct {
        int integer;    //使用率的整数部分
        int decimal;    //使用率的小数部分
    }occupyRam, occupySwap;         /* 使用率；按   80.23 % 记 */
};

/**
 *  get_ram_usage - 获取RAM使用率情况
 *  
 *  param[in]   info: 见结构 struct ram_occupy 详解
 *  
 */
void get_ram_usage(struct ram_occupy *info)
{
	/* Conversion constants. */
	const double megabyte = 1024 * 1024;
    /* Obtain system statistics. */
	struct sysinfo si;
	sysinfo (&si);

    info->totalRamMemMb = (unsigned long)(si.totalram / megabyte);
    info->freeRamMemMb = (unsigned long)(si.freeram / megabyte);
    info->totalSwapMb = (unsigned long)(si.totalswap / megabyte);
    info->freeSwapMb = (unsigned long)(si.freeswap / megabyte);

    double occupyRam = (info->totalRamMemMb - info->freeRamMemMb)*1.0/info->totalRamMemMb;
    double occupySwap = (info->totalSwapMb - info->freeSwapMb)*1.0/info->totalSwapMb;

    info->occupyRam.integer = (int)(occupyRam*100);
    info->occupyRam.decimal = (int)(occupyRam*10000/100);
    info->occupySwap.integer = (int)(occupySwap*100);
    info->occupySwap.decimal = (int)(occupySwap*10000/100);


}


int main ()
{
    struct ram_occupy info;
    
    get_ram_usage(&info);

    printf("RAM:  %d.%d%% \n", info.occupyRam.integer, info.occupyRam.decimal);
    printf("Swap: %d.%d%% \n", info.occupySwap.integer, info.occupySwap.decimal);


	return 0;
}
