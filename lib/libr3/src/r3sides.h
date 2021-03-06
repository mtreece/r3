/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3SIDES_H_
#define _R3SIDES_H_

#include <config.h>

#include <r3/r3colors.h>

#include "r3cube.h"

typedef struct r3cell {
    /// the color of this cell
    int color;

    /// a brother r3cell is one which shares the same physical cube-block of
    /// this cell; ergo, are permanent neighbors, across dimensions.
    /// Note: the brother relationship is transitive.
    struct r3cell *brothers[MAX_NUM_BROTHERS + 1];

    /// a neighbor r3cell is one which is adjacent, and on the same plane;
    /// ergo, these neighbors are frequently modified on move operations.
    struct r3cell *neighbors[MAX_NUM_NEIGHBORS + 1];

    /// the side this cell is part of
    struct r3side *side;

    /// the row & col on the .side this belongs to
    unsigned row, col;
} r3cell;

typedef struct r3side {
    /// the arrangement of r3cell elements on this side
    struct r3cell *cells[NUM_ROWS][NUM_COLS];

    /// the cube this side is part of
    struct r3cube *cube;
} r3side;

#endif /* _R3SIDES_H_ */
// vim: ts=4 sts=4 sw=4 et :
