//
// server.h
//
//
// Created by Gayatri Singh on 9/26/14.
//
//
#ifndef _server_h
#define _server_h
#include "queue.h"
#define LOWRQ 1
#define HIGHRQ 3
#define MEDRQ 2
#define KEYL 1
#define KEYM 4567
#define KEYH 6789


typedef struct serverProcess
{
	int pid;
	int LowSemID;
	int LowMsgQID;
	int MedSemID;
	int MedMsgQID;
	int HighSemID;
	int HighMsgQID;
	//int serverUnlock;
} server;



server * initializeServer(int);
extern int initsem(key_t key, int nsems);
int returnBytes();
void req_handler(request_param *);
void request_schedule(server *);
int msgQisEmpty(int msgQID);


#endif
