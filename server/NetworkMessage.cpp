#include "NetworkMessage.h"

#include <stdlib.h>

NetworkMessage::NetworkMessage() {
    bConstructorComplete = true;
}

NetworkMessage::NetworkMessage(unsigned int uiID, unsigned int uiSize, unsigned char * pucData) {
    networkApiMessage.messageId = uiID;
    networkApiMessage.uiDataSize = uiSize;
    networkApiMessage.pucData = pucData;

    bConstructorComplete = true;
}

NetworkMessage::NetworkMessage(CircularBuffer * circularBuffer) {
    bConstructorComplete = false;

    unsigned int uiLength;

    unsigned char * pucValues = circularBuffer->get(&uiLength);

    if (uiLength >= 4) {
        networkApiMessage.messageId = *((unsigned int *) &pucValues[0]);
    }

    if (uiLength >= 8) {
        networkApiMessage.uiDataSize = *((unsigned int *) &pucValues[4]);
    }

    if (uiLength >= 8 + networkApiMessage.uiDataSize) {
        networkApiMessage.pucData =
            networkApiMessage.uiDataSize == 0 ?
                NULL : new unsigned char[networkApiMessage.uiDataSize];
        for (unsigned int i = 0; i < networkApiMessage.uiDataSize; i++) {
            networkApiMessage.pucData[i] = pucValues[i + 8];
        }

        circularBuffer->erase(8 + networkApiMessage.uiDataSize);

        bConstructorComplete = true;
    }

}

NetworkMessage::~NetworkMessage() {
    if (networkApiMessage.pucData != NULL) {
        delete networkApiMessage.pucData;
        networkApiMessage.pucData = NULL;
    }
}

bool NetworkMessage::isConstructorComplete() {
    return bConstructorComplete;
}

unsigned int NetworkMessage::getMessageID() {
    return networkApiMessage.messageId;
}

unsigned int NetworkMessage::getMessageSize() {
    return networkApiMessage.uiDataSize;
}

unsigned char * NetworkMessage::getData() {
    return networkApiMessage.pucData;
}
