#pragma once

#include "Configuration.h"
#include "CircularBuffer.h"
#include "NetworkMessage.h"

#include <list>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

using namespace std;

#define NA_MAX_RECEIVE_BUFFER_SIZE			(1024 * 8) 

#define NA_MAX_RECEIVE_CIRCULAR_BUFFER_SIZE	(1024 * 16)

#define NO_SOCKET_ERROR                     0

typedef struct clientData {
    int clientSocket;

    char pcAddress[30];

    int iPort;

    bool bKeepRecivingMessages;

    unsigned int uiClientID;

    clientData() {
        clientSocket = 0;

        memset(pcAddress, 0x0, 30);

        iPort = 0;

        bKeepRecivingMessages = true;

        uiClientID = 0;
    }
} ClientData;

class NetworkApi {

public:
    NetworkApi(unsigned int _uiServerPort);

    ~NetworkApi();

    ErrorCode na_listen(bool * bEnd);

    ErrorCode getLastErrorCode();

    //! @param msg message which will be send to client
    ErrorCode na_sendMessage(NetworkMessage * msg, unsigned int uiClientID);

    //! @param msg message which will be send to client
    ErrorCode na_sendMessage(NetworkMessage * msg, ClientData * clientData);

    //! @param msg pointer to message which will be received from client
    ErrorCode na_reciveMessage(NetworkMessage * msg, unsigned int uiClientID);

    virtual void na_processMessage(NetworkMessage * networkMessage, ClientData * clientData) = 0;

private:
    ErrorCode openServerSocket(unsigned int uiServerPort);

    unsigned int getClientID();

    ClientData * getClient(unsigned int uiClientID);

private:
    unsigned int uiServerPort;

    const char * pcClientName;
    unsigned int uiClientPort;

    unsigned int uiClientID;

    ErrorCode ecLastError;

    int serverSocket;      // Uticnica na kojoj server ocekuje poruke

    struct sockaddr_in sraddr; // Struktura sa IP adresom i prolazom protokola za server
    struct sockaddr_in claddr; // Struktura sa IP adresom i prolazom protokola za klijenta
};
