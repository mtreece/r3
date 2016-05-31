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
#include <stdio.h>

#include <r3/r3_move.h>
#include <r3/r3_synclinks.h>
#include <r3/r3moves.h>

#include "r3cube.h"
#include "r3sides.h"

typedef struct {
    /// the r3cube object to operate on
    r3cube *cube;

    /// the next cell to return for get_next operation
    r3cell *cur_cell;

    /// the initial row for the side
    int row0;

    /// the initial col for the side
    int col0;

    /**
     * Side Row/Column Translators:
     *
     * This is a confusing concept, so bear with me.
     *
     * The r3 model fundamentally uses the orientation of the cube for how
     * sides and cells are structured. For every side but one (the rear face),
     * the row,col layout is symmetric; i.e, given any <row,col> pair, <r,c>,
     * when a selector moves/rotates that column (c) either up or down, the new
     * cell's <row,col> pair will be the same, _except_ for transactions that
     * involve a cell "from" or "to" the rear face.
     *
     * When picking the <row,col> dimensional layout, it was arbitrai... I
     * mean, intelligently decided the <row,col> meaning would be symmetric in
     * the left/right direction. By geometric nature, however, this has an
     * unfortunate consequence in how UP/DOWN translations occur.
     *
     * To put it another way...
     *
     * Imagine a normal Rubik's cube, but with faces labeled similar to the
     * layout described in /doc/cube-layout.txt. Imagine now that you're facing
     * the cube in some given orientation, and you decide to rotate the top row
     * to the right by a distance of 2 sides -- causing the front-facing cells
     * to relocate to the rear facing side. Focus on the upper-left cell. In
     * this scenario, the original <r,c> is <0,0>, and after relocation, it
     * will continue to be <0,0> (when "glancing" at the rear face).
     *
     * Now, instead of rotating twice "right", imagine you instead rotated the
     * first _column_ UP by one. Same situation: the upper-left cell retains
     * the <0,0> location.
     *
     * Now, instead of rotating UP only once, do it twice. The upper-left cell
     * of the front face will now relocate to the rear-side of the cube. But
     * that upper-left cell is now curiously at the bottom of the rear-side.
     * Indeed, on careful inspection, one will find that the new <r,c> of this
     * cell will be <2,2>. This very phenomenon is why the srt and sct mappings
     * exist.
     */

    /// Side Row Translator. See above for how this is used.
    int (*srt)(int);

    /// Side Column Translator. See above for how this is used.
    int (*sct)(int);
} ctx_t;

static r3cell *get_next_horiz(ctx_t *ctx)
{
    assert(ctx);
    r3cell *out = ctx->cur_cell;
    if (NULL == out) {
        return NULL;
    }

    int (*srt)(int) = ctx->srt;
    int (*sct)(int) = ctx->sct;

    int row = srt(ctx->cur_cell->row);
    int col = sct(ctx->cur_cell->col);
    int newcol = col + 1;

    row = srt(row);
    newcol = sct(newcol);

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

    int (*srt)(int) = ctx->srt;
    int (*sct)(int) = ctx->sct;

    int row = srt(ctx->cur_cell->row);
    int col = sct(ctx->cur_cell->col);
    int newrow = row + 1;

    col = sct(col);
    newrow = srt(newrow);

    if (0 > newrow || NUM_ROWS - 1 < newrow) {
        ctx->cur_cell = NULL;
    } else {
        ctx->cur_cell = ctx->cur_cell->side->cells[newrow][col];
    }

    return out;
}

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

#if !defined(NDEBUG)
    // ensure that a, b are really neighbors
    for (r3cell **n = a->neighbors; *n; ++n) {
        if (*n == b) {
            goto cont;
        }
    }
    assert(0);

cont:
#endif

    // (re-)write each of a's neighbors, sans b
    for (r3cell **n = a->neighbors; *n; ++n) {
        // if this neighbor isn't b, go ahead and proceed to (re-)write it;
        // otherwise, skip over
        if (*n != b) {
            *ptr++ = *n;
        }
    }

    // finally, cap it off with a NULL
    *ptr = NULL;

    // we recursively call ourself so-as to symmetrically handle both a, b; to
    // prevent an infinite loop, only call ourself if we're currently in the
    // first pass
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

static int r3_row_reverse(int r)
{
    return NUM_ROWS - 1 - r;
}

static int r3_col_reverse(int c)
{
    return NUM_COLS - 1 - c;
}

static int r3_row_direct(int r)
{
    return r;
}

static int r3_col_direct(int c)
{
    return c;
}

int r3_move(r3cube *cube, unsigned direction, unsigned selector)
{
    ctx_t ctx;                       // context
    int vertical;                    // direction vertical? else, horizontal
    int *sidelist;                   // ordered list of sides
    int nsides;                      // number of sides to iterate
    r3cell *(*get_next)(ctx_t *ctx); // iterator
    int (*parallel_cell)(r3cell*, r3cell*); // cells along the same line?
    int (**rowtrans)(int);           // row translator, per side
    int (**coltrans)(int);           // col translator, per side

    if (!cube) {
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
            rowtrans = (int (*[])(int)){r3_row_direct, r3_row_direct,
                                        r3_row_reverse, r3_row_direct,
                                        r3_row_direct, NULL};
            coltrans = (int (*[])(int)){r3_col_direct, r3_col_direct,
                                        r3_col_reverse, r3_col_direct,
                                        r3_col_direct, NULL};
            break;

        case R3_DOWN:
            vertical = 1;
            get_next = get_next_vert;
            parallel_cell = parallel_cell_vert;
            ctx.row0 = 0;
            ctx.col0 = selector;
            sidelist = (int []){0,2,5,1};
            nsides = 4;
            rowtrans = (int (*[])(int)){r3_row_direct, r3_row_direct,
                                        r3_row_reverse, r3_row_direct,
                                        r3_row_direct, NULL};
            coltrans = (int (*[])(int)){r3_col_direct, r3_col_direct,
                                        r3_col_reverse, r3_col_direct,
                                        r3_col_direct, NULL};
            break;

        case R3_LEFT:
            vertical = 0;
            get_next = get_next_horiz;
            parallel_cell = parallel_cell_horiz;
            ctx.row0 = selector;
            ctx.col0 = 0;
            sidelist = (int []){0,3,5,4};
            nsides = 4;
            rowtrans = (int (*[])(int)){r3_row_direct, r3_row_direct,
                                        r3_row_direct, r3_row_direct,
                                        r3_row_direct, NULL};
            coltrans = (int (*[])(int)){r3_col_direct, r3_col_direct,
                                        r3_col_direct, r3_col_direct,
                                        r3_col_direct, NULL};
            break;

        case R3_RIGHT:
            vertical = 0;
            get_next = get_next_horiz;
            parallel_cell = parallel_cell_horiz;
            ctx.row0 = selector;
            ctx.col0 = 0;
            sidelist = (int []){0,4,5,3};
            nsides = 4;
            rowtrans = (int (*[])(int)){r3_row_direct, r3_row_direct,
                                        r3_row_direct, r3_row_direct,
                                        r3_row_direct, NULL};
            coltrans = (int (*[])(int)){r3_col_direct, r3_col_direct,
                                        r3_col_direct, r3_col_direct,
                                        r3_col_direct, NULL};
            break;

        default:
            return -1;
    }

    if (selector > (vertical ? NUM_COLS : NUM_ROWS) - 1) {
        return -1;
    }

    /**
     * This structure notes a "todo"-like record for how to either link or
     * unlink a pair of r3cell structs.
     */
    typedef struct {
        r3cell *a;
        r3cell *b;
    } link_record;

    link_record links[NUM_SIDES * MAX_ROW_COLS * MAX_NUM_NEIGHBORS];
    link_record unlinks[NUM_SIDES * MAX_ROW_COLS * MAX_NUM_NEIGHBORS];

    link_record *linkptr = links;
    link_record *unlinkptr = unlinks;

    r3side *sides[NUM_SIDES_RING + 1 + 1]; // +1 loop-around, +1 NULL
    for (int i = 0; i < nsides; ++i) {
        sides[i] = &cube->sides[sidelist[i]];
    }
    sides[nsides] = &cube->sides[0];
    sides[nsides + 1] = NULL;

    int (**srtptr)(int) = rowtrans;
    int (**sctptr)(int) = coltrans;

    for (r3side **s = sides; *(s+1); ++s) {
        r3cell *c;

        // s row,col translate
        int (*srt)(int) = *srtptr++;
        int (*sct)(int) = *sctptr++;

        /*
         * NOTE: s1ct may not be needed for translation. Current unit tests
         * continue to pass (several thousand pseudorandom iterations) with
         * s1ct removed.
         *
         * Need to continue investigating this and logically reason about why
         * it isn't needed...
         *
         * It may be a special case for 6x3x3 that's allowing this to pass
         * without issue. If future unit tests that better / more extensively
         * test expected state (vs simple assertions) appear to fail, consider
         * reverting this patch to see if it helps in any way.
         *
         * TODO: investigate & either remove this TODO or fix the issue.
         *
         */

        // s1 row translate
        int (*s1rt)(int) = *srtptr;

        ctx.srt = srt;
        ctx.sct = srt;
        ctx.cur_cell = (*s)->cells[srt(ctx.row0)][sct(ctx.col0)];
        while ((c = get_next(&ctx))) {
            assert(c->side == *s);

            r3cell *cn = (*(s+1))->cells[s1rt(srt(c->row))][c->col];

            // record old, non-parallel neighbors
            for (r3cell **n = c->neighbors; *n; ++n) {
                if (!parallel_cell(c, *n)) {
                    unlinkptr->a = c;
                    unlinkptr->b = *n;
                    ++unlinkptr;
                }
            }

            // record new, non-parallel neighbors
            for (r3cell **n = cn->neighbors; *n; ++n) {
                if (!parallel_cell(cn, *n)) {
                    linkptr->a = c;
                    linkptr->b = *n;
                    ++linkptr;
                }
            }

            // TODO: optimize! Shouldn't do this check each iteration...
            // Not updating to use s*t() functions here for performance
            // reasons; we already know neither c, nor cn will be the last face
            if (&cube->sides[0] == cn->side) {
                if (0 == cn->row && (0 == cn->col || 1 == cn->col)) {
                    c->row = cn->row;
                    c->col = cn->col;
                    c->side = cn->side;
                    cn->side->cells[c->row][c->col] = c;
                }
            }
        }
    }

    for (link_record *r = unlinks; r < unlinkptr; ++r) {
        if (!unlink_cell(r->a, r->b, 0)) {
            // shouldn't fail
            assert(0);
        }
    }

    for (link_record *r = links; r < linkptr; ++r) {
        if (!link_cell(r->a, r->b, 0)) {
            // shouldn't fail
            assert(0);
        }
    }

    int res = r3_synclinks(cube);

#if !defined(NDEBUG)
    _r3_cube_check_integrity(cube);
#endif

    return res;
}

// vim: et ts=4 sw=4 :
