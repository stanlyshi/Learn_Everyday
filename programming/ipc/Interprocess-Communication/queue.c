#include "queue.h"

extern int initsem(key_t key, int nsems)  /* key from ftok() */
{
    int i;
    union semun arg;
    struct semid_ds buf;
    struct sembuf sb;
    int semid;

    semid = semget(key, nsems, IPC_CREAT | 0666);

    if (semid >= 0) { /* we got it first */
        sb.sem_op = 1; sb.sem_flg = 0;
        arg.val = 1;

        for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) { 
            /* do a semop() to "free" the semaphores. */
            /* this sets the sem_otime field, as needed below. */
            if (semop(semid, &sb, 1) == -1) {
                int e = errno;
                semctl(semid, 0, IPC_RMID); /* clean up */
                errno = e;
                return -1; /* error, check errno */
            }
        }

    }

    return semid;
}

/* Create an empty queue */
request_q * create()
{    
    printf("queue is about to be created...\n");
    request_q * queue = (request_q*)malloc(sizeof(request_q));
    queue->head = NULL;
    queue->end = NULL;	
    queue->count = 0;
    printf("queue was created...\n");
    return queue;
}
 
/* Returns queue size */
int queuesize(request_q * queue)
{
    return queue->count;
}
 
/* Enqueing the queue */
void enq(request_param * elem,request_q * queue)
{
   printf("attempting enqueue...\n");

    if (queue->end == NULL)
    {   printf("it was ascertained that queue is currently empty...\n");     
        queue->end = elem;
        (queue->end)->next = NULL;
        queue->head = queue->end;
	printf("request param has beeen put into empty queue\n");
    }
    
    else
    {
        printf("it was ascertained the queue has some elements alread....\n");
        (queue->end)->next = elem;
        elem->next = NULL;
        queue->end = elem;
    }
    queue->count++;
    printf("enqueue succeeded...\n");
}
 
/* Displaying the queue elements */
void display(request_q * queue)
{
    curr = queue->head;
 	rear = queue->end;
 	
    if ((curr == (struct request *)NULL) && (rear == (struct request *)NULL))
    {
        printf("Queue is empty");
        return;
    }
    while (curr != NULL)
    {
        printf("%d %d\n", curr->request_id, curr->pid);
        curr = curr->next;
    }
}
 
/* Dequeing the queue */
request_param * deq(request_q * queue)
{
    
    curr = queue->head;
    request_param * returned_request = curr;
 	
    if (curr == NULL)
    {
       return NULL;
    }
    else if (curr->next != NULL)
    {
        curr = curr->next;
        queue->head = curr;
        queue->count--;
        return returned_request;            
     }
     else
     {

          queue->head = NULL;
          queue->end = NULL;
          queue->count--;
     
          return returned_request;
      }
      return returned_request;
}
 
/* Returns 1 if queue is empty else returns 0 */
int empty(request_q * queue)
{
     //printf("checking if queue is empty...\n");
     if ((queue->head == NULL) && (queue->end == NULL)){
	//printf("queue is empty...\n");
        return 1;
      }
      //printf("queue is not empty...\n");
      return 0;
}
