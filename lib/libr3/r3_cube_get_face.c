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

#include "r3cube.h"
#include "r3sides.h"
#include "r3_cube_get_face.h"

const r3side *r3_cube_get_face(r3cube *cube, unsigned side)
{
    if (!cube) {
        return NULL;
    }

    if (side >= NUM_SIDES) {
        return NULL;
    }

    return &cube->sides[side];
}

// vim: et ts=4 sw=4 :
