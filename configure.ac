#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([battleship], [1,0])
AM_INIT_AUTOMAKE
#AC_CONFIG_SRCDIR([client/cl_main.c] [server/srv_main.c])
AC_CONFIG_HEADERS([config.h:config.in])
# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
ACLOCAL_AM_FLAGS
# Checks for libraries.

AC_CHECK_LIB(curses, initscr)
# Checks for header files.
AC_CHECK_HEADERS([netdb.h netinet/in.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile]
				[server/Makefile]
				[client/Makefile])
#AC_CONFIG_SUBDIRS([AI])
AC_OUTPUT
