#include "CircularBuffer.h"

#include <iostream>
#include <stdio.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

CircularBuffer::CircularBuffer(unsigned int _uiSize) :
    uiLength(_uiSize) {
    pucBuffer = new unsigned char[uiLength];

    uiHead = uiTail = 0;

    bBufferFull = false;
}

CircularBuffer::~CircularBuffer() {
    if (pucBuffer != NULL) {
        delete pucBuffer;
        pucBuffer = NULL;
    }
}

void CircularBuffer::write(unsigned char * pucValues, unsigned int uiSize) {
    pthread_mutex_lock(&mutex);

    cout << endl << "CB write: head = " << uiHead << ", tail = " << uiTail << ", size = " << uiSize;
    cout << endl << "[ ";

    for (unsigned int i = 0; i < uiSize; i++) {
        pucBuffer[uiTail++] = pucValues[i];
        printf("0x%02x ", pucValues[i]);
        uiTail = uiTail % uiLength;

        if (uiTail > uiHead)
            bBufferFull = (uiTail - uiHead) == uiLength;
        else
            bBufferFull = (uiTail + uiSize - uiHead) == uiLength;

        if (bBufferFull == true)
            break;
    }

    cout << "]";

    pthread_mutex_unlock(&mutex);
}

unsigned char * CircularBuffer::read() {

    pthread_mutex_lock(&mutex);

    unsigned char * pucRet;

    unsigned int uiSize = uiTail > uiHead ? uiTail - uiHead : uiTail + uiLength - uiHead;
    cout << endl << "CB read: head = " << uiHead << ", tail = " << uiTail << ", size = " << uiSize;
    cout << endl << "[ ";

    pucRet = new unsigned char[uiSize];

    for (unsigned int i = 0; i < uiSize; i++) {
        pucRet[i] = pucBuffer[uiHead++];
        printf("0x%02x ", pucRet[i]);
        uiHead = uiHead % uiLength;
    }

    cout << "]";

    pthread_mutex_unlock(&mutex);

    return pucRet;
}

/**
 * @brief Just read values from circular buffer. Don't change head or tail values.
 */
unsigned char * CircularBuffer::get(unsigned int * puiLength) {
    pthread_mutex_lock(&mutex);

    unsigned char * pucRet;

    unsigned int uieadCopy = uiHead;

    if (uiTail == uiHead) {
        *puiLength = 0;
    } else if (uiTail > uiHead) {
        *puiLength = uiTail - uiHead;
    } else {
        *puiLength = uiTail + uiLength - uiHead;
    }
    cout << endl << "CB get: head = " << uiHead << ", tail = " << uiTail << ", size = "
        << *puiLength;
    pucRet = *puiLength == 0 ? NULL : new unsigned char[*puiLength];
    cout << endl << "[ ";
    for (unsigned int i = 0; i < *puiLength; i++) {
        pucRet[i] = pucBuffer[uieadCopy++];
        printf("0x%02x ", pucRet[i]);
        uieadCopy = uieadCopy % uiLength;
    }
    cout << "]";
    pthread_mutex_unlock(&mutex);

    return pucRet;
}

void CircularBuffer::erase(unsigned int uiHowMuch) {
    pthread_mutex_lock(&mutex);

    cout << endl << "CB erase: head = " << uiHead << ", tail = " << uiTail << ", how much = "
        << uiHowMuch;

    uiHead += uiHowMuch;
    uiHead = uiHead % uiLength;

    pthread_mutex_unlock(&mutex);
}

bool CircularBuffer::isFull() {
    return bBufferFull;
}
