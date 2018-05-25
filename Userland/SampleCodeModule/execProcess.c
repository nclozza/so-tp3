#include "stdLib.h"
#include "shell.h"
#include "sysCallLib.h"

typedef void (*entry_point)(int, char **);


int execProcess(void *function, int argc, char **argv, char *name, int foreground)
{
	int pid = sysExec(function, argc, argv, name);
	if (foreground == 1)
	{
		sysSetForeground(pid);
	}	
	return pid;
}
