/*
 * Copyright (C) 2014-2016 Michael T. Reece.
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

    // transitive property
    if (is_brother) {
        // iterate over a brothers
        for (r3cell **bro = &a->brothers[0]; *bro; ++bro) {
            // ignore the one we just added
            if (*bro == b) {
                continue;
            }

            // ignore if already a known brother
            int already_known = 0;
            for (r3cell **knownbro = &b->brothers[0]; *knownbro; ++knownbro) {
                if (*knownbro == *bro) {
                    already_known = 1;
                    break;
                }
            }

            if (already_known) {
                continue;
            }

            // link b with the found brother
            link_neighbors(b, *bro, 1);
        }
    }
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
    int celln = 0;

    assert(NUM_SIDES == sizeof(colors)/sizeof(int));

    for(int i = 0; i < NUM_SIDES; ++i)
    {
        for(int j = 0; j < NUM_ROWS; ++j)
        {
            for(int k = 0; k < NUM_COLS; ++k)
            {
                r3cell **c = &cube->sides[i].cells[j][k];
                *c = &cube->cellspace[celln++];

                (*c)->color = colors[i];
                (*c)->side = &cube->sides[i];
                (*c)->row = j;
                (*c)->col = k;
                memset((*c)->brothers, 0x00, sizeof((*c)->brothers));
                memset((*c)->neighbors, 0x00, sizeof((*c)->neighbors));
            }
        }
    }

    // link corner-brothers ////////////////////////////////////////////////////
    // link side[0] upper-left
    link_neighbors(cube->sides[0].cells[0][0], cube->sides[3].cells[0][2], 1);
    link_neighbors(cube->sides[0].cells[0][0], cube->sides[1].cells[2][0], 1);

    // link side[0] upper-right
    link_neighbors(cube->sides[0].cells[0][2], cube->sides[4].cells[0][0], 1);
    link_neighbors(cube->sides[0].cells[0][2], cube->sides[1].cells[2][2], 1);

    // link side[0] bottom-left
    link_neighbors(cube->sides[0].cells[2][0], cube->sides[3].cells[2][2], 1);
    link_neighbors(cube->sides[0].cells[2][0], cube->sides[2].cells[0][0], 1);

    // link side[0] bottom-right
    link_neighbors(cube->sides[0].cells[2][2], cube->sides[2].cells[0][2], 1);
    link_neighbors(cube->sides[0].cells[2][2], cube->sides[4].cells[2][0], 1);

    // link side[5] upper-left
    link_neighbors(cube->sides[5].cells[0][0], cube->sides[4].cells[0][2], 1);
    link_neighbors(cube->sides[5].cells[0][0], cube->sides[1].cells[0][2], 1);

    // link side[5] upper-right
    link_neighbors(cube->sides[5].cells[0][2], cube->sides[3].cells[0][0], 1);
    link_neighbors(cube->sides[5].cells[0][2], cube->sides[1].cells[0][0], 1);

    // link side[5] bottom-left
    link_neighbors(cube->sides[5].cells[2][0], cube->sides[4].cells[2][2], 1);
    link_neighbors(cube->sides[5].cells[2][0], cube->sides[2].cells[2][2], 1);

    // link side[5] bottom-right
    link_neighbors(cube->sides[5].cells[2][2], cube->sides[3].cells[2][0], 1);
    link_neighbors(cube->sides[5].cells[2][2], cube->sides[2].cells[2][0], 1);

    // link non-corner brothers ////////////////////////////////////////////////
    // link side[0] non-corners
    link_neighbors(cube->sides[0].cells[1][0], cube->sides[3].cells[1][2], 1);
    link_neighbors(cube->sides[0].cells[0][1], cube->sides[1].cells[2][1], 1);
    link_neighbors(cube->sides[0].cells[1][2], cube->sides[4].cells[1][0], 1);
    link_neighbors(cube->sides[0].cells[2][1], cube->sides[2].cells[0][1], 1);

    // link side[1] non-corners
    link_neighbors(cube->sides[1].cells[1][0], cube->sides[3].cells[0][1], 1);
    link_neighbors(cube->sides[1].cells[0][1], cube->sides[5].cells[0][1], 1);
    link_neighbors(cube->sides[1].cells[1][2], cube->sides[4].cells[0][1], 1);

    // link side[2] non-corners
    link_neighbors(cube->sides[2].cells[1][0], cube->sides[3].cells[2][1], 1);
    link_neighbors(cube->sides[2].cells[1][2], cube->sides[4].cells[2][1], 1);
    link_neighbors(cube->sides[2].cells[2][1], cube->sides[5].cells[2][1], 1);

    // link side[3] non-corners
    link_neighbors(cube->sides[3].cells[1][0], cube->sides[5].cells[1][2], 1);

    // link side[4] non-corners
    link_neighbors(cube->sides[4].cells[1][2], cube->sides[5].cells[1][0], 1);

    // link side[5] non-corners
    /* none - all previous links, via transitive property, covered side[5] */

    // link non-brother neighbors //////////////////////////////////////////////
    for (int s = 0; s < NUM_SIDES; ++s) {
        r3side *side = &cube->sides[s];

        // link across cols for each row
        for (int r = 0; r < NUM_ROWS; ++r) {
            for (int c = 0; c < NUM_COLS - 1; ++c) {
                link_neighbors(side->cells[r][c], side->cells[r][c + 1], 0);
            }
        }

        // link across rows for each col
        for (int c = 0; c < NUM_COLS; ++c) {
            for (int r = 0; r < NUM_ROWS - 1; ++r) {
                link_neighbors(side->cells[r][c], side->cells[r + 1][c], 0);
            }
        }
    }

    cube->facing = &cube->sides[0];
    cube->anchors[0] = cube->facing->cells[0][0];
    cube->anchors[1] = cube->facing->cells[0][1];

    return 0;
}

// vim: et ts=4 sw=4 :
