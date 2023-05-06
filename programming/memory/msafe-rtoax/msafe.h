/**
 *  内存安全使用接口
 *  
 *  荣涛  2020年11月23日
 */
#ifndef __MSAFE_H
#define __MSAFE_H 1


enum msafe_status {
	MSAFE_DISABLED = -1,	//检查未打开
	MSAFE_OK,				//OK
	MSAFE_FREE,			//Double Free错误
	MSAFE_HEAD,			//堆内存块的头部越界
	MSAFE_TAIL,			//堆内存块的尾部越界
};

const char *const msafe_strerror(enum msafe_status status);

//TODO


#endif /*<__MSAFE_H>*/