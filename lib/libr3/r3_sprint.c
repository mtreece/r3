/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <stdio.h>
#include <stddef.h>

#include "r3cube.h"

#define SPRINT_FORMAT \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n" \
"\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"| | | |  | | | |  | | | |  | | | |\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"| | | |  | | | |  | | | |  | | | |\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"| | | |  | | | |  | | | |  | | | |\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"\n" \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n" \
"         | | | |\n" \
"         +-+-+-+\n"





int r3_sprint(char *buffer, size_t *buflen, r3cube *cube)
{
    if (!cube) {
        return -1;
    }

    *buflen = sprintf((char *)buffer, SPRINT_FORMAT);
    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
