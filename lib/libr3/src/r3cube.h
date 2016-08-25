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

struct r3cube;
typedef struct r3cube r3cube;

#include "r3sides.h"

struct r3cube {
    /// position/rotation when viewing and operating
    /// (not yet supported)
    int position;

    /// the sides of this cube
    r3side sides[NUM_SIDES];

    /// storage space for all cells that make up this cube
    r3cell cellspace[NUM_SIDES * NUM_ROWS * NUM_COLS];

    /// two adjacent cells, [0][0], [0][1], of the user-facing r3side. This
    /// information is used to help anchor the cube's orientation whenever
    /// reconstructing sides from cell-links. These anchors should be updated
    /// internally whenever an operation occurs which affect them.
    r3cell *anchors[2];
};

#if !defined(NDEBUG)
static inline void _r3_cube_check_integrity(const r3cube *cube)
{
    const r3cell (*cs)[sizeof(cube->cellspace) / sizeof(cube->cellspace[0])]
        = &cube->cellspace;

    for (unsigned i = 0; i < sizeof(*cs) / sizeof((*cs)[0]); ++i) {
        const r3cell *c = &(*cs)[i];
        int row = c->row;
        int col = c->col;
        assert(row < NUM_ROWS);
        assert(col < NUM_COLS);

        unsigned checks = 0;
        for (int s = 0; s < NUM_SIDES; ++s) {
            if (&cube->sides[s] == c->side) {
                ++checks;
            }
        }
        assert(1 == checks);
        assert(c->side->cells[row][col] == c);
    }

    for (unsigned i = 0; i < sizeof(cube->sides)/sizeof(cube->sides[0]); ++i) {
        assert(cube->sides[i].cube == cube);
    }
}
#endif

#endif /* _R3CUBE_H_ */
// vim: ts=4 sts=4 sw=4 et :
