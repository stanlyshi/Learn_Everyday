/**
 * Declaration of a shm queue
 *
 *  Created on: 2016.7.10
 *  Author: WK <18402927708@163.com>
 *  Modify: RT <rongtao@sylincom.com> 2019.09.27
 *
 *  Based on transaction pool, features
 *  1) support single writer but multiple reader processes/threads
 *  2) support timestamping for each data
 *  3) support auto detecting and skipping corrupted elements
 *  4) support variable user data size
 *  5) use highly optimized gettimeofday() to speedup sys time
 */
#ifndef __SHM_QUEUE_HEADER__
#define __SHM_QUEUE_HEADER__

#include <time.h>
#include <signal.h>


struct __shmqueue_head_t;

typedef struct __shmqueue_head_t *sq_t;


/**
 *  Create a shm queue
 *  Parameters:
 *     shm_key      - shm key
 *     ele_size     - preallocated size for each element 
 *     ele_count    - preallocated number of elements   
 *  Returns a shm queue pointer or NULL if failed    
 */
sq_t shmqueue_create(uint64_t shm_key, int ele_size, int ele_count);


/**
 *  Shm key exist or not
 *  Parameters:
 *     shm_key      - shm key 
 *  Returns a shm queue pointer or NULL if failed    
 */
int shmqueue_exist(uint64_t shm_key);


/**
 *  Open an existing shm queue for reading data
 *  Parameters:
 *     shm_key      - shm key 
 *  Returns a shm queue pointer or NULL if failed    
 */
sq_t shmqueue_open(uint64_t shm_key);


/**
 *  Set signal parameters if you wish to enable signaling on data write
 *  Parameters:
 *      sq           - shm_queue pointer returned by shmqueue_create
 *      signum       - sig num to be sent to the reader, e.g. SIGUSR1
 *      sig_ele_num  - only send signal when data element count exceeds sig_ele_num
 *      sig_proc_num - send signal to up to this number of processes once
 *  Returns 0 on success, < 0 on failure
 */
int shmqueue_set_sigparam(sq_t sq, int signum, int sig_ele_num, int sig_proc_num);

/**
 *  Register the current process ID, so that it will be able to recived signal
 *  Note: you don't need to unregister the current process ID, it will be removed
 *          automatically next time register_signal is called if it no longer exists
 *  Parameters:
 *      sq  - shm_queue pointer returned by shmqueue_open
 *  Returns a signal index for shmqueue_sigon/shmqueue_sigoff, or < 0 on failure
 */
int shmqueue_register_signal(sq_t sq);

/**
 *  Turn on/off signaling for current process
 *  Parameters:
 *      sq  - shm_queue pointer returned by shmqueue_open
 *      sigindex - returned by shmqueue_register_signal()
 *  Returns 0 on success, -1 if parameter is bad
 */
int shmqueue_sigon(sq_t sq, int sigindex);
int shmqueue_sigoff(sq_t sq, int sigindex);

/**
 *  Destroy queue created by shmqueue_create()
 */
void shmqueue_destroy(sq_t queue);

/**
 *  Add data to end of shm queue
 *  Returns 0 on success or
 *     -1 - invalid parameter
 *     -2 - shm queue is full
 *  Note: here we assume only one process can put to the queue
 *     for multi-thread/process support, you need to introduce a lock by yourself
 */
int shmqueue_put(sq_t queue, void *data, int datalen);

/**
 *  Retrieve data
 *  On success, buf is filled with the first queue data
 *  this function is multi-thread/multi-process safe
 *  Returns the data length or
 *      0 - no data in queue
 *     -1 - invalid parameter
 */
int shmqueue_get(sq_t queue, void *buf, int buf_sz, struct timeval *enqueue_time);

/**
 *  Get usage rate
 *  Returns a number from 0 to 99
 */
int shmqueue_get_usage(sq_t queue);

/**
 *  Get number of used blocks
 */
int shmqueue_get_used_blocks(sq_t queue);

/**
 *  If a queue operation failed, call this function to get an error reason
 */
const char *shmqueue_errorstr();


#endif

