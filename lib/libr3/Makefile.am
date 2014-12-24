#
# Copyright (C) 2014 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
lib_LIBRARIES = libr3.a

libr3_a_SOURCES = \
		  r3_init.c \
		  r3_move.c \
		  r3_sprint.c \
		  r3_synclinks.c \
		  util.h

include_HEADERS = \
		  r3.h \
		  r3_init.h \
		  r3_move.h \
		  r3_sprint.h \
		  r3_synclinks.h \
		  r3colors.h \
		  r3cube.h \
		  r3moves.h \
		  r3sides.h

# TODO -ggdb only on ./configure option
AM_CFLAGS = -std=c99 -Wall -Wextra -Werror -ggdb
