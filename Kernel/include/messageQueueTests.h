#ifndef MESSAGEQUEUETESTS_H
#define MESSAGEQUEUETESTS_H

void testMessageQueueIsCreated();
void givenAMessageName();
void givenASize();
void whenNameAndSizeArePassedToCreate();
void thenIdIsReturned();
void testMessageQueueIsOpened();
void whenMessageNameIsPassedToOpen();
void thenExistingIdIsReturned();
void testMessageQueueIsWritten();
void givenContent();
void whenContentAndIdArePassedToWrite();
void thenTestIsASuccess();
void testMessageQueueIsRead();
void givenABuffer();
void whenBufferAndIdArePassedToRead();
void thenCheckIfBufferAndContentAreEqual();
void testMessageQueueIsClosed();
void whenIdIsPassedToClose();
int runMessageQueueTests();

#endif