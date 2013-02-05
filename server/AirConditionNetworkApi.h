#pragma once

#include "Configuration.h"
#include "NetworkApi.h"
#include "RcEmulator.h"

class AirConditionNetworkApi: public NetworkApi{

public:
	AirConditionNetworkApi(unsigned int uiServerPort);

	~AirConditionNetworkApi();

	ErrorCode powerOn(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode powerOff(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode increaseTemperature(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode decreaseTemperature(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode increaseExhaust(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode decreaseExhaust(NetworkMessage * networkMessage, ClientData * clientData);

	ErrorCode getLastErrorCode();

	void na_processMessage(NetworkMessage * networkMessage, ClientData * clientData);

private:
	bool bSignalEnd;

	ErrorCode errorCode;

	RcEmulator * rcEmulator;
};
