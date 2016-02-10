/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <stdlib.h>

#include "r3cube.h"
#include "r3sides.h"

#include "r3_get_facing_cell.h"

const r3cell *r3_get_facing_cell(r3cube *cube, int row, int col)
{
    if (!cube) {
        return NULL;
    }

    if ((row < 0 || row >= NUM_ROWS) || (col < 0 || col >= NUM_COLS)) {
        return NULL;
    }

    return cube->facing->cells[row][col];
}

// vim: et ts=4 sw=4 :
