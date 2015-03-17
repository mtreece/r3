#
# Copyright (C) 2015 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

TESTS = check_libr3
check_PROGRAMS = check_libr3

check_libr3_SOURCES = check_libr3.c \
                      check_libr3-core.c
check_libr3_CFLAGS = @CHECK_CFLAGS@
check_libr3_LDADD = @CHECK_LIBS@
check_libr3_LDFLAGS = -Wl,check_libr3.map

EXTRA_DIST = $(TESTS)

# vim: et ts=4 sw=4 :
