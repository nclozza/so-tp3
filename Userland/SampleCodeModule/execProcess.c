#include "stdLib.h"
#include "shell.h"
#include "sysCallLib.h"

typedef void (*entry_point)(int, char **);


int execProcess(void *function, int argc, char **argv, char *name, int foreground)
{
	int pid = sysExec(function,foreground, argc, argv, name);

	return pid;
}
