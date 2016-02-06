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

#include "r3cube.h"
#include "r3sides.h"
#include "r3_cube_get_face.h"

r3side *r3_cube_get_face(r3cube *cube)
{
    assert(cube);
    assert(cube->facing);

#if !defined(NDEBUG)
    for (int i = 0; i < NUM_SIDES; ++i) {
        if (&cube->sides[i] == cube->facing) {
            goto cont;
        }
    }
    assert(0);

cont:
#endif

    return cube->facing;
}

// vim: et ts=4 sw=4 :
