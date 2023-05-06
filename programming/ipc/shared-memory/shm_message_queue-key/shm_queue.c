/*
 * shm_queue.c
 * Implementation of a shm queue
 *
 *  Created on: 2016.7.10
 *  Author: WK <18402927708@163.com>
 *
 *  Based on implementation of transaction queue 基于事务队列的实现
 */
#include <stdint.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <dirent.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
       

#include "shm_queue.h"

#define START_TOKEN    0x0000db03 // token to martk the valid start of a node

#define MAX_READER_PROC_NUM	64 // maximum allowable processes to be signaled when data arrived

#define CAS32(ptr, val_old, val_new)({ char ret; __asm__ __volatile__("lock; cmpxchgl %2,%0; setz %1": "+m"(*ptr), "=q"(ret): "r"(val_new),"a"(val_old): "memory"); ret;})

static char errmsg[256];

const char *shmqueue_errorstr()
{
	return errmsg;
}

struct __shmqueue_node_head_t
{
	uint32_t start_token; // 0x0000db03, if the head position is corrupted, find next start token
	uint32_t datalen; // length of stored data in this node
	struct timeval enqueue_time;

	// the actual data are stored here 真实的数据存储在这里
	unsigned char data[0];

} __attribute__((packed));

struct __shmqueue_head_t
{
	int ele_size;
	int ele_count;

    unsigned long shm_key;  //need a key rongtao: TODO
    int shmid; //shm id
    
	volatile int head_pos; // head position in the queue, pointer for reading
	volatile int tail_pos; // tail position in the queue, pointer for writting

	int data_signum; // signum to send to the reader processes if requested
	int sig_node_num; // send signal to processes when data node excceeds this count
	int sig_process_num; // send signal to up to this number of processes each time

	volatile int pidnum; // number of processes currently registered for signal delivery 
	volatile pid_t pidset[MAX_READER_PROC_NUM]; // registered pid list
	volatile uint8_t sigmask[(MAX_READER_PROC_NUM+7)/8]; // bit map for pid waiting on signal
    
	struct __shmqueue_node_head_t nodes[0];
};

// Increase head/tail by val
#define SQ_ADD_HEAD(queue, val) 	(((queue)->head_pos+(val))%((queue)->ele_count+1))
#define SQ_ADD_TAIL(queue, val) 	(((queue)->tail_pos+(val))%((queue)->ele_count+1))

// Next position after head/tail
#define SQ_NEXT_HEAD(queue) 	SQ_ADD_HEAD(queue, 1)
#define SQ_NEXT_TAIL(queue) 	SQ_ADD_TAIL(queue, 1)

#define SQ_ADD_POS(queue, pos, val)     (((pos)+(val))%((queue)->ele_count+1))

#define SQ_IS_QUEUE_FULL(queue) 	(SQ_NEXT_TAIL(queue)==(queue)->head_pos)
#define SQ_IS_QUEUE_EMPTY(queue)	((queue)->tail_pos==(queue)->head_pos)

#define SQ_EMPTY_NODES(queue) 	(((queue)->head_pos+(queue)->ele_count-(queue)->tail_pos) % ((queue)->ele_count+1))
#define SQ_USED_NODES(queue) 	((queue)->ele_count - SQ_EMPTY_NODES(queue))

#define SQ_EMPTY_NODES2(queue, head) (((head)+(queue)->ele_count-(queue)->tail_pos) % ((queue)->ele_count+1)) 
#define SQ_USED_NODES2(queue, head) ((queue)->ele_count - SQ_EMPTY_NODES2(queue, head))

// The size of a node
#define SQ_NODE_SIZE_ELEMENT(ele_size)	(sizeof(struct __shmqueue_node_head_t)+ele_size)
#define SQ_NODE_SIZE(queue)            	(SQ_NODE_SIZE_ELEMENT((queue)->ele_size))

// Convert an index to a node_head pointer
#define SQ_GET(queue, idx) ((struct __shmqueue_node_head_t *)(((char*)(queue)->nodes) + (idx)*SQ_NODE_SIZE(queue)))

// Estimate how many nodes are needed by this length
#define SQ_NUM_NEEDED_NODES(queue, datalen) 	((datalen) + sizeof(struct __shmqueue_node_head_t) + SQ_NODE_SIZE(queue) -1) / SQ_NODE_SIZE(queue)


// optimized gettimeofday
#include "opt_time.h"

static inline int is_pid_valid(pid_t pid) //如果有一个进程就会在proc文件夹中建立一个以pid为名称的文件夹
{
	if(pid==0) return 0;

	char piddir[256];
	snprintf(piddir, sizeof(piddir), "/proc/%u", pid);
	DIR *d = opendir(piddir);
	if(d==NULL)
		return 0;
	closedir(d);
	return 1;
}

static inline void verify_and_remove_bad_pids(struct __shmqueue_head_t *sq)
{
	int i;
	int oldpidnum = (int)sq->pidnum;
	int newpidnum = oldpidnum;
	// test and remove invalid pids so that they won't be signaled
	if(newpidnum<0 || newpidnum>MAX_READER_PROC_NUM)
	{
		newpidnum = MAX_READER_PROC_NUM;
		if(!CAS32(&sq->pidnum, oldpidnum, newpidnum))
			return;
	}
	for(i=newpidnum-1; i>=0 && !is_pid_valid((pid_t)sq->pidset[i]); i--)
	{
		shmqueue_sigoff(sq, i);
		if(!CAS32(&sq->pidnum, i+1, i)) // conflict detected
			break;
	}
	for(i--; i>=0; i--)
	{
		pid_t oldpid = (pid_t)sq->pidset[i];
		if(!is_pid_valid(oldpid))
		{
			shmqueue_sigoff(sq, i);
			CAS32(&sq->pidset[i], oldpid, 0); // if conflict occurs, simply ignore it
		}
	}
}


// Set signal parameters if you wish to enable signaling on data write
// Parameters:
//      sq           - shm_queue pointer returned by shmqueue_create
//      signum       - sig num to be sent to the reader, e.g. SIGUSR1
//      sig_ele_num  - only send signal when data element count exceeds sig_ele_num
//      sig_proc_num - send signal to up to this number of processes once
// Returns 0 on success, < 0 on failure
int shmqueue_set_sigparam(struct __shmqueue_head_t *sq, int signum, int sig_ele_num, int sig_proc_num)
{
    if(!sq)
    {
        snprintf(errmsg, sizeof(errmsg), "NULL sq_t value.\n");
	    return -1;
    }
    
	sq->data_signum = signum;
	sq->sig_node_num = sig_ele_num;
	sq->sig_process_num = sig_proc_num;
	verify_and_remove_bad_pids(sq);

	if(sq->pidnum>0) // print the registered pids
	{
		int i;
		printf("Registered pids:\n");
		for(i=0; i<sq->pidnum; i++)
            if(sq->pidset[i])
			    printf("%-6u %s", (uint32_t)sq->pidset[i], (i+1)%5==0?"\n":"");
		printf("\n");
	}

	return 0;
}

// Register the current process ID, so that it will be able to recived signal
// Note: you don't need to unregister the current process ID, it will be removed
// automatically next time register_signal is called if it no longer exists
// Parameters:
//      sq  - shm_queue pointer returned by shmqueue_open
// Returns a signal index for shmqueue_sigon/shmqueue_sigoff, or < 0 on failure
int shmqueue_register_signal(struct __shmqueue_head_t *sq)
{
	pid_t pid = getpid();
	verify_and_remove_bad_pids(sq);

	int i;
	for(i=0; i<sq->pidnum; i++)
	{
		if(!sq->pidset[i])
		{
			// if i is taken by someone else, try next
			// else set pidset[i] to our pid and return i
			if(CAS32(&sq->pidset[i], 0, pid))
				return i;
		}
	}

	while(1) // CAS loop
	{
		int pidnum = (int)sq->pidnum;
		if(pidnum>=MAX_READER_PROC_NUM)
		{
			snprintf(errmsg, sizeof(errmsg), "pid num exceeds maximum of %u", MAX_READER_PROC_NUM);
			return -1;
		}
		if(CAS32(&sq->pidnum, pidnum, pidnum+1))
		{
			sq->pidset[pidnum] = (volatile pid_t)pid;
			return pidnum;
		}
	}
}


// Turn on/off signaling for current process
// Parameters:
//      sq  - shm_queue pointer returned by shmqueue_open
//      sigindex - returned by shmqueue_register_signal()
// Returns 0 on success, -1 if parameter is bad
int shmqueue_sigon(struct __shmqueue_head_t *sq, int sigindex)
{
	if((uint32_t)sigindex<(uint32_t)sq->pidnum)
	{
		__sync_fetch_and_or(sq->sigmask+(sigindex/8), (uint8_t)1<<(sigindex%8)); //把指定的位置为1
		return 0;
	}
	snprintf(errmsg, sizeof(errmsg), "sigindex is invalid");
	return -1;
}

int shmqueue_sigoff(struct __shmqueue_head_t *sq, int sigindex)
{
	if((uint32_t)sigindex<(uint32_t)sq->pidnum)
	{
		__sync_fetch_and_and(sq->sigmask+(sigindex/8), (uint8_t)~(1U<<(sigindex%8)));//把指定的位置为0
		return 0;
	}
	snprintf(errmsg, sizeof(errmsg), "sigindex is invalid");
	return -1;
}


// shm operation wrapper  
static char *attach_shm(long iKey, long iSize, int iFlag)
{
	int shmid;
	char* shm;

	if((shmid=shmget(iKey, iSize, iFlag)) < 0)
	{
		printf("shmget(key=%ld, size=%ld): %s\n", iKey, iSize, strerror(errno)); 
		return NULL;
	}

	if((shm = shmat(shmid, NULL ,0))==(char *)-1)
	{
		perror("shmat");
		return NULL;
	}

    /* save shmID */
    struct __shmqueue_head_t *_shm = (struct __shmqueue_head_t *)shm;
    _shm->shmid = shmid;
    
	// avoid systemOS swapping this memory
	/*if(mlock(shm, iSize)<0)
	{
		perror("mlock");
		shmdt(shm);
		return NULL;
	}*/
    
	return shm;
}

// shm operation wrapper  
static struct __shmqueue_head_t *open_shm_queue(long shm_key, long ele_size, long ele_count, int create)
{
	long allocate_size;
	struct __shmqueue_head_t *shm;

	if(create)
	{
		ele_size = (((ele_size + 7)>>3) << 3); // align to 8 bytes (ele_size+7)&~7;
		// We need an extra element for ending control
		allocate_size = sizeof(struct __shmqueue_head_t) + SQ_NODE_SIZE_ELEMENT(ele_size)*(ele_count+1);
		// Align to 4MB boundary
		allocate_size = (allocate_size + (4UL<<20) - 1) & (~((4UL<<20)-1));  //4M对齐
		printf("shm size needed for queue: %lu.\n", allocate_size);
	}
	else
	{
		allocate_size = 0;
	}

	if (!(shm = (struct __shmqueue_head_t *)attach_shm(shm_key, allocate_size, 0666)))
	{
		if (!create) return NULL;
		if (!(shm = (struct __shmqueue_head_t *)attach_shm(shm_key, allocate_size, 0666|IPC_CREAT)))
			return NULL;

		memset(shm, 0, allocate_size);
		shm->ele_size = ele_size;
		shm->ele_count = ele_count;
		return shm;
	}
	else if(create) // verify parameters if open for writing 
	{
		if(shm->ele_size!=ele_size || shm->ele_count!=ele_count) 
		{
            snprintf(errmsg, sizeof(errmsg), "Shm already exist.\n");
            
			printf("shm parameters mismatched: \n");
			printf("    given:  ele_size=%ld, ele_count=%ld\n", ele_size, ele_count);
			printf("    in shm: ele_size=%d, ele_count=%d\n", shm->ele_size, shm->ele_count);

#if 1           
			shmdt(shm);
			return NULL;
#else
            return shm;
#endif
		}
	}

	return shm;
}


sq_t shmqueue_create(uint64_t shm_key, int ele_size, int ele_count)
{
	struct __shmqueue_head_t *queue;

	if(ele_size<=0 || ele_count<=0 || shm_key<=0) // invalid parameter
	{
		snprintf(errmsg, sizeof(errmsg), "Bad argument");
		return NULL;
	}
	queue = open_shm_queue(shm_key, ele_size, ele_count, 1);
	if(queue==NULL)
	{
		snprintf(errmsg, sizeof(errmsg), "Get shm failed");
		return NULL;
	}
	return queue;
}

int shmqueue_exist(uint64_t shm_key)
{
#define _IPCS_CMD   "ipcs -m | grep %#08lx | awk '{print $1}'"

    char _key[20] = {0}, _in_key[20] = {0}, _cmd[256] = {0};

    sprintf(_cmd, _IPCS_CMD, shm_key);
    
    FILE *fp = popen(_cmd, "r");

    fgets(_key, strlen(_key), fp);
    sprintf(_in_key, "%#08lx", shm_key);
    
    if(strcmp(_key, _in_key) == 0)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 1;
}


// Open an existing shm queue for reading data
sq_t shmqueue_open(uint64_t shm_key)
{
	struct __shmqueue_head_t *queue = open_shm_queue(shm_key, 0, 0, 0);
	if(queue==NULL)
	{
		snprintf(errmsg, sizeof(errmsg), "Open shm failed");
		return NULL;
	}
	return queue;
}

// Destroy TP created by shmqueue_create()
void shmqueue_destroy(sq_t queue)
{
    if(!queue)
    {
        snprintf(errmsg, sizeof(errmsg), "NULL sq_t value.\n");
	    return;
    }
    
	shmdt(queue);
	// do nothing for now
}


// Add data to end of shm queue
// Returns 0 on success or
//     -1 - invalid parameter
//     -2 - shm queue is full
int shmqueue_put(sq_t queue, void *data, int datalen)
{
    if(!queue)
    {
        snprintf(errmsg, sizeof(errmsg), "NULL sq_t value.\n");
	    return -1;
    }
    
	uint32_t idx;
	struct __shmqueue_node_head_t *node;
	int nr_nodes;
	int new_tail;
    
    // Maximum bytes allowed for a queue data 队列数据所允许的最大字节数
#define MAX_SHMQUEUE_DATA_LENGTH	65536   //2^16

	if(queue==NULL || data==NULL || datalen<=0 || datalen>MAX_SHMQUEUE_DATA_LENGTH)
	{
		snprintf(errmsg, sizeof(errmsg), "Bad argument");
		return -1;
	}

	// calculate the number of nodes needed   计算数据需要多少块
	nr_nodes = SQ_NUM_NEEDED_NODES(queue, datalen);

	if(SQ_EMPTY_NODES(queue)<nr_nodes)
	{
		snprintf(errmsg, sizeof(errmsg), "Not enough for new data");
		return -2;
	}

	idx = queue->tail_pos;
	node = SQ_GET(queue, idx);
	new_tail = SQ_ADD_TAIL(queue, nr_nodes);

	if(new_tail < queue->tail_pos) // wrapped back  //如果出现反包
	{
		// We need a set of continuous nodes
		// So skip the empty nodes at the end, and begin allocation at index 0
		idx = 0;
		new_tail = nr_nodes;
		node = SQ_GET(queue, 0);

		if(queue->head_pos-1 < nr_nodes) //head_pos标识的是空闲的包个数 tail_pos 标识的是使用的包个数
		{
			snprintf(errmsg, sizeof(errmsg), "Not enough for new data");
			return -2; // not enough empty nodes
		}
	}

	// initialize the new node
	node->start_token = START_TOKEN;
	node->datalen = datalen;
	opt_gettimeofday(&node->enqueue_time, NULL);  //插入节点时候的时间
	memcpy(node->data, data, datalen);
	queue->tail_pos = new_tail;

	// now signal the reader wait on queue
	if(queue->data_signum && // needs signaling    信号触发被设置而且 当已经使用的节点数超高了信号要求的节点数
		SQ_USED_NODES(queue)>=queue->sig_node_num) // element num reached
	{
		int i, nr;
		// signal at most queue->sig_process_num processes
		for(i=0,nr=0; i<(int)queue->pidnum && nr<queue->sig_process_num; i++) //分别给不同的进程发信号
		{
			if(queue->pidset[i] && queue->sigmask[i/8] & 1<<(i%8))
			{
				kill((pid_t)queue->pidset[i], SIGUSR1);
				nr ++;
				shmqueue_sigoff(queue, i); // avoids being signaled again
			}
		}
	}
	return 0;
}

int shmqueue_get_usage(sq_t queue)
{
	return queue->ele_count? ((SQ_USED_NODES(queue))*100)/queue->ele_count : 0;
}

int shmqueue_get_used_blocks(sq_t queue)
{
	return SQ_USED_NODES(queue);
}

// Retrieve data
// On success, buf is filled with the first queue data
// Returns the data length or
//     0  - no data in queue
//     -1 - invalid parameter
int shmqueue_get(sq_t queue, void *buf, int buf_sz, struct timeval *enqueue_time)
{
	struct __shmqueue_node_head_t *node;

	int nr_nodes, datalen;
	int old_head, new_head, head;

	if(queue==NULL || buf==NULL || buf_sz<1)
	{
		snprintf(errmsg, sizeof(errmsg), "Bad argument");
		return -1;
	}

	head = old_head = queue->head_pos;
	do
	{
		if(queue->tail_pos==head) // end of queue
		{
			if(head!=old_head && CAS32(&queue->head_pos, old_head, head))
			{
				new_head = head;
				datalen = 0;
				break;
			}
			// head_pos not advanced or changed by someone else, simply returns
			return 0;
		}

		node = SQ_GET(queue, head);
		if(node->start_token!=START_TOKEN)
		{
			head = SQ_ADD_POS(queue, head, 1);
			continue;
		}
		datalen = node->datalen;
		nr_nodes = SQ_NUM_NEEDED_NODES(queue, datalen);
		if(SQ_USED_NODES2(queue, head) < nr_nodes)
		{
			head = SQ_ADD_POS(queue, head, 1);
			continue;
		}
		new_head = SQ_ADD_POS(queue, head, nr_nodes);
		if(CAS32(&queue->head_pos, old_head, new_head))
		{
			if(enqueue_time)
				*enqueue_time = node->enqueue_time;
			if(datalen > buf_sz)
			{
				snprintf(errmsg, sizeof(errmsg), "Data length(%u) exceeds supplied buffer size of %u", datalen, buf_sz);
				return -2;
			}
			memcpy(buf, node->data, datalen);
			break;
		}
		else // head_pos changed by someone else, start over
		{
			old_head = queue->head_pos;
			head = old_head;
		}
	} while(1);

	while(old_head!=new_head)
	{
		node = SQ_GET(queue, old_head);
		// reset start_token so that this node will not be treated as a starting node of data
		node->start_token = 0;
		old_head = SQ_ADD_POS(queue, old_head, 1);
	}

	return datalen;
}

