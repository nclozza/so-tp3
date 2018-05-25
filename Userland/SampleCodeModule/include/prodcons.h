#ifndef PRODCONS_H
#define PRODCONS_H

#define BUFFER_SIZE 10

#define MAX_PRODUCERS 5
#define MAX_CONSUMERS 5

#define ADD_PRODUCER 'p'
#define ADD_CONSUMER 'c'
#define REMOVE_PRODUCER 'o'
#define REMOVE_CONSUMER 'x'
#define QUIT 'q'

void runProdCons();
void producer();
void consumer();
void putItemIntoBuffer();
void removeItemFromBuffer();
void closeSemaphoresAndMutexes();
void removeLastProducer(int *prodc, int *producers);
void removeLastConsumer(int *consc, int *consumers);

#endif