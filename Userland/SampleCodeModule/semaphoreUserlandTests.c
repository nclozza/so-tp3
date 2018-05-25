#include "stdLib.h"
#include "shell.h"
#include <stdint.h>
#include "sysCallLib.h"

char* name;
int openedSemaphore = -1;
int testingSemaphore = -1;
int testingSemaphoreValue;
int listSize = 0;
int size = 0;

void givenAName();
void whenNameIsPassedToOpen();
void thenSemaphoreIsReturned();
void givenASemaphore();
void whenSemaphoreIsPosted();
void thenValueOfSemaphoreIncreases();
void thenSizeOfSemaphoreListRemainsTheSame();
void thenSizeOfSemaphoreListIncreases();
void whenSemaphoreIsClosed();
void thenSizeOfSemaphoreListDecreases();
void whenWait();

void testSemaphoreIsCreated()
{
	givenAName();
	whenNameIsPassedToOpen();
	thenSemaphoreIsReturned();
	thenSizeOfSemaphoreListIncreases();
}

void testSemaphoreValueIncreasesWhenPostedTo()
{
	givenASemaphore();
	whenSemaphoreIsPosted();
	thenValueOfSemaphoreIncreases();
}

void testIfSemaphoreAlreadyCreatedDoesNotCreateNewOne()
{
	givenAName();
	whenNameIsPassedToOpen();
	thenSizeOfSemaphoreListRemainsTheSame();
	thenSemaphoreIsReturned();
}

void testSemaphoreIsClosed()
{
	givenASemaphore();
	whenSemaphoreIsClosed();
	thenSizeOfSemaphoreListDecreases();
}

void testWait()
{
	givenASemaphore();
	whenWait();
}

void givenAName()
{
	name = "SemNameForTest";
}

void whenNameIsPassedToOpen()
{
	openedSemaphore = sysSemOpen(name);
}

void thenSemaphoreIsReturned()
{
	checkIsNotMinusOne(openedSemaphore);
}

void givenASemaphore()
{
	if(testingSemaphore == -1 && openedSemaphore !=-1)
		testingSemaphore = openedSemaphore;	
}

void whenWait()
{
	sysSemWait(testingSemaphore);
}
void whenSemaphoreIsPosted()
{
	testingSemaphoreValue = sysSemPost(testingSemaphore);
}

void thenValueOfSemaphoreIncreases()
{
	checkIsNotMinusOne(testingSemaphoreValue);
}

void thenSizeOfSemaphoreListRemainsTheSame()
{
	size = sysSemaphoresListSize();
	checkSizeOfSemaphoreList(listSize,size);	
}

void thenSizeOfSemaphoreListIncreases()
{
	size = sysSemaphoresListSize();
	listSize++;
	checkSizeOfSemaphoreList(listSize,size);
}

void whenSemaphoreIsClosed()
{
	if(testingSemaphore!=-1)
	{
		sysSemClose(testingSemaphore);
		testingSemaphore = -1;
	}			
}
void thenSizeOfSemaphoreListDecreases()
{	
	listSize--;
	size = sysSemaphoresListSize();
	checkSizeOfSemaphoreList(listSize,size);
}

void finishedTesting()
{
	sysSemClose(openedSemaphore);
	sysFreeSemaphoresList();
}

void runUserlandSemaphoreTests()
{
	sysPrintString("Testing semaphore is created...\n",128,128,128);
	testSemaphoreIsCreated();

	sysPrintString("Testing semaphore value increases when posted...\n",128,128,128);
	testSemaphoreValueIncreasesWhenPostedTo();

	sysPrintString("Testing if semaphore already exists, none is created\n",128,128,128);
	testIfSemaphoreAlreadyCreatedDoesNotCreateNewOne();

	sysPrintString("Testing closing a semaphore...\n",128,128,128);
	testSemaphoreIsClosed();

	// sysPrintString("Testing wait on semaphore, should lock process...\n",128,128,128);
	// testWait();

	finishedTesting();	
}