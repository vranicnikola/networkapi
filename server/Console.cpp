#include "Console.h"
#include "Configuration.h"

#include <iostream>

using namespace std;

enum{
	EXIT = 1
};

DWORD WINAPI networkThread(LPVOID param);

HANDLE finishSignal;

AirConditionNetworkApi * network;

Console::Console(){
	DWORD msgHndId;

	cout << "Air Condition Server" << endl;

	finishSignal = CreateSemaphore(0, 0, 2, 0);
	network = NULL;
	CreateThread(NULL, NULL, networkThread, (LPVOID) NULL, NULL, &msgHndId);        
}

Console::~Console(){
	if(network != NULL){
		delete network;
		network = NULL;
	}
}

void Console::start(){
	unsigned int uiChoice;
	
	bool bWork = true;

	while(bWork == true){
		uiChoice = showMainMenu();

		switch(uiChoice){
		
		case EXIT:
			bWork = false;
			break;
		}
	}

	// 1) waitForFinish
	// 2) network
	ReleaseSemaphore(finishSignal, 2, 0);
}

unsigned int Console::showMainMenu(){
	unsigned int uiChoice;
	
	cout << endl << "1 - ";
	cout << endl << "2 - ";
	cout << endl << "3 - ";
	cout << endl << "4 - ";
	cout << endl << "5 - ";
	cout << endl << EXIT << " - exit";

	cout << endl << "Choice: ";
	cin >> uiChoice;
	
	return uiChoice;
}

void Console::waitForFinish(){
	WaitForSingleObject(finishSignal, INFINITE);
}

DWORD WINAPI networkThread(LPVOID param){
	network = new AirConditionNetworkApi(DEFAULT_SERVER_PORT);
	WaitForSingleObject(finishSignal, INFINITE);
	return 0;
}
