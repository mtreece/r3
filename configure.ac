#
# Copyright (C) 2014-2015 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

AC_INIT([r3], [0.0])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB
AM_PROG_AR
AC_CONFIG_HEADERS([config.h])

dnl options which probably ought to be turned on if you're doing development
AC_MSG_CHECKING([whether or not to build with common developer options])
AC_ARG_ENABLE([common-dev-opts],
	    AS_HELP_STRING([--enable-common-dev-opts],
			   [turn on common options for development]),
	   [COMMONDEVOPTS=$enableval], [])
if test "x$COMMONDEVOPTS" == "xyes"; then
	AC_MSG_RESULT([yes])
	ASSERTIONS=yes
	BUILD_TESTS=yes
else
	AC_MSG_RESULT([no])
fi

AC_MSG_CHECKING([whether or not to turn on assertions])
AC_ARG_ENABLE([assertions],
	    AS_HELP_STRING([--enable-assertions],
			   [turn on assert() for development]),
	   [ASSERTIONS=$enableval], [])
if test "x$ASSERTIONS" != "xyes"; then
	AC_MSG_RESULT([no])
	AC_DEFINE([NDEBUG], , [if defined, assertions do nothing])
else
	AC_MSG_RESULT([yes])
fi

AC_MSG_CHECKING([whether or not to build tests])
AC_ARG_ENABLE([build-tests],
	    AS_HELP_STRING([--enable-build-tests],
			   [build the 'make check' functionality]),
	   [BUILD_TESTS=$enableval], [])
if test "x$BUILD_TESTS" = "xyes"; then
	AC_MSG_RESULT([yes])
	PKG_CHECK_MODULES([CHECK], [check >= 0.9.14])
else
	AC_MSG_RESULT([no])
fi
AM_CONDITIONAL([BUILD_TESTS], [test "x$BUILD_TESTS" == "xyes"])

AC_CONFIG_FILES([Makefile
		 lib/libr3/Makefile
		 src/Makefile
		 test/Makefile])

AC_OUTPUT
