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

static int R = DR;
static int G = DG;
static int B = DB;

int timeZone = -3;
uint64_t m;
int s;

void printName(int argc, char *argv[])
{
	if (argc > 1)
	{
		sysSemWait(s);
		for (int i = 0; i < 100; i++)
		{
			// sysMutexDown(m);
			sysPrintString("I'm process: ", 0, 155, 255);
			sysPrintString(argv[1], 0, 155, 255);
			// sysMutexUp(m);
		}
		sysSemPost(s);
	}
	else
	{
		sysPrintString("Wrong amount of parameters for print command\n\
		Use command help for guidelines\n",
									 CB, CG, CR);
	}
	sysEndProcess();
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
		sysEndProcess();

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
		else if (strcmp(input1, "setTimeZone\n") == 0)
		{
			sysPrintString(SET_TIME_ZONE_INS, B, G, R);
		}
		else if (strcmp(input1, "setFontColor\n") == 0)
		{
			sysPrintString(SET_FONT_COLOR_INS, B, G, R);
		}
		else if (strcmp(input1, "clear\n") == 0)
		{
			sysPrintString(CLEAR_INS, B, G, R);
		}
		else if (strcmp(input1, "calculate\n") == 0)
		{
			sysPrintString(CALCULATE_INS, B, G, R);
		}
		else if (strcmp(input1, "exit\n") == 0)
		{
			sysPrintString(EXIT_INS, B, G, R);
		}
		else if (strcmp(input1, "plot\n") == 0)
		{
			sysPrintString(PLOT_INS, B, G, R);
		}
		else if (strcmp(input1, "opcode\n") == 0)
		{
			sysPrintString(OPCODE_INS, B, G, R);
		}
		else if (strcmp(input1, "prodcons\n") == 0)
		{
			sysPrintString(PRODCONS_INS, B, G, R);
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

	sysEndProcess();
}
void echo(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		sysPrintString(argv[i], B, G, R);
		sysPrintString(" ", B, G, R);
	}

	sysPrintString("\n", B, G, R);

	sysEndProcess();
}

void clear(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for clear\n", CB, CG, CR);

		sysEndProcess();
	}

	sysClear();
	sysEndProcess();
}

void calculate(int argc, char *argv[])
{
	/*calculate operation n1 n2*/
	if (argc != 4)
		sysEndProcess();
	char *input01 = argv[1];
	char *input02 = argv[2];
	char *input03 = argv[3];

	int ver = calculateVerifications(argc, input02, input03);

	if (ver)
	{
		int input2 = toInt(input02);
		int input3 = toInt(input03);

		int ans = runCalculate(input01, input2, input3);

		sysPrintString("Calculated: ", 255, 255, 0);
		sysPrintInt(ans, 255, 255, 0);
		sysPrintString("\n", 255, 255, 0);
	}
	sysEndProcess();
}

void opcode(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for opcode\n", CB, CG, CR);
		sysEndProcess();
	}
	opcodeGenerator();
	sysEndProcess();
}

void prodcons(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for prodcons\n", CB, CG, CR);
		sysEndProcess();
	}

	runProdCons();
	sysEndProcess();
}

void ps(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("No extra parameters for ps\n", CB, CG, CR);
		sysEndProcess();
	}
	sysPrintPIDS();
	sysEndProcess();
}

void plot(int argc, char *argv[])
{

	if (argc != (GRAPH_PARAMETERS + 1))
	{
		sysPrintString("Wrong amount of parameters for plot command\n\
		Use command help for guidelines\n",
									 CB, CG, CR);

		sysEndProcess();
	}

	for (int i = 1; i <= GRAPH_PARAMETERS; i++)
	{
		if (!isNum(argv[i]))
		{
			sysPrintString("Wrong parameters passed to plot command\n\
			Use command help for guidelines\n",
										 CB, CG, CR);

			sysEndProcess();
		}
	}

	graphMain(toFloat(argv[1]), toFloat(argv[2]), toFloat(argv[3]));

	sysEndProcess();
}

void displayTime(int argc, char *argv[])
{
	if (argc != 1)
	{
		sysPrintString("Wrong parameters: displayTime\n", CB, CG, CR);
		sysEndProcess();
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

	sysEndProcess();
}

void setTimeZone(int argc, char *argv[])
{
	int input1 = toInt(argv[1]);
	if (argc != 2)
	{
		sysPrintString("Wrong parameters: setTimeZone timezone\n", CB, CG, CR);
		sysEndProcess();
	}
	else
	{
		if (input1 > 12 || input1 < -11)
		{
			sysPrintString("Timezone values must be between -11 and +12\n", CB, CG, CR);
			sysEndProcess();
		}
		timeZone = input1;
		sysEndProcess();
	}
}

void exit(int argc, char *argv[])
{
	sysExitShell();
}

void setFontColor(int argc, char *argv[])
{
	if (argc != 2)
	{
		sysPrintString("Wrong parameters for setFontColor\n", CB, CG, CR);
		sysEndProcess();
	}
	if (strcmp(argv[2], "red") == 0)
	{
		sysEndProcess();
	}
	else if (strcmp(argv[2], "green") == 0)
	{
		sysEndProcess();
	}
	else if (strcmp(argv[2], "blue") == 0)
	{
		sysEndProcess();
	}
	else if (strcmp(argv[2], "default") == 0)
	{
		sysEndProcess();
	}
	else
	{
		sysPrintString("Wrong parameters for setFontColor\n", CB, CG, CR);
		sysEndProcess();
	}

	sysPrintString("Set font color\n", B, G, R);
}

void whileTrue()
{
	sysWhileTrue();
	sysEndProcess();
}

void kill(int argc, char *argv[])
{
	if (argc != 2)
	{
		sysPrintString("Wrong parameters for kill\n", CB, CG, CR);
		sysEndProcess();
	}

	int pid = toInt(argv[1]);
	sysDeleteThisProcess(pid);
	sysEndProcess();
}
