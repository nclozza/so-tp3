#include "memoryManager.h"
#include "scheduler.h"
#include "dirs.h"
#include "videoDriver.h"
#include "mutex.h"
#include "processes.h"
#include "thread.h"
#include "defs.h"
#include "interrupts.h"

static void addThread(threadADT t);
static void setNextCurrent();

/* Procesos actualmente bloqueados */
static blockedThread *firstBlockedThread;

/* Proceso actualmente corriendo */
static nodeList *current = NULL;
static nodeList *prev = NULL;

threadADT getCurrentThread()
{
	return current->t;
}

process* getCurrentProcess()
{
	return getProcessByPid(getThreadPid(current->t));
}

uint64_t nextThread(uint64_t current_rsp)
{	
	if (current == NULL)
	{
		return current_rsp;
	}

	decreaseQuantum();

	if (current->quantum > 0)
		return current_rsp;

	current->quantum = QUANTUM;

	setThreadRsp(current->t, current_rsp);

	prev = current;
	current = current->next;

	setNextCurrent();

	return getThreadRsp(current->t);
}

uint64_t runThread(threadADT new_thread)
{
	int pid;

	addThread(new_thread);

	pid = getThreadPid(new_thread);

	if (pid == 0)
		_changeThread(getThreadRsp(current->t));

	return pid;
}

static void addThread(threadADT t)
{
	nodeList *new_node = (nodeList *)malloc(sizeof(*new_node));

	new_node->t = t;
	new_node->quantum = QUANTUM;

	if (current == NULL)
	{
		current = new_node;
		current->next = current;
		prev = current;
	}
	else
	{
		new_node->next = current->next;
		current->next = new_node;
	}
}

void yieldThread()	
{	
	current->next->quantum += 1;
	current->quantum = 0;	
	_yieldThread();	
}

void killThread()
{
	nodeList *n = current;
	removeThread(n->t);
	prev->next = current->next;
	current = current->next;
	setNextCurrent();
	free((void *)n);
	increaseQuantum();
	_changeThread(getThreadRsp(current->t));
}

static void setNextCurrent()
{
	while (isThreadBlocked(current->t) || isThreadDeleted(current->t))
	{
		nodeList *next = current->next;

		if (isThreadDeleted(current->t))
		{
			prev->next = next;
			removeThread(current->t);
			free((void *)current);
		}
		else
			prev = current;

		current = next;
	}
}

void printBlockedThreadsList()
{
	blockedThread *temp = firstBlockedThread;
	while (temp != NULL)
	{
		int pid = getThreadPid(temp->t);
		printString("PID: ", 0, 155, 255);
		printInt(pid, 0, 155, 255);
		printString("\n", 0, 155, 255);
		temp = temp->next;
	}
}

void increaseQuantum()
{
	current->quantum += 1;
}

void decreaseQuantum()
{
	current->quantum -= 1;
}

void block(queueADT queue)
{
  blockThread(current->t);
  enqueue(queue, current);
}

void unblock(queueADT queue)
{
	nodeList *node = dequeue(queue);
	if(node != NULL)
	{
		if(isThreadDeleted(node->t))
		{
			unblock(queue);
		}
  
		unblockThread(current->t);
		addThread(current->t);
	}
}