/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3CUBE_H_
#define _R3CUBE_H_

#include <config.h>

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

#endif /* _R3CUBE_H_ */
// vim: et ts=4 sw=4 :
