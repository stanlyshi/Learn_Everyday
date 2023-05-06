//
// client.c
//
//
// Created by Gayatri Singh on 9/26/14.
//
//
#include "client.h"

request_param * initializeRequestParam(int request_id,int pid, int numrequests, int id_Shared_Memory, int id_Shared_Semaphore)
{
	printf("about to create request param...\n");
	request_param * rp = (request_param*)malloc(sizeof(request_param));
	rp->request_id = request_id;
	rp->pid = pid;
	rp->shared_Memory_id = id_Shared_Memory;
	rp->shared_Memory_semaphore_id = id_Shared_Semaphore;
	rp->next = NULL;
	rp->num_requests = numrequests;
	printf("have created request param...\n");
	return rp;
}

// client holds all the request
client * initializeClient(int sync_or_async, int priority,pid_t pid,int num_requests)
{
	client * new = (client*)malloc(sizeof(client));


	int key = pid;
	int keySem = key+1,idSem;
	int n = num_requests  * sizeof(int);
	//int idShMem = Allocate_Result_Shared_Memory(num_requests * sizeof(result_struct),pid);
	int idShMem = shmget(key, n, IPC_CREAT | 0666);

	//Creating Semaphore lock
	if((idSem = initsem(keySem,1)) ==-1){
		perror("initsem");	
		exit(1);
	}

	new->pid = pid;
	if(priority==0)
		new->priority = 1234;
	else if(priority==1)
		new->priority = 4567;
	else
		new->priority = 6789;
	printf("priority of queue is %d\n",new->priority);
	new->async=sync_or_async;
	new->rq = create();
	new->res_shared_Memory_key = key;
	new->res_shared_Memory_id = idShMem;
	new->res_semaphore_id = idSem;
	new->numrequests = num_requests;
	printf("client has shared memory id %d and shared memory semaphore id %d\n",idShMem,idSem);
	int i;


	struct sembuf sb;
	sb.sem_num =0;
	sb.sem_op = -1; /*set to allocate resources*/
 	sb.sem_flg = SEM_UNDO;
	printf("Locking result shared memory semaphore\n");
	if(semop(idSem,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	}
	printf("Have locked result shared memory semaphore\n");

	int * rs = (int*)shmat(idShMem, NULL, 0);

	for(i=0;i < num_requests;i++){
		printf("loop %d of initializing request param..\n",i);
		request_param * rp = initializeRequestParam(i,pid,num_requests,new->res_shared_Memory_id,new->res_semaphore_id);
		enq(rp,new->rq);
		*(rs+i) = -1;

	}

	printf("Unlock shared memory\n");
	sb.sem_op = 1;
	if(semop(idSem,&sb,1)==-1){
	  perror("semop");
	  exit(1);
	}

	printf("have initialized client...\n");
	return new;
}
/**
* returns nothing
*/

void callServiceAsync(int priority,client * cp)
{
	printf("calling service async...\n");

	struct sembuf sb;
	sb.sem_num =0;
	sb.sem_op = -1; /*set to allocate resources*/
 	sb.sem_flg = SEM_UNDO;
	int sem_id= semget(priority+1,1,0666);
	int msgq_id;
	msgbuf msg;
	msg.mtype=1;

	if(semop(sem_id,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	}


	if((msgq_id=msgget(priority,0666 | IPC_CREAT))==-1)
	{
		perror("msgget");
		exit(1);
	}

	while(!empty(cp->rq)){
		request_param * req = deq(cp->rq);
		msg.rp = *req;
		if(msgsnd(msgq_id,&msg,sizeof(request_param),MSG_NOERROR)==-1)
		{
			perror("msgsend");
		}	
	}

	sb.sem_op = 1;
	if(semop(sem_id,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	}

	printf("call is complete...\n");
	return;
}
/**
*returns actual bytes
*/
int callServiceSync(int priority, client * cp)
{
	printf("calling service sync...\n");

	struct sembuf sb;
	sb.sem_num =0;
	sb.sem_op = -1; /*set to allocate resources*/
 	sb.sem_flg = SEM_UNDO;
	int sem_id= semget(priority+1,1,0666);
	int msgq_id;

	msgbuf msg;
	msg.mtype=1;

	printf("Locking msg queue semaphore\n");
	if(semop(sem_id,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	}

	if((msgq_id=msgget(priority,0666 | IPC_CREAT))==-1)
	{
		perror("msgget");
		exit(1);
	}
	while(!empty(cp->rq)){
		request_param * req = deq(cp->rq);
		msg.rp = *req;
		if(msgsnd(msgq_id,&msg,sizeof(request_param),MSG_NOERROR)==-1)
		{
			perror("msgsend");
		}	
	}

	printf("unlocking queue semaphore\n");
	sb.sem_op = 1;
	if(semop(sem_id,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	}
	printf("call is complete...\n");

	int returnedValue = waitForResults(cp);
	printf("I have returned to sync\n");
	return returnedValue;
}

/**
* This function waits for results in shared memory-when it gets the lock it performs an
* additive combination encryption algorithm on the returned Bytes.
*/
int waitForResults(client * cp)
{
	printf("waiting for results...\n");
	//struct sembuf sb;
	//sb.sem_num =0;
	//sb.sem_op = -1; /*set to allocate resources*/
 	//sb.sem_flg = SEM_UNDO;
	//sleep(30);
	//printf("Locking result shared memory semaphore\n");
	//if(semop(cp->res_semaphore_id,&sb,1)==-1){
	//	 perror("semop");
	//	 exit(1);
	//}
	//printf("Have locked result shared memory semaphore\n");
	int * rs = (int*)shmat(cp->res_shared_Memory_id, NULL, 0);
	printf("mapped shared memory semaphore\n");

	int i = 0;
	int returnValue=0;
	//Apply top secret encryption algorithm to returned bytes from server
	printf("num requests is %d and shared memory id is %d\n",cp->numrequests,cp->res_shared_Memory_id);	
	for(i=0;i<cp->numrequests;i++)
	{
		//returnValue += *((rs + i)->randomBytes);
		printf("I am inside the for loop\n");
		while(*(rs+i) < 0);
		int * new_result = rs + i;
		returnValue += *(new_result);
		printf("received bytes %d\n",returnValue);
	}
	//detach from shared memory
	if(shmdt(rs)==-1)
	{
		perror("shmdt");
		exit(1);
	}
	//delete shared memory;
	if(shmctl(cp->res_shared_Memory_id,IPC_RMID,NULL)==-1)
	{
		perror("shmctl");
		exit(1);
	}

	//destroy semaphore;
	//if(semctl(cp->res_semaphore_id,0,IPC_RMID))
	//{
	//	perror("semctl");
	//	exit(1);
	//}
	//printf("unlocking semaphore\n");	
	//sb.sem_op = 1;
	//if(semop(cp->res_semaphore_id,&sb,1)==-1){
	//	 perror("semop");
	//	 exit(1);
	//}
	printf("received results...\n");
	return returnValue;
}
/**
* This function deliberately loops through 10000 calls to rand() in order to give the illusion
* of having work to do.
*/
int doSomething()
{
	printf("I am doing something\n");
	int randnum,i;
	for(i=0; i < 10000000; i++)
		randnum = rand();
	printf("I have done something\n");
	return randnum;
}
/**
* This function compares the bytes returned from doSomething() and server
* It returns 1 if they are the same else it returns false
*/
int useResultBytes(int r1, int r2)
{
	if(r1==r2)
		return 1;
	else return 0;
}


int main(void) 
{
    
    /* Our process ID and Session ID */
    pid_t pid, sid;
    int NUM_CLIENT=4;
    //printf("Enter number of clients:\n");
    //scanf("%d",&NUM_CLIENT);
    /* Fork off the parent process */
    int i =0,returnedBytes=0,originalBytes=0,comp=0;
    int sync_or_async=0,priority=1,num_requests=1;
    int msec;
    clock_t start,diff;
    FILE * f = fopen("IPC_Performance_stats","a");
    while(i < NUM_CLIENT)
    {	 
	pid = fork();
       
    	if (pid < 0) {
        	printf("client did not create\n");
        	exit(EXIT_FAILURE);
    	}
    	/* If we got a good PID, then
     	we can exit the parent process. */
    	if (pid > 0) {
        	//i++;
		if(i%2==0 || i==0)
			sync_or_async = 1;
		else sync_or_async = 0;
		num_requests = ++i;
		if(priority == 0)
			priority = 1;
		else if(priority == 1)
			priority = 2;
		else if(priority==2)
		 	 priority = 0;
		
    	}
	else{
	client * c = initializeClient(sync_or_async,priority,pid,num_requests);
	printf("Starting client %d which has %d sync_or_async %d num_requests %d priority\n",i,c->async,c->numrequests,c->priority);
	fprintf(f,"Starting client %d which has %d sync_or_async %d num_requests %d priority\n",i,c->async,c->numrequests,c->priority);
	switch(sync_or_async){
		case 0:
		start = clock();
		returnedBytes = callServiceSync(c->priority,c);
		originalBytes = doSomething();
		comp = useResultBytes(originalBytes,returnedBytes);
		if(comp)
		 printf("The resultBytes and the original bytes are the same\n");
		else printf("The resultBytes and the original bytes are not the same\n");
		diff = clock() - start;
		msec = diff * 1000/CLOCKS_PER_SEC;
		fprintf(f,"Client using sync %d: Time taken %d seconds %d milliseconds\n",i,msec/1000,msec%100);
		break;
		case 1:
		 start = clock();
		 callServiceAsync(c->priority,c);
		 originalBytes = doSomething();
		 returnedBytes = waitForResults(c);
		 comp = useResultBytes(originalBytes,returnedBytes);
		 if(comp)
		  printf("The resultBytes and the original bytes are the same\n");
		 else printf("The resultBytes and the original bytes are not the same\n");
		  diff = clock() - start;
		  msec = diff * 1000/CLOCKS_PER_SEC;
		  fprintf(f,"Client using async %d: Time taken %d seconds %d milliseconds\n",i, msec/1000,msec%100);
		 break;
	}
	exit(EXIT_SUCCESS);
	return;
	}			
	
    }
   fclose(f);
   exit(EXIT_SUCCESS);
   return;			
}
