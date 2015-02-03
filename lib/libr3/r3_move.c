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
#include <stdio.h>

#include "r3cube.h"
#include "r3sides.h"
#include "r3moves.h"
#include "r3_move.h"
#include "r3_synclinks.h"

#define VERTICAL_VECTOR_OF_FACES ((int []){0, 1, 5, 2})
#define HORIZONTAL_VECTOR_OF_FACES ((int []){0, 4, 5, 3})

typedef struct {
    /// the r3cube object to operate on
    r3cube *cube;

    /// the amount (+/- 1) to increment
    int increment;

    /// the next cell to return for get_next operation
    r3cell *cur_cell;
} ctx_t;

static r3cell *get_next_horiz(ctx_t *ctx)
{
    assert(ctx);
    r3cell *out = ctx->cur_cell;
    return out;
}

static r3cell *get_next_vert(ctx_t *ctx)
{
    if (!ctx) {
        return NULL;
    }
    return NULL;
}

// TODO: optimize this; should store off this information with cells
static int get_side(r3cube *cube, r3cell *c)
{
    for (int i = 0; i < NUM_SIDES; ++i) {
        if (&cube->sides[i] == c->side) {
            return i;
        }
    }

    assert(0);
    return -1;
}

int r3_move(r3cube *cube, int direction, int selector)
{
    ctx_t ctx;                       // context
    int vertical;                    // direction vertical? else, horizontal
    unsigned int iterations;         // number of r3cells to traverse
    int *sidelist;                   // ordered list of sides
    int nsides;                      // number of sides to iterate
    r3cell *(*get_next)(ctx_t *ctx); // iterator

    if (!cube || 0 > selector) {
        return -1;
    }

    ctx.cube = cube;
    switch (direction) {
        case R3_UP:
            vertical = 1;
            iterations = NUM_SIDES * NUM_ROWS;
            get_next = get_next_vert;
            ctx.increment = -1;
            ctx.cur_cell = cube->sides[0].cells[0][selector];
            sidelist = (int []){0,1,5,2};
            nsides = 4;
            break;
        case R3_DOWN:
            vertical = 1;
            iterations = NUM_SIDES * NUM_ROWS;
            get_next = get_next_vert;
            ctx.increment = 1;
            ctx.cur_cell = cube->sides[0].cells[0][selector];
            sidelist = (int []){0,2,5,1};
            nsides = 4;
            break;

        case R3_LEFT:
            vertical = 0;
            iterations = NUM_SIDES * NUM_COLS;
            get_next = get_next_horiz;
            ctx.increment = -1;
            ctx.cur_cell = cube->sides[0].cells[selector][0];
            sidelist = (int []){0,3,5,4};
            nsides = 4;
            break;
        case R3_RIGHT:
            vertical = 0;
            iterations = NUM_SIDES * NUM_COLS;
            get_next = get_next_horiz;
            ctx.increment = 1;
            ctx.cur_cell = cube->sides[0].cells[selector][0];
            sidelist = (int []){0,4,5,3};
            nsides = 4;
            break;
        default:
            return -1;
    }

    if (selector > (vertical ? NUM_COLS : NUM_ROWS) - 1) {
        return -1;
    }

    for (int i = 0; i < nsides; ++i) {
        r3cell *c = get_next(&ctx);
        int row = c->row;
        int col = c->col;
        int side = get_side(cube, c);
        int nside = sidelist[++i];
        r3cell *cn = cube->sides[nside].cells[row][col];

        if(!row){return -1;}
        if(!col){return -1;}
        if(!side){ return -1; }
        if(!nside){return -1;}
        if(!cn){ return -1; }
        if(!iterations){ return -1; }
    }

#if 0
    for (unsigned int i = 0; i < iterations; ++i) {
        r3cell *c;

    }
#endif

    r3_synclinks(cube);

    return 0;
}

int xr3_move(r3cube *cube, int direction, int selector)
{
    int vertical;                       // direction is vertical? else, horizontal
    r3cell cellbuffer[MAX_ROW_COLS];    // buffer for moving cells during operation
    int *rotvector;                     // rotation vector of face-idx
    int n_rotvector;                    // length of rotvector

    if (!cube) {
        return -1;
    }

    if (0 > selector) {
        return -1;
    }

    switch (direction) {
        case R3_UP:
        case R3_DOWN:
            vertical = 1;
            break;
        case R3_LEFT:
        case R3_RIGHT:
            vertical = 0;
            break;
        default:
            return -1;
    }

    if (selector > (vertical ? NUM_COLS : NUM_ROWS) - 1) {
        return -1;
    }

    if (vertical) {
        // these are the face-indices & order that are hit in vertical case
        rotvector = VERTICAL_VECTOR_OF_FACES;
        n_rotvector = sizeof(VERTICAL_VECTOR_OF_FACES) / sizeof(int);
    } else {
        // these are the face-indices & order that are hit in horizontal case
        rotvector = HORIZONTAL_VECTOR_OF_FACES;
        n_rotvector = sizeof(HORIZONTAL_VECTOR_OF_FACES) / sizeof(int);
    }

    for (int i = n_rotvector - 1; i >= 0; --i) {
        for (int j = 0; j < (vertical ? NUM_COLS : NUM_ROWS); ++j) {
            void *dest;
            void *src;
            if (vertical) {
                if (i == n_rotvector - 1) {
                    // first case, special, backup to temporary buffer
                    dest = &cellbuffer[j];
                    src = &cube->sides[rotvector[i]].cells[j][selector];
                } else if (i == 0) {
                    // last case, special, restore from temporary buffer
                    dest = &cube->sides[rotvector[i]].cells[j][selector];
                    src = &cellbuffer[j];
                } else {
                    dest = &cube->sides[i].cells[j][selector];
                    src = &cube->sides[rotvector[i - 1]].cells[j][selector];
                }
            } else {
                if (i == n_rotvector - 1) {
                    // first case, special, backup to temporary buffer
                    dest = &cellbuffer[j];
                    src = &cube->sides[rotvector[i]].cells[selector][j];
                } else if (i == 0) {
                    // last case, special, restore from temporary buffer
                    dest = &cube->sides[rotvector[i]].cells[selector][j];
                    src = &cellbuffer[j];
                } else {
                    dest = &cube->sides[i].cells[selector][j];
                    src = &cube->sides[rotvector[i - 1]].cells[selector][j];
                }
            }
            memcpy(dest, src, sizeof(r3cell));
        }
    }

    return 0;
}

// vim: et ts=4 sw=4 :
