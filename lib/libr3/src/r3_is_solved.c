/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <assert.h>

#include <r3/r3_is_solved.h>

#include "r3cube.h"

int r3_is_solved(const r3cube *cube)
{
#if !defined(NDEBUG)
    _r3_cube_check_integrity(cube);
#endif

    for (unsigned s = 0; s < NUM_SIDES; ++s) {
        int color = cube->sides[s].cells[0][0]->color;
        for (unsigned r = 0; r < NUM_ROWS; ++r) {
            for (unsigned c = 0; c < NUM_COLS; ++c) {
                if (cube->sides[s].cells[r][c]->color != color) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

// vim: ts=4 sts=4 sw=4 et :
