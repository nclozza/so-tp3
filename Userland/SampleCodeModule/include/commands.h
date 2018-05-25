#ifndef COMMANDS_H
#define COMMANDS_H

#define CMD_SIZE 23
void printName(int argc, char *argv[]);
void createMutex();
void createSem();
void help(int argc, char *argv[]);
void echo(int argc, char *argv[]);
void displayTime(int argc, char *argv[]);
void setTimeZone(int argc, char *argv[]);
void setFontColor(int argc, char *argv[]);
void clear(int argc, char *argv[]);
void calculate(int argc, char *argv[]);
void plot(int argc, char *argv[]);
void exit(int argc, char *argv[]);
void opcode(int argc, char *argv[]);
void prodcons(int argc, char *argv[]);
void ps(int argc, char *argv[]);
void whileTrue();
void kill(int argc, char *argv[]);

typedef struct
{
	char *name;
	void (*function)(int argc, char *argv[]);
} command;

#endif
