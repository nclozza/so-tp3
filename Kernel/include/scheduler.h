#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "thread.h" 
#include "processes.h"
#include "genericQueue.h"
#include "defs.h"

#define MAX_PROCESSES 128
#define QUANTUM 1

typedef struct node
{
	int quantum;
	threadADT t;
	struct node *next;
} nodeList;

typedef struct blockedThread
{
	threadADT t;
	int semId;
	int isMutex;
	struct blockedThread *next;
} blockedThread;

typedef struct blockedThread* blockedThreadADT;
void printBlockedThreadsList();

uint64_t nextThread(uint64_t current_rsp);

uint64_t runThread(threadADT new_thread);
void killThread();
void yieldThread();

void _changeThread(uint64_t rsp);
void _yieldThread();

threadADT getCurrentThread();
process* getCurrentProcess();
void increaseQuantum();
void decreaseQuantum();

void block(queueADT queue);
void unblock(queueADT queue);


#endif
