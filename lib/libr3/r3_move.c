/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>
#include <assert.h>

#include "r3cube.h"
#include "r3sides.h"
#include "r3moves.h"
#include "r3_move.h"

int r3_move(r3cube *cube, int direction, int selector)
{
    int vertical;  // direction is vertical? else, horizontal

    if (!cube) {
        return -1;
    }

    if (0 > selector) {
        return -1;
    }

    switch (direction) {
        case R3_UP:
        case R3_DOWN:
            vertical = 1;
            break;
        case R3_LEFT:
        case R3_RIGHT:
            vertical = 0;
            break;
        default:
            return -1;
    }

    if (selector > (vertical ? NUM_COLS : NUM_ROWS) - 1) {
        return -1;
    }

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
