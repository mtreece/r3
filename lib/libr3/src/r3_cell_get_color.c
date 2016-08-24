/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <r3/r3colors.h>
#include <r3/r3_cell_get_color.h>

#include "r3cube.h"
#include "r3sides.h"

int r3_cell_get_color(const r3cell *cell)
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

// vim: ts=4 sts=4 sw=4 et :
