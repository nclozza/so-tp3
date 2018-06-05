#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include "interrupts.h"
#include "videoDriver.h"
#include "idtLoader.h"
#include "stdio.h"
#include "scheduler.h"
#include "pageallocator.h"
#include "init.h"
//#include "buddyAllocator.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
	return (void *)((uint64_t)&endOfKernel + PageSize * 8 //The size of the stack itself, 32KiB
									- sizeof(uint64_t)										//Begin at the top of the stack
	);
}

void *initializeKernelBinary()
{

	void *moduleAddresses[] = {
			sampleCodeModuleAddress,
			sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int twoPow(int num)
{
	if (num == 0)
	{
		return 1;
	}
	return 2 * twoPow(num - 1);
}

int main()
{
	load_idt();
	paintBackGround();
	//initializePageAllocator();

	createHeap();

	runThread(getThread(createProcess((uint64_t)init, 1, 0, 0, "init"), 0));

	while (1)
	{
		_hlt();
	}

	return 0;
}
