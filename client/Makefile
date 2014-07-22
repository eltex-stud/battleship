CC=gcc
CFLAGS=-g -c -Wall -Werror
LDFLAGS=-lncurses -lmenu -lpanel
SOURCES=*.c
OBJECTS=*.o
BINARY=CLIENT

all: $(BINARY)

$(BINARY): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(BINARY) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean: 
	rm $(OBJECTS) $(BINARY)
