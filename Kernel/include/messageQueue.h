#ifndef BLOCKING_MESSAGE_H
#define BLOCKING_MESSAGE_H

#include "lib.h"
#include "genericQueue.h"
#include "memorymanager.h"
#include "defs.h"
#include "scheduler.h"

#define MESSAGE_OPERATIONS 5
#define MAX_QUEUES 10
#define MAX_SIZE_BUFFER 10

#define READ 1
#define WRITE 2

typedef struct messageCDT *messageADT;

void msgString(char *buffer);
int createMessage(char *name, int messageSize);
int openMessage(char *name, int arg2);
int readMessage(char *buffer, int id);
int writeMessage(char *content, int id);
int closeMessage(char *arg1, int id);
int executeMessage(int operation, char *arg1, int arg2);
void setupMessages();
uint8_t initMsg(int msgId);
uint8_t openQueue(int i, queueADT *array);
void destroyMsg(int msgId);
void initMessages();
void blockMessage(int i, int type);
void unblockMessage(int i, int type);

#endif