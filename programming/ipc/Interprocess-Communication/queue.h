/*
 * C Program to Implement Queue Data Structure for request parameters
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <math.h>

#define MAX_RETRIES 10

union semun
{
	int val;
	struct semid_ds * buf;
	ushort *array;
};


typedef struct request{
    int request_id;
    int pid;
    int shared_Memory_id;
    int shared_Memory_semaphore_id;
    struct request * next;
    int num_requests;
} request_param;

typedef struct my_msgbuf{
	long mtype;
	request_param rp;
} msgbuf;

typedef struct result
{
    int result_id;
    int * request_id;
    int * randomBytes;
} result_struct;

typedef struct queue
{
	request_param * head;
	request_param * end;
	int count;
} request_q;

struct request * curr, *rear;
extern request_q * ServerL, *ServerM, *ServerH;

extern int initsem(key_t key, int nsems);
void enq(request_param * elem,request_q * queue);
request_param * deq(request_q * queue);
int empty(request_q * queue);
void display(request_q * queue);
request_q * create();
int queuesize(request_q * queue);
request_param * make_requestParam(int id);
