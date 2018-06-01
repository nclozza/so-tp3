#include "pipes.h"
#include "testlib.h"
#include <stdio.h>
#include <stdlib.h>
#include "videoDriver.h"

char* pipeTestName;
int id = -1;
int pipeAux;

void givenAPipeName();
void whenAPipeIsCreated();
void thenAPipeIdIsNotNegative();
void givenAnOpenPipe();
void whenWriteInPipe();
void thenReturnGreaterThanZero();
void whenReadInPipe();
void whenAPipeIsClosed();
void thenReturnIsOne();

void testCreatePipe()
{
    givenAPipeName();
    whenAPipeIsCreated();
    thenAPipeIdIsNotNegative();
}

void testWriteInPipe()
{
    givenAnOpenPipe();
    whenWriteInPipe();
    thenReturnGreaterThanZero();
}

void testReadInPipe()
{
    givenAnOpenPipe();
    whenReadInPipe();
    thenReturnGreaterThanZero();
}

void testClosePipe()
{
    givenAnOpenPipe();
    whenAPipeIsClosed();
    thenReturnIsOne();
}

void givenAPipeName()
{
    pipeTestName = "TestPipe";
}

void whenAPipeIsCreated()
{
    createPipeMutex();
    id = pipeOpen(pipeTestName);
}

void thenAPipeIdIsNotNegative()
{
    checkIsPositive(id);        
}

void givenAnOpenPipe()
{
    if (id < 0)
    {
        givenAPipeName();
        whenAPipeIsCreated();
    }
}

void whenWriteInPipe()
{
    pipeAux = pipeWrite(id, "prueba", 2);
}

void thenReturnGreaterThanZero()
{
    checkIsGreaterThanZero(pipeAux);
}

void whenReadInPipe()
{
    pipeAux = pipeRead(id, "prueba", 2);
}

void whenAPipeIsClosed()
{
    pipeAux = pipeClose(id);
}

void thenReturnIsOne()
{
    checkAreEqual(1,pipeAux);
}

void runPipesTests()
{
	printString("Testing create a Pipe...\n", 128, 128, 128);
	testCreatePipe();

    printString("Testing write in Pipe...\n", 128, 128, 128);
    testWriteInPipe();

    printString("Testing read in Pipe...\n", 128, 128, 128);
    testReadInPipe();

    printString("Testing close a Pipe...\n", 128, 128, 128);
    testClosePipe();

    closePipeMutex();
}