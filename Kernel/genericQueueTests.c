#include "genericQueue.h"
#include "testlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "videoDriver.h"

void testGenericQueueIsCreated();
void givenAQueue();
void whenQueueIsCreated();
void thenTheQueueIsReturned();
void thenTheQueueIsEmpty();
void givenAQueueAndAnElement();
void whenElementIsAddedToTheQueue();
void thenTheQueueIsNotEmpty();
void testElementPeekedFromQueue();
void whenElementIsPeeked();
void testElementDequeuedFromQueue();
void whenElementIsDequeued();
void finishedGenericQueueTests();
void runGenericQueueTests();

static queueADT queue;
static queueElement element;

void testGenericQueueIsCreated()
{
  givenAQueue();
  whenQueueIsCreated();
  thenTheQueueIsReturned();
  thenTheQueueIsEmpty();
}

void givenAQueue()
{
}

void whenQueueIsCreated()
{
  queue = createQueue();
}

void thenTheQueueIsReturned()
{
  checkIsNotNull(queue);
}

void thenTheQueueIsEmpty()
{
  if (queueIsEmpty(queue))
  {
    ok();
  }
  else
  {
    fail();
  }
}

void testElementAddedToQueue()
{
  givenAQueueAndAnElement();
  whenElementIsAddedToTheQueue();
  thenTheQueueIsNotEmpty();
}

void givenAQueueAndAnElement()
{
  element = (char *)"First Element";
}

void whenElementIsAddedToTheQueue()
{
  enqueue(queue, element);
}

void thenTheQueueIsNotEmpty()
{
  if (!queueIsEmpty(queue))
  {
    ok();
  }
  else
  {
    fail();
  }
}

void testElementPeekedFromQueue()
{
  givenAQueue();
  whenElementIsPeeked();
  thenTheQueueIsNotEmpty();
}

void whenElementIsPeeked()
{
  peek(queue);
}

void testElementDequeuedFromQueue()
{
  givenAQueue();
  whenElementIsDequeued();
  thenTheQueueIsEmpty();
}

void whenElementIsDequeued()
{
  dequeue(queue);
}

void finishedGenericQueueTests()
{
  deleteQueue(queue);
}

void runGenericQueueTests()
{
  printString("Testing Queue is created\n", 128, 128, 128);
  testGenericQueueIsCreated();

  printString("Testing Element is added to Queue\n", 128, 128, 128);
  testElementAddedToQueue();

  printString("Testing Element is peeked\n", 128, 128, 128);
  testElementPeekedFromQueue();

  printString("Testing Element is dequeued\n", 128, 128, 128);
  testElementDequeuedFromQueue();

  finishedGenericQueueTests();
}
