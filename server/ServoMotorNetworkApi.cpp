#include "ServoMotorNetworkApi.h"

ServoMotorNetworkApi::ServoMotorNetworkApi(unsigned int uiServerPort) :
    NetworkApi(uiServerPort) {
    errorCode = EC_OK;

    if (getLastErrorCode() != EC_OK) {
        return;
    }

    bSignalEnd = false;
    na_listen(&bSignalEnd);
}

ServoMotorNetworkApi::~ServoMotorNetworkApi() {
    bSignalEnd = true;
}

ErrorCode ServoMotorNetworkApi::handlerTemplate(NetworkMessage * networkMessage,
    ClientData * clientData, int doneErrorCode, int failErrorCode) {
    // TODO
    // 1) Do command

    // 2) Report to device
    NetworkMessage response(doneErrorCode, 0, NULL);

    na_sendMessage(&response, clientData);
    return EC_OK;
}

ErrorCode ServoMotorNetworkApi::getLastErrorCode() {
    return errorCode;
}

void ServoMotorNetworkApi::na_processMessage(NetworkMessage * networkMessage,
    ClientData * clientData) {
    ErrorCode ec;
    switch (networkMessage->getMessageID()) {
        case NM_OPEN1_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN1_DONE, NM_OPEN1_FAILD);
            break;
        case NM_OPEN2_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN2_DONE, NM_OPEN2_FAILD);
            break;
        case NM_OPEN3_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN3_DONE, NM_OPEN3_FAILD);
            break;
        case NM_OPEN4_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN4_DONE, NM_OPEN4_FAILD);
            break;
        case NM_OPEN5_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN5_DONE, NM_OPEN5_FAILD);
            break;
        case NM_OPEN6_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN6_DONE, NM_OPEN6_FAILD);
            break;
        case NM_OPEN7_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN7_DONE, NM_OPEN7_FAILD);
            break;
        case NM_OPEN8_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN8_DONE, NM_OPEN8_FAILD);
            break;
        case NM_OPEN9_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN9_DONE, NM_OPEN9_FAILD);
            break;
        case NM_OPEN10_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN10_DONE, NM_OPEN10_FAILD);
            break;
        case NM_OPEN11_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN11_DONE, NM_OPEN11_FAILD);
            break;
        case NM_OPEN12_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN12_DONE, NM_OPEN12_FAILD);
            break;
        case NM_OPEN13_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN13_DONE, NM_OPEN13_FAILD);
            break;
        case NM_OPEN14_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN14_DONE, NM_OPEN14_FAILD);
            break;
        case NM_OPEN15_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN15_DONE, NM_OPEN15_FAILD);
            break;
        case NM_OPEN16_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN16_DONE, NM_OPEN16_FAILD);
            break;
        case NM_OPEN17_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_OPEN17_DONE, NM_OPEN17_FAILD);
            break;
        case NM_CLOSE1_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE1_DONE, NM_CLOSE1_FAILD);
            break;
        case NM_CLOSE2_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE2_DONE, NM_CLOSE2_FAILD);
            break;
        case NM_CLOSE3_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE3_DONE, NM_CLOSE3_FAILD);
            break;
        case NM_CLOSE4_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE4_DONE, NM_CLOSE4_FAILD);
            break;
        case NM_CLOSE5_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE5_DONE, NM_CLOSE5_FAILD);
            break;
        case NM_CLOSE6_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE6_DONE, NM_CLOSE6_FAILD);
            break;
        case NM_CLOSE7_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE7_DONE, NM_CLOSE7_FAILD);
            break;
        case NM_CLOSE8_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE8_DONE, NM_CLOSE8_FAILD);
            break;
        case NM_CLOSE9_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE9_DONE, NM_CLOSE9_FAILD);
            break;
        case NM_CLOSE10_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE10_DONE, NM_CLOSE10_FAILD);
            break;
        case NM_CLOSE11_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE11_DONE, NM_CLOSE11_FAILD);
            break;
        case NM_CLOSE12_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE12_DONE, NM_CLOSE12_FAILD);
            break;
        case NM_CLOSE13_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE13_DONE, NM_CLOSE13_FAILD);
            break;
        case NM_CLOSE14_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE14_DONE, NM_CLOSE14_FAILD);
            break;
        case NM_CLOSE15_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE15_DONE, NM_CLOSE15_FAILD);
            break;
        case NM_CLOSE16_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE16_DONE, NM_CLOSE16_FAILD);
            break;
        case NM_CLOSE17_REQUEST:
            ec = handlerTemplate(networkMessage, clientData, NM_CLOSE17_DONE, NM_CLOSE17_FAILD);
            break;
        default:
            cout << endl << "WARNING: undefine message.";
    }
    if (ec != EC_OK) {
        cout << endl << "Error code: " << ec;
    }
}
