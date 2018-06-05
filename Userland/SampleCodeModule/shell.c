#include <stdint.h>
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "semaphoreUserlandTests.h"
#include "commands.h"
#include "execProcess.h"
#include "sysCallLib.h"
#define STEP 10
#define BUFFERSIZE 1024

static int R = DR;
static int G = DG;
static int B = DB;
static int isRunning = 1;
static command commands[] = {
		{"help\0", help},
		{"help\n", help},
		{"echo\0", echo},
		{"echo\n", echo},
		{"displayTime\n", displayTime},
		{"setTimeZone\n", setTimeZone},
		{"setFontColor\n", setFontColor},
		{"clear\n", clear},
		{"calculate\0", calculate},
		{"calculate\n", calculate},
		{"plot\0", plot},
		{"plot\n", plot},
		{"exit\n", exit},
		{"opcode\0", opcode},
		{"opcode\n", opcode},
		{"prodcons\0", prodcons},
		{"prodcons\n", prodcons},
		{"ps\0", ps},
		{"ps\n", ps},
		{"print\n", printName},
		{"print\0", printName},
		{"whileTrue\n", whileTrue},
		{"kill\0", kill}};

//static int timeZone = -3;

void startShell()
{
	sysPrintString("Shell initialized\n", CB, CG, CR);
	char string[MAX_WORD_LENGTH] = {0};
	char lastString[MAX_WORD_LENGTH] = {0};
	int counter = 0;
	char ch;
	//runUserlandSemaphoreTests();
	createMutex();
	createSem();
	sysPrintString("$> ", CB, CG, CR);

	while (isRunning)
	{

		sysGetChar(&ch);
		if (counter < MAX_WORD_LENGTH || ch == '\n' || ch == '\b')
		{

			sysWriteChar(ch, B, G, R);

			string[counter] = ch;
			(ch != 0) ? counter++ : counter;

			if (ch == '\n')
			{
				reset(lastString, strleng(lastString));
				copy(lastString, string, strleng(string) - 1);
				callFunction(string);
				if (isRunning)
					sysPrintString("$> ", CB, CG, CR);
				reset(string, strleng(string));
				counter = 0;
			}

			if (ch == '\b')
			{
				(counter != 0) ? string[counter--] = 0 : counter;
				(counter != 0) ? string[counter--] = 0 : counter;
			}
			if (ch == 15 && counter == 1)
			{ //UPARROW
				int len = strleng(lastString);
				sysPrintString(lastString, B, G, R);
				copy(string, lastString, len);
				counter = len;
			}
			if (ch == 14)
			{
				while (counter)
				{
					sysWriteChar('\b', B, G, R);
					(counter != 0) ? string[counter--] = 0 : counter;
					(counter != 0) ? string[counter] = 0 : counter;
				}
			}
		}
	}
}

int callFunction(char *buffer)
{
	if (buffer == NULL)
	{
		return 1;
	}

	int words;
	char **argv;
	int commandPid;

	int foreground = 1;
	if (*buffer == '&')
	{
		buffer++;
		foreground = 0;
	}

	parseParams(buffer, &words, &argv);
	int i, valid = 0;
	for (i = 0; i < CMD_SIZE && valid == 0; i++)
	{
		if (strcmp(argv[0], commands[i].name) == 0)
		{
			commandPid = execProcess(commands[i].function, words, argv, commands[i].name, foreground);
			valid = 1;
			sysWait(commandPid);
		}
	}

	if (valid == 0)
		sysPrintString("Wrong input\n", CB, CG, CR);

	return 1;
}

void parseParams(char *command, int *argc, char ***argv)
{
	char buffer[BUFFERSIZE];
	int count = 0, size = 0, i = 0, j = 0;
	do
	{
		if (command[i] != ' ' && command[i] != 0)
		{
			buffer[j] = command[i];
			j++;
		}
		else if (j != 0)
		{
			if (size - count == 0)
			{
				size += STEP;
				(*argv) = (char **)sysMalloc(sizeof(void *) * size);
			}
			(*argv)[count] = (char *)sysMalloc(sizeof(char) * (j + 1));
			for (int k = 0; k < j; k++)
			{
				(*argv)[count][k] = buffer[k];
			}
			(*argv)[count][j] = 0; //Null terminated
			count++;
			j = 0;
		}
	} while (command[i++] != 0);

	(*argc) = count;
}
