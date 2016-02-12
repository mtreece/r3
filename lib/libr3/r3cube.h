/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3CUBE_H_
#define _R3CUBE_H_

#include <config.h>

#include <assert.h>

#include "r3sides.h"

typedef struct {
    /// position/rotation when viewing and operating
    /// (not yet supported)
    int position;

    /// the sides of this cube
    r3side sides[NUM_SIDES];

    /// the side facing the user
    r3side *facing;

    /// storage space for all cells that make up this cube
    r3cell cellspace[NUM_SIDES * NUM_ROWS * NUM_COLS];

    /// two adjacent cells, [0][0], [0][1], of the user-facing r3side. This
    /// information is used to help anchor the cube's orientation whenever
    /// reconstructing sides from cell-links. These anchors should be updated
    /// internally whenever an operation occurs which affect them.
    r3cell *anchors[2];
} r3cube;

static inline void _r3_cube_check_integrity(const r3cube *cube)
{
#if !defined(NDEBUG)
    unsigned checks = 0;

    assert(cube->facing);

    for (unsigned i = 0; i < NUM_SIDES; ++i) {
        if (&cube->sides[i] == cube->facing) {
            ++checks;
        }
    }
    assert(1 == checks);

    const r3cell (*cs)[sizeof(cube->cellspace) / sizeof(cube->cellspace[0])]
        = &cube->cellspace;

    for (unsigned i = 0; i < sizeof(*cs) / sizeof((*cs)[0]); ++i) {
        const r3cell *c = &(*cs)[i];
        int row = c->row;
        int col = c->col;

        checks = 0;
        for (int s = 0; s < NUM_SIDES; ++s) {
            if (&cube->sides[s] == c->side) {
                ++checks;
            }
        }
        assert(1 == checks);
        assert(c->side->cells[row][col] == c);
    }
#endif
}

#endif /* _R3CUBE_H_ */
// vim: et ts=4 sw=4 :
