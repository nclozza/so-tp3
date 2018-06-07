#include "pipes.h"
#include "processes.h"
#include "scheduler.h"
#include "mutex.h"
#include "genericQueue.h"
#include "lib.h"
#include "memoryManager.h"
#include "memoryAllocator.h"
#include "videoDriver.h"
#include "time.h"
#include "defs.h"
#include "thread.h"

static int pipeState = 0;
static int id = 0;

typedef struct thread_t
{
  int tid;
  int pid;
  int status;
  int foreground;
  uint64_t rsp;
  uint64_t stackPage;
  struct thread_t *waiting;
} thread_t;

typedef struct mutex_t
{
  char *name;
  int value;
  int id;
  process *blockedProcesses[MAX_PROCESSES];
} mutex_t;

typedef struct
{
  char buffer[BUF_SIZE];
  int freeSlot;
  int current;
  int bufFull;
} circularBuffer;

typedef struct
{
  char name[PIPE_NAME_LEN];
  circularBuffer cBuffer;
  int state;
  mutex_t *pipeMutex;
  queueADT readQueue;
} pipe;

typedef struct
{
  int bytes;
  int bytesRead;
  void *buffer;
  pipe *f;
  thread_t *readerP;
} readRequest;

typedef struct
{
  char name[PIPE_NAME_LEN];
  int fds;
} pipeInfo;

static pipe openPipes[MAX_PIPES];
static mutex_t *pipeArrayLock;

static int tryRead(readRequest *r);
static pipe createNewPipe(char *name);
static void sendToReaders(queueADT readQueue);
static readRequest *createReadRequest(int pipeId, void *buf, int bytes);
static int isOpen(int id);
static void releaseReaders(queueADT q);

static circularBuffer createCircularBuffer();
static int writeCircularBuffer(circularBuffer *cBuf, const void *source, int bytes);
static int readCircularBuffer(circularBuffer *cBuf, void *dest, int bytes);

static void AddPipeInfo(pipeInfo *addInfo, int id);

void createPipeMutex()
{
  pipeArrayLock = mutexInit("pipesQueueMutex");
}

void closePipeMutex()
{
  mutexClose(pipeArrayLock);
}

int pipeOpen(char *name)
{
  int k;

  mutexLock(pipeArrayLock);

  for (k = 0; k < MAX_PIPES && openPipes[k].state == OPEN; k++)
  {
    if (strcmpKernel(name, openPipes[k].name) == 0)
    {
      setFileOpen(getCurrentThread(), k);
      mutexUnlock(pipeArrayLock);
      return k;
    }
  }

  if (k == MAX_PIPES)
  {
    mutexUnlock(pipeArrayLock);
    return MAX_PIPES_OPEN_ERROR;
  }

  openPipes[k] = createNewPipe(name);

  mutexUnlock(pipeArrayLock);

  setFileOpen(getCurrentThread(), k);

  return k;
}

static pipe createNewPipe(char *name)
{
  pipe f;
  strcpyKernel(f.name, name);
  f.state = OPEN;
  f.cBuffer = createCircularBuffer();
  f.pipeMutex = mutexInit(name);
  f.readQueue = createQueue();
  return f;
}

static circularBuffer createCircularBuffer()
{
  circularBuffer b;

  b.bufFull = 0;
  b.current = 0;
  b.freeSlot = 0;

  return b;
}

static int isOpen(int id)
{
  return id < MAX_PIPES && openPipes[id].state == OPEN;
}

int pipeWrite(int id, const void *buf, int bytes)
{
  int writeBytes;
  pipe *f;

  if (isOpen(id) && bytes > 0)
  {
    f = &openPipes[id];

    mutexLock(f->pipeMutex);

    writeBytes = writeCircularBuffer(&f->cBuffer, buf, bytes);

    sendToReaders(f->readQueue);

    mutexUnlock(f->pipeMutex);

    return writeBytes;
  }
  else if (bytes <= 0)
  {
    return 0;
  }

  return PIPE_NOT_OPEN_ERROR;
}

static int tryRead(readRequest *r)
{
  if (r->f->cBuffer.bufFull > 0)
  {
    r->bytesRead = readCircularBuffer(&r->f->cBuffer, r->buffer, r->bytes);
    return 1;
  }
  else
  {
    return 0;
  }
}

static void sendToReaders(queueADT readQueue)
{
  while (!queueIsEmpty(readQueue) && tryRead(peek(readQueue)))
  {
    readRequest *r = dequeue(readQueue);
    unblockThread(r->readerP);
  }
}

static readRequest *createReadRequest(int pipeId, void *buf, int bytes)
{
  readRequest *r = (readRequest *)malloc(PAGE_SIZE);
  r->bytes = bytes;
  r->buffer = buf;
  r->f = &openPipes[pipeId];
  r->readerP = getCurrentThread();
  return r;
}

int pipeRead(int id, void *buf, int bytes)
{
  int canRead = 0;
  int n;

  if (isOpen(id) && bytes > 0)
  {
    pipe *f = &openPipes[id];

    readRequest *r = createReadRequest(id, buf, bytes);

    mutexLock(f->pipeMutex);

    if (queueIsEmpty(f->readQueue))
      canRead = tryRead(r);

    if (!canRead)
    {
      enqueue(f->readQueue, r);
      mutexUnlock(f->pipeMutex);
      blockThread(r->readerP);
      yieldThread();
    }
    else
    {
      mutexUnlock(f->pipeMutex);
    }

    if (!isOpen(id))
      return PIPE_NOT_OPEN_ERROR;

    n = r->bytesRead;

    free((void *)r);
    return n;
  }
  else
  {
    return 0;
  }

  return PIPE_NOT_OPEN_ERROR;
}

int pipeClose(int id)
{
  mutexLock(pipeArrayLock);

  if (isOpen(id))
  {
    pipe *f = &openPipes[id];
    releaseReaders(f->readQueue);
    destroyQueue(f->readQueue);
    f->state = CLOSED;

    mutexUnlock(pipeArrayLock);
    return 1;
  }

  mutexUnlock(pipeArrayLock);
  return PIPE_NOT_OPEN_ERROR;
}

static void releaseReaders(queueADT q)
{
  readRequest *req;
  while (!queueIsEmpty(q))
  {
    req = dequeue(q);
    unblockThread(req->readerP);
  }
}

static int writeCircularBuffer(circularBuffer *cBuf, const void *source, int bytes)
{
  int writeBytes = (cBuf->bufFull + bytes) > BUF_SIZE ? BUF_SIZE - cBuf->bufFull : bytes;
  int aux = cBuf->freeSlot;

  cBuf->freeSlot = (cBuf->freeSlot + writeBytes) % BUF_SIZE;

  if (aux < cBuf->freeSlot)
  {
    memcpy(cBuf->buffer + aux, source, writeBytes);
  }
  else
  {
    int firstBytes = BUF_SIZE - aux;
    int secondBytes = writeBytes - firstBytes;

    memcpy(cBuf->buffer + aux, source, firstBytes);
    memcpy(cBuf->buffer, source + firstBytes, secondBytes);
  }

  cBuf->bufFull += writeBytes;
  return writeBytes;
}

static int readCircularBuffer(circularBuffer *cBuf, void *dest, int bytes)
{
  int readBytes = cBuf->bufFull < bytes ? cBuf->bufFull : bytes;
  int aux = cBuf->current;

  cBuf->current = (cBuf->current + readBytes) % BUF_SIZE;

  if (aux < cBuf->current)
  {
    memcpy(dest, cBuf->buffer + aux, readBytes);
  }
  else
  {
    int firstBytes = BUF_SIZE - aux;
    int secondBytes = readBytes - firstBytes;

    memcpy(dest, cBuf->buffer + aux, firstBytes);
    memcpy(dest + firstBytes, cBuf->buffer, secondBytes);
  }

  cBuf->bufFull -= readBytes;

  return readBytes;
}

int getPipesInfo(pipeInfo infoQueue[])
{
  int i, j;

  mutexLock(pipeArrayLock);

  for (i = j = 0; i < MAX_PIPES; i++)
  {
    if (openPipes[i].state == OPEN)
      AddPipeInfo(&infoQueue[j++], i);
  }

  mutexUnlock(pipeArrayLock);

  return j;
}

static void AddPipeInfo(pipeInfo *addInfo, int id)
{
  strcpyKernel(addInfo->name, openPipes[id].name);
  addInfo->fds = id + FILE_DESCRIPTORS;
}

void setID(int newID)
{
  id = newID;
}

int getID()
{
  return id;
}

void setPipeState()
{
  pipeState = 1;
}

void clearPipeState()
{
  pipeState = 0;
}

int getPipeState()
{
  return pipeState;
}
