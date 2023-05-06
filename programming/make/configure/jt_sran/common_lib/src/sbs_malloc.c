/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/**************************************************************************** 
 ** Copyright (c) 2005-2008 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: lte_malloc.c
 **
 ** Description: ...
 ** Replace the malloc, free, 
 ** and record the info about malloc&free has been used 
 **
 ** Current Version: 1.0
 **
 ** Author: Yuxin Yang (yangyuxin@ict.ac.cn)
 **
 ** Date: 06.10.29
*****************************************************************************/ 



/* Dependencies ------------------------------------------------------------- */
#include <memory.h>
#include <sbs_err.h>
#include <sbs_file.h>
#include <sbs_malloc.h>

#include <sbs_type.h>
#include "sbs_list.h"
#include "sbs_malloc.h"
#include "sbs_file.h"
#include "sbs_err.h"
/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

ListType g_mem_alloc_free_info;
ListType g_mem_free_erro_info;
pthread_mutex_t g_malloc_mutexlock = PTHREAD_MUTEX_INITIALIZER;

int remain_count = 0;     /*  record the nodes still in the list after free   */
int free_remain = 0;
int my_malloc_used = 0;   /*  record the number that the malloc has been used */
int my_free_used = 0;     /*  record the number that the free has been used   */
static char *file_path = "mem_record.txt";

/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
 * Replace the function of malloc(size)
 *
 * Input:  size: the size of the memory which user want to malloc
 *        pFile: to record name of the file in which malloc() had 
 *                  been used
 *         line: to record the line number on which malloc() had 
 *                  been used
 *                     
 * Output: return void*, the memory address that user had malloced
 *                   -1, error in malloc()
 ******************************************************************************/
void *test_malloc(int size, char *pFile, int line)
{

	void *malloc_p = NULL; /*the address of memory malloc from system.*/
	int file_len = 1;	/* the length of the file-name */
	char* counter = pFile;/* temp counter to count the length of file-name */
    MemoInfoNode *MemoInfoNode_p = NULL;/* record the info of mem assigning */
    
    /*count the length of pFile.*/
	while (*counter != '\0') {
		counter++;
		file_len++;
	}
	pthread_mutex_lock(&g_malloc_mutexlock);
	
	/*malloc mem.*/
	malloc_p = malloc(sizeof(MemoInfoNode) + size + file_len);
	MemoInfoNode_p = (MemoInfoNode*)malloc_p;
//	memset(MemoInfoNode_p, 0, sizeof(MemoInfoNode));
	
	MemoInfoNode_p->total = sizeof(MemoInfoNode) + size + file_len;
	MemoInfoNode_p->size = size;
	MemoInfoNode_p->malloc_count++;
	
	MemoInfoNode_p->line = line;
	memcpy((UINT8 *)malloc_p + sizeof(MemoInfoNode) + size, pFile, file_len);
	
	add_list(&g_mem_alloc_free_info, (NodeType*)MemoInfoNode_p);
	my_malloc_used++;

	pthread_mutex_unlock(&g_malloc_mutexlock);
	return (malloc_p + sizeof(MemoInfoNode));
}

/*******************************************************************************
 * Replace the function of free(mem_p)
 *
 * Input: mem_P: pointer to the address which should be released
 *
 * Output: 
 ******************************************************************************/
void test_free(void *mem_p, char *pFile, int line)
{

	int file_len = 1; 	/* the length of the file-name */
	char* counter = NULL;	/* temp counter to count the length of file-name */
	FreeNode *free_node_p = NULL;
	void *free_p, *free_node_adress_p = NULL;
	MemoInfoNode *node_p = NULL;
	pthread_mutex_lock(&g_malloc_mutexlock);
	
	/*calculate actual pointer malloc from system.*/
	if (mem_p) {
		free_p = ((UINT8 *)mem_p - sizeof(MemoInfoNode));
		node_p = (MemoInfoNode *)free_p;
	}
	if ((!mem_p) || (node_p->malloc_count != 1)) {
		/*released twice.*/
		counter = pFile;
		while (*counter != '\0') {
			counter++;
			file_len++;
		}
		free_node_adress_p = malloc(sizeof(FreeNode) + file_len);
		free_node_p = (FreeNode*)free_node_adress_p;
		memcpy((UINT8 *)free_node_adress_p + sizeof(FreeNode), pFile, file_len);
		free_node_p->line = line;
		add_list(&g_mem_free_erro_info, (NodeType*)free_node_p);
	} else {
		/*release.*/
		delete_list((ListType *)&g_mem_alloc_free_info,
								(NodeType *)node_p);
		node_p->malloc_count--;
	}
	my_free_used++;
	pthread_mutex_unlock(&g_malloc_mutexlock);
	return;
}
/*******************************************************************************
 * print out the info which reflects the malloc/free used 
 * the message has been written into a .txt doc
 *
 * Input: 
 *
 * Output: return Return_ALL_RIGHT, successfully done
 * 		 -1, error in print_mem_info
 ******************************************************************************/
int print_mem_info(void)
{
	MemoInfoNode *node_p = NULL;
	FreeNode *f_p = NULL;
	FILE *file_p = NULL;
	printf("malloc() used %d, free() used %d \n",
				my_malloc_used, my_free_used);
	pthread_mutex_lock(&g_malloc_mutexlock);
	
	if ((file_p = open_file(file_path,"w+")) == NULL) {
		printf("File open error! \n");
		return Return_FAIL;
	}				
	
	/*print mem leaked infomation*/
	if (g_mem_alloc_free_info.count == 0) {
		printf("All the memory that has been assigned has been released!\n");
	} else {
		if (printf_file(file_p, "Malloc used %d, Free used %d \n", 
					my_malloc_used, my_free_used) < 0) {
			printf("Write in File error! \n");
			return Return_FAIL;
		}

		remain_count = g_mem_alloc_free_info.count;
		printf("remain:%d\n",remain_count);
		node_p = (MemoInfoNode*)get_list(&g_mem_alloc_free_info);
		while (node_p) {
			if (printf_file(file_p,"Mem leaked in file %s, lines in %d\n",
			(char *)((char *)node_p + sizeof(MemoInfoNode) + node_p->size), 
					node_p->line) < 0) {
				printf("Write in File error! \n");
				return Return_FAIL;
			}
			free(node_p);
			node_p = (MemoInfoNode*)get_list(&g_mem_alloc_free_info);
		}
	}
	
	/*print double free infomation.*/
	if (g_mem_free_erro_info.count == 0) {
		printf(
			"There is no mem which has been freed 2 times\n");
	} else {
		free_remain = g_mem_free_erro_info.count;
		f_p = (FreeNode*)get_list(&g_mem_free_erro_info);
		while (f_p) {        
			if (printf_file(file_p,
					"Mem freed 2 times in file %s, lines in %d\n", 
					(char *)((void *)f_p + sizeof(FreeNode)), f_p->line) < 0) {
				printf("Write in File error! \n");
				return Return_FAIL;
			}
			free(f_p);
			f_p = (FreeNode*)get_list(&g_mem_free_erro_info);
		}
	}

	if (printf_file(file_p, "The number of node still in MeM list is %d \n", 
				remain_count) < 0) {
		printf("Remain_count writen in File error! \n");
		return Return_FAIL;
	}
	if (printf_file(file_p, "The number of node freed 2times is %d \n", 
				free_remain) < 0) {
		printf("Remain_count writen in File error! \n");
		return Return_FAIL;
	}
	if (close_file(file_p) < 0) {
		printf("Close File Operation error! \n");
		return Return_FAIL;
	}
	printf("The number of mem not freed is %d \n", remain_count);
    printf("Number of node that has been freed 2 times is %d\n", free_remain);
	pthread_mutex_unlock(&g_malloc_mutexlock);
	return Return_ALL_RIGHT;
}
