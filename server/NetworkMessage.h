#pragma once

#include "CircularBuffer.h"

enum{
	NM_NO_ID,
	
	NM_POWER_ON_REQUEST,
    NM_POWER_ON_DONE,
    NM_POWER_ON_FAILD,

    NM_POWER_OFF_REQUEST,
    NM_POWER_OFF_DONE,
    NM_POWER_OFF_FAILD,
        
    NM_INCREASE_TEMPERATURE_REQUEST,
    NM_INCREASE_TEMPERATURE_DONE,
    NM_INCREASE_TEMPERATURE_FAILD,

    NM_DECREASE_TEMPERATURE_REQUEST,
    NM_DECREASE_TEMPERATURE_DONE,
    NM_DECREASE_TEMPERATURE_FAILD,

    NM_INCREASE_EXHAUST_REQUEST,
    NM_INCREASE_EXHAUST_DONE,
    NM_INCREASE_EXHAUST_FAILD,

    NM_DECREASE_EXHAUST_REQUEST,
    NM_DECREASE_EXHAUST_DONE,
    NM_DECREASE_EXHAUST_FAILD,
};

typedef struct networkApiMessage{
	
	unsigned int messageId;

	unsigned int uiDataSize;

	unsigned char * pucData;

	networkApiMessage(){
		uiDataSize = 0;
		pucData = 0;
	}
}NetworkApiMessage;

class NetworkMessage{

public:
	NetworkMessage();

	NetworkMessage(unsigned int uiID,  unsigned int uiSize, unsigned char * pucData);

	NetworkMessage(CircularBuffer * circularBuffer);

	~NetworkMessage();

	bool isConstructorComplete();

	unsigned int getMessageID();

	unsigned int getMessageSize();

	unsigned char * getData();

private:
	NetworkApiMessage networkApiMessage;

	bool bConstructorComplete;

};