#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <stdint.h>
#include "lib.h"

uint64_t malloc(uint64_t size);
void free(void *page);
void* realloc(void* ptr, uint64_t size);

#endif
