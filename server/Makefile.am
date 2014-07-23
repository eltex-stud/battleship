AUTOMAKE_OPTIONS = foreign

#AM_LDFLAGS = -ldl
AM_CFLAGS = -std=c99
AM_LDFLAGS = -lpthread
AM_CPPFLAGS = -I. -I.. -I../include

bin_PROGRAMS = server
server_SOURCES = \
srv_logic.c      \
srv_main.c       \
srv_net.c
