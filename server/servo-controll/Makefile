CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-L/usr/xenomai/lib -lm -lnative -lxenomai
INCLDUES=-I/usr/xenomai/include
SOURCES=main.c gpio.c log.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=railroad

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(INCLDUES) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLDUES)$< -o $@
