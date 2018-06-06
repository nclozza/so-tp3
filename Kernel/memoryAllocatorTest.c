#include "testlib.h"
#include "memoryAllocator.h"
#include "memoryManager.h"
#include "videoDriver.h"
#include "memoryAllocatorTest.h"

void *requestedPage = 0;
void *requestedStackPage = 0;
int index = 0;

void testAllocateAPageSizeMemory()
{
  whenAPageIsRequested();
  thenACorrectPageAddressIsReturned();
}

void testAllocateStackSizeMemory()
{
  whenAStackPageIsRequested();
  thenACorrectStackPageAddressIsReturned();
}

void testReleasePage()
{
  whenPageIsReleased();
}

void testReleaseStackPage()
{
  whenStackPageIsReleased();
}

void whenAStackPageIsRequested()
{
  requestedStackPage = malloc(MB);
}

void whenAPageIsRequested()
{
  requestedPage = malloc(PAGE_SIZE);
}

void thenACorrectStackPageAddressIsReturned()
{
  checkIsNotNull(requestedStackPage);
}

void thenACorrectPageAddressIsReturned()
{
  checkIsNotNull(requestedPage);
}

void whenPageIsReleased()
{
  free(requestedPage);
}

void whenStackPageIsReleased()
{
  free(requestedStackPage);
}

void runPageAllocatorTests()
{
  printString("Testing allocating page when stack is empty...\n", 128, 128, 128);
  testAllocateAPageSizeMemory();
  printString("Testing release of page...\n", 128, 128, 128);
  testReleasePage();
  printString("Testing allocating 1 mega page...\n", 128, 128, 128);
  testAllocateStackSizeMemory();
  printString("Testing releasing 1 mega page...\n", 128, 128, 128);
  testReleaseStackPage();
}