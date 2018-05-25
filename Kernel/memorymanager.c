#include <stdint.h>
#include "pageallocator.h"
#include "videoDriver.h"
#include "lib.h"

void *malloc(uint64_t size)
{
	if (size <= PAGE_SIZE)
	{
		return (void *)getAvailablePage();
	}
	else
	{
		return (void *)getStackPage();
	}
}

void free(void *page)
{
	if ((uint64_t)page < MEMORY_PAGES_END)
	{
		releasePage((uint64_t)page);
	}
	else if ((uint64_t)page > MEMORY_PAGES_END)
	{
		releaseStackPage((uint64_t)page);
	}
	else
	{
		printString("Error in freeing memory\n", 0, 0, 255);
	}
}

void *realloc(void *ptr, uint64_t size)
{
	void *newptr = malloc(size);
	memcpy(newptr, ptr, size);
	free(ptr);
	return newptr;
}