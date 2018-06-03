#include "stdLib.h"
#include "shell.h"
#include "mathLib.h"
#include <stdint.h>
#include "sysCallLib.h"

#define MAX_DIGITS 20

extern int sysCall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

void sysPrintString(char *string, int B, int G, int R)
{
  int len = strleng(string);
  int i;
  for (i = 0; i < len; i++)
  {
    sysWriteChar(string[i], B, G, R);
  }
}

void sysWriteChar(char ch, unsigned char color_blue, unsigned char color_green, unsigned char color_red)
{
  sysCall(4, ch, color_blue, color_green, color_red, 0);
}

void sysPrintInt(int num, int B, int G, int R)
{
  int dig = countDigits(num);
  char numbers[MAX_DIGITS] = {};
  int count = 0;
  int remainder;

  if (num < 0)
  {
    sysWriteChar('-', B, G, R);
  }

  while (count != dig)
  {
    remainder = absInt(num % 10);
    numbers[dig - 1 - count++] = remainder + 48;
    num /= 10;
  }

  numbers[dig] = '\0';

  sysPrintString(numbers, B, G, R);
}

void sysPrintFloat(float num, int B, int G, int R)
{
  sysPrintInt((int)num, B, G, R);
  num -= (int)num;
  int aux;
  sysPrintString(".", B, G, R);
  for (int i = 1; i < DECIMAL_PLACES; i++)
  {
    aux = num * tenPow(i);
    aux = aux % 10;
    aux = aux < 0 ? -aux : aux;
    sysPrintInt((int)aux, B, G, R);
  }
}

void sysGetChar(char *ch)
{
  sysCall(2, (uint64_t)ch, 0, 0, 0, 0);
}

void sysGetTime(int *buffer)
{
  buffer[0] = sysCall(1, 0, 0, 0, 0, 0); // Seconds
  buffer[1] = sysCall(1, 2, 0, 0, 0, 0); // Minutes
  buffer[2] = sysCall(1, 4, 0, 0, 0, 0); // Hours
  buffer[3] = sysCall(1, 7, 0, 0, 0, 0); // Day
  buffer[4] = sysCall(1, 8, 0, 0, 0, 0); // Month
  buffer[5] = sysCall(1, 9, 0, 0, 0, 0); // Year
}

void sysClear()
{
  sysCall(3, 0, 0, 0, 0, 0);
}

void sysPaintPixel(int x, int y, char B, char G, char R)
{
  sysCall(5, x, y, B, G, R);
}

uint64_t sysMalloc(uint64_t bytes)
{
  return sysCall(6, bytes, 0, 0, 0, 0);
}

void sysFree(uint64_t address)
{
  sysCall(7, address, 0, 0, 0, 0);
}

void sysMutexUp(uint64_t mut)
{
  sysCall(8, mut, 0, 0, 0, 0);
}

void sysMutexDown(uint64_t mut)
{
  sysCall(9, mut, 0, 0, 0, 0);
}

uint64_t sysMutexInit(char *name)
{
  return sysCall(10, (uint64_t)name, 0, 0, 0, 0);
}

uint64_t sysMutexClose(uint64_t mut)
{
  return sysCall(11, mut, 0, 0, 0, 0);
}

int sysSemOpen(char *name)
{
  return (int)sysCall(12, (uint64_t)name, 0, 0, 0, 0);
}

int sysSemClose(int id)
{
  return (int)sysCall(13, id, 0, 0, 0, 0);
}

int sysSemWait(int id)
{
  return (int)sysCall(14, id, 0, 0, 0, 0);
}

int sysSemPost(int id)
{
  return (int)sysCall(15, id, 0, 0, 0, 0);
}

int sysSemaphoresListSize()
{
  return (int)sysCall(16, 0, 0, 0, 0, 0);
}

void sysFreeSemaphoresList()
{
  sysCall(17, 0, 0, 0, 0, 0);
}

int sysCreateMessage(uint64_t name, uint64_t messageSize)
{
  return (int)sysCall(18, name, messageSize, 0, 0, 0);
}

int sysOpenMessage(uint64_t name, uint64_t arg2)
{
  return (int)sysCall(19, name, arg2, 0, 0, 0);
}

int sysReadMessage(uint64_t buffer, uint64_t id)
{
  return (int)sysCall(20, buffer, id, 0, 0, 0);
}

int sysWriteMessage(uint64_t content, uint64_t id)
{
  return (int)sysCall(21, content, id, 0, 0, 0);
}

int sysCloseMessage(uint64_t arg1, uint64_t id)
{
  return (int)sysCall(22, arg1, id, 0, 0, 0);
}

int sysExec(void *function,int foreground, int argc, char **argv, char *name)
{
  return (uint64_t)sysCall(23, (uint64_t)function, foreground, argc, (uint64_t)argv, (uint64_t)name);
}
void sysSetForeground(int pid)
{
  sysCall(24, (uint64_t)pid, 0, 0, 0, 0);
}
void sysEndThread()
{
  sysCall(25, 0, 0, 0, 0, 0);
}
int sysPpid()
{
  return (int)sysCall(26, 0, 0, 0, 0, 0);
}

void sysPrintPIDS()
{
  sysCall(27, 0, 0, 0, 0, 0);
}

void sysExitShell()
{
  sysCall(28, 0, 0, 0, 0, 0);
}

uint64_t sysGetPage()
{
  return sysCall(29, 0, 0, 0, 0, 0);
}

void sysPrintBlockedThreads()
{
  sysCall(30, 0, 0, 0, 0, 0);
}

int sysDeleteThisThread(int pid)
{
  return sysCall(31, (uint64_t)pid, 0, 0, 0, 0);
}

void sysWhileTrue()
{
  sysCall(32, 0, 0, 0, 0, 0);
}

int sysPipeOpen(char * name)
{
  return sysCall(33,(uint64_t)name, 0, 0, 0, 0);
}

int sysPipeClose(int id)
{
  return sysCall(34,(uint64_t)id, 0, 0, 0, 0);
}

int sysPipeWrite(int id, const void * buf, int bytes)
{
  return sysCall(35,(uint64_t)id, (uint64_t)buf, (uint64_t)bytes, 0, 0);
}

int sysPipeRead(int id, void * buf, int bytes)
{
  return sysCall(36,(uint64_t)id, (uint64_t)buf, (uint64_t)bytes, 0, 0);
}

void sysCreatePipeMutex()
{
  sysCall(37, 0, 0, 0, 0, 0);
}
void sysWait(int pid)
{
  sysCall(50,(uint64_t)pid,0,0,0,0);
}

void sysCreateThread(int foreground, void * function, int argc, char** argv)
{
  sysCall(51,foreground, (uint64_t)function, argc, (uint64_t)argv,0);
}
