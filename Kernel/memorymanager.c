#include <stdint.h>
#include "pageallocator.h"
#include "videoDriver.h"
#include "lib.h"

void *malloc(uint64_t size)
{
	if (size <= PAGE_SIZE)
	{
		return allocPage(1);
	}
	else
	{
		return allocPage(MB / PAGE_SIZE);
	}
}

void free(void *page)
{
	if (page != NULL)
	{
		deallocPage((char *)page);
	}
}

void *realloc(void *ptr, uint64_t size)
{
	void *newptr = malloc(size);
	memcpy(newptr, ptr, size);
	free(ptr);
	return newptr;
}