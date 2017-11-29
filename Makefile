

OUTPUT   = rms-cli

SOURCE   =  src/main.cpp

CC       = @g++ -Wall -pthread
CFLAGS   = -I../src/ -std=c++11

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS += -DDEBUG -g -Og
    OUTPUT  = rms-cli_debug
else
    CFLAGS += -DNDEBUG -O3
endif

LDFLAGS = -g
LDLIBS = -lrtaudio

all:
	$(CC) $(LDFLAGS) -o $(OUTPUT) $(SOURCE) $(SRCFILE) $(CFLAGS) $(LDLIBS) 

clean:
	rm $(OUTPUT)

