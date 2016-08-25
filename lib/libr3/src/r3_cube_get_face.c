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
#include <stdlib.h>

#include <r3/r3_cube_get_face.h>

#include "r3cube.h"
#include "r3sides.h"

const r3side *r3_cube_get_face(const r3cube *cube, unsigned side)
{
    if (!cube) {
        return NULL;
    }

    if (side >= NUM_SIDES) {
        return NULL;
    }

#if !defined(NDEBUG)
    _r3_cube_check_integrity(cube);
#endif

    return &cube->sides[side];
}

// vim: ts=4 sts=4 sw=4 et :
