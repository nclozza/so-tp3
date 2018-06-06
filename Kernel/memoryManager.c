#include <stdint.h>
#include "memoryAllocator.h"
#include "videoDriver.h"
#include "lib.h"
#include "memoryManager.h"

void *malloc(uint64_t size)
{
	return allocMemory(CEIL((double)size / PAGE_SIZE));
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