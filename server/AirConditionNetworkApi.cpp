#include "AirConditionNetworkApi.h"

AirConditionNetworkApi::AirConditionNetworkApi(unsigned int uiServerPort):
NetworkApi(uiServerPort)
{
	errorCode = EC_OK;

	if(getLastErrorCode() != EC_OK){
		return;
	}

	bSignalEnd = false;
#ifdef _DEBUG
	rcEmulator = new RcEmulator("RcEmulatord.dll", "airCondition.rcc");
#else
	rcEmulator = new RcEmulator("RcEmulator.dll", "airCondition.rcc");
#endif
	na_listen(&bSignalEnd);
}

AirConditionNetworkApi::~AirConditionNetworkApi(){
	bSignalEnd = true;

	if(rcEmulator){
		delete rcEmulator;
		rcEmulator = NULL;
	}
}

ErrorCode AirConditionNetworkApi::powerOn(NetworkMessage * request, ClientData * clientData){
	
	unsigned int responseID = NM_POWER_ON_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}

ErrorCode AirConditionNetworkApi::powerOff(NetworkMessage * request, ClientData * clientData){
	
	unsigned int responseID = NM_POWER_OFF_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}
	
ErrorCode AirConditionNetworkApi::increaseTemperature(NetworkMessage * request, ClientData * clientData){
	
	unsigned int responseID = NM_INCREASE_TEMPERATURE_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}

ErrorCode AirConditionNetworkApi::decreaseTemperature(NetworkMessage * request, ClientData * clientData){
	
	unsigned int responseID = NM_DECREASE_TEMPERATURE_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}

ErrorCode AirConditionNetworkApi::increaseExhaust(NetworkMessage * request, ClientData * clientData){
	
	unsigned int responseID = NM_INCREASE_EXHAUST_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}

ErrorCode AirConditionNetworkApi::decreaseExhaust(NetworkMessage * request, ClientData * clientData){

	unsigned int responseID = NM_DECREASE_EXHAUST_DONE;

	// TODO
	// 1) Do USB command


	// 2) Report to device
	NetworkMessage response(responseID, 0, NULL);

	na_sendMessage(&response, clientData);
	return EC_OK;
}


ErrorCode AirConditionNetworkApi::getLastErrorCode(){
	return errorCode;
}

void AirConditionNetworkApi::na_processMessage(NetworkMessage * networkMessage, ClientData * clientData){
	switch(networkMessage->getMessageID()){
	case NM_POWER_ON_REQUEST:
		powerOn(networkMessage, clientData);
		break;
	case NM_POWER_OFF_REQUEST:
		powerOff(networkMessage, clientData);
		break;
	case NM_INCREASE_TEMPERATURE_REQUEST:
		increaseTemperature(networkMessage, clientData);
		break;
	case NM_DECREASE_TEMPERATURE_REQUEST:
		decreaseTemperature(networkMessage, clientData);
		break;
	case NM_INCREASE_EXHAUST_REQUEST:
		increaseExhaust(networkMessage, clientData);
		break;
	case NM_DECREASE_EXHAUST_REQUEST:
		decreaseExhaust(networkMessage, clientData);
		break;
	default:
		cout << endl << "WARNING: undefine message.";
	}
}
