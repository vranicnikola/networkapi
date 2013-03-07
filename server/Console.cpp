#include "Console.h"
#include "Configuration.h"
#include "ServoMotorNetworkApi.h"

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

enum {
	EXIT = 1
};

void * networkThread(void * ptr);

sem_t finishSignal;

ServoMotorNetworkApi * network;

Console::Console() {
	pthread_t id;

	cout << "Air Condition Server" << endl;

	sem_init(&finishSignal, 0, 2);
	network = NULL;
	pthread_create(&id, NULL, &networkThread, NULL);
	pthread_join(id, NULL);
	sem_destroy (&finishSignal);
}

Console::~Console() {
	if (network != NULL) {
		delete network;
		network = NULL;
	}
}

void Console::start() {
	unsigned int uiChoice;

	bool bWork = true;

	while (bWork == true) {
		uiChoice = showMainMenu();

		switch (uiChoice) {

		case EXIT:
			bWork = false;
			break;
		}
	}

	// 1) waitForFinish
	// 2) network
	sem_post (&finishSignal);
	sem_post(&finishSignal);
}

unsigned int Console::showMainMenu() {
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

void Console::waitForFinish() {
	sem_wait (&finishSignal);
}

void * networkThread(void * param) {
	network = new ServoMotorNetworkApi(DEFAULT_SERVER_PORT);
	sem_wait(&finishSignal);
	return NULL;
}
