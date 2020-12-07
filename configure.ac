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
AC_CHECK_LIB([lmdb], [mdb_env_open])
PKG_CHECK_MODULES([CHECK], [check >= 0.13.0], [],
		   [echo "Check not installed; tests will be unable to run"])

# Checks for header files.
AC_CHECK_HEADERS([string.h unistd.h fcntl.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([mkdir])

AC_ARG_ENABLE([git],
[--enable-git	Enable the git interface script],
[case "${enableval}" in
  yes) git=true ;;
  no)  git=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-git]) ;;
esac],[git=false])
AM_CONDITIONAL([GIT], [test x$git = xtrue])

AC_CONFIG_FILES([
Makefile
src/Makefile
git/Makefile
tests/Makefile
])

AC_OUTPUT
