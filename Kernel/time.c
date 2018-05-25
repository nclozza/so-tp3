#include "time.h"
#include "scheduler.h"
#include "defs.h"
#include "memorymanager.h"
#include "videoDriver.h"
#include "interrupts.h"

static unsigned long ticks = 0;

/* Controla la cantidad de ticks del timer tick */
void timerHandler()
{
	ticks++;
}

int ticksElapsed()
{
	return ticks;
}

int secondsElapsed()
{
	return ticks / 18;
}
