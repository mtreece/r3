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
#include <stdlib.h>
#include <string.h>

#include "r3cube.h"
#include "r3_synclinks.h"

#define abs(x) (x >= 0 ? x : -x)

/**
 * @brief
 * Given the previous two anchors, find the next two anchors along the same
 * vector path.
 *
 * @param[in] a Array of four r3cell* - [0,1] = input two anchors, [2,3] =
 *            input two anchors prior to that, alongside the same axis of
 *            discovery
 * @param[out] n Location to store array of two r3cell*, the output anchors
 *
 * @return nonzero if next anchors found
 * @return zero if next anchors not found (i.e. hit side barrier)
 */
static int next_anch(r3cell *a[], r3cell *n[])
{
    r3cell *a1 = a[0];   // first known anchor
    r3cell *a2 = a[1];   // second known anchor
    r3cell *p1 = a[2];   // first known previous anchor
    r3cell *p2 = a[3];   // second known previous anchor
    r3cell **o1 = &n[0]; // output for 1st new anchor
    r3cell **o2 = &n[1]; // output for 2nd new anchor
    int pair_found;      // boolean flag if next pair was found

    for (r3cell **c = a1->neighbors; *c; ++c) {
        if (a2 == *c || p1 == *c || p2 == *c) {
            // if either adj neighbor or prev 2 anchors, skip over
            continue;
        }

        // iterate over this cell's neighbor
        for (r3cell **cc = (*c)->neighbors; *cc; ++cc) {
            if (*cc == a1) {
                // ignore the neighbor we just came from
                continue;
            }

            // for each of the 2nd anchor's neighbors...
            for (r3cell **d = a2->neighbors; *d; ++d) {
                // discovered the two next anchors...
                if (*d == *cc) {
                    *o1 = *c;
                    *o2 = *d;
                    pair_found = 1;
                    goto done;
                }
            }
        }
    }

    pair_found = 0;

done:
    return pair_found;
}

/**
 * @brief
 * Given 4 adj r3cell structs, rotate their vector-direction.
 *
 * @param[in] clockwise if true, clockwise, else counter-clockwise
 * @param[in,out] a Array of the 4 r3cell pointers; re-arrange on output
 */
static void rotateblock(int clockwise, r3cell *a[])
{
    r3cell *old[4];

    memcpy(old, a, sizeof(old));
    a[0] = old[clockwise ? 2 : 1];
    a[1] = old[clockwise ? 0 : 3];
    a[2] = old[clockwise ? 3 : 0];
    a[3] = old[clockwise ? 1 : 2];
}

/**
 * @brief
 * Given the 4 old anchors (for direction) & 2 new anchors, place the 2 new
 * anchors on the given r3side.
 *
 * @param[in] side The r3side these cells will reside on
 * @param[in] o Array of 4 r3cell*, from the old anchors
 * @param[in] n Array of 2 new r3cell*
 */
static void update_grid(r3side *side, r3cell *o[], r3cell *n[])
{
    int o2row, o2col;
    int o3row, o3col;

    // fudge the numbers if we have NULL old values
    if (NULL == o[2]) {
        assert(NULL == o[3]);
        if (0 == o[0]->row && 0 == o[1]->row) {
            o2row = -1;
            o3row = -1;
            o2col = o[0]->col;
            o3col = o[1]->col;
        } else if ((NUM_ROWS - 1 == o[0]->row)
                && (NUM_ROWS - 1 == o[1]->row)) {
            o2row = NUM_ROWS;
            o2col = o[0]->col;
            o3row = NUM_ROWS;
            o3col = o[1]->col;
        } else if (0 == o[0]->col && 0 == o[1]->col) {
            o2row = o[0]->row;
            o2col = -1;
            o3row = o[1]->row;
            o3col = -1;
        } else if ((NUM_COLS - 1 == o[0]->col)
                && (NUM_COLS - 1 == o[1]->col)) {
            o2row = o[0]->row;
            o2col = NUM_COLS;
            o3row = o[1]->row;
            o3col = NUM_COLS;
        } else {
            assert(0);

            // return to make the compiler happy in non-assert builds
            return;
        }
    } else {
        o2row = o[2]->row;
        o2col = o[2]->col;
        o3row = o[3]->row;
        o3col = o[3]->col;
    }

    if (o[0]->row == o2row) {
        assert(o[1]->row == o3row);
        n[0]->row = o[0]->row;
        n[1]->row = o[1]->row;

        n[0]->col = o[0]->col + (o[0]->col - o2col);
        n[1]->col = o[1]->col + (o[1]->col - o3col);
    } else {
        assert(o[0]->col == o2col);
        assert(o[1]->col == o3col);

        n[0]->col = o[0]->col;
        n[1]->col = o[1]->col;

        n[0]->row = o[0]->row + (o[0]->row - o2row);
        n[1]->row = o[1]->row + (o[1]->row - o3row);
    }

    side->cells[n[0]->row][n[0]->col] = n[0];
    side->cells[n[1]->row][n[1]->col] = n[1];
}

/**
 * @brief
 * Given two up/down, left/right adjacent edge r3cell structs, that are
 * properly set on the grid, reconstruct the entire r3side. Note, the input
 * cells must fit the following criteria:
 *   1) adjacent by row or col
 *   2) one cell is a corner cell
 *   3) the c* order is important; they must be passed in such that c1 is to
 *      the left of c2, when oriented in such a way that advancement is
 *      downward. If you don't know what this means, compile with assertions
 *      enabled, and it will crash if you have them backwards.
 *
 * @param[in] side The r3side this operation is reconstructing.
 * @param[in] c1 The 1st, good r3cell
 * @param[in] c2 the 2nd, good r3cell
 *
 * @retval 0 success
 */
static int syncside(r3side *side, r3cell *c1, r3cell *c2)
{
    r3cell *oanchors[4]; // old anchors
    r3cell *nanchors[2]; // new anchors

    oanchors[0] = c1;
    oanchors[1] = c2;
    oanchors[2] = NULL;
    oanchors[3] = NULL;

    int clockwise; // counter/clockwise alternator flag

    // assert the cells are adjacent
    assert(1 >= abs(c1->row - c2->row) && 1 >= abs(c1->col - c2->col));

    if (0 == c1->row && 0 == c2->row) {
        // assert we're on the edge
        assert(0 == c1->col || 0 == c2->col
                || NUM_COLS - 1 == c1->col || NUM_COLS - 1 == c2->col);

        // if the two anchors are upper-right, they will move down, and then
        // need to turn clockwise
        clockwise = (NUM_COLS - 1 == c1->col) || (NUM_COLS - 1 == c2->col);

        // assert params in necessary order (TODO: generalize the algorithm)
        assert(c1->col < c2->col);
    } else if (0 == c1->col && 0 == c2->col) {
        // assert we're on the edge
        assert(0 == c1->row || 0 == c2->row
                || NUM_ROWS - 1 == c1->row || NUM_ROWS - 1 == c2->row);

        // if the two anchors are upper-left, they will move right, and then
        // need to turn clockwise
        clockwise = (0 == c1->row) || (0 == c2->row);

        // assert params in necessary order (TODO: generalize the algorithm)
        assert(c1->row > c2->row);
    } else if (NUM_ROWS - 1 == c1->row && NUM_ROWS - 1 == c2->row) {
        // assert we're on the edge
        assert(0 == c1->col || 0 == c2->col
                || NUM_COLS - 1 == c1->col || NUM_COLS - 1 == c2->col);

        // if the anchors are lower-left, they will move up, & then need to
        // turn clockwise
        clockwise = (0 == c1->col) || (0 == c2->col);

        // assert params in necessary order (TODO: generalize the algorithm)
        assert(c1->col > c2->col);
    } else if (NUM_COLS - 1 == c1->col && NUM_COLS - 1 == c2->col) {
        // assert we're on the edge
        assert(0 == c1->row || 0 == c2->row
                || NUM_ROWS - 1 == c1->row || NUM_ROWS - 1 == c2->row);

        // if the two anchors are lower-right, they will move left, and then
        // need to turn clockwise
        clockwise = (NUM_ROWS - 1 == c1->row) || (NUM_ROWS - 1 == c2->row);

        // assert params in necessary order (TODO: generalize the algorithm)
        assert(c1->row < c2->row);
    } else {
        // otherwise, invalid input
        assert(0);

        // return in error to make the compiler happy in non-assert builds
        return -1;
    }

    while (1) {
        if (!next_anch(oanchors, nanchors)) {
            // re-orient perpendicular
            rotateblock(clockwise, oanchors);
            if (!next_anch(oanchors, nanchors)) {
                // if we have NULL results after the turn, we have finished
                // traversing the grid; this is our halting condition
                break;
            }
            assert(nanchors[0]);
            assert(nanchors[1]);

            update_grid(side, oanchors, nanchors);

            oanchors[3] = oanchors[1];
            oanchors[2] = oanchors[0];
            oanchors[1] = nanchors[1];
            oanchors[0] = nanchors[0];

            // rotate again for zig-zag motion
            rotateblock(clockwise, oanchors);

            // now, reset direction
            clockwise = !clockwise;

            // populate nanchors for fall-through to rest of function
            if (!next_anch(oanchors, nanchors)) {
                // we oughtn't reach this
                assert(0);
            }
        }
        assert(nanchors[0]);
        assert(nanchors[1]);

        // add these two new anchors into the grid
        update_grid(side, oanchors, nanchors);

        // reset for next iteration
        oanchors[3] = oanchors[1];
        oanchors[2] = oanchors[0];
        oanchors[1] = nanchors[1];
        oanchors[0] = nanchors[0];
    }

    return 0;
}

int r3_synclinks(r3cube *cube)
{
    r3side *side;
    r3cell *(*c)[NUM_COLS];
    r3cell **b1, **b2;
    r3cell /* **n1, */ **n2;

    if (!cube) {
        return -1;
    }

    // invalidate all the cells in the cube
    for (unsigned int i = 0; i < sizeof(cube->cellspace) / sizeof(r3cell); ++i) {
        cube->cellspace[i].dirty = 1;
    }

    // start with the anchors on sides[0]; these should be synced already on
    // any operation
    side = &cube->sides[0];
    c = side->cells;
    c[0][0]->dirty = 0;
    c[0][1]->dirty = 0;
    syncside(side, c[0][0], c[0][1]);

    // now tackle side[1]
    // strategy: find sides[1].cells[2][0..1] by looking at
    // sides[0].cells[0][0..1] brothers & the brothers' neighbors
    for (b1 = c[0][0]->brothers; *b1; ++b1) {
        for (b2 = c[0][1]->brothers; *b2; ++b2) {
            for (n2 = (*b2)->neighbors; *n2; ++n2) {
                if (*b1 == *n2) {
                    // assign newly-found 1st anchor for this side
                    cube->sides[1].cells[2][0] = *b1;
                    (*b1)->row = 2;
                    (*b1)->col = 0;
                    (*b1)->side = &cube->sides[1];

                    // assign newly-found 2nd anchor for this side
                    cube->sides[1].cells[2][1] = *b2;
                    (*b2)->row = 2;
                    (*b2)->col = 1;
                    (*b2)->side = &cube->sides[1];
                    goto eloop1;
                }
            }
        }
    }
    /* should always skip over this ... */
    assert(0);

    /* return in error to make the compiler happy in non-assert builds */
    return -1;
eloop1:
    syncside(&cube->sides[1], *b2, *b1);

    // now tackle side[2]
    for (b1 = c[2][0]->brothers; *b1; ++b1) {
        for (b2 = c[2][1]->brothers; *b2; ++b2) {
            for (n2 = (*b2)->neighbors; *n2; ++n2) {
                if (*b1 == *n2) {
                    // assign newly-found 1st anchor for this side
                    cube->sides[2].cells[0][0] = *b1;
                    (*b1)->row = 0;
                    (*b1)->col = 0;
                    (*b1)->side = &cube->sides[2];

                    // assign newly-found 2nd anchor for this side
                    cube->sides[2].cells[0][1] = *b2;
                    (*b2)->row = 0;
                    (*b2)->col = 1;
                    (*b2)->side = &cube->sides[2];
                    goto eloop2;
                }
            }
        }
    }
    /* should always skip over this ... */
    assert(0);

    /* return in error to make the compiler happy in non-assert builds */
    return -1;
eloop2:
    syncside(&cube->sides[2], *b1, *b2);

    // now tackle side[3]
    for (b1 = c[0][0]->brothers; *b1; ++b1) {
        for (b2 = c[1][0]->brothers; *b2; ++b2) {
            for (n2 = (*b2)->neighbors; *n2; ++n2) {
                if (*b1 == *n2) {
                    // assign newly-found 1st anchor for this side
                    cube->sides[3].cells[0][2] = *b1;
                    (*b1)->row = 0;
                    (*b1)->col = 2;
                    (*b1)->side = &cube->sides[3];

                    // assign newly-found 2nd anchor for this side
                    cube->sides[3].cells[1][2] = *b2;
                    (*b2)->row = 1;
                    (*b2)->col = 2;
                    (*b2)->side = &cube->sides[3];
                    goto eloop3;
                }
            }
        }
    }
    /* should always skip over this ... */
    assert(0);

    /* return in error to make the compiler happy in non-assert builds */
    return -1;
eloop3:
    syncside(&cube->sides[3], *b1, *b2);

    // now tackle side[4]
    for (b1 = c[0][2]->brothers; *b1; ++b1) {
        for (b2 = c[1][2]->brothers; *b2; ++b2) {
            for (n2 = (*b2)->neighbors; *n2; ++n2) {
                if (*b1 == *n2) {
                    // assign newly-found 1st anchor for this side
                    cube->sides[4].cells[0][0] = *b1;
                    (*b1)->row = 0;
                    (*b1)->col = 0;
                    (*b1)->side = &cube->sides[4];

                    // assign newly-found 2nd anchor for this side
                    cube->sides[4].cells[1][0] = *b2;
                    (*b2)->row = 1;
                    (*b2)->col = 0;
                    (*b2)->side = &cube->sides[4];
                    goto eloop4;
                }
            }
        }
    }
    /* should always skip over this ... */
    assert(0);

    /* return in error to make the compiler happy in non-assert builds */
    return -1;
eloop4:
    syncside(&cube->sides[4], *b2, *b1);

    // now tackle side[5]
    // reposition `side`, `c` to side[4] as a known-good starting point
    side = &cube->sides[4];
    c = side->cells;
    for (b1 = c[0][2]->brothers; *b1; ++b1) {
        for (b2 = c[1][2]->brothers; *b2; ++b2) {
            for (n2 = (*b2)->neighbors; *n2; ++n2) {
                if (*b1 == *n2) {
                    // assign newly-found 1st anchor for this side
                    cube->sides[5].cells[0][0] = *b1;
                    (*b1)->row = 0;
                    (*b1)->col = 0;
                    (*b1)->side = &cube->sides[5];

                    // assign newly-found 2nd anchor for this side
                    cube->sides[5].cells[1][0] = *b2;
                    (*b2)->row = 1;
                    (*b2)->col = 0;
                    (*b2)->side = &cube->sides[5];
                    goto eloop5;
                }
            }
        }
    }
    /* should always skip over this ... */
    assert(0);

    /* return in error to make the compiler happy in non-assert builds */
    return -1;
eloop5:
    syncside(&cube->sides[5], *b2, *b1);

    return 0;
}

// vim: et ts=4 sw=4 :
