#pragma once

class Console{

public:
	Console();

	~Console();

	void start();
	
	unsigned int showMainMenu();

	void waitForFinish();
};
