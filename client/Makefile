CC=gcc
CFLAGS=-g -c -Wall -Werror
LDFLAGS=-lncurses -lmenu -lpanel -lpthread
SOURCES=*.c
OBJECTS=*.o
BINARY=client

all: $(BINARY)

$(BINARY): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(BINARY) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean: 
	rm $(OBJECTS) $(BINARY)
