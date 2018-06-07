#ifndef PAGEALLOCATORTEST_H_
#define PAGEALLOCATORTEST_H_

void whenAPageIsRequested();
void thenACorrectPageAddressIsReturned();
void whenPageIsReleased();
void whenAStackPageIsRequested();
void thenACorrectStackPageAddressIsReturned();
void whenStackPageIsReleased();
void testAllocateAPageSizeMemory();
void testAllocateStackSizeMemory();
void testReleasePage();
void testReleaseStackPage();
void runPageAllocatorTests();

#endif