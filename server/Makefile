CC=g++
CFLAGS=-c -Wall
LDFLAGS=-L/usr/xenomai/lib -lm -lnative -lxenomai -lpthread
INCLDUES=-I/usr/xenomai/include
SOURCES= \
	Main.cpp \
	Console.cpp \
	CircularBuffer.cpp \
	NetworkApi.cpp \
	NetworkMessage.cpp \
	ServoMotorNetworkApi.cpp
	#servo-controll/main.c \
	#servo-controll/gpio.c \
	#servo-controll/log.c \
	
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=railroad-server
ODIR=obj

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(INCLDUES) $(LDFLAGS) $(OBJECTS) -o $@

$(ODIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLDUES) $< -o $@
	
clean:
	rm -f $(ODIR)/*.o
