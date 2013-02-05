#pragma once

#include "Configuration.h"
#include "CircularBuffer.h"
#include "NetworkMessage.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <list>
#include <iostream>

using namespace std;

#define NA_MAX_RECEIVE_BUFFER_SIZE			(1024 * 8) 

#define NA_MAX_RECEIVE_CIRCULAR_BUFFER_SIZE	(1024 * 16)

typedef struct clientData{
	SOCKET clientSocket;	
	
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
}ClientData;

class NetworkApi{
	
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
	WSAData wsaData;

	unsigned int uiServerPort;
	
	const char * pcClientName;
	unsigned int uiClientPort;

	unsigned int uiClientID;
	
	ErrorCode ecLastError;

	SOCKET serverSocket;      // Uticnica na kojoj server ocekuje poruke
	
	SOCKADDR_IN sraddr; // Struktura sa IP adresom i prolazom protokola za server
	SOCKADDR_IN claddr; // Struktura sa IP adresom i prolazom protokola za klijenta
};
