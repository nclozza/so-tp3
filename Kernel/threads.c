#include "thread.h"
#include "processes.h"
#include "defs.h"
#include "memoryManager.h"
#include "memoryAllocator.h"
#include "videoDriver.h"
typedef struct thread_t
{
  int tid;
  int pid;
  int status;
  int foreground;
  uint64_t rsp;
  uint64_t stackPage;
  struct thread_t *waiting;
  uint64_t openFds;
} thread_t;
void aboutThread(threadADT t)
{
  printString("\nThread info", 0, 155, 255);
  printString("\nPID: ", 0, 155, 255);
  printInt(t->pid, 0, 155, 255);
  printString("\nTID: ", 0, 155, 255);
  printInt(t->tid, 0, 155, 255);
  printString("\nSTATUS: ", 0, 155, 255);
  printInt(t->status, 0, 155, 255);
  printString("\nFOREGROUND: ", 0, 155, 255);
  printInt(t->foreground, 0, 155, 255);
  printString("\nWAITING THREAD: ", 0, 155, 255);
  if (t->waiting != NULL)
    printInt(t->waiting->pid, 0, 155, 255);
  printString("\n-----------------------\n", 0, 155, 255);
}
threadADT createThread(int pid, int foreground, uint64_t rsp, int argc, char *argv[], int tid)
{
  threadADT newTCB = (threadADT)malloc(sizeof(thread_t));
  newTCB->tid = tid;
  newTCB->pid = pid;
  newTCB->waiting = NULL;
  newTCB->foreground = foreground;
  newTCB->status = 1;
  newTCB->openFds = 0;
  newTCB->stackPage = (uint64_t)malloc(MB);
  newTCB->rsp = createNewThreadStack(rsp, newTCB->stackPage, argc, (uint64_t)argv);
  addToProcess(newTCB, pid, tid);
  return newTCB;
}

/* Llena el stack para que sea hookeado al cargar un nuevo proceso
** https://bitbucket.org/RowDaBoat/wyrm */

uint64_t createNewThreadStack(uint64_t rip, uint64_t stackPage, uint64_t argc, uint64_t argv)
{
  stackFrame *newStackFrame = (stackFrame *)stackPage - 1;

  newStackFrame->gs = 0x001;
  newStackFrame->fs = 0x002;
  newStackFrame->r15 = 0x003;
  newStackFrame->r14 = 0x004;
  newStackFrame->r13 = 0x005;
  newStackFrame->r12 = 0x006;
  newStackFrame->r11 = 0x007;
  newStackFrame->r10 = 0x008;
  newStackFrame->r9 = 0x009;
  newStackFrame->r8 = 0x00A;
  newStackFrame->rsi = argv;
  newStackFrame->rdi = argc;
  newStackFrame->rbp = 0x00D;
  newStackFrame->rdx = 0x00E;
  newStackFrame->rcx = 0x00F;
  newStackFrame->rbx = 0x010;
  newStackFrame->rax = 0x011;
  newStackFrame->rip = rip;
  newStackFrame->cs = 0x008;
  newStackFrame->eflags = 0x202;
  newStackFrame->rsp = (uint64_t) & (newStackFrame->base);
  newStackFrame->ss = 0x000;
  newStackFrame->base = 0x000;

  return (uint64_t)&newStackFrame->gs;
}

int removeThread(threadADT thread)
{
  if (thread == NULL)
    return -1;
  removeThreadFromProcess(getProcessByPid(thread->pid), thread->tid);
  free((void *)thread->stackPage);
  threadADT waiting = thread->waiting;
  free((void *)thread);
  unblockThread(waiting);
  return 0;
}

void setThreadRsp(threadADT t, uint64_t rsp)
{
  if (t != NULL)
    t->rsp = rsp;
}

uint64_t getThreadRsp(threadADT t)
{
  if (t != NULL)
    return t->rsp;
  return -1;
}

uint64_t getThreadPid(threadADT t)
{
  if (t != NULL)
    return t->pid;
  return -1;
}

void blockThread(threadADT t)
{
  if (t != NULL && t->status != DELETE)
  {
    t->status = BLOCKED;
  }
}

void unblockThread(threadADT t)
{
  if (t != NULL && t->status != DELETE)
    t->status = READY;
}

int isThreadBlocked(threadADT t)
{
  if (t != NULL)
    return t->status == BLOCKED;
  return 1;
}

int isThreadReady(threadADT t)
{
  if (t != NULL)
    return t->status == READY;
  return 1;
}

void setThreadForeground(threadADT t)
{
  if (t != NULL && t->pid != 0)
  {
    t->foreground = 1;
  }
}

int isThreadForeground(threadADT t)
{
  if (t != NULL)
  {
    return t->foreground == 1;
  }

  return 0;
}

int deleteThread(threadADT t)
{
  if (t != NULL && t->pid != 1 && t->pid != 0)
    t->status = DELETE;

  return t != NULL;
}

int isThreadDeleted(threadADT t)
{
  if (t != NULL)
    return t->status == DELETE;
  return 0;
}

void putThreadOnWait(threadADT t1, threadADT t2)
{
  if (t1 == NULL || t2 == NULL)
    return;
  blockThread(t1);
  t2->waiting = t1;
}

int setFileOpen(threadADT t, int fd)
{
  if (fd >= MAX_FDS)
    return 0;
  t->openFds |= 1 << fd; // Settea bit en posicion fd en 1
  return 1;
}

int fileIsOpen(threadADT t, int fd)
{
  return fd < MAX_FDS && t->openFds;
}

int getThreadStatus(threadADT t)
{
  if (t != NULL)
  {
    return t->status;
  }
  return -1;
}