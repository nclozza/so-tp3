#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#define SUCCESS 0
#define FAIL 1

void *memset(void *destination, int32_t c, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
int strcmpKernel(const char *s1, const char *s2);
int strlenKernel(const char *s);
void strcpyKernel(char *d, const char *s);
void strcatKernel(char *d, const char *s);

#endif
