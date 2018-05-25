#include <stdint.h>
#include "shell.h"
#include "stdLib.h"
#include "stdio.h"
#include "plotLib.h"
#include "mathLib.h"
#include "sysCallLib.h"

int calculateVerifications(int words, char *input2, char *input3)
{
	if (isNum(input2) == 2 || isNum(input3) == 2)
	{
		sysPrintString("Wrong parameters for calculate: Numbers must be integer\n", CB, CG, CR);
		return 0;
	}
	if (words != 4 || !isNum(input2) || !isNum(input3))
	{
		//veryfing that there are four inputs and that the last two are numbers
		sysPrintString("Wrong parameters for calculate\n", CB, CG, CR);
		return 0;
	}
	return 1;
}

int runCalculate(char *func, int param1, int param2)
{

	int (*operations[4])(int x, int y);
	operations[0] = add;
	operations[1] = subtract;
	operations[2] = multiply;
	operations[3] = divide;
	char *operationsName[4] = {"add\0", "subtract\n", "multiply\n", "divide\n"};

	for (int i = 0; i < 4; ++i)
	{
		if (strcmp(func, operationsName[i]) == 0)
		{
			sysPrintInt( operations[i](param1, param2),0,155,255);
			return 0;
		}
	}

	return 0;
}