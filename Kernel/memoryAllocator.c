#include <stdint.h>
#include <stdio.h>
#include "videoDriver.h"
#include "memoryAllocator.h"

typeBuddyArray buddyArray;
char *baseMemory = (char *)DATA_ADDRESS;

void initializeMemoryAllocator()
{
	int i;
	int currentElementsInLevel;
	int level;
	int maxElementsInLevel;

	for (i = 0, level = 1, currentElementsInLevel = 0, maxElementsInLevel = 1; i < HEAPSIZE; i++, currentElementsInLevel++)
	{
		if (maxElementsInLevel == currentElementsInLevel)
		{
			currentElementsInLevel = 0;
			maxElementsInLevel = maxElementsInLevel * 2;
			level++;
		}
		buddyArray.occupied[i] = EMPTY;
		buddyArray.base[i] = ((TOTAL_MEMORY / maxElementsInLevel) * currentElementsInLevel) + baseMemory;
	}

	buddyArray.heapLevel = level;
}

void *allocMemoryRecursive(int index, int currentLevel, int level)
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

	void *leftChild = allocMemoryRecursive(LCHILD(index), currentLevel + 1, level);

	if (leftChild == NULL)
	{
		void *rightChild = allocMemoryRecursive(RCHILD(index), currentLevel + 1, level);
		if (rightChild == NULL)
		{
			return NULL;
		}
		if (buddyArray.occupied[RCHILD(index) - 1] == BUDDY_FULL)
		{
			buddyArray.occupied[index - 1] = BUDDY_FULL;
		}
		return rightChild;
	}

	buddyArray.occupied[index - 1] = PARTIALLY_FULL;
	return leftChild;
}

void *allocMemory(uint64_t pages)
{
	if (pages == 0 || pages > NUMBER_OF_PAGES)
	{
		return NULL;
	}
	int depth = getDepthInTheHeap(pages);
	if (depth > buddyArray.heapLevel)
	{
		return NULL;
	}

	return allocMemoryRecursive(1, 1, depth);
}

int getDepthInTheHeap(uint64_t pages)
{
	int i = 0;
	while (pages != 1)
	{
		if (pages % 2)
		{
			pages = (pages / 2) + 1;
		}
		else
		{
			pages = (pages / 2);
		}
		i++;
	}

	return buddyArray.heapLevel - (i);
}

void deallocMemory(char *page)
{
	int index = (page - baseMemory) / PAGE_SIZE;
	buddyArray.occupied[index + (HEAPSIZE / 2)] = EMPTY;
	deallocMemoryRecursive((index + (HEAPSIZE / 2)) + 1);
}

void deallocMemoryRecursive(int index)
{
	if (index != 1)
	{
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
		deallocMemoryRecursive(PARENT(index));
	}
}
