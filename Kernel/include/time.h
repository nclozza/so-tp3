#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include "processes.h"

typedef struct c_sleep_process
{
	process *p;
	uint64_t ticks;
	struct c_sleep_process *next;
} sleep_process;

void timerHandler();
int ticksElapsed();
int secondsElapsed();
extern int getTimeRTC(int field);
int sleep(uint64_t milliseconds);

#endif
