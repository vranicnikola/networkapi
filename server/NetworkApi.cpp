#include "NetworkApi.h"

#include <errno.h>
#include <string.h>
#include <map>

void * receiveMessageThread(void * param);

void * senderMessageThread(void * param);

list<ClientData *> clients;

map<unsigned int, list<NetworkMessage *> > receivePipeline;

CircularBuffer * circularBuffer;

typedef struct {
    ClientData * clientData;
    NetworkApi * networkApi;
} ReceiveThreadParam;

NetworkApi::NetworkApi(unsigned int _uiServerPort) :
    uiServerPort(_uiServerPort) {
    uiClientID = 0;

    circularBuffer = new CircularBuffer(NA_MAX_RECEIVE_CIRCULAR_BUFFER_SIZE);

    // 1) open server socket
    ecLastError = openServerSocket(uiServerPort);
}

NetworkApi::~NetworkApi() {

    // 1) close socket
    close(serverSocket);
    list<ClientData *>::iterator i;

    // 2) clean clients list
    for (i = clients.begin(); i != clients.end(); i++) {
        delete *i;
    }
    clients.clear();

    // 3) purge recived messages

    // 4) others
    if (circularBuffer != NULL) {
        delete circularBuffer;
        circularBuffer = NULL;
    }
}

ErrorCode NetworkApi::getLastErrorCode() {
    return ecLastError;
}

ErrorCode NetworkApi::openServerSocket(unsigned int uiServerPort) {
    memset(&sraddr, '0', sizeof(sraddr));

    sraddr.sin_family = AF_INET;
    sraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sraddr.sin_port = htons(uiServerPort);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket != NO_SOCKET_ERROR) {
        cout << endl << "ERROR: Problem pri stvaranju uticnice! Kod greske: " << strerror(errno);
        return EC_ERROR;
    }

    if (bind(serverSocket, (struct sockaddr*) &sraddr, sizeof(sraddr)) != NO_SOCKET_ERROR) {
        cout << endl << "ERROR: Bind problem! Kod greske: " << strerror(errno);
        return EC_ERROR;
    }

    if (listen(serverSocket, NA_MAX_RECEIVE_BUFFER_SIZE) != NO_SOCKET_ERROR) {
        cout << endl << "ERROR: Listen problem! Kod greske: " << strerror(errno);
        return EC_ERROR;
    }

    return EC_OK;
}

ErrorCode NetworkApi::na_listen(bool * bEnd) {
    unsigned int uiLength;

    while (*bEnd == false) {

        memset(&claddr, 0x00, sizeof(struct sockaddr_in));
        uiLength = sizeof(struct sockaddr);

        int client = accept(serverSocket, (struct sockaddr*) &claddr, &uiLength);
        if (client != NO_SOCKET_ERROR) {
            close(serverSocket);
            return EC_ERROR;
        }

        ClientData * clientData = new ClientData();

        clientData->clientSocket = client;
        sprintf(clientData->pcAddress, "%s", inet_ntoa(claddr.sin_addr));
        clientData->iPort = ntohs(((struct sockaddr_in*) &claddr)->sin_port);

        clientData->uiClientID = getClientID();

        clients.push_back(clientData);

        ReceiveThreadParam * receiveThreadParam = new ReceiveThreadParam();
        receiveThreadParam->clientData = clientData;
        receiveThreadParam->networkApi = this;

        cout << endl << "INFO: client connected: " << clientData->pcAddress;

        pthread_t id;
        pthread_create(&id, NULL, &receiveMessageThread, (void*) receiveThreadParam);
    }

    return EC_OK;
}

//! @param msg message which will be send to client
ErrorCode NetworkApi::na_sendMessage(NetworkMessage * msg, unsigned int uiClientID) {
    ClientData * clientData = getClient(uiClientID);

    int iLen = sizeof(msg->getMessageID()) + sizeof(msg->getMessageSize())
        + msg->getMessageSize();

    cout << endl << "Response: len: " << iLen;
    cout << endl << "[ ";
    for (int i = 0; i < iLen; i++) {
        printf("0x%02x ", ((unsigned char *) msg)[i]);
    }
    cout << "]";
    if (send(clientData->clientSocket, (char *) msg, iLen, 0) == iLen) {
        return EC_OK;
    } else {
        return EC_ERROR;
    }
}

//! @param msg message which will be send to client
ErrorCode NetworkApi::na_sendMessage(NetworkMessage * msg, ClientData * clientData) {
    return na_sendMessage(msg, clientData->uiClientID);
}

//! @param msg pointer to message which will be received from client
ErrorCode NetworkApi::na_reciveMessage(NetworkMessage * msg, unsigned int uiClientID) {
    msg = NULL;

    map<unsigned int, list<NetworkMessage *> >::iterator pointer = receivePipeline.find(uiClientID);

    if (pointer != receivePipeline.end()) {
        list<NetworkMessage *> messages = receivePipeline[uiClientID];

        msg = messages.empty() ? NULL : messages.front();
    }

    return EC_OK;
}

void * receiveMessageThread(void * param) {

    // 1) vars
    ReceiveThreadParam * receiveThreadParam;

    ClientData * clientData;

    NetworkApi * networkApi;

    int clientSocket;

    unsigned char pucRcvBuffer[NA_MAX_RECEIVE_BUFFER_SIZE];

    char clientAddress[30];

    NetworkMessage * networkMessage;

    // 2) init vars
    receiveThreadParam = (ReceiveThreadParam *) param;
    clientData = receiveThreadParam->clientData;
    networkApi = receiveThreadParam->networkApi;

    clientSocket = clientData->clientSocket;

    memset(clientAddress, 0x0, 30);
    strcpy(clientAddress, clientData->pcAddress);

    // TODO
    // Add circular buffer for parsing message
    // 3) processing
    int ret = 0;

    while (clientData->bKeepRecivingMessages == true) {
        memset(pucRcvBuffer, 0x0, NA_MAX_RECEIVE_BUFFER_SIZE);

        ret = recv(clientSocket, (char *) pucRcvBuffer, NA_MAX_RECEIVE_BUFFER_SIZE, 0);

        if (ret <= 0) {
            cout << endl << "INFO: client disconnected: " << clientAddress;
            break;
        }

        // 1) write new data to circular buffer
        circularBuffer->write(pucRcvBuffer, ret);

        while (true) {
            networkMessage = new NetworkMessage(circularBuffer);

            if (networkMessage->isConstructorComplete() == true) {
                // process message
                networkApi->na_processMessage(networkMessage, clientData);
            } else {
                break;
            }
        }
    }

    clients.remove(clientData);
    delete clientData;
    clientData = NULL;

    delete receiveThreadParam;
    receiveThreadParam = NULL;

    close(clientSocket);

    return NULL;
}

unsigned int NetworkApi::getClientID() {
    return uiClientID++;
}

ClientData * NetworkApi::getClient(unsigned int uiClientID) {
    list<ClientData *>::iterator i;
    for (i = clients.begin(); i != clients.end(); i++) {
        if ((*i)->uiClientID == uiClientID) {
            return (*i);
        }
    }

    return NULL;
}
