#include "scheduler.h"
#include "processes.h"
#include "testlib.h"
#include "videoDriver.h"

threadADT testThread = NULL;
char* threadName;
int newThreadRIP;
int argc;
int argv;
int testAux;
int testPid;

void givenAThread();
void whenThreadIsCreated();
void thenThreadIsNotNull();
void givenACreatedThread();
void whenThreadIsRemoved();
void thenThreadIsNull();
void whenThreadIsBlocked();
void thenThreadStatusIsBlocked();
void givenABlockedThread();
void whenAThreadIsUnblocked();
void thenThreadStatusIsReady();
void givenAThreadPid();
void whenGetAThreadByPid();
void thenPidsAreEqual();
void givenACreatedThread();
void whenAllThreadPagesAreNull();
void thenThreadDataPageCountIsZero();

void testCreateThread()
{
    givenAThread();
    whenThreadIsCreated();
    thenThreadIsNotNull();
}


void testBlockThread()
{
    givenACreatedThread();
    whenThreadIsBlocked();
    thenThreadStatusIsBlocked();
}

void testUnblockThread()
{
    givenABlockedThread();
    whenAThreadIsUnblocked();
    thenThreadStatusIsReady();
}


void testRemoveThread()
{
    givenACreatedThread();
    whenThreadIsRemoved();
    thenThreadIsNull();
}


void givenAThread()
{
    threadName = "threadTesting";
    newThreadRIP = 10;
    argc = 0;
    argv = 0;
}

void whenThreadIsCreated()
{    
    testThread = getThread(createProcess((uint64_t) newThreadRIP, 1,(uint64_t) argc, (uint64_t) argv, threadName),0);
}

void thenThreadIsNotNull()
{
    checkIsNotNull((void*)testThread);
}

void givenACreatedThread()
{
    if (testThread == NULL)
    {
        givenAThread();
        whenThreadIsCreated();
    }        
}

void whenThreadIsRemoved()
{
    removeThread(testThread);
    testAux = isThreadDeleted(testThread);
}

void thenThreadIsNull()
{
    checkIsZero(testAux);
}

void whenThreadIsBlocked()
{
    blockThread(testThread);
}

void thenThreadStatusIsBlocked()
{    
    if (isThreadBlocked(testThread))
    {
        testAux = 0;
        checkIsZero(testAux);
    }
    else
    {
        testAux = 1;
        checkIsZero(testAux);
    }
}

void givenABlockedThread()
{
    if (!isThreadBlocked(testThread))
    {
        givenACreatedThread();
        whenThreadIsBlocked();
    }
}


void whenAThreadIsUnblocked()
{
    unblockThread(testThread);
}

void thenThreadStatusIsReady()
{
    if (isThreadReady(testThread))
    {
        testAux = 0;
        checkIsZero(testAux);
    }
    else
    {
        testAux = 1;
        checkIsZero(testAux);
    }
}

void whenThreadIsRunning()
{
    testPid =(int) runThread(testThread);
}

void thenPidIsNotZero()
{
    checkIsNotZero(testPid);
}

void thenPidsAreEqual()
{
    checkAreEqual(testPid, getThreadPid(testThread));
}


void runSchedulerTests()
{
    printString("Testing thread is created...\n", 128, 128, 128);
    testCreateThread();

    printString("Testing thread is blocked...\n", 128, 128, 128);    
    testBlockThread();
    
    printString("Testing thread is unblocked...\n", 128, 128, 128);    
    testUnblockThread();

    printString("Testing Thread is removed...\n", 128, 128, 128);
    testRemoveThread();

}