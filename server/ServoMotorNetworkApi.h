#pragma once

#include "Configuration.h"
#include "NetworkApi.h"

class ServoMotorNetworkApi: public NetworkApi {

public:
    ServoMotorNetworkApi(unsigned int uiServerPort);

    ~ServoMotorNetworkApi();

    ErrorCode handlerTemplate(NetworkMessage * networkMessage, ClientData * clientData,
        int doneErrorCode, int failErrorCode);

    ErrorCode getLastErrorCode();

    void na_processMessage(NetworkMessage * networkMessage, ClientData * clientData);

private:
    bool bSignalEnd;

    ErrorCode errorCode;
};
