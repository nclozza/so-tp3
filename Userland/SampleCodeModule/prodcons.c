#include <stdint.h>
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "prodcons.h"
#include "sysCallLib.h"


uint64_t bufferMutex = 0;
int fullSem = 0;
int emptySem = 0;
static int items = 0;
static int tid = 1;
int producers[MAX_PRODUCERS];
int consumers[MAX_CONSUMERS];

void putItemIntoBuffer()
{
  items++;
  sysPrintString("Added an item\n",0,155,255);
  sysPrintString("Total items: ", 0,155,255);
  sysPrintInt(items,0,155,255);
  sysPrintString("\n\n", 0,155,255);
}

void removeItemFromBuffer()
{
  items--;
  sysPrintString("Removed an item\n",0,155,255);
  sysPrintString("Total items: ", 0,155,255);
  sysPrintInt(items,0,155,255);
  sysPrintString("\n\n", 0,155,255);
}

void producer()
{
  while(1)
  { 
    sysSemWait(fullSem);
    sysMutexDown(bufferMutex);

    sysPrintString("Producer",0,155,255);
    sysPrintString("\n", 0,155,255);
    putItemIntoBuffer();

    sysMutexUp(bufferMutex);
    sysSemPost(emptySem);
  }
}

void consumer()
{
  while(1)
  {
    sysSemWait(emptySem);
    sysMutexDown(bufferMutex);

    sysPrintString("Consumer",0,155,255);
    sysPrintString("\n", 0,155,255);
    removeItemFromBuffer();

    sysMutexUp(bufferMutex);
    sysSemPost(fullSem);
  }
}

void closeSemaphoresAndMutexes()
{
  sysMutexClose(bufferMutex);
  sysSemClose(fullSem);
  sysSemClose(emptySem);
}

void removeLastProducer(int *prodc)
{
  if(*prodc == 0)
  {
    return;
  }
  else
  {
    sysMutexDown(bufferMutex);
    int lastProd = producers[(*prodc)-1];
    sysRemoveThreadFromProcess(lastProd);
    *prodc -= 1;
    sysMutexUp(bufferMutex);
    return;
  }
}

void removeLastConsumer(int *consc)
{
  if(*consc == 0) 
  {
    return;
  }
  else
  {
    sysMutexDown(bufferMutex);
    int lastConsc = consumers[(*consc)-1];
    sysRemoveThreadFromProcess(lastConsc);
    *consc -= 1;
    sysMutexUp(bufferMutex);
    return;
  }
}

void runProdCons()
{
  char c;  
  int prodc = 0;
  int consc = 0;
  items = 0;  
  bufferMutex = sysMutexInit("bufferMutex");    
  fullSem = sysSemOpen("fullSem");
  emptySem = sysSemOpen("emptySem");
  sysSemWait(emptySem); //Semaphores are initialized in 1 and I need it to start with 0;

  for(int i = 0; i < BUFFER_SIZE - 1; i++)
  {
    sysSemPost(fullSem);
  }

  sysPrintString("- Press 'p' to add a producer\n", 0, 155, 255);
  sysPrintString("- Press 'c' to add a consumer\n", 0, 155, 255);
  sysPrintString("- Press 'o' to remove a producer\n", 0, 155, 255);
  sysPrintString("- Press 'x' to remove a consumer\n", 0, 155, 255);
  sysPrintString("- Press 'q' to quit prodcons\n", 0, 155, 255);
  sysPrintString("- Press 'l' to show ps\n\n", 0, 155, 255);


  while(1)
  {
    sysGetChar(&c);
    switch(c)
    {
      case ADD_PRODUCER:
      if(prodc >= MAX_PRODUCERS)
      {
        sysPrintString("Can't add more producers\n\n", 0, 155, 255);
      }
      else
      {
        sysPrintString("Added producer ", 0, 155, 255);
        sysPrintInt(prodc + 1, 0, 155, 255);
        sysPrintString("\n\n", 0, 155, 255);        
        sysCreateThread(1,producer,0,NULL);      
        producers[prodc] = tid;
        prodc++;
        tid++;        
      }
      break;

      case ADD_CONSUMER:
      if(consc >= MAX_CONSUMERS)
      {
        sysPrintString("Can't add more consumers\n\n", 0, 155, 255);
      }
      else
      {
        sysPrintString("Added consumer ", 0, 155, 255);
        sysPrintInt(consc + 1, 0, 155, 255);
        sysPrintString("\n\n", 0, 155, 255);                
        sysCreateThread(1,consumer,0,NULL);        
        consumers[consc] = tid;
        consc++;
        tid++;        
      }
      break;

      case REMOVE_PRODUCER:
      if(prodc == 0)
      {
        sysPrintString("Can't remove more producers\n\n", 0, 155, 255);
      }
      else
      {
        removeLastProducer(&prodc);        
        sysPrintString("Removed producer ", 0, 155, 255);        
        sysPrintString("\n\n", 0, 155, 255);
      }
      break;

      case REMOVE_CONSUMER:
      if(consc == 0)
      {
        sysPrintString("Can't remove more consumers\n\n", 0, 155, 255);
      }
      else
      {
        removeLastConsumer(&consc);        
        sysPrintString("Removed consumer ", 0, 155, 255);        
        sysPrintString("\n\n", 0, 155, 255);
      }
      break;

      case QUIT:      
      while (tid > 0) {
        sysRemoveThreadFromProcess(tid);
        tid--;
      }
      sysPrintString("Quitting successfully\n\n", 0, 155, 255);
      closeSemaphoresAndMutexes();
      return;

      case PS:
      sysPrintPIDS();      
      break;
    }
  }
}