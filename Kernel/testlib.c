#include <stdlib.h>
#include <stdio.h>
#include "testlib.h"
#include <stdint.h>
#include "videoDriver.h"

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

void checkIsZero(int value)
{
	if (value == 0)
	{
		ok();
	}
	else
	{
		fail();
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
	if(value == -1)
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
	printString("Ok\n", 0, 255, 0);
}

void fail(char *withError)
{
	printString("Fail\n", 0, 0, 255);
}