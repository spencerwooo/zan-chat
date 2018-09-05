#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(test, 1.0, ff@126.com)
AM_INIT_AUTOMAKE(test, 1.0)
AC_CONFIG_SRCDIR([createStatusMenu.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h locale.h malloc.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero getcwd gettimeofday inet_ntoa memchr memset mkdir setlocale strdup strstr strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
