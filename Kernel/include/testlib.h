#ifndef TESTLIB_H_
#define TESTLIB_H_

#include <stdint.h>

void checkIsNotNull(void *value);
void checkIsNull(void *value);
void checkAreEqual(uint64_t value1, uint64_t value2);
void checkAreNotEqual(uint64_t value1, uint64_t value2);
void checkIsNotZero(int value);
void checkIsNotMinusOne(int value);
void checkIsZero(int value);
void checkSizeOfSemaphoreList(int mysize, int size);
void checkIsPositive(int i);
void checkIsGreaterThanZero(int i);
void ok();
void fail();

#endif
