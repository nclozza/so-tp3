#include <stdint.h>
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "prodconspipes.h"
#include "sysCallLib.h"
#include "execProcess.h"

//uint64_t bufferMutex = 0;
//int fullSem = 0;
//int emptySem = 0;
//int prodPipe;
//int consPipe;
static int itemsPipes = 0;

/*void putItemIntoBuffer()
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
}*/

void producerPipes(int argc)
{
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);

  int read;

  while(1)
  { 
    //sysSemWait(fullSem);
    //sysMutexDown(bufferMutex);
    sysPipeRead(prodPipe, &read, sizeof(int));

    sysPrintString("Producer: ",0,155,255);
    sysPrintInt(argc,0,155,255);
    sysPrintString("\n", 0,155,255);

    int randItem = rand();
    sysPrintString("Added an item: ",0,155,255);
    sysPrintInt(randItem,0,155,255);
    sysPrintString("\n", 0,155,255);

    sysPipeWrite(consPipe, &randItem, sizeof(int));

    itemsPipes++;
    sysPrintString("Total items: ", 0,155,255);
    sysPrintInt(itemsPipes,0,155,255);
    sysPrintString("\n\n", 0,155,255);


    //putItemIntoBuffer();

    //sysMutexUp(bufferMutex);
    //sysSemPost(emptySem);
  }
}

void consumerPipes(int argc)
{
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);

  int empty = 0;
  int read;

  while(1)
  {
    //sysSemWait(emptySem);
    //sysMutexDown(bufferMutex);

    sysPipeRead(consPipe, &read, sizeof(int));

    sysPrintString("Consumer: ",0,155,255);
    sysPrintInt(argc,0,155,255);
    sysPrintString("\n", 0,155,255);

    sysPrintString("Removed an item: ",0,155,255);
    sysPrintInt(read,0,155,255);
    sysPrintString("\n", 0,155,255);

    sysPipeWrite(prodPipe, &empty, sizeof(int));

    itemsPipes--;
    sysPrintString("Total items: ", 0,155,255);
    sysPrintInt(itemsPipes,0,155,255);
    sysPrintString("\n\n", 0,155,255);

    //removeItemFromBuffer();

    //sysMutexUp(bufferMutex);
    //sysSemPost(fullSem);
  }
}

/*void closePipesAndMutexes()
{
  sysPipeClose()
  sysClosePipeMutex();
}*/

void removeLastProducerPipes(int *prodc, int *producers)
{
  if(*prodc == 0)
  {
    return;
  }
  else
  {
    //sysMutexDown(bufferMutex);
    sysDeleteThisThread(producers[--*prodc]);
    //sysMutexUp(bufferMutex);
    return;
  }
}

void removeLastConsumerPipes(int *consc, int *consumers)
{
  if(*consc == 0) 
  {
    return;
  }
  else
  {
    sysDeleteThisThread(consumers[--*consc]);
  }
}

void runProdConsPipes()
{
  char c;
  int prodc = 0;
  int consc = 0;
  int producers[MAX_PRODUCERS];
  int consumers[MAX_CONSUMERS];
  int empty = 0;

  itemsPipes = 0;
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);
  sysCreatePipeMutex();
  //bufferMutex = sysMutexInit("bufferMutex");
  //fullSem = sysSemOpen("fullSem");
  //emptySem = sysSemOpen("emptySem");
  //sysSemWait(emptySem); //Semaphores are initialized in 1 and I need it to start with 0

  //Initialize the pipe with zeros indicating the amount of space available to produce
  for(int i = 0; i < BUFFER_SIZE - 1; i++)
  {
    sysPipeWrite(prodPipe, &empty, sizeof(int)); 
  }

  sysPrintString("- Press 'p' to add a producer\n", 0, 155, 255);
  sysPrintString("- Press 'c' to add a consumer\n", 0, 155, 255);
  sysPrintString("- Press 'o' to remove a producer\n", 0, 155, 255);
  sysPrintString("- Press 'x' to remove a consumer\n", 0, 155, 255);
  sysPrintString("- Press 'q' to quit prodcons\n\n", 0, 155, 255);


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
        producers[prodc] = execProcess(producerPipes, prodc + 1, NULL, "producerPipes", 1);
        prodc++;
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
        consumers[consc] = execProcess(consumerPipes, consc + 1, NULL, "consumerPipes", 1);
        consc++;
      }
      break;

      case REMOVE_PRODUCER:
      if(prodc == 0)
      {
        sysPrintString("Can't remove more producers\n\n", 0, 155, 255);
      }
      else
      {
        removeLastProducerPipes(&prodc, producers);
        sysPrintString("Removed producer ", 0, 155, 255);
        sysPrintInt(prodc + 1, 0, 155, 255);
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
        removeLastConsumerPipes(&consc, consumers);
        sysPrintString("Removed consumer ", 0, 155, 255);
        sysPrintInt(consc + 1, 0, 155, 255);
        sysPrintString("\n\n", 0, 155, 255);
      }
      break;

      case QUIT:
      while (consc > 0) {
        removeLastConsumerPipes(&consc, consumers);
      } 
      while (prodc > 0) {
        removeLastProducerPipes(&prodc, producers);
      }
      sysPrintString("Quitting successfully\n\n", 0, 155, 255);

      //sysPrintString("1\n\n", 0, 155, 255);
      //sysPipeClose(prodPipe);
      //sysPrintString("2\n\n", 0, 155, 255);
      //sysPipeClose(consPipe);
      //sysPrintString("3\n\n", 0, 155, 255);
      //sysClosePipeMutex();
      //sysPrintString("4\n\n", 0, 155, 255);
      return;
    }
  }
}