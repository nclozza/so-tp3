#include "messageQueue.h"
#include "testlib.h"
#include "memoryManager.h"
#include "videoDriver.h"
#include "lib.h"
#include "messageQueueTests.h"

#define SUCCESS 0
#define FAIL 1

char* messageName;
char* messageBuffer;
char* messageContent;
int messageSize;
int messageId, messageId2;
int messageTest;

void testMessageQueueIsCreated()
{
	givenAMessageName();
	givenASize();
	whenNameAndSizeArePassedToCreate();
	thenIdIsReturned();
}

void givenAMessageName()
{
	messageName = "MessageQueueNameForTest";
}

void givenASize()
{
	messageSize = 10;
}

void whenNameAndSizeArePassedToCreate()
{
	messageId = createMessage(messageName, messageSize);
}

void thenIdIsReturned()
{
	checkIsZero(messageId);
}

void testMessageQueueIsOpened()
{
	givenAMessageName();
	whenMessageNameIsPassedToOpen();
	thenExistingIdIsReturned();
}

void whenMessageNameIsPassedToOpen()
{
	messageId2 = openMessage(messageName, 0);
}

void thenExistingIdIsReturned()
{
	checkAreEqual(messageId, messageId2);
}

void testMessageQueueIsWritten()
{
	givenContent();
	whenContentAndIdArePassedToWrite();
	thenTestIsASuccess();
}

void givenContent()
{
	messageContent = "content";
}

void whenContentAndIdArePassedToWrite()
{
	messageTest = writeMessage(messageContent, messageId);
}

void thenTestIsASuccess()
{
	checkIsZero(messageTest);	//zero = success
}

void testMessageQueueIsRead()
{
	givenABuffer();
	whenBufferAndIdArePassedToRead();
	thenCheckIfBufferAndContentAreEqual();
}

void givenABuffer()
{
	messageBuffer = (char*) malloc(messageSize*MAX_SIZE_BUFFER+1);
}

void whenBufferAndIdArePassedToRead()
{
	messageTest = readMessage(messageBuffer, messageId);
}

void thenCheckIfBufferAndContentAreEqual()
{	
	int testAux = -1;
	if(messageTest == 0)
	{	//success
		testAux = strcmpKernel(messageBuffer, messageContent);
	}
	checkIsZero(testAux);
}

void testMessageQueueIsClosed()
{
	whenIdIsPassedToClose();
	thenTestIsASuccess();
}

void whenIdIsPassedToClose()
{
	messageTest = closeMessage("", messageId);
}

int runMessageQueueTests()
{
	printString("Testing if messageQueue is created...\n", 128, 128, 128);
	testMessageQueueIsCreated();

	printString("Testing if messageQueue is opened...\n", 128, 128, 128);
	testMessageQueueIsOpened();

	printString("Testing if messageQueue is written...\n", 128, 128, 128);
	testMessageQueueIsWritten();

	printString("Testing if messageQueue is read...\n", 128, 128, 128);
	testMessageQueueIsRead();

	printString("Testing if messageQueue is closed...\n", 128, 128, 128);
	testMessageQueueIsClosed();

	//finishedTesting();

	return 0;
}

