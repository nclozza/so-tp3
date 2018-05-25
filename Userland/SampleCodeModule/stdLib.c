#include "stdLib.h"
#include "shell.h"
#include "mathLib.h"
#include <stdint.h>
#include "sysCallLib.h"
#define MAX_DIGITS 20

extern int sysCall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
static unsigned long int next = 1;

void *memcpy(void *destination, const void *source, uint64_t length)
{
  /*
  * memcpy does not support overlapping buffers, so always do it
  * forwards. (Don't change this without adjusting memmove.)
  *
  * For speedy copying, optimize the common case where both pointers
  * and the length are word-aligned, and copy word-at-a-time instead
  * of byte-at-a-time. Otherwise, copy by bytes.
  *
  * The alignment logic below should be portable. We rely on
  * the compiler to be reasonably intelligent about optimizing
  * the divides and modulos out. Fortunately, it is.
  */
  uint64_t i;

  if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
      (uint64_t)source % sizeof(uint32_t) == 0 &&
      length % sizeof(uint32_t) == 0)
  {
    uint32_t *d = (uint32_t *)destination;
    const uint32_t *s = (const uint32_t *)source;

    for (i = 0; i < length / sizeof(uint32_t); i++)
      d[i] = s[i];
  }
  else
  {
    uint8_t *d = (uint8_t *)destination;
    const uint8_t *s = (const uint8_t *)source;

    for (i = 0; i < length; i++)
      d[i] = s[i];
  }

  return destination;
}

void reset(char *string, int size)
{
  for (int i = 0; i < size; i++)
  {
    *(string + i) = 0;
  }
}


void copy(char *copy, char *original, int len)
{
  for (int i = 0; i < len; i++)
  {
    *(copy + i) = *(original + i);
  }
}

int subStrleng(const char *s, const char c)
{
  int i = 0;

  while (*(s + i) != c && *(s + i) != 0)
  {
    i++;
  }

  return i;
}

int strleng(const char *s)
{
  return subStrleng(s, '\0');
}


int countDigits(int num)
{
  int dig = 1;
  while ((num /= 10) != 0)
    dig++;
  return dig;
}

void checkIsNotNull(void *value)
{
  if (value == NULL)
  {
    fail();
  }
  else
  {
    ok();
  }
}
void checkIsNull(void *value)
{
  if (value != NULL)
  {
    fail();
  }
  else
  {
    ok();
  }
}

void checkAreNotEqual(uint64_t value1, uint64_t value2)
{
  if (value1 == value2)
  {
    fail();
  }
  else
  {
    ok();
  }
}
void checkAreEqual(uint64_t value1, uint64_t value2)
{
  if (value1 != value2)
  {
    fail();
  }
  else
  {
    ok();
  }
}

void checkIsNotZero(int value)
{
  if (value == 0)
  {
    fail();
  }
  else
  {
    ok();
  }
}

void checkIsNotMinusOne(int value)
{
  if (value == -1)
  {
    fail();
  }
  else
  {
    ok();
  }
}

void checkSizeOfSemaphoreList(int mysize, int size)
{
  if (size != mysize)
  {
    fail();
  }
  else
  {
    ok();
  }
}

void ok()
{
  sysPrintString("Ok\n", 0, 255, 0);
}

void fail()
{
  sysPrintString("Fail\n", 0, 0, 255);
}

int rand()
{
  next = next * 1103515245 + 12345;
  return (unsigned int)(next / 65536) % 32768;
}