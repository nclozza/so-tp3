#include <stdint.h>
#include "commands.h"
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "prodcons.h"
#include "calculate.h"
#include "sysCallLib.h"
#include "prodconspipes.h"

static int R = DR;
static int G = DG;
static int B = DB;
static int id = 0;
static int length = 0;

int timeZone = -3;
uint64_t m;
int s;

void printName(int argc, char *argv[])
{
	if (argc > 1)
	{
		// sysSemWait(s);
		for (int i = 0; i < 100; i++)
		{
			sysMutexDown(m);
			sysPrintString("I'm printing: ", 0, 155, 255);
			sysPrintString(argv[1], 0, 155, 255);
			sysMutexUp(m);
		}
		// sysSemPost(s);
	}
	else
	{
		sysPrintString("Wrong amount of parameters for print command\n\
		Use command help for guidelines\n",
									 CB, CG, CR);
	}
	sysEndThread();
}

void createMutex()
{
	m = sysMutexInit("printMutex");
}
void createSem()
{
	s = sysSemOpen("printSem");
}
void help(int argc, char *argv[])
{

	if (argc > 2)
		sysEndThread();

	if (argc == 2)
	{
		char *input1 = argv[1];

		if (strcmp(input1, "echo\n") == 0)
		{
			sysPrintString(ECHO_INS, B, G, R);
		}
		else if (strcmp(input1, "displayTime\n") == 0)
		{
			sysPrintString(DISPLAY_TIME_INS, B, G, R);
		}
		else if (strcmp(input1, "clear\n") == 0)
		{
			sysPrintString(CLEAR_INS, B, G, R);
		}
		else if (strcmp(input1, "exit\n") == 0)
		{
			sysPrintString(EXIT_INS, B, G, R);
		}
		else if (strcmp(input1, "opcode\n") == 0)
		{
			sysPrintString(OPCODE_INS, B, G, R);
		}
		else if (strcmp(input1, "prodcons\n") == 0)
		{
			sysPrintString(PRODCONS_INS, B, G, R);
		}
		else if (strcmp(input1, "prodconsPipes\n") == 0)
		{
			sysPrintString(PRODCONS_PIPES_INS, B, G, R);
		}
		else if (strcmp(input1, "ps\n") == 0)
		{
			sysPrintString(PS_INS, B, G, R);
		}
		else if (strcmp(input1, "whileTrue\n") == 0)
		{
			sysPrintString(WHILETRUE, B, G, R);
		}
		else if (strcmp(input1, "kill\n") == 0)
		{
			sysPrintString(KILL, B, G, R);
		}
		else
		{
			sysPrintString("Not a valid command\n", CB, CG, CR);
		}
	}
	else
	{
		sysPrintString(helpIns, B, G, R);
	}

	sysEndThread();
}
void echo(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		sysPrintString(argv[i], B, G, R);
		sysPrintString(" ", B, G, R);
	}

	sysPrintString("\n", B, G, R);

	sysEndThread();
}

void echoPIPE(int argc, char *argv[])
{
	sysCreatePipeMutex();
	id = sysPipeOpen("echoPIPE");
	length = strleng(argv[1]);
	sysPipeWrite(id, argv[1], length);

	sysEndThread();
}

void printPIPE(int argc, char *argv[])
{
	sysPrintString("printPIPE\n", 0, 155, 255);
	char **buffer = (char **)sysMalloc(1);
	buffer[1] = (char *)sysMalloc(1);
	sysPipeRead(id, buffer[1], length);
	buffer[1][length] = '\n';

	printName(2, buffer);

	sysFree((uint64_t)buffer[1]);
	sysFree((uint64_t)buffer);

	sysPipeClose(id);
	sysClosePipeMutex();
	sysEndThread();
}

void clear(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for clear\n", CB, CG, CR);

		sysEndThread();
	}

	sysClear();
	sysEndThread();
}

void opcode(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for opcode\n", CB, CG, CR);
		sysEndThread();
	}
	opcodeGenerator();
	sysEndThread();
}

void prodcons(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for prodcons\n", CB, CG, CR);
		sysEndThread();
	}

	runProdCons();
	sysEndThread();
}

void prodconsPipes(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for prodconsPipes\n", CB, CG, CR);
		sysEndThread();
	}

	runProdConsPipes();
	sysEndThread();
}

void ps(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for ps\n", CB, CG, CR);
		sysEndThread();
	}
	sysPrintPIDS();
	sysEndThread();
}

void displayTime(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("Wrong parameters: displayTime\n", CB, CG, CR);
		sysEndThread();
	}
	int timeBuff[6];

	sysGetTime(timeBuff);
	sysPrintInt((timeBuff[2] + timeZone) % 24, B, G, R);
	sysPrintString(":", B, G, R);
	if (timeBuff[1] / 10 == 0)
		sysPrintString("0", B, G, R);
	sysPrintInt(timeBuff[1], B, G, R);
	sysPrintString(":", B, G, R);
	if (timeBuff[0] / 10 == 0)
		sysPrintString("0", B, G, R);
	sysPrintInt(timeBuff[0], B, G, R);

	sysPrintString(" - ", B, G, R);
	if ((timeBuff[2] + timeZone) / 24 != 0)
		sysPrintInt(timeBuff[3] + 1, B, G, R);
	else
		sysPrintInt(timeBuff[3], B, G, R);

	sysPrintString("/", B, G, R);
	sysPrintInt(timeBuff[4], B, G, R);
	sysPrintString("/", B, G, R);
	sysPrintInt(timeBuff[5], B, G, R);
	sysPrintString("\n", B, G, R);

	sysEndThread();
}

void exit(int argc, char *argv[])
{
	sysExitShell();
}

void whileTrue()
{
	sysWhileTrue();
	sysEndThread();
}

void kill(int argc, char *argv[])
{
	if (argc != 2)
	{
		sysPrintString("Wrong parameters for kill\n", CB, CG, CR);
		sysEndThread();
	}

	int pid = toInt(argv[1]);
	sysDeleteThisThread(pid);
	sysEndThread();
}
