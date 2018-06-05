#include "init.h"
#include "scheduler.h"
#include "videoDriver.h"
#include "tests.h"
#include "processes.h"
#include "memorymanager.h"

static void *const sampleCodeModuleAddress = (void *)0x400000;

void init()
{		
	//runTests();
	runThread(getThread(createProcess((uint64_t)sampleCodeModuleAddress,1, 0,0, "shell"),0));		
	while (1)
	{
		_hlt();
	}
}
