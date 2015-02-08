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
#define ADJSIZE (sizeof(((r3cell *)0)->neighbors))

typedef struct {
    /// the r3cube object to operate on
    r3cube *cube;

    /// the next cell to return for get_next operation
    r3cell *cur_cell;

    /// the initial row for the side
    int row0;

    /// the initial col for the side
    int col0;
} ctx_t;

static r3cell *get_next_horiz(ctx_t *ctx)
{
    assert(ctx);
    r3cell *out = ctx->cur_cell;
    if (NULL == out) {
        return NULL;
    }

    int row = ctx->cur_cell->row;
    int col = ctx->cur_cell->col;
    int newcol = col + 1;

    if (0 > newcol || NUM_COLS - 1 < newcol) {
        ctx->cur_cell = NULL;
    } else {
        ctx->cur_cell = ctx->cur_cell->side->cells[row][newcol];
    }

    return out;
}

static r3cell *get_next_vert(ctx_t *ctx)
{
    assert(ctx);
    r3cell *out = ctx->cur_cell;
    if (NULL == out) {
        return NULL;
    }

    int row = ctx->cur_cell->row;
    int col = ctx->cur_cell->col;
    int newrow = row + 1;

    if (0 > newrow || NUM_ROWS - 1 < newrow) {
        ctx->cur_cell = NULL;
    } else {
        ctx->cur_cell = ctx->cur_cell->side->cells[newrow][col];
    }

    return out;
}

#if 0
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
#endif /* 0 */

static int parallel_cell_horiz(r3cell *c1, r3cell *c2)
{
#if !defined(NDEBUG)
    for (r3cell **n = c1->neighbors; *n; ++n) {
        if (*n == c2) {
            goto cont;
        }
    }
    assert(0);

cont:
#endif

    // assume c1 & c2 are actually neighbors
    return c1->row == c2->row;
}

static int parallel_cell_vert(r3cell *c1, r3cell *c2)
{
#if !defined(NDEBUG)
    for (r3cell **n = c1->neighbors; *n; ++n) {
        if (*n == c2) {
            goto cont;
        }
    }
    assert(0);

cont:
#endif

    // assume c1 & c2 are actually neighbors
    return c1->col == c2->col;
}

// TODO: factor this *link_cell functionality into a common location; see
// r3_init::link_neighbors
static int unlink_cell(r3cell *a, r3cell *b, int iteration)
{
    r3cell **ptr = a->neighbors;
    for (r3cell **n = a->neighbors; *n; ++n) {
        if (*n != b) {
            *ptr++ = *n;
        }
    }

    // finally, cap it off with a NULL
    *ptr = NULL;

    if (0 == iteration) {
        if (!unlink_cell(b, a, 1)) {
            return 0;
        }
    }

    return 1;
}
static int link_cell(r3cell *a, r3cell *b, int iteration)
{
    r3cell **ptr = a->neighbors;

    // find our first free spot
    while (*++ptr);

    // space available to link the cell?
    if (NULL != ptr[1]) {
        return 0;
    }

    // link the cell from this direction...
    *ptr = b;

    // now do the other direction, if need be
    if (0 == iteration) {
        if (!link_cell(b, a, 1)) {
            return 0;
        }
    }

    return 1;
}

int r3_move(r3cube *cube, int direction, int selector)
{
    ctx_t ctx;                       // context
    int vertical;                    // direction vertical? else, horizontal
    int *sidelist;                   // ordered list of sides
    int nsides;                      // number of sides to iterate
    r3cell *(*get_next)(ctx_t *ctx); // iterator
    int (*parallel_cell)(r3cell*, r3cell*); // cells along the same line?

    if (!cube || 0 > selector) {
        return -1;
    }

    ctx.cube = cube;
    switch (direction) {
        case R3_UP:
            vertical = 1;
            get_next = get_next_vert;
            parallel_cell = parallel_cell_vert;
            ctx.row0 = 0;
            ctx.col0 = selector;
            sidelist = (int []){0,1,5,2};
            nsides = 4;
            break;
        case R3_DOWN:
            vertical = 1;
            get_next = get_next_vert;
            parallel_cell = parallel_cell_vert;
            ctx.row0 = 0;
            ctx.col0 = selector;
            sidelist = (int []){0,2,5,1};
            nsides = 4;
            break;

        case R3_LEFT:
            vertical = 0;
            get_next = get_next_horiz;
            parallel_cell = parallel_cell_horiz;
            ctx.row0 = selector;
            ctx.col0 = 0;
            sidelist = (int []){0,3,5,4};
            nsides = 4;
            break;
        case R3_RIGHT:
            vertical = 0;
            get_next = get_next_horiz;
            parallel_cell = parallel_cell_horiz;
            ctx.row0 = selector;
            ctx.col0 = 0;
            sidelist = (int []){0,4,5,3};
            nsides = 4;
            break;
        default:
            return -1;
    }

    if (selector > (vertical ? NUM_COLS : NUM_ROWS) - 1) {
        return -1;
    }

    /* TODO: make more efficient.
     * This is unneeded overhead, but simplifies the algorithm.
     */
    r3side side0;
    memcpy(&side0, &cube->sides[0], sizeof(side0));
    r3side *sides[NUM_SIDES + 1];
    for (int i = 0; i < nsides; ++i) {
        sides[i] = &cube->sides[sidelist[i]];
    }
    sides[nsides] = &side0;
    sides[nsides + 1] = NULL;

    for (r3side **s = sides; *(s+1); ++s) {
        ctx.cur_cell = (*s)->cells[ctx.row0][ctx.col0];
        r3cell *c;
        while ((c = get_next(&ctx))) {
            assert(c->side == *s);
            r3cell *cn = (*(s+1))->cells[c->row][c->col];

            // unlink old, non-parallel neighbors
            for (r3cell **n = c->neighbors; *n; ++n) {
                if (!parallel_cell(c, *n)) {
                    // <c, *n> are adj, but not along direction vector; need to
                    // remove this link
                    if (!unlink_cell(c, *n, 0)) {
                        // shouldn't fail
                        assert(0);
                    }
                }
            }

            // link with new, non-parallel neighbors
            for (r3cell **n = cn->neighbors; *n; ++n) {
                if (!parallel_cell(cn, *n)) {
                    // <cn, *n> are adj, but not along direction vector; need to
                    // link this with its new neighbor
                    if (!link_cell(c, *n, 0)) {
                        // shouldn't fail
                        assert(0);
                    }
                }
            }

            // TODO: optimize! Shouldn't do this check each iteration...
            if (0 == cn->side) {
                if (0 == cn->row && (0 == cn->col || 1 == cn->col)) {
                    c->row = cn->row;
                    c->col = cn->col;
                    c->side = cn->side;
                    cn->side->cells[c->row][c->col] = c;
                }
            }
        }
    }

    return r3_synclinks(cube);
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
