#pragma once

class CircularBuffer{

public:
	CircularBuffer(unsigned int _uiSize);

	~CircularBuffer();

	void write(unsigned char * pucValues, unsigned int uiSize);

	unsigned char * read();
	
	unsigned char * get(unsigned int * puiLength);

	void erase(unsigned int uiHowMuch);

	bool isFull();

private:

	unsigned char *pucBuffer;
    
    unsigned int uiHead, uiTail, uiLength;
    
    bool bBufferFull;
};
