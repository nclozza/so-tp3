#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <stdint.h>
#include "lib.h"

#define CEIL(VARIABLE) ((VARIABLE - (int)VARIABLE) == 0 ? (int)VARIABLE : (int)VARIABLE + 1)

void *malloc(uint64_t size);
void free(void *page);
void *realloc(void *ptr, uint64_t size);

#endif
