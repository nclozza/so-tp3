#include "pipes.h"
#include "processes.h"
#include "scheduler.h"
#include "mutex.h"
#include "genericQueue.h"
#include "lib.h"
#include "memorymanager.h"
#include "pageallocator.h"
#include "videoDriver.h"
#include "time.h"
#include "defs.h"

typedef struct mutex_t
{
	char* name;
	int value;
	int id;	
	process* blockedProcesses[MAX_PROCESSES];
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
  mutex_t* pipeMutex;
  queueADT readQueue;
} pipe;

typedef struct 
{
  int bytes;
  int bytesRead;
  void * buffer;
  pipe * f;
  process * readerP;
} readRequest;

typedef struct fifo_info {
	char name[PIPE_NAME_LEN];
	int fds;
} pipeInfo;

static pipe openPipes[MAX_PIPES];
static mutex_t * pipeArrayLock;

static int tryRead(readRequest * r);
static pipe createNewPipe(char * name);
static void sendToReaders(queueADT readQueue);
static readRequest * createReadRequest(int pipeId, void * buf, int bytes);
static int isOpen(int id);
static void releaseReaders(queueADT q);

static circularBuffer createCircularBuffer();
static int writeCircularBuffer(circularBuffer * cBuf, const void * source, int bytes);
static int readCircularBuffer(circularBuffer * cBuf, void * dest, int bytes);

static void AddPipeInfo(pipeInfo * addInfo, int id);

void createPipeMutex() {
  pipeArrayLock = mutexInit("pipesQueueMutex");
}

void closePipeMutex(){
  mutexClose(pipeArrayLock);
}

int pipeOpen(char * name) {
  int k;

  mutexLock(pipeArrayLock);

  for (k = 0; k < MAX_PIPES && openPipes[k].state == OPEN; k++) {
    if (strcmpKernel(name, openPipes[k].name) == 0) {
      setFileOpen(getCurrentProcess(), k);
      mutexUnlock(pipeArrayLock);
      return k;
    }
  }

  if (k == MAX_PIPES) {
    mutexUnlock(pipeArrayLock);
    return MAX_PIPES_OPEN_ERROR;
  }

  openPipes[k] = createNewPipe(name);

  mutexUnlock(pipeArrayLock);

  setFileOpen(getCurrentProcess(), k);

  return k;
}

static pipe createNewPipe(char * name) {
  pipe f;
  strcpyKernel(f.name, name);
  f.state = OPEN;
  f.cBuffer = createCircularBuffer();
  f.pipeMutex = mutexInit(name); // TODO: Ver que nombre pasarle.
  f.readQueue = createQueue();
  return f;
}

static circularBuffer createCircularBuffer() {
  circularBuffer b;

  b.bufFull = 0;
  b.current = 0;
  b.freeSlot = 0;

  return b;
}

static int isOpen(int id) {
  return id < MAX_PIPES && openPipes[id].state == OPEN && \
    fileIsOpen(getCurrentProcess(), id);
}

int pipeWrite(int id, const void * buf, int bytes) {
  int writeBytes;
  pipe * f;


  if (isOpen(id) && bytes > 0) {
    f = &openPipes[id];

    mutexLock(f->pipeMutex);

    writeBytes = writeCircularBuffer(&f->cBuffer, buf, bytes);

    sendToReaders(f->readQueue);

    mutexUnlock(f->pipeMutex);

    return writeBytes;
  }
  else if (bytes <= 0) {
    return 0;
  }

  return PIPE_NOT_OPEN_ERROR;
}

static int tryRead(readRequest * r) {
  if (r->f->cBuffer.bufFull > 0) {  // el buffer tenga los bytes que quiero leer
    r->bytesRead = readCircularBuffer(&r->f->cBuffer, r->buffer, r->bytes);
    return 1; // lee
  }
  else {
    return 0; // no lee
  }
}

static void sendToReaders(queueADT readQueue) {
  while(!queueIsEmpty(readQueue) && tryRead(peek(readQueue))) {
    // manda a los lectores hasta que uno no pueda leer mas
    readRequest * r = dequeue(readQueue);
    unblockProcess(r->readerP);// leyo, entonces lo saca de los que estan esperando leer
  }
}

static readRequest * createReadRequest(int pipeId, void * buf, int bytes) {
  readRequest * r = (readRequest *) getAvailablePage();
  r->bytes = bytes;
  r->buffer = buf;
  r->f = &openPipes[pipeId];
  r->readerP = getCurrentProcess();
  return r;
}

int pipeRead(int id, void * buf, int bytes) {
  int canRead = 0;
  int n;

  if (isOpen(id) && bytes > 0) {
    pipe * f = &openPipes[id];

    readRequest * r = createReadRequest(id, buf, bytes);

    mutexLock(f->pipeMutex);

    if (queueIsEmpty(f->readQueue)) // primer reader
      canRead = tryRead(r);

    if (!canRead) {
      enqueue(f->readQueue, r);
      mutexUnlock(f->pipeMutex);
      blockProcess(r->readerP);
      yieldProcess(); // esta bloqueado
    }
    else {
      mutexUnlock(f->pipeMutex); // TODO: OJO que el tryRead bloquea, puede que nunca se llegue a este unlock
    }

    if (!isOpen(id)) // se desbloqueo porque cerraron el pipe;
      return PIPE_NOT_OPEN_ERROR;

    n = r->bytesRead;

    storePage((uint64_t) r);
    return n;
  }
  else {
    return 0;
  }

  return PIPE_NOT_OPEN_ERROR;
}

/* TODO: hacer algo con los fds */
int pipeClose(int id) {
  mutexLock(pipeArrayLock);

  if (isOpen(id)) {
    pipe * f = &openPipes[id];
    releaseReaders(f->readQueue);
    destroyQueue(f->readQueue);
    f->state = CLOSED;

    mutexUnlock(pipeArrayLock);
    return 1;
  }

  mutexUnlock(pipeArrayLock);
  return PIPE_NOT_OPEN_ERROR;
}

static void releaseReaders(queueADT q) {
  readRequest * req;
  while (!queueIsEmpty(q)) {
    req = dequeue(q);
    unblockProcess(req->readerP);
  }
}

static int writeCircularBuffer(circularBuffer * cBuf, const void * source, int bytes) {
  int writeBytes = (cBuf->bufFull + bytes) > BUF_SIZE ? BUF_SIZE - cBuf->bufFull : bytes;
  int aux = cBuf->freeSlot;

  cBuf->freeSlot = (cBuf->freeSlot + writeBytes) % BUF_SIZE;

  if (aux < cBuf->freeSlot) {
    memcpy(cBuf->buffer+aux, source, writeBytes);
  }
  else {
    int firstBytes = BUF_SIZE - aux;
    int secondBytes = writeBytes - firstBytes;

    memcpy(cBuf->buffer+aux, source, firstBytes); /* Copia hasta el final del buffer */
    memcpy(cBuf->buffer, source+firstBytes, secondBytes); /* Copia a partir del comienzo del buffer lo que faltaba de source */
  }

  cBuf->bufFull += writeBytes;
  return writeBytes;
}

static int readCircularBuffer(circularBuffer * cBuf, void * dest, int bytes) {
  int readBytes = cBuf->bufFull < bytes ? cBuf->bufFull : bytes;
  int aux = cBuf->current;

  cBuf->current = (cBuf->current + readBytes) % BUF_SIZE;

  if (aux < cBuf->current) {
    memcpy(dest, cBuf->buffer + aux, readBytes);
  }
  else {
    int firstBytes = BUF_SIZE - aux;
    int secondBytes = readBytes - firstBytes;

    memcpy(dest, cBuf->buffer + aux, firstBytes);
    memcpy(dest+firstBytes, cBuf->buffer, secondBytes);
  }

  cBuf->bufFull -= readBytes;

  return readBytes;
}

int getPipesInfo(pipeInfo infoQueue[]) {
  int i, j;

  mutexLock(pipeArrayLock);

  for (i = j = 0; i < MAX_PIPES; i++) {
    if (openPipes[i].state == OPEN)
      AddPipeInfo(&infoQueue[j++], i);
  }
  
  mutexUnlock(pipeArrayLock);

  return j;
}

static void AddPipeInfo(pipeInfo * addInfo, int id) {
  strcpyKernel(addInfo->name, openPipes[id].name);
  addInfo->fds = id + FILE_DESCRIPTORS;
}