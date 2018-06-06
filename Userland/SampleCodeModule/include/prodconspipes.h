#ifndef PRODCONS_PIPES_H
#define PRODCONS_PIPES_H

#define BUFFER_SIZE 10

#define MAX_PRODUCERS 5
#define MAX_CONSUMERS 5

#define PRODUCER_PIPE "Producer pipe"
#define CONSUMER_PIPE "Consumer pipe"

#define ADD_PRODUCER 'p'
#define ADD_CONSUMER 'c'
#define REMOVE_PRODUCER 'o'
#define REMOVE_CONSUMER 'x'
#define QUIT 'q'

void runProdConsPipes();
void producerPipes();
void consumerPipes();
// putItemIntoBuffer();
//void removeItemFromBuffer();
//void closeSemaphoresAndMutexes();
void removeLastProducerPipes(int *prodc, int *producers);
void removeLastConsumerPipes(int *consc, int *consumers);

#endif