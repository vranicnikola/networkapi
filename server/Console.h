#pragma once

#include "AirConditionNetworkApi.h"

#include <Windows.h>

class Console{

public:
	Console();

	~Console();

	void start();
	
	unsigned int showMainMenu();

	void waitForFinish();
};
