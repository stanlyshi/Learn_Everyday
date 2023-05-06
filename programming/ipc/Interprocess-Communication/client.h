//
// client.h
//
//
// Created by Gayatri Singh on 9/26/14.
//
//
#ifndef _client_h
#define _client_h

#include "queue.h"

typedef struct clientProcess
{
	pid_t pid;
	int priority;
	int async;
	int res_shared_Memory_key;
	int res_shared_Memory_id; 
	int res_semaphore_id;
	int numrequests;
	request_q * rq;
} client;

request_param * initializeRequestParam(int request_id,int pid, int numrequests, int id_Shared_Memory, int id_Shared_Semaphore);

void callServiceAsync(int priority,client * cp);

int callServiceSync(int priority, client * cp);

client * initializeClient(int sync_or_async, int priority,pid_t pid,int num_requests);

int waitForResults(client * cp);

int doSomething();

int useResultBytes();

#endif
