#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ze], [0.0.1], [fork on GitHub])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([LMDB], [lmdb])
AC_SUBST([LMDB_CFLAGS])
AC_SUBST([LMDB_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([mkdir])

AC_CONFIG_FILES([
Makefile
src/Makefile
])

AC_OUTPUT