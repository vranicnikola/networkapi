#include "NetworkApi.h"

#include <map>

DWORD WINAPI receiveMessageThread(LPVOID param);

DWORD WINAPI senderMessageThread(LPVOID param);

list<ClientData *> clients;

map<unsigned int, list<NetworkMessage *>> receivePipeline;

HANDLE receivePipeSemaphore;

CircularBuffer * circularBuffer;

typedef struct{
	ClientData * clientData;
	NetworkApi * networkApi;
}ReceiveThreadParam;

NetworkApi::NetworkApi(unsigned int _uiServerPort):
	uiServerPort(_uiServerPort)
{
	uiClientID = 0;

	circularBuffer = new CircularBuffer(NA_MAX_RECEIVE_CIRCULAR_BUFFER_SIZE);

	receivePipeSemaphore = CreateSemaphore(0, 1, 1, 0);

	// 1) open server socket
	ecLastError = openServerSocket(uiServerPort);
}
	
NetworkApi::~NetworkApi(){

	// 1) close socket
	closesocket(serverSocket);    

	WSACleanup();

	// 2) clean clients list
	for(auto i = clients.begin(); i != clients.end(); i++){
		delete *i;
	}
	clients.clear();

	// 3) purge recived messages

	// 4) others
	if(circularBuffer != NULL){
		delete circularBuffer;
		circularBuffer = NULL;
	}

	CloseHandle(receivePipeSemaphore);
}

ErrorCode NetworkApi::getLastErrorCode(){
	return ecLastError;
}

ErrorCode NetworkApi::na_listen(bool * bEnd){
	int iLength;
	
	while(*bEnd == false){

		memset( &claddr, 0x00, sizeof( SOCKADDR_IN ));
		iLength = sizeof(SOCKADDR);

		SOCKET client = accept( serverSocket,( SOCKADDR* )&claddr, &iLength );
		if( client == INVALID_SOCKET )
		{
			int errorCode = WSAGetLastError();
			closesocket( serverSocket );
			WSACleanup();
			
			return EC_ERROR;
		}

		ClientData * clientData = new ClientData();
	
		clientData->clientSocket = client;
		sprintf( clientData->pcAddress, "%s", inet_ntoa( claddr.sin_addr ));
		clientData->iPort = ntohs((( SOCKADDR_IN* )&claddr )->sin_port );

		clientData->uiClientID = getClientID();

		clients.push_back(clientData);

		DWORD msgHndId;
        
		ReceiveThreadParam * receiveThreadParam = new ReceiveThreadParam();
		receiveThreadParam->clientData = clientData;
		receiveThreadParam->networkApi = this;

		cout << endl << "INFO: client connected: " << clientData->pcAddress;

        CreateThread(NULL, NULL, receiveMessageThread, (LPVOID) receiveThreadParam, NULL, &msgHndId );        
	}

	return EC_OK;
}

//! @param msg message which will be send to client
ErrorCode NetworkApi::na_sendMessage(NetworkMessage * msg, unsigned int uiClientID){
	ClientData * clientData = getClient(uiClientID);

	unsigned int uiLen = sizeof(msg->getMessageID()) + sizeof(msg->getMessageSize()) + msg->getMessageSize();
	
	cout << endl << "Response: len: " << uiLen;
	cout << endl << "[ ";
	for(unsigned int i = 0; i < uiLen; i++){
		printf("0x%02x ", ((unsigned char *)msg)[i]);
	}
	cout << "]";
	if(send(clientData->clientSocket, (char *)msg, uiLen, 0) == uiLen){
		return EC_OK;
	}
	else{
		return EC_ERROR;
	}
}

//! @param msg message which will be send to client
ErrorCode NetworkApi::na_sendMessage(NetworkMessage * msg, ClientData * clientData){
	return na_sendMessage(msg, clientData->uiClientID);
}

//! @param msg pointer to message which will be received from client
ErrorCode NetworkApi::na_reciveMessage(NetworkMessage * msg, unsigned int uiClientID){
	msg = NULL;
	
	auto pointer = receivePipeline.find(uiClientID);
	
	if(pointer != receivePipeline.end()){
		list<NetworkMessage *> messages = receivePipeline[uiClientID];

		msg = messages.empty() ? NULL : messages.front();
	}

	return EC_OK;
}

ErrorCode NetworkApi::openServerSocket(unsigned int uiServerPort){

	if( WSAStartup( MAKEWORD( 2, 1 ), &wsaData ) != 0 ){
		cout << endl << "ERROR: Nije pronadjen Winsock 2.1 ili bolji. Kod greske:" <<  WSAGetLastError();
		return EC_ERROR;
	}

	memset( &sraddr, 0x00, sizeof( SOCKADDR_IN ));
	sraddr.sin_family = AF_INET;
	sraddr.sin_addr.S_un.S_addr = INADDR_ANY;
	sraddr.sin_port = htons(uiServerPort);

	serverSocket = INVALID_SOCKET;
	serverSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( SOCKET_ERROR == serverSocket ){
		cout << endl << "ERROR: Problem pri stvaranju uticnice! Kod greske: " << WSAGetLastError();
		WSACleanup();
		return EC_ERROR;
	}

	if( bind( serverSocket, ( SOCKADDR* )&sraddr, sizeof( SOCKADDR )) == SOCKET_ERROR ){
		cout << endl << "ERROR: Bind problem! Kod greske: " << WSAGetLastError();
		WSACleanup();
		return EC_ERROR;
	}

	if( listen(serverSocket, NA_MAX_RECEIVE_BUFFER_SIZE) == SOCKET_ERROR ){
		cout << endl << "ERROR: Listen problem! Kod greske: " << WSAGetLastError();
		WSACleanup();
		return EC_ERROR;
	}

	return EC_OK;
}

DWORD WINAPI receiveMessageThread(LPVOID param){

	// 1) vars
	ReceiveThreadParam * receiveThreadParam;
	
	ClientData * clientData;
	
	NetworkApi * networkApi;
	
	SOCKET clientSocket;

	unsigned char pucRcvBuffer[NA_MAX_RECEIVE_BUFFER_SIZE];

	WORD clientPort;
	
	char clientAddress[30];

	NetworkMessage * networkMessage;

	unsigned int uiOffset;

	// 2) init vars
	receiveThreadParam = ( ReceiveThreadParam * ) param;
	clientData = receiveThreadParam->clientData;
	networkApi = receiveThreadParam->networkApi;

	clientSocket = clientData->clientSocket;

	clientPort = clientData->iPort;
	
	memset(clientAddress, 0x0, 30);
	strcpy_s(clientAddress, clientData->pcAddress);

	uiOffset = sizeof(networkMessage->getMessageID()) + sizeof(networkMessage->getMessageSize());

	// TODO
	// Add circular buffer for parsing message
	// 3) processing
    int ret = 0;

	while(clientData->bKeepRecivingMessages == true){
		memset(pucRcvBuffer, 0x0, NA_MAX_RECEIVE_BUFFER_SIZE);

        ret = recv(clientSocket, (char *) pucRcvBuffer, NA_MAX_RECEIVE_BUFFER_SIZE, 0 );
        
        if( ret <= 0 ){
		    cout << endl << "INFO: client disconnected: " << clientAddress;
            break;
	    }

		// 1) write new data to circular buffer
		circularBuffer->write(pucRcvBuffer, ret);

		while(true){
			networkMessage = new NetworkMessage(circularBuffer);

			if(networkMessage->isConstructorComplete() == true){
				// process message
				networkApi->na_processMessage(networkMessage, clientData);
			}
			else{
				break;
			}
		}            
                    
		//WaitForSingleObject(receivePipeSemaphore, INFINITE);
		//receivePipeline[clientData->uiClientID].push_back(networkMessage);
		//ReleaseSemaphore(receivePipeSemaphore, 1, 0);	
    }

    clients.remove(clientData);
    delete clientData;
	clientData = NULL;

	delete receiveThreadParam;
	receiveThreadParam = NULL;

	return EC_OK;
}

unsigned int NetworkApi::getClientID(){
	return uiClientID++;
}

ClientData * NetworkApi::getClient(unsigned int uiClientID){

	for(auto i = clients.begin(); i != clients.end(); i++){
		if( (*i)->uiClientID == uiClientID){
			return (*i);
		}
	}

	return NULL;
}
