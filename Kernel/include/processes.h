#ifndef PROCESSES_H
#define PROCESSES_H

#include <stdint.h>
#include <thread.h> 

#include "defs.h"
#include "mutex.h"

#define MAX_DATA_PAGES 64
#define MAX_PROCESS_NAME 64
#define MAX_THREADS 14

typedef struct
{
  char status;
  char name[MAX_PROCESS_NAME];
  uint64_t dataPageCount;
  void *dataPage[MAX_DATA_PAGES];
  uint64_t pid;
  uint64_t ppid;
  //uint64_t cpid;
  threadADT threads[MAX_THREADS];
  uint64_t threadCount;
} process;


int insertProcess(process *p);

process *createProcess(uint64_t newProcessRIP, int foreground, uint64_t argc, uint64_t argv, const char *name);

process *getProcessByPid(uint64_t pid);

void setNullAllProcessPages(process *process);

void setNullAllThreads(process *process);

int removeProcess(process *p);

void addDataPage(process *p, void *page);

void exitShell();

uint64_t getProcessPid(process *p);

uint64_t getProcessPpid(process *p);

uint64_t getProcessesNumber();

uint64_t getProcessThreadCount(int pid);

void printPIDS();

void whileTrue();

void removeThreadFromProcess(process* p, int tid);

threadADT getThread(process* p, int tid);

int deleteThisProcess(int pid);

#endif
