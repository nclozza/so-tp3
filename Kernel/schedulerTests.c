#include "scheduler.h"
#include "processes.h"
#include "testlib.h"
#include "videoDriver.h"

process* testProcess = NULL;
char* processName;
int newProcessRIP;
int argc;
int argv;
int testAux;
int testPid;

void givenAProcess();
void whenProcessIsCreated();
void thenProcessIsNotNull();
void givenACreatedProcess();
void whenProcessIsRemoved();
void thenProcessIsNull();
void whenProcessIsBlocked();
void thenProcesStatusIsBlocked();
void givenABlockedProcess();
void whenAProcessIsUnblocked();
void thenProcessStatusIsReady();
void givenAProcessPid();
void whenGetAProcessByPid();
void thenPidsAreEqual();
void givenACreatedProcess();
void whenAllProcessPagesAreNull();
void thenProcessDataPageCountIsZero();

void testCreateProcess()
{
    givenAProcess();
    whenProcessIsCreated();
    thenProcessIsNotNull();
}


void testBlockProcess()
{
    givenACreatedProcess();
    whenProcessIsBlocked();
    thenProcesStatusIsBlocked();
}

void testUnblockProcess()
{
    givenABlockedProcess();
    whenAProcessIsUnblocked();
    thenProcessStatusIsReady();
}

void testGetProcessByPid()
{
    givenAProcessPid();
    whenGetAProcessByPid();
    thenPidsAreEqual();    
}

void testSetNullProcessPages()
{
    givenACreatedProcess();
    whenAllProcessPagesAreNull();
    thenProcessDataPageCountIsZero();
}

void testRemoveProcess()
{
    givenACreatedProcess();
    whenProcessIsRemoved();
    thenProcessIsNull();
}


void givenAProcess()
{
    processName = "processTesting";
    newProcessRIP = 10;
    argc = 0;
    argv = 0;
}

void whenProcessIsCreated()
{
    testProcess = createProcess((uint64_t) newProcessRIP, (uint64_t) argc, (uint64_t) argv, processName);
}

void thenProcessIsNotNull()
{
    checkIsNotNull((void*)testProcess);
}

void givenACreatedProcess()
{
    if (testProcess == NULL)
    {
        givenAProcess();
        whenProcessIsCreated();
    }        
}

void whenProcessIsRemoved()
{
    deleteProcess(testProcess);
    removeProcess(testProcess);
    testAux = isProcessDeleted(testProcess);
}

void thenProcessIsNull()
{
    checkIsZero(testAux);
}

void whenProcessIsBlocked()
{
    blockProcess(testProcess);
}

void thenProcesStatusIsBlocked()
{
    if (testProcess->status == BLOCKED)
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

void givenABlockedProcess()
{
    if (testProcess->status != BLOCKED)
    {
        givenACreatedProcess();
        whenProcessIsBlocked();
    }
}


void whenAProcessIsUnblocked()
{
    unblockProcess(testProcess);
}

void thenProcessStatusIsReady()
{
    if (testProcess->status == READY)
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

void whenProcessIsRunning()
{
    testPid =(int) runProcess(testProcess);
}

void thenPidIsNotZero()
{
    checkIsNotZero(testPid);
}

void givenAProcessPid()
{
    givenACreatedProcess();
    testPid = testProcess->pid;
}

void whenGetAProcessByPid()
{
    testProcess = getProcessByPid(testPid);
}

void thenPidsAreEqual()
{
    checkAreEqual(testPid, testProcess->pid);
}

void whenAllProcessPagesAreNull()
{
    setNullAllProcessPages(testProcess);
}

void thenProcessDataPageCountIsZero()
{
    checkIsZero(testProcess->dataPageCount);
}


void runSchedulerTests()
{
    printString("Testing process is created...\n", 128, 128, 128);
    testCreateProcess();

    printString("Testing process is blocked...\n", 128, 128, 128);    
    testBlockProcess();
    
    printString("Testing process is unblocked...\n", 128, 128, 128);    
    testUnblockProcess();

    printString("Testing get a process by PID...\n", 128, 128, 128);    
    testGetProcessByPid();

    printString("Testing set null all process pages...\n", 128, 128, 128);
    testSetNullProcessPages();

    printString("Testing process is removed...\n", 128, 128, 128);
    testRemoveProcess();

}