CC=gcc
CFLAGS=-g -c -Wall -Werror
CPPFLAGS=-I. -I.. -I../include
LDFLAGS=-lncurses -lmenu -lpanel -lpthread
SOURCES=$(wildcard *.c)
MAIN=cl_main.c
OBJECTS=$(patsubst %.c,%.o,$(filter-out $(MAIN),$(SOURCES)))
BINARY=client

all: $(BINARY)

$(BINARY): $(MAIN) $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $(BINARY) $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(SOURCES)

clean: 
	rm $(OBJECTS) $(BINARY)
