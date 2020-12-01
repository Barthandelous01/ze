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
PKG_CHECK_MODULES([CHECK], [check >= 0.13.0])
AC_SUBST([LMDB_CFLAGS])
AC_SUBST([LMDB_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([mkdir])

AC_ARG_ENABLE([git],
	AS_HELP_STRING([--enable-git], [Enable the git interface script]))
AM_CONDITIONAL([GIT], [test x$git = xtrue])

AC_CONFIG_FILES([
Makefile
src/Makefile
git/Makefile
tests/Makefile
])

AC_OUTPUT
