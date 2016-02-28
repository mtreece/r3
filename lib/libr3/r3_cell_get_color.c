/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include "r3colors.h"
#include "r3cube.h"
#include "r3sides.h"

#include "r3_cell_get_color.h"

int r3_cell_get_color(r3cell *cell)
{
    if (!cell) {
        return -1;
    }

#if !defined(NDEBUG)
    assert(cell->side);
    assert(cell->side->cube);
    _r3_cube_check_integrity(cell->side->cube);
#endif

    return cell->color;
}

// vim: et ts=4 sw=4 :
