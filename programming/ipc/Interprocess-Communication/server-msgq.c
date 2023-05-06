#include "server.h"

server * initializeServer(int pid)
{
	server * s;
	s = (server*)malloc(sizeof(server));
	s->pid = pid;

	int keyLow = 1234, keyMed = 4567, keyHigh = 6789, shm1,shm2,shm3;
	int keySemLow = 1235, keySemMed = 4568, keySemHigh =6790, idl, idm,idh;

	if((idl = initsem(keySemLow,1)) ==-1){
		perror("initsem");	
		exit(1);
	}

	if((shm1 = msgget(keyLow, 0666 | IPC_CREAT))==-1){
		perror("msgget");
		exit(1);
	}


	if((idm = initsem(keySemMed,1)) ==-1){
		perror("initsem");	
		exit(1);
	}

	if((shm2 = msgget(keyMed, 0666 | IPC_CREAT))==-1){
		perror("msgget");
		exit(1);
	}


	if((idh = initsem(keySemHigh,1)) ==-1){
		perror("initsem");	
		exit(1);
	}

	if((shm3 = msgget(keyHigh, 0666 | IPC_CREAT))==-1){
		perror("msgget");
		exit(1);
	}
	
	s->LowSemID = idl;
	s->LowMsgQID = shm1;

	s->MedSemID = idm;
	s->MedMsgQID = shm2;

	s->HighSemID = idh;
	s->HighMsgQID = shm3;
	return s;
}

/*int isEmpty()
{
	if(empty(s->serverL) && empty(s->serverL) && empty(s->serverL))
		return 1;
	else
		return 0;
}*/

int returnBytes()
{
	int r = rand();
	return r;
}

void req_handler(request_param * rpp)
{
		int value;
		int nr = rpp->num_requests;
		int r = rpp->request_id;
		printf("Client has %d requests and we are servicing %d\n",nr,r);
		//int r = rpp->request_id;
		int *curr,*result;
		int smid = rpp->shared_Memory_id;
		int idsemp = rpp->shared_Memory_semaphore_id;
		printf("shared memory id is %d and shared memory semaphore id is %d\n",smid,idsemp);
		struct sembuf sb;

		//result = (result_struct *)MapSharedMemory(smid);
		result = (int*)shmat(smid, NULL, 0);
		value = returnBytes();
		printf("bytes to be returned are %d\n",value);
		//LockSemaphore(idsemp,SEM_SERVER);
		//Does this lock code need to be inside a while loop till lock is available??
		
		sb.sem_num = 0;
		sb.sem_op = -1;
		sb.sem_flg = SEM_UNDO;
		semop(idsemp,&sb,1);
		
		//result[r] = (result_struct*)malloc(sizeof(result_struct));
		*(result+r) = value;
		printf("I have written %d to shared mem\n",*(result+r));
		if(r == nr-1)
		{
			//detach from shared memory
			if(shmdt(result)==-1)
			{
				perror("shmdt");
				exit(1);
			}
			//UnlockSemaphore(idsemp,SEM_SERVER);
			printf("unlocking shared memory semaphore\n");		
			sb.sem_op = 1;
			semop(idsemp,&sb,1);
			
			printf("unlock was successful\n");
			
		}
		
				
}	
	
	
int msgQisEmpty(int msgQID)
{	
	struct msqid_ds buf;
	if(msgctl(msgQID,IPC_STAT,&buf)==-1)
	{
		perror("msgctl");
	}
	uint msg = (uint)(buf.msg_qnum);
	if(msg==0)
		return 1;
	else return 0;
}
void request_schedule(server * s)
{

 	struct sembuf sb;
	sb.sem_num =0;
	sb.sem_op = -1; /*set to allocate resources*/
 	sb.sem_flg = SEM_UNDO;
	msgbuf buf;
	int i;
	//int qsareempty = 1;
	while(1)
	{
	    printf("looping...\n");
  	    //Lock high priority message queue
	     if(semop(s->HighSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }

	      for(i=0;i<HIGHRQ;i++)
	      {
		if(msgQisEmpty(s->HighMsgQID))
		{
		   i = HIGHRQ;		
		}
		else
		{	
                  printf("scheduling a high priority request...\n");				
		  
		  if(msgrcv(s->HighMsgQID,&buf,sizeof(request_param),0,MSG_NOERROR)){
			perror("msgrcv");
		  }
		  req_handler(&(buf.rp));
		}
	      }

              sb.sem_op =1;
  	    //UnLock high priority message queue
	     if(semop(s->HighSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }
              sb.sem_op=-1;
  	    //Lock med priority message queue
	     if(semop(s->MedSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }
	      for(i=0;i<MEDRQ;i++)
	      {	
		if(msgQisEmpty(s->MedMsgQID))
		{
		   i = MEDRQ;		
		}
		else
		{	
                  printf("scheduling a med priority request...\n");				
		  
		  if(msgrcv(s->MedMsgQID,&buf,sizeof(request_param),0,MSG_NOERROR)){
			perror("msgrcv");
		  }
		  req_handler(&(buf.rp));
		}
	      }

             sb.sem_op =1;
  	    //UnLock med priority message queue
	     if(semop(s->MedSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }
              sb.sem_op=-1;
  	    //Lock low priority message queue
	     if(semop(s->LowSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }
	      for(i=0;i<LOWRQ;i++)
	      {	
		if(msgQisEmpty(s->LowMsgQID))
		{
		   i = LOWRQ;		
		}
		else
		{	
                  printf("scheduling a low priority request...\n");				
		  
		  if(msgrcv(s->LowMsgQID,&buf,sizeof(request_param),0,0)){
			perror("msgrcv");
		  }
		  req_handler(&(buf.rp));
		}
	      }
             sb.sem_op =1;
  	    //UnLock high priority message queue
	     if(semop(s->LowSemID,&sb,1)==-1){
		 perror("semop");
		 exit(1);
	      }
	     sleep(60);
	}

}

int main(void) {
    
    /* Our process ID and Session ID */
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        printf("daemon did not create\n");
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
     we can exit the parent process. */
    if (pid > 0) {
        printf("daemon was created\n");
	exit(EXIT_SUCCESS);
    }   
    
    /* Change the current working directory
    if ((chdir("/")) < 0) {
         Log the failure 
        printf("the new child process was not able to change the current working directory\n");
        exit(EXIT_FAILURE);
    } */
    
    /* Close out the standard file descriptors */
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);
    
    /* Daemon-specific initialization goes here */
    
    /* The Big Loop */
    printf("about to initialize server...\n");
    server * s = initializeServer(pid);  
    printf("about to start request scheduler...\n");        	
    request_schedule(s);
	
    exit(EXIT_SUCCESS);
}

		
		
	

	
		
	
		
			
			


