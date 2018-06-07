#include <stdint.h>
#include "memoryAllocator.h"
#include "videoDriver.h"
#include "lib.h"
#include "memoryManager.h"

void *malloc(uint64_t size)
{
	void *ret = allocMemory(CEIL((double)size / PAGE_SIZE));

	if (ret == NULL)
	{
		printString("OUT OF MEMORY MALLOC\n", 255, 0, 0);
	}

	return ret;
}

void free(void *page)
{
	if (page != NULL)
	{
		deallocMemory((char *)page);
	}
}

void *realloc(void *ptr, uint64_t size)
{
	void *newptr = malloc(size);
	memcpy(newptr, ptr, size);
	free(ptr);
	return newptr;
}