/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3SIDES_H_
#define _R3SIDES_H_

#include <config.h>

#include "r3colors.h"

// TODO: put in build system & generalize
#define NUM_SIDES 6
#define NUM_COLS 3
#define NUM_ROWS 3
#define MAX_ROW_COLS (NUM_COLS > NUM_ROWS ? NUM_COLS : NUM_ROWS)

/// current impl - max of 2 brothers, in the corder-edge case
#define MAX_NUM_BROTHERS 2

/// current impl - max of 4 neighbors, in the center-of-face case
#define MAX_NUM_NEIGHBORS 4

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
} r3cell;

typedef struct r3side {
    r3cell cells[NUM_ROWS][NUM_COLS];
} r3side;

#endif /* _R3SIDES_H_ */
// vim: et ts=4 sw=4 sts=-1 :
