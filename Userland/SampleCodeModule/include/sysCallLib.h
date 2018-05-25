#ifndef SYSCALLLIB_H_
#define SYSCALLLIB_H_

#include <stdint.h>

void sysPrintString(char *string, int R, int G, int B);
void sysPrintInt(int num, int B, int G, int R);
void sysPrintFloat(float num, int B, int G, int R);
void sysGetTime(int *buffer);
void sysNewLine();
void sysReadInput(char *buffer);
void sysClear();
void sysPaintPixel(int x, int y, char R, char G, char B);
void sysWriteChar(char ch, unsigned char color_blue, unsigned char color_green, unsigned char color_red);
void sysGetChar(char *ch);
uint64_t sysMalloc(uint64_t bytes);
void sysFree(uint64_t address);
int sysExec(void *function, int argc, char **argv, char *name);
void sysEndProcess();
void sysSetForeground(int pid);
int sysPpid();
void sysExitShell();
uint64_t sysGetPage();
void sysPrintBlockedProcesses();
void sysMutexUp(uint64_t mut);
void sysMutexDown(uint64_t mut);
uint64_t sysMutexInit(char *name);
uint64_t sysMutexClose(uint64_t mut);
int sysSemOpen(char *name);
int sysSemClose(int id);
int sysSemWait(int id);
int sysSemPost(int id);
int sysSemaphoresListSize();
void sysFreeSemaphoresList();
int sysCreateMessage(uint64_t name, uint64_t messageSize);
int sysOpenMessage(uint64_t name, uint64_t arg2);
int sysReadMessage(uint64_t buffer, uint64_t id);
int sysWriteMessage(uint64_t content, uint64_t id);
void sysPrintPIDS();
int sysDeleteThisProcess(int pid);
void sysWhileTrue();

#endif
