#include <stdint.h>
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "prodconspipes.h"
#include "sysCallLib.h"

int producersPipes[MAX_PRODUCERS];
int consumersPipes[MAX_CONSUMERS];
static int tidPipes = 1;
static int itemsPipes = 0;

void producerPipes()
{
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);

  int read;

  while(1)
  { 
    sysPipeRead(prodPipe, &read, sizeof(int));

    sysPrintString("Producer",0,155,255);
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
  }
}

void consumerPipes()
{
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);

  int empty = 0;
  int read;

  while(1)
  {
    sysPipeRead(consPipe, &read, sizeof(int));

    sysPrintString("Consumer",0,155,255);
    sysPrintString("\n", 0,155,255);

    sysPrintString("Removed an item: ",0,155,255);
    sysPrintInt(read,0,155,255);
    sysPrintString("\n", 0,155,255);

    sysPipeWrite(prodPipe, &empty, sizeof(int));

    itemsPipes--;
    sysPrintString("Total items: ", 0,155,255);
    sysPrintInt(itemsPipes,0,155,255);
    sysPrintString("\n\n", 0,155,255);
  }
}

void removeLastProducerPipes(int *prodc)
{
  if(*prodc == 0)
  {
    return;
  }
  else
  {
    int lastProd = producersPipes[(*prodc)-1];
    sysRemoveThreadFromProcess(lastProd);
    *prodc -= 1;
    return;
  }
}

void removeLastConsumerPipes(int *consc)
{
  if(*consc == 0) 
  {
    return;
  }
  else
  {
    int lastCons = consumersPipes[(*consc)-1];
    sysRemoveThreadFromProcess(lastCons);
    *consc -= 1;
    return;
  }
}

void runProdConsPipes()
{ 
  char c;
  int prodc = 0;
  int consc = 0;
  int empty = 0;

  itemsPipes = 0;
  int prodPipe = sysPipeOpen(PRODUCER_PIPE);
  int consPipe = sysPipeOpen(CONSUMER_PIPE);
  sysCreatePipeMutex();

  //Initialize the pipe with zeros indicating the amount of space available to produce
  for(int i = 0; i < BUFFER_SIZE; i++)
  {
    sysPipeWrite(prodPipe, &empty, sizeof(int)); 
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
        sysCreateThread(1,producerPipes,0,NULL);      
        producersPipes[prodc] = tidPipes;
        prodc++;
        tidPipes++;        
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
        sysCreateThread(1,consumerPipes,0,NULL);        
        consumersPipes[consc] = tidPipes;
        consc++;
        tidPipes++;        
      }
      break;

      case REMOVE_PRODUCER:
      if(prodc == 0)
      {
        sysPrintString("Can't remove more producers\n\n", 0, 155, 255);
      }
      else
      {
        removeLastProducerPipes(&prodc);        
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
        removeLastConsumerPipes(&consc);        
        sysPrintString("Removed consumer ", 0, 155, 255);        
        sysPrintString("\n\n", 0, 155, 255);
      }
      break;

      case QUIT: 
      while (tidPipes >= 0) {
        sysRemoveThreadFromProcess(tidPipes);
        tidPipes--;
      }

      sysPipeClose(prodPipe);
      sysPipeClose(consPipe);
      sysClosePipeMutex();
      sysPrintString("Quitting successfully\n\n", 0, 155, 255);
      return;

      case PS:
      sysPrintPIDS();      
      break;
    }
  }
}