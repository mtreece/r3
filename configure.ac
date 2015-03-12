#
# Copyright (C) 2014 Michael T. Reece.
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
AC_CONFIG_FILES([Makefile src/Makefile lib/libr3/Makefile])

AC_ARG_ENABLE([assertions],
	    AS_HELP_STRING([--enable-assertions],
			   [turn on assert() for development]),
	   [ASSERTIONS=$enableval], [ASSERTIONS="no"])
if test "x$ASSERTIONS" != "xyes"; then
	AC_SUBST(WITH_ASSERTIONS)
	AC_DEFINE([NDEBUG], , [If defined, assertions do nothing])
fi

AC_OUTPUT
