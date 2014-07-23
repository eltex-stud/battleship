AUTOMAKE_OPTIONS = foreign

AM_CFLAGS = -std=c99 -Wno-error
AM_LDFLAGS = -lncurses -lmenu -lpanel -lpthread
AM_CPPFLAGS = -I. -I.. -I../include

bin_PROGRAMS = client
client_SOURCES = \
cl_gui.c         \
cl_logic.c       \
cl_main.c        \
cl_net.c
