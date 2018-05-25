#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#define NULL ((void *)0)


typedef struct sem_t *semADT;

int semOpen(char *name);
int semPost(int id);
int semWait(int id);
int semaphoresListSize();
int semClose(int id);
void freeSemaphoresList();

#endif
