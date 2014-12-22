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

/*
 * Side to sides-number:
 *
 *      1
 *   3  0  4  5
 *      2
 *
 */
#define SPRINT_FORMAT \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n"

// TODO: handle this
#if NUM_SIDES != 6
#error "(NUM_SIDES != 6) not yet supported"
#endif

static char _r3_sprint_to_char(int color)
{
    char out;
    switch (color) {
        case R3_WHITE:
            out = 'W';
            break;
        case R3_RED:
            out = 'R';
            break;
        case R3_BLUE:
            out = 'B';
            break;
        case R3_ORANGE:
            out = 'O';
            break;
        case R3_GREEN:
            out = 'G';
            break;
        case R3_YELLOW:
            out = 'Y';
            break;
        default:
            out = '?';
            break;
    }
    return out;
}

int r3_sprint(char *buffer, size_t *buflen, r3cube *cube)
{
    if (!cube || !buflen) {
        return -1;
    }

    *buflen = sprintf((char *)buffer, SPRINT_FORMAT,
            /* top face => side 1 */
            _r3_sprint_to_char(cube->sides[1].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[1].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[1].cells[0][2].color),

            _r3_sprint_to_char(cube->sides[1].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[1].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[1].cells[1][2].color),

            _r3_sprint_to_char(cube->sides[1].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[1].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[1].cells[2][2].color),


            /* row 0: <3> <0> <4> <5> */
            _r3_sprint_to_char(cube->sides[3].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[3].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[3].cells[0][2].color),
            _r3_sprint_to_char(cube->sides[0].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[0].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[0].cells[0][2].color),
            _r3_sprint_to_char(cube->sides[4].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[4].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[4].cells[0][2].color),
            _r3_sprint_to_char(cube->sides[5].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[5].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[5].cells[0][2].color),

            /* row 1: <3> <0> <4> <5> */
            _r3_sprint_to_char(cube->sides[3].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[3].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[3].cells[1][2].color),
            _r3_sprint_to_char(cube->sides[0].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[0].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[0].cells[1][2].color),
            _r3_sprint_to_char(cube->sides[4].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[4].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[4].cells[1][2].color),
            _r3_sprint_to_char(cube->sides[5].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[5].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[5].cells[1][2].color),

            /* row 2: <3> <0> <4> <5> */
            _r3_sprint_to_char(cube->sides[3].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[3].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[3].cells[2][2].color),
            _r3_sprint_to_char(cube->sides[0].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[0].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[0].cells[2][2].color),
            _r3_sprint_to_char(cube->sides[4].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[4].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[4].cells[2][2].color),
            _r3_sprint_to_char(cube->sides[5].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[5].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[5].cells[2][2].color),

            /* bottommost face => side 2 */
            _r3_sprint_to_char(cube->sides[2].cells[0][0].color),
            _r3_sprint_to_char(cube->sides[2].cells[0][1].color),
            _r3_sprint_to_char(cube->sides[2].cells[0][2].color),

            _r3_sprint_to_char(cube->sides[2].cells[1][0].color),
            _r3_sprint_to_char(cube->sides[2].cells[1][1].color),
            _r3_sprint_to_char(cube->sides[2].cells[1][2].color),

            _r3_sprint_to_char(cube->sides[2].cells[2][0].color),
            _r3_sprint_to_char(cube->sides[2].cells[2][1].color),
            _r3_sprint_to_char(cube->sides[2].cells[2][2].color)
    );
    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
