#
# Copyright (C) 2015-2016 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

if BUILD_TESTS
TESTS = $(check_PROGRAMS)
check_PROGRAMS = check_libr3
endif

check_libr3_SOURCES = check_libr3.c \
                      check_libr3.h \
                      check_libr3-core.c \
                      check_libr3-moves.c \
                      check_libr3.map
check_libr3_CFLAGS = @PROJ_CFLAGS@ @CHECK_CFLAGS@
check_libr3_CPPFLAGS = -I$(top_srcdir)/lib/libr3
check_libr3_LDADD = @CHECK_LIBS@
check_libr3_LDADD += $(top_builddir)/lib/libr3/libr3.a
check_libr3_LDFLAGS = -Wl,$(top_srcdir)/test/check_libr3.map

EXTRA_DIST = $(TESTS)

# vim: et ts=4 sw=4 :
