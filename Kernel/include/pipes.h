#ifndef _PIPES_H_
#define _PIPES_H_

#define MAX_PIPES 128

#define PIPE_NAME_LEN 64
#define BUF_SIZE 1024

#define PIPE_NOT_OPEN_ERROR -1
#define MAX_PIPES_OPEN_ERROR -2

#define OPEN 1
#define CLOSED 0

int pipeOpen(char *name);

int pipeClose(int id);

int pipeWrite(int id, const void *buf, int bytes);

int pipeRead(int id, void *buf, int bytes);

void createPipeMutex();
void closePipeMutex();

void setID(int newID);
int getID();
void setPipeState();
void clearPipeState();
int getPipeState();
int getLength();
void addLength();
void clearLenght();

#endif