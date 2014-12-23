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

#include <stdio.h>

static void link_neighbors(r3cell *a, r3cell *b, int is_brother)
{
    r3cell **next_an;
    r3cell **next_bn;

    if (is_brother) {
        next_an = &a->brothers[0];
        next_bn = &b->brothers[0];
    } else {
        next_an = &a->neighbors[0];
        next_bn = &b->neighbors[0];
    }

    // find first non-NULL slot for each
    while(*next_an && b != *next_an) ++next_an;
    while(*next_bn && a != *next_bn) ++next_bn;

    // ensure we're not re-performing an already linked pair
    // NOTE: the 1st "&&-clause" is redundant from the 2nd; I'm listing it as-is
    // only to emphasize
    assert(b != *next_an && *next_an == NULL);
    assert(a != *next_bn && *next_bn == NULL);

    if (is_brother) {
        assert(MAX_NUM_BROTHERS > ((next_an - &a->brothers[0])));
        assert(MAX_NUM_BROTHERS > ((next_bn - &b->brothers[0])));
    } else {
        assert(MAX_NUM_NEIGHBORS > ((next_an - &a->neighbors[0])));
        assert(MAX_NUM_NEIGHBORS > ((next_bn - &b->neighbors[0])));
    }

    *next_an = b;
    *next_bn = a;
}

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
        for(int j = 0; j < NUM_ROWS; ++j)
        {
            for(int k = 0; k < NUM_COLS; ++k)
            {
                cube->sides[i].cells[j][k].color = colors[i];
                memset(cube->sides[i].cells[j][k].brothers, 0x00,
                        sizeof(((r3cell *)NULL)->brothers));
                memset(cube->sides[i].cells[j][k].neighbors, 0x00,
                        sizeof(((r3cell *)NULL)->neighbors));
            }
        }
    }

    // link brothers ///////////////////////////////////////////////////////////
    // link side[0] upper-left
    link_neighbors(&cube->sides[0].cells[0][0], &cube->sides[3].cells[0][2], 1);
    link_neighbors(&cube->sides[0].cells[0][0], &cube->sides[1].cells[2][0], 1);

    // link side[0] upper-right
    link_neighbors(&cube->sides[0].cells[0][2], &cube->sides[4].cells[0][0], 1);
    link_neighbors(&cube->sides[0].cells[0][2], &cube->sides[1].cells[2][2], 1);

    // link side[0] bottom-left
    link_neighbors(&cube->sides[0].cells[2][0], &cube->sides[3].cells[2][2], 1);
    link_neighbors(&cube->sides[0].cells[2][0], &cube->sides[2].cells[0][0], 1);

    // link side[0] bottom-right
    link_neighbors(&cube->sides[0].cells[2][2], &cube->sides[2].cells[0][2], 1);
    link_neighbors(&cube->sides[0].cells[2][2], &cube->sides[4].cells[2][0], 1);

    // link side[5] upper-left
    link_neighbors(&cube->sides[5].cells[0][0], &cube->sides[4].cells[0][2], 1);
    link_neighbors(&cube->sides[5].cells[0][0], &cube->sides[1].cells[0][2], 1);

    // link side[5] upper-right
    link_neighbors(&cube->sides[5].cells[0][2], &cube->sides[3].cells[0][0], 1);
    link_neighbors(&cube->sides[5].cells[0][2], &cube->sides[1].cells[0][0], 1);

    // link side[5] bottom-left
    link_neighbors(&cube->sides[5].cells[2][0], &cube->sides[4].cells[2][2], 1);
    link_neighbors(&cube->sides[5].cells[2][0], &cube->sides[2].cells[2][2], 1);

    // link side[5] bottom-right
    link_neighbors(&cube->sides[5].cells[2][2], &cube->sides[3].cells[2][0], 1);
    link_neighbors(&cube->sides[5].cells[2][2], &cube->sides[2].cells[2][0], 1);
    //link_neighbors(&cube->sides[5].cells[2][2], &cube->sides[2].cells[2][2], 1);

    cube->start = &cube->sides[0].cells[0][0];

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
