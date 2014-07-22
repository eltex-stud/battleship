CC=gcc
CFLAGS=-g -c -Wall -Werror
CPPFLAGS=-I. -I..
LDFLAGS=-lncurses -lmenu -lpanel
SOURCES=*.c
OBJECTS=*.o
BINARY=server

all: $(BINARY)

$(BINARY): $(OBJECTS) 
	$(CC) $(CPPFLAGS) $(OBJECTS) -o $(BINARY) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SOURCES)

clean: 
	rm $(OBJECTS) $(BINARY)
