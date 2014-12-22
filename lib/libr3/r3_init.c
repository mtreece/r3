/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>
#include <assert.h>
#include <string.h>

#include "r3cube.h"
#include "r3sides.h"
#include "r3_init.h"

int r3_init(r3cube *cube)
{
    if (!cube) {
        return -1;
    }
    memset(cube, 0x00, sizeof(r3cube));
    cube->position = 0;

    int colors[] = {
        R3_WHITE,
        R3_RED,
        R3_BLUE,
        R3_ORANGE,
        R3_GREEN,
        R3_YELLOW,
    };

    assert(NUM_SIDES == sizeof(colors)/sizeof(int));

    for(int i = 0; i < NUM_SIDES; ++i)
    {
        for(int j = 0; j < CUBE_HEIGHT; ++j)
        {
            for(int k = 0; k < CUBE_WIDTH; ++k)
            {
                cube->sides[i].cells[j][k].color = colors[i];
            }
        }
    }

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
