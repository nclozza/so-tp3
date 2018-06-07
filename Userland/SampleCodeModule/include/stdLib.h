#ifndef STDLIB_H_
#define STDLIB_H_

#include <stdint.h>

#define IS_DIGIT(c) ((c - '0') >= 0 && (c - '0') <= 9)
#define DECIMAL_PLACES 4
void *memcpy(void *destination, const void *source, uint64_t length);
void reset(char *string, int size);
int strleng(const char *s);
int countDigits(int num);
void copy(char *copy, char *original, int len);
int subStrleng(const char *s, const char c);

void checkIsNotNull(void* value);
void checkIsNull(void* value);
void checkAreEqual(uint64_t value1, uint64_t value2);
void checkAreNotEqual(uint64_t value1, uint64_t value2);
void checkIsNotZero(int value);
void checkIsNotMinusOne(int value);
void checkSizeOfSemaphoreList(int mysize, int size);
void ok();
void fail();
int rand();
void strcat(char *d, const char *s);
int closeMessage(uint64_t arg1, uint64_t id);

#endif
