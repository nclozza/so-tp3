// #include "testlib.h"
// #include "pageallocator.h"
// #include "videoDriver.h"

// void whenAPageIsRequested();
// void thenACorrectPageAddressIsReturned();
// void givenARequestedPage();
// void whenPageIsReleased();
// void thenPageIsInsertedInStack();
// void thenIndexInStackDecreases();
// void whenAStackPageIsRequested();
// void thenACorrectStackPageAddressIsReturned();
// void givenARequestedStackPage();
// void whenStackPageIsReleased();
// void thenStackPageIsInsertedInStack();

// uint64_t requestedPage = 0;
// uint64_t requestedPage2 = 0;
// uint64_t requestedStackPage = 0;
// uint64_t requestedStackPage2 = 0;
// int index = 0;

// void testAllocatePageWhenStackEmpty()
// {
// 	whenAPageIsRequested();
// 	thenACorrectPageAddressIsReturned();
// }

// void testReleasePage()
// {
// 	givenARequestedPage();
// 	whenPageIsReleased();
// 	thenPageIsInsertedInStack();
// }

// void testAllocatePageWhenStackNotEmpty()
// {
// 	whenAPageIsRequested();
// 	thenACorrectPageAddressIsReturned();
// }

// void testAllocateStackPage()
// {
// 	whenAStackPageIsRequested();
// 	thenACorrectStackPageAddressIsReturned();
// }
// void testReleaseStackPage()
// {
// 	givenARequestedStackPage();
// 	whenStackPageIsReleased();
// 	thenStackPageIsInsertedInStack();
// }
// void whenAStackPageIsRequested()
// {
// 	requestedStackPage = getStackPage();
// }
// void givenARequestedStackPage()
// {
// 	requestedStackPage2 = getStackPage();
// }
// void whenStackPageIsReleased()
// {
// 	releaseStackPage(requestedStackPage2);
// }
// void thenStackPageIsInsertedInStack()
// {
// 	if (requestedStackPage2 == peekAvailableStackPage())
// 	{
// 		printString("Ok\n", 0, 255, 0);
// 	}
// 	else
// 	{
// 		printString("Stack page not inserted in stack\n", 0, 0, 255);
// 	}
// }
// void whenAPageIsRequested()
// {
// 	index = getIndexInStack();
// 	requestedPage = getAvailablePage();
// }
// void thenACorrectStackPageAddressIsReturned()
// {
// 	checkAreNotEqual(requestedStackPage, 0);
// }
// void thenACorrectPageAddressIsReturned()
// {
// 	checkAreNotEqual(requestedPage, 0);
// }
// void givenARequestedPage()
// {
// 	requestedPage2 = getAvailablePage();
// }
// void whenPageIsReleased()
// {
// 	releasePage(requestedPage2);
// }
// void thenPageIsInsertedInStack()
// {
// 	if (requestedPage2 == peekAvailablePage())
// 	{
// 		printString("Ok\n", 0, 255, 0);
// 	}
// 	else
// 	{
// 		printString("Page not inserted in stack\n", 0, 0, 255);
// 	}
// }
// void thenIndexInStackDecreases()
// {
// 	index--;
// 	checkAreEqual(index, getIndexInStack());
// }

void runPageAllocatorTests()
{
// 	printString("Testing allocating page when stack is empty...\n", 128, 128, 128);
// 	testAllocatePageWhenStackEmpty();
// 	printString("Testing release of page...\n", 128, 128, 128);
// 	testReleasePage();
// 	printString("Testing allocating page when stack is not empty...\n", 128, 128, 128);
// 	testAllocatePageWhenStackNotEmpty();
// 	printString("Testing allocating 1 mega page...\n", 128, 128, 128);
// 	testAllocateStackPage();
// 	printString("Testing releasing 1 mega page...\n", 128, 128, 128);
// 	testReleaseStackPage();
}