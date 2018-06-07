#include "processes.h"
#include "defs.h"
#include "lib.h"
#include "init.h"
#include "memoryAllocator.h"
#include "mutex.h"
#include "memoryManager.h"
#include "scheduler.h"
#include "videoDriver.h"

static void freeDataPages(process *p);

static process *processesTable[MAX_PROCESSES] = {NULL};

static uint64_t processesNumber = 0;

int insertProcess(process *p)
{
  int i;

  for (i = 0; i < MAX_PROCESSES; i++)
  {
    if (processesTable[i] == NULL)
    {
      processesNumber++;
      p->pid = i;
      processesTable[i] = p;
      return i;
    }
  }

  return -1;
}

process *createProcess(uint64_t newProcessRIP, int foreground, uint64_t argc, uint64_t argv, const char *name)
{
  process *newProcess = (process *)malloc(sizeof(*newProcess));
  strcpyKernel(newProcess->name, name);
  setNullAllProcessPages(newProcess);
  insertProcess(newProcess);
  setNullAllThreads(newProcess);
  newProcess->threads[0] = createThread(newProcess->pid, foreground, newProcessRIP, argc, (char **)argv, 0);
  newProcess->threadCount = 1;

  if (newProcess->pid != 0)
  {
    newProcess->ppid = getProcessPid(getCurrentProcess());
  }
  else
  {
    newProcess->ppid = 0;
  }

  return newProcess;
}

process *getProcessByPid(uint64_t pid)
{
  if (pid < MAX_PROCESSES && processesTable[pid] != NULL)
  {
    return processesTable[pid];
  }

  return NULL;
}

void setNullAllProcessPages(process *process)
{
  int i;

  for (i = 0; i < MAX_DATA_PAGES; i++)
  {
    process->dataPage[i] = NULL;
  }

  process->dataPageCount = 0;
}

void setNullAllThreads(process *process)
{
  int i;

  for (i = 0; i < MAX_THREADS; i++)
  {
    process->threads[i] = NULL;
  }

  process->threadCount = 0;
}

int removeProcess(process *p)
{

  if (p != NULL)
  {
    processesNumber--;
    freeDataPages(p);
    processesTable[p->pid] = NULL;

    for (int i = 0, x = 0; i < MAX_THREADS && x < p->threadCount; i++)
    {
      if (removeThread(p->threads[i]) == 0)
        x++;
    }
    free((void *)p->threads);
    free((void *)p);
    return 0;
  }
  return -1;
}

/* Libera las páginas de datos usadas por el proceso. */
static void freeDataPages(process *p)
{
  int i;

  for (i = 0; i < MAX_DATA_PAGES && p->dataPageCount > 0; i++)
  {
    if (p->dataPage[i] != NULL)
    {
      free((void *)p->dataPage[i]);
      p->dataPageCount -= 1;
    }
  }
}

void addDataPage(process *p, void *page)
{
  int i = 0;

  while (i < MAX_DATA_PAGES && p->dataPage[i] != NULL)
    i++;

  if (i < MAX_DATA_PAGES)
  {
    p->dataPageCount += 1;
    p->dataPage[i] = page;
  }
}

void exitShell()
{
  process *shell = getProcessByPid(1);
  shell->status = DELETE;
}

uint64_t getProcessPid(process *p)
{
  if (p != NULL)
    return p->pid;
  return -1;
}

uint64_t getProcessPpid(process *p)
{
  if (p != NULL)
    return p->ppid;
  return -1;
}

uint64_t getProcessesNumber()
{
  return processesNumber;
}

void printPIDS()
{
  int i;
  for (i = 0; i < processesNumber; i++)
  {
    if (processesTable[i] != NULL)
    {
      printString("PID: ", 0, 155, 255);
      printInt(processesTable[i]->pid, 0, 155, 255);
      printString("\n", 0, 155, 255);

      printString("PPID: ", 0, 155, 255);
      printInt(processesTable[i]->ppid, 0, 155, 255);
      printString("\n", 0, 155, 255);

      printString("Name: ", 0, 155, 255);
      printString(processesTable[i]->name, 0, 155, 255);
      printString("\n", 0, 155, 255);
      printString("Threads: \n", 0, 155, 255);
      for (int j = 0; j < processesTable[i]->threadCount; j++)
      {
        printString("    TID: ", 0, 155, 255);
        printInt(j, 0, 155, 255);
        printString("\n", 0, 155, 255);
        printString("    Status: ", 0, 155, 255);
        char printStatus = getThreadStatus(processesTable[i]->threads[j]);
        if (printStatus == RUNNING)
        {
          printString("Running", 0, 155, 255);
        }
        else if (printStatus == READY)
        {
          printString("Ready", 0, 155, 255);
        }
        else if (printStatus == BLOCKED)
        {
          printString("Blocked", 0, 155, 255);
        }
        else if (printStatus == DELETE)
        {
          printString("Awaiting Deletion", 0, 155, 255);
        }
        else
        {
          printString("Error", 0, 155, 255);
        }
        printString("\n", 0, 155, 255);
      }

      printString("Data Page: ", 0, 155, 255);
      printInt((uint64_t)processesTable[i]->dataPage, 0, 155, 255);
      printString("\n", 0, 155, 255);

      printString("-------------------------------\n", 0, 155, 255);
    }
  }
}

void whileTrue()
{
  while (1)
  {
    _hlt();
  }
}

void removeThreadFromProcess(process *p, int tid)
{
  if (p == NULL || tid > MAX_THREADS || tid < 0)
    return;
  deleteThread(p->threads[tid]);
  p->threads[tid] = NULL;
  p->threadCount--;
  if (p->threadCount == 0)
    removeProcess(p);
}

threadADT getThread(process *p, int tid)
{
  if (p != NULL)
  {
    return p->threads[tid];
  }
  return NULL;
}

int deleteThisProcess(int pid)
{
  if (pid != 0 && pid != 1)
  {
    return removeProcess(getProcessByPid(pid));
  }

  return 0;
}

uint64_t getProcessThreadCount(int pid)
{
  return processesTable[pid]->threadCount;
}

int getAndIncreaseThreadCount(process *p)
{
  if (p != NULL)
  {
    int n = p->threadCount;
    p->threadCount++;
    return n;
  }
  return -1;
}

void addToProcess(threadADT t, int pid, int tid)
{
  if (processesTable[pid] != NULL)
  {
    processesTable[pid]->threads[tid] = t;
  }
}