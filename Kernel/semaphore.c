#include <stdint.h>
#include "semaphore.h"
#include "memorymanager.h"
#include "lib.h"
#include "processes.h"
#include "scheduler.h"
#include "videoDriver.h"

static semADT* semaphores;
static int id = 0;
static int numberOfSemaphores = 0;

typedef struct sem_t
{
	char *name;
	int value;
	int id;
	threadADT blockedThreads[MAX_THREADS];
} sem_t;

int semOpen(char *name)
{
	int i;
	for (i = 0; i < numberOfSemaphores; i++)
	{
		if (strcmpKernel(name, semaphores[i]->name) == 0)
		{
			return semaphores[i]->id;
		}
	}
	semADT newSemaphore = (semADT)malloc(sizeof(sem_t));
	newSemaphore->name = (char *)malloc(strlenKernel(name) + 1);
	strcpyKernel(newSemaphore->name, name);
	newSemaphore->value = 1;
	newSemaphore->id = id;
	for(int i = 0; i < MAX_THREADS; i++)
	{
		newSemaphore->blockedThreads[i] = NULL;
	}
	id++;
	numberOfSemaphores++;
	semaphores = (semADT *)realloc(semaphores,numberOfSemaphores  * sizeof(semADT));
	semaphores[numberOfSemaphores - 1] = newSemaphore;
	return newSemaphore->id;
}

int semPost(int id)
{
	sem_t* sem = NULL;
	int i;
	for(i = 0; i < numberOfSemaphores; i++)
	{
		if(semaphores[i]->id == id)
		{			
			sem = semaphores[i];
		}
	}
	if(sem == NULL)
		return 1;
	
	if(sem->value<=0)
	{
		for(int i = 0; i < MAX_THREADS; i++){		
			unblockThread(sem->blockedThreads[i]);
		}
	}
	sem->value++;
	return sem->value;
}

int semWait(int id)
{
	sem_t* sem = NULL;
	int i;
	for(i = 0; i < numberOfSemaphores; i++)
	{
		if(semaphores[i]->id == id)
		{			
			sem = semaphores[i];
		}
	}
	if(sem == NULL)
		return 1;

	sem->value--;
	if(sem->value<0)
	{
		sem->value = 0;
		
		while(sem->value == 0)
		{
			threadADT t = getCurrentThread();		
			blockThread(t);
			sem->blockedThreads[getThreadPid(t)]= t;		
			yieldThread();
		}
		sem->value = 0;
	}	
	return 0;
}

int semaphoresListSize()
{
	return numberOfSemaphores;
}

int semClose(int id)
{
	int i;
	for (i = 0; i < numberOfSemaphores; i++)
	{
		if(semaphores[i]->id == id)
		{			
			free(semaphores[i]->name);
			free(semaphores[i]);

			int j;
			for (j = i; j < numberOfSemaphores - 1; j++)
			{
				semaphores[j] = semaphores[j + 1];
			}

			numberOfSemaphores--;

			return 0;
		}
	}
	return 1;
}

void freeSemaphoresList()
{
	free(semaphores);
}
