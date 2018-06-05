#ifndef PAGEALLOCATOR_H_
#define PAGEALLOCATOR_H_

//*Address for size of ram*/
#define SYSTEM_RAM_ADDRESS 0x5020

/*Paginas de 4k*/
#define PAGE_SIZE 0x1000

/*Stacks de 1MB*/
#define MB 0x100000

/*Size of 4k-pages stack*/
#define PAGE_QTY 255

#define DATA_ADDRESS 0x500000
/*Ending address for 4k pages*/
#define MEMORY_PAGES_END ((PAGE_QTY + 1) * PAGE_SIZE + DATA_ADDRESS)

/*Amount of 1mb processes given*/
#define MAX_PROCESSES 128

void initializePageAllocator();
uint64_t getAvailablePage();
void releasePage(uint64_t page);
uint64_t peekAvailablePage();
int getIndexInStack();
uint64_t getAvailableIndex();
uint64_t getStackPage();
void releaseStackPage(uint64_t stackpage);
uint64_t peekAvailableStackPage();

//-------------------------------------------------------------------------------------

#include <lib.h>
#include "defs.h"
//Constants of the System
#define PAGE_SIZE 0x1000              //4KB
#define MEMORY (PAGE_SIZE * 1024 * 2) //8MB
#define NUMBER_OF_PAGES (MEMORY / PAGE_SIZE)
#define HEAPSIZE ((NUMBER_OF_PAGES * 2) - 1) //this size is the number of elements in the array
#define NUMBER_OF_PAGES_TO_ALLOC(size) (((size) % (PAGE_SIZE)) ? ((size) / (PAGE_SIZE) + 1) : ((size) / (PAGE_SIZE)))
/* Moving and marking the Heap */
#define PARENT(i) ((i) >> 1)
#define LCHILD(i) ((i) << 1)
#define RCHILD(i) (((i) << 1) + 1)
#define AMILEFT(i) !((i) % 2)
#define SIBLING(i) ((i) % 2 ? ((i)-1) : ((i) + 1))
#define BUDDY_FULL 2
#define EMPTY 0
#define PARTIALLY_FULL 1

//Structures for Heap------------------------------
//occupied is 0 if it's empty, -1 if it's full and 1 if it's partially full

typedef char typeOccupied;
typedef void *typeBase;
typedef struct
{
  typeOccupied occupied[HEAPSIZE];
  typeBase base[HEAPSIZE];
  int heapLevel;

} typeBuddyArray;

//---------------------------------------------------

//Prototype Functions--------------------------------
//Heap and BuddyArray Functions
typeBuddyArray createHeap();
int isValid(void *page);
void *getNextPageRecursive(int index, int currentLevel, int level);
void *getBaseMemoryWithIndex(int i, int maxElementsInLevel, int elementNumber);
void freeUpRecursive(int index);
//Allocation Functions
void *allocPage(uint64_t pages);
int deallocPage(char *address);
int getLevel(uint64_t pages);
void printHeap(typeBuddyArray buddyArray);

#endif