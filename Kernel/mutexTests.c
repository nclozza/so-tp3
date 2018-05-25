#include "mutex.h"
#include "testlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "videoDriver.h"

char* mutexName;
mutexADT openedMutex = NULL;
mutexADT testingMutex = NULL;
int testingMutexValue;
int listSizeMutex = 0;
int sizeAux = 0;

void givenAMutexName();
void whenMutexIsPassedToOpen();
void thenMutexIsReturned();
void givenAMutex();
void givenAMutexLocked();
void whenMutexIsLocked();
void thenValueOfMutexDecreases();
void whenUnlockedMutex();
void thenValueOfMutexIncreases();
void thenSizeOfMutexListRemainsTheSame();
void thenSizeOfMutexListIncreases();
void whenMutexIsClosed();
void thenSizeOfMutexListDecreases();

void testMutexIsCreated()
{
	givenAMutexName();
	whenMutexIsPassedToOpen();
	thenMutexIsReturned();
	thenSizeOfMutexListIncreases();
}

void testMutexValueDecreasesWhenLock()
{
	givenAMutex();
	whenMutexIsLocked();
	thenValueOfMutexDecreases();
}

void testMutexValueIncreasesWhenUnlock()
{
	givenAMutexLocked();
    whenUnlockedMutex();
	thenValueOfMutexIncreases();
}

void testIfMutexAlreadyCreatedDoesNotCreateNewOne()
{
	givenAMutexName();
	whenMutexIsPassedToOpen();
	thenSizeOfMutexListRemainsTheSame();
	thenMutexIsReturned();
}

void testMutexIsClosed()
{
	givenAMutex();
	whenMutexIsClosed();
	thenSizeOfMutexListDecreases();
}


void givenAMutexName()
{
	mutexName = "MutNameForTest";
}

void whenMutexIsPassedToOpen()
{
	openedMutex = mutexInit(mutexName);
}

void thenMutexIsReturned()
{
	checkIsNotNull((void*)openedMutex);
}

void givenAMutex()
{
	if(testingMutex == NULL)
		testingMutex = openedMutex;
}

void whenMutexIsLocked()
{
	testingMutexValue = mutexLock(testingMutex);
}

void thenValueOfMutexDecreases()
{
	checkIsZero(testingMutexValue);
}

void givenAMutexLocked()
{

}

void whenUnlockedMutex()
{
    testingMutexValue = mutexUnlock(testingMutex);
}

void thenValueOfMutexIncreases()
{
    checkIsNotZero(testingMutexValue);
}

void thenSizeOfMutexListRemainsTheSame()
{
	sizeAux = mutexListSize();
	checkSizeOfSemaphoreList(listSizeMutex,sizeAux);	
}

void thenSizeOfMutexListIncreases()
{
	sizeAux = mutexListSize();
	listSizeMutex++;
	checkSizeOfSemaphoreList(listSizeMutex,sizeAux);
}

void whenMutexIsClosed()
{
	if(testingMutex!=NULL)
		mutexClose(testingMutex);
}
void thenSizeOfMutexListDecreases()
{
	listSizeMutex--;
	sizeAux = mutexListSize();
	checkSizeOfSemaphoreList(listSizeMutex,sizeAux);
}

void finishedMutexTesting()
{
	mutexClose(openedMutex);
	freeMutexList();
}

void runMutexTests()
{
	printString("Testing Mutex is created...\n", 128, 128, 128);
	testMutexIsCreated();

	printString("Testing Mutex value decreases when lock...\n", 128, 128, 128);
	testMutexValueDecreasesWhenLock();

	printString("Testing Mutex value increases when unlock...\n", 128, 128, 128);
	testMutexValueIncreasesWhenUnlock();

	printString("Testing if Mutex already exists, none is created\n", 128, 128, 128);
	testIfMutexAlreadyCreatedDoesNotCreateNewOne();

	printString("Testing closing a Mutex...\n", 128, 128, 128);
	testMutexIsClosed();

	finishedMutexTesting();
}