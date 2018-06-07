#include "processes.h"
#include "interrupts.h"
#include "videoDriver.h"
#include "time.h"
#include "keyboardDriver.h"
#include "memoryManager.h"
#include "mutex.h"
#include "semaphore.h"
#include "messageQueue.h"
#include "scheduler.h"
#include "memoryAllocator.h"
#include "pipes.h"

#define ERROR 1
#define SUCCESS 0

uint64_t mallocMemory(uint64_t size)
{
  void *page = (void *)malloc(size);
  addDataPage(getCurrentProcess(), page);
  //add Process to scheduler
  return (uint64_t)page;
}

void freeMemory(uint64_t page)
{
  free((void *)page);
}
void setForeground(int pid)
{
  process* p = getCurrentProcess();
  setThreadForeground(getThread(p,0));
}
uint64_t sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9)
{
  switch (rdi)
  {
  case 1:
    getTimeRTC(rsi);
    return SUCCESS;
  case 2:
    getChar((char *)rsi);
    return SUCCESS;
  case 3:
    clearScreen();
    return SUCCESS;
  case 4:
    writeChar((char)rsi, rdx, rcx, r8);
    return SUCCESS;
  case 5:
    paintPixel(rsi, rdx, (char)rcx, (char)r8, (char)r9);
    return SUCCESS;
  case 6:
    return mallocMemory(rsi);
  case 7:
    freeMemory(rsi);
    return SUCCESS;
  case 8:
    mutexUnlock((mutexADT)rsi);
    return SUCCESS;
  case 9:
    mutexLock((mutexADT)rsi);
    return SUCCESS;
  case 10:
    return (uint64_t)mutexInit((char *)rsi);
  case 11:
    return (uint64_t)mutexClose((mutexADT)rsi);
  case 12:
    return semOpen((char *)rsi);
  case 13:
    return semClose((int)rsi);
  case 14:
    return semWait((int)rsi);
  case 15:
    return semPost((int)rsi);
  case 16:
    return semaphoresListSize();
  case 17:
    freeSemaphoresList();
    return SUCCESS;
  case 18:
    return createMessage((char *)rsi, (int)rdx);
  case 19:
    return openMessage((char *)rsi, (int)rdx);
  case 20:
    return readMessage((char *)rsi, (int)rdx);
  case 21:
    return writeMessage((char *)rsi, (int)rdx);
  case 22:
    return closeMessage((char *)rsi, (int)rdx);
  case 23:
    return runThread(getThread(createProcess(rsi, rdx, rcx, r8,(char *)r9), 0));
  case 24:
    setForeground((int)rsi);
    return SUCCESS;
  case 25:
    killThread();
    return SUCCESS;
  case 26:
    return getProcessPpid(getCurrentProcess());
  case 27:
    printPIDS();
    return SUCCESS;
  case 28:
    exitShell();
    return SUCCESS;
  case 29:
    return (uint64_t)malloc(PAGE_SIZE);
  case 30:
    printBlockedThreadsList();
    return SUCCESS;
  case 31:
    return deleteThisProcess((int) rsi);
  case 32:
    whileTrue();
    return SUCCESS;
  case 33:
    return pipeOpen((char*) rsi);
  case 34:
    return pipeClose((int) rsi);
  case 35:
    return pipeWrite((int) rsi, (const void *) rdx, (int) rcx);    
  case 36:
    return pipeRead((int) rsi, (void *) rdx, (int) rcx);  
  case 37:
    createPipeMutex();
    return SUCCESS;
  case 38:
    closePipeMutex();
    return SUCCESS;   
  case 50:
    putThreadOnWait(getCurrentThread(), getThread(getProcessByPid((int)rsi), 0));
    yieldThread();
    return SUCCESS;
  case 51:
    return runThread(createThread(getProcessPid(getCurrentProcess()), rsi,rdx,rcx,(char**)r8, getAndIncreaseThreadCount(getCurrentProcess())));
  case 52:
    removeThreadFromProcess(getCurrentProcess(), (int)rsi);
    return SUCCESS;
  }
  return ERROR;
}
