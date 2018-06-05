#include <stdint.h>
#include <stdio.h>
#include "videoDriver.h"
#include "pageallocator.h"

static uint64_t size = 1;

static uint64_t pageStack[PAGE_QTY];
static uint64_t availablePage = 0;
static int indexInStack = 0;

static uint64_t megasStack[MAX_PROCESSES];
static uint64_t stackPageIndex = 0;
static uint64_t availableStackPage = 0;

static uint64_t reserved = 0;
static uint64_t reservedStack = 0;

extern uint8_t endOfKernel;

void restorePages();

void initializePageAllocator()
{
	uint64_t ram = *((uint64_t *)SYSTEM_RAM_ADDRESS);
	size = (ram * MB) / PAGE_SIZE;
	reserved = (uint64_t)&endOfKernel / (PAGE_SIZE);
	availablePage = (reserved + 2);
	reservedStack = (availablePage + PAGE_QTY) * PAGE_SIZE;
	availableStackPage = reservedStack;
}

uint64_t getStackPage()
{
	void *stackPage = allocPage(MB / PAGE_SIZE);

	if (stackPage == NULL)
	{
		printString("OUT OF MEMORY\n", 0, 155, 255);
		while (1)
			;
	}

	return stackPage;

	// if (stackPageIndex != 0)
	// {
	// 	uint64_t stackpage = megasStack[stackPageIndex];
	// 	stackPageIndex--;
	// 	return stackpage;
	// }
	// else if (availableStackPage < (MAX_PROCESSES * MB + reservedStack))
	// {
	// 	uint64_t stackpage = availableStackPage;
	// 	availableStackPage += MB;
	// 	return stackpage;
	// }
	// else
	// {
	// 	//out of 1mb pages
	// 	printString("OUT OF MEMORY\n", 0, 155, 255);
	// 	while (1)
	// 		;
	// }
}

void releaseStackPage(uint64_t stackpage)
{
	

	stackPageIndex++;
	if (stackPageIndex < MAX_PROCESSES)
	{
		megasStack[stackPageIndex] = stackpage;
	}
	else
	{
		//restoreStackPages();
	}
}

uint64_t peekAvailableStackPage()
{
	if (stackPageIndex != 0)
	{
		uint64_t stackpage = megasStack[stackPageIndex];
		return stackpage;
	}
	else if (availableStackPage < (MAX_PROCESSES + reservedStack))
	{
		uint64_t stackpage = availableStackPage * MB;
		return stackpage;
	}
	else
	{
		printString("OUT OF MEMORY\n", 0, 155, 255);
		while (1)
			;
	}
}
uint64_t getAvailablePage()
{
	void *page = allocPage(1);

	if (page == NULL)
	{
		printString("OUT OF MEMORY\n", 0, 155, 255);
		while (1)
			;
	}

	return page;

	if (indexInStack != 0)
	{
		uint64_t page = pageStack[indexInStack];
		indexInStack--;
		return page;
	}
	else if (availablePage < (PAGE_QTY + reserved + 1))
	{
		uint64_t page = availablePage * PAGE_SIZE;
		availablePage++;
		return page;
	}
	else
	{
		//out of 4k pages
		printString("OUT OF MEMORY\n", 0, 155, 255);
		while (1)
			;
	}
}
uint64_t peekAvailablePage()
{
	if (indexInStack != 0)
	{
		uint64_t page = pageStack[indexInStack];
		return page;
	}
	else if (availablePage < (PAGE_QTY + reserved + 1))
	{
		uint64_t page = availablePage * PAGE_SIZE;
		return page;
	}
	else
	{
		printString("OUT OF MEMORY\n", 0, 155, 255);
		while (1)
			;
	}
}
void releasePage(uint64_t page)
{
	indexInStack++;
	if (indexInStack < PAGE_QTY)
	{
		pageStack[indexInStack] = page;
	}
	else
	{
		restorePages();
	}
}

void restorePages()
{
	// indexInStack = 0;
	// availablePage = reserved + 1;
}
int getIndexInStack()
{
	return indexInStack;
}
uint64_t getAvailableIndex()
{
	return availablePage;
}

//----------------------------------------------------------------------------------------

typeBuddyArray buddyArray;
char *baseMemory;

int anotherLevel(uint64_t v);

//creates a Heap in baseMemory (char*)0x1000000 for memory book keeping
typeBuddyArray createHeap()
{
	int i;
	int currentElementsInLevel;
	int level;
	int maxElementsInLevel;
	baseMemory = (char *)0x1000000;
	for (i = 0, level = 1, currentElementsInLevel = 0, maxElementsInLevel = 1; i < HEAPSIZE; i++, currentElementsInLevel++)
	{
		if (maxElementsInLevel == currentElementsInLevel)
		{
			currentElementsInLevel = 0;
			maxElementsInLevel = maxElementsInLevel * 2;
			level++;
		}
		buddyArray.occupied[i] = EMPTY;
		buddyArray.base[i] = getBaseMemoryWithIndex(i + 1, maxElementsInLevel, currentElementsInLevel);
	}
	printInt(level, 255, 155, 0);
	printString("\n", 255, 155, 0);
	buddyArray.heapLevel = level;
	return buddyArray;
}

//the first position of the array is 1
void *getBaseMemoryWithIndex(int i, int maxElementsInLevel, int elementNumber)
{
	long memoryPortion = MEMORY / maxElementsInLevel;
	return (memoryPortion * elementNumber) + baseMemory;
}

void *getNextPageRecursive(int index, int currentLevel, int level)
{
	if (buddyArray.occupied[index - 1] == BUDDY_FULL)
	{
		return NULL;
	}

	if (currentLevel == level)
	{
		if (buddyArray.occupied[index - 1] == PARTIALLY_FULL)
		{
			return NULL;
		}

		buddyArray.occupied[index - 1] = BUDDY_FULL;
		return buddyArray.base[index - 1];
	}

	void *lchild = getNextPageRecursive(LCHILD(index), currentLevel + 1, level);

	if (lchild == NULL)
	{
		void *rchild = getNextPageRecursive(RCHILD(index), currentLevel + 1, level);
		if (rchild == NULL)
		{
			return NULL;
		}
		if (buddyArray.occupied[RCHILD(index) - 1] == BUDDY_FULL)
		{
			buddyArray.occupied[index - 1] = BUDDY_FULL;
		}
		return rchild;
	}

	buddyArray.occupied[index - 1] = PARTIALLY_FULL;
	return lchild;
}

void *allocPage(uint64_t pages)
{
	if (pages == 0 || pages > NUMBER_OF_PAGES)
	{
		return NULL;
	}
	int level = getLevel(pages);
	if (level > buddyArray.heapLevel)
	{
		return NULL;
	}

	return getNextPageRecursive(1, 1, level);
}

//calculatesin what level of the tree the amount of pages fit in
int getLevel(uint64_t pages)
{
	int i = 0;
	int rta = pages;
	while (rta != 1)
	{
		if (rta % 2)
		{
			rta = (rta / 2) + 1;
		}
		else
		{
			rta = (rta / 2);
		}
		i++;
	}
	return buddyArray.heapLevel - (i);
}

int anotherLevel(uint64_t v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

int deallocPage(char *page)
{
	int ans;
	if (isValid(page))
	{
		int index = (page - baseMemory) / PAGE_SIZE;
		buddyArray.occupied[index + (HEAPSIZE / 2)] = EMPTY;
		freeUpRecursive((index + (HEAPSIZE / 2)) + 1);
		ans = 0;
	}
	else
	{
		ans = -1;
	}
	return ans;
}

int isValid(void *page)
{
	return 1;
}

void freeUpRecursive(int index)
{

	if (index == 1)
	{

		return;
	}
	if (buddyArray.occupied[PARENT(index) - 1] == BUDDY_FULL && buddyArray.occupied[SIBLING(index) - 1] == EMPTY)
	{
		buddyArray.occupied[PARENT(index) - 1] = EMPTY;
	}
	else if (buddyArray.occupied[PARENT(index) - 1] == BUDDY_FULL && buddyArray.occupied[SIBLING(index) - 1] == BUDDY_FULL)
	{
		buddyArray.occupied[PARENT(index) - 1] = PARTIALLY_FULL;
	}
	else if (buddyArray.occupied[PARENT(index) - 1] == PARTIALLY_FULL && buddyArray.occupied[SIBLING(index) - 1] == EMPTY)
	{
		buddyArray.occupied[PARENT(index) - 1] = EMPTY;
	}
	freeUpRecursive(PARENT(index));
}
