#
# Copyright (C) 2014-2015 Michael T. Reece.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

SUBDIRS = lib/libr3 src/ test/

# clean gcov-related files from the codebase
clean-gcov:
	find \( \
	       -name '*.gcov' \
	       -or -name '*.gcno' \
	       -or -name '*.gcda' \
	     \) -print -delete
