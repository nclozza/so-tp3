#ifndef PAGEALLOCATOR_H_
#define PAGEALLOCATOR_H_

#include <lib.h>
#include "defs.h"
#include "dirs.h"

/*Paginas de 4k*/
#define PAGE_SIZE 0x1000

/*Stacks de 1MB*/
#define MB 0x100000

/*Amount of 1mb processes given*/
#define MAX_PROCESSES 128

#define TOTAL_MEMORY (PAGE_SIZE * 1024 * 2) //8MB

#define NUMBER_OF_PAGES (TOTAL_MEMORY / PAGE_SIZE)

#define HEAPSIZE ((NUMBER_OF_PAGES * 2) - 1)

enum buddyStates
{
  EMPTY,
  PARTIALLY_FULL,
  BUDDY_FULL
};

/* This is equals to divide by 2, but doing it with bitwise it's slightly faster */
#define PARENT(i) ((i) >> 1)

/* This is equals to multiply by 2 */
#define LCHILD(i) ((i) << 1)
#define RCHILD(i) (((i) << 1) + 1)
#define SIBLING(i) ((i) % 2 ? ((i)-1) : ((i) + 1))

void initializeMemoryAllocator();

typedef struct
{
  char occupied[HEAPSIZE];
  void *base[HEAPSIZE];
  int heapLevel;

} typeBuddyArray;

void *allocMemory(uint64_t pages);
void *allocMemoryRecursive(int index, int currentLevel, int level);

void deallocMemory(char *page);
void deallocMemoryRecursive(int index);

int getDepthInTheHeap(uint64_t pages);

#endif