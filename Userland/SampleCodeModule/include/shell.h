#ifndef SHELL_H
#define SHELL_H

#define MAX_WORD_LENGTH 124
#define MAX_WORDS 32
#define CR 255
#define CG 255
#define CB 0
#define DR 0
#define DG 255
#define DB 255
#define NULL ((void *)0)
#define helpIns "\necho arguments ...       - Prints arguments\n\
				displayTime                 - Prints date and time to screen\n\
				clear                       - Clears screen\n\
				help (optional)command      - Displays help instructions for command\n\
				exit                        - Exits the shell\n\
				prodcons                    - Offers a solution for the producer-consumer problem using semaphores\n\
				prodconsPipes               - Offers a solution for the producer-consumer problem using pipes\n\
				opcode                      - Tests opcode exception\n\
				ps                          - Prints a list of all processes\n\
				print arguments argument    - Create process 'print' and print the arguments\n\
				whileTrue                   - Create an empty while 1 process\n\
				kill pid                    - Kill the process with the given pid\n"

#define ECHO_INS "Receives a variable amount of arguments, prints them in the screen (max 32).\n"
#define DISPLAY_TIME_INS "Receives no arguments. Prints current date and time.\n\
							Default timezone is UTC-3.\n"
#define CLEAR_INS "Removes information from screen, starts over.\n"
#define EXIT_INS "Quits the shell.\n"
#define OPCODE_INS "Creates opcode exception for testing purposes.\n"
#define PRODCONS_INS "Receives no arguments. Producers put items into a buffer and consumers retrieve them.\n"
#define PRODCONS_PIPES_INS "Receives no arguments. Producers put items into a buffer and consumers retrieve them.\n"
#define PS_INS "Receives no arguments. Lists all processes and their properties.\n"
#define WHILETRUE "Receives no arguments. Create an emprty process with a while 1. Run in background.\n"
#define KILL "Receives 1 argument, correspond to the pid of the process to kill\n"

#define ERROR 2
#define EXITCODE 14

void parseParams(char *command, int *argc, char ***argv);
void startShell();
int callFunction(char *buffer);
int graph(char input[][MAX_WORD_LENGTH], int words);

int overflowGenerator(int n);
int calculateVerifications(int words, char *input2, char *input3);
int checkPipe(int *argc, char ***argv);

extern void opcodeGenerator();

#endif
