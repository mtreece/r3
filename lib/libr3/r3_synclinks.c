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
            if (*cc == *c) {
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

static int syncside(r3cube *cube, r3side *side, r3cell *c1, r3cell *c2)
{
    r3cell *oanchors[4]; // old anchors
    r3cell *nanchors[2]; // new anchors

    oanchors[0] = c1;
    oanchors[1] = c2;
    oanchors[2] = NULL;
    oanchors[3] = NULL;

    int north = 0;
    while (1) {
        if (!next_anch(oanchors, nanchors)) {
            assert(!nanchors[0]);
            assert(!nanchors[1]);

            // re-orient perpendicular
            r3cell *pa1 = oanchors[north? 2 : 1]; // perpendicular anch
            r3cell *pa2 = oanchors[north? 0 : 3]; // perpendicular anch
            r3cell *op1 = oanchors[north? 3 : 0]; // old perpendicular anch
            r3cell *op2 = oanchors[north? 1 : 2]; // old perpendicular anch
            oanchors[0] = pa1;
            oanchors[1] = pa2;
            oanchors[3] = op1;
            oanchors[4] = op2;

            if (!next_anch(oanchors, nanchors)) {
                // if we have NULL results after the turn, we have finished
                // traversing the grid; this is our halting condition
                break;
            }

            // gridify these new-found anchors
            // Note, when turning like this, the row will be shared, and the
            // column will always be increasing in both north, south cases.
            nanchors[0]->row = oanchors[0];
            nanchors[0]->col = oanchors[0] + 1;
            nanchors[1]->row = oanchors[1];
            nanchors[1]->col = oanchors[1] + 1;
            side->cells[nanchors[0]->row][nanchors[0]->col] = nanchors[0];
            side->cells[nanchors[1]->row][nanchors[1]->col] = nanchors[1];

            // now, reset direction
            north = !north;

            // and hook into the rest of this function (falling through)
            if (north) {
                pa1 = nanchors[1];
                pa2 = oanchors[1];
                op1 = nanchors[0];
                op2 = oanchors[0];
            } else {
                pa1 = oanchors[0];
                pa2 = nanchors[0];
                op1 = oanchors[1];
                op2 = nanchors[1];
            }
            oanchors[0] = pa1;
            oanchors[1] = pa2;
            oanchors[3] = op1;
            oanchors[4] = op2;

            // populate nanchors for fall-through to rest of function
            if (!next_anch(oanchors, nanchors)) {
                // we oughtn't reach this
                assert(0);
            }
        }
        assert(nanchors[0]);
        assert(nanchors[1]);

        // add these two new anchors into the grid
        nanchors[0]->row = oanchors[0] + (north ? -1 : 1);
        nanchors[0]->col = oanchors[0];
        nanchors[1]->row = oanchors[1] + (north ? -1 : 1);
        nanchors[1]->col = oanchors[1];
        side->cells[nanchors[0]->row][nanchors[0]->col] = nanchors[0];
        side->cells[nanchors[1]->row][nanchors[1]->col] = nanchors[1];

        // reset for next iteration
        oanchors[3] = oanchors[1];
        oanchors[2] = oanchors[0];
        oanchors[1] = nanchors[1];
        oanchors[0] = nanchors[0];
    }

    return 0;
}

#if 0
/**
 * @brief
 * Given two up/down, left/right adjacent edge r3cell structs, that are
 * properly set on the grid, reconstruct the entire r3side.
 *
 * @param[in] cube The r3cube this operation is performed on.
 * @param[in] side The r3side this operation is reconstructing.
 * @param[in] c1 The 1st, good r3cell
 * @param[in] c2 the 2nd, good r3cell
 *
 * @retval 0 success
 */
static int syncside(r3cube *cube, r3side *side, r3cell *c1, r3cell *c2)
{
    /*
     * Thought -- rewrite this algorithm, to use a new static function,
     * find_next_pair, which will accept the two anchors (non-NULL), the
     * previous two anchors from the reverse-direction (can be NULL if on a
     * boundary), and will output the next two anchors. If the outputs are
     * NULL, then it means a boundary was hit.
     *
     */

    r3cell **n1, **n11, **n2/*, **n21*/;
    int dvector[2];

    if (!cube || !side || !c1 || !c2) {
        return -1;
    }

    if ((c1->row != c2->row) && (c1->col != c2->col)) {
        return -1;
    }

    if ((c1->row != 0 && c1->row != NUM_ROWS - 1)
            && (c1->col != 0 && c2->col != NUM_COLS - 1)) {
        return -1;
    }

    if (NUM_ROWS - 1 == c1->row && NUM_ROWS - 1 == c2->row) {
        dvector[0] = -1;
        dvector[1] = 0;
    } else if (0 == c1->row && 0 == c2->row) {
        dvector[0] = 1;
        dvector[1] = 0;
    } else if (NUM_COLS - 1 == c1->col && NUM_COLS - 1 == c2->col) {
        dvector[0] = 0;
        dvector[1] = -1;
    } else {
        assert(0 == c1->col && 0 == c2->col);
        dvector[0] = 0;
        dvector[1] = 1;
    }

    // take the first cell, look at its neighbors' neighbors compare each of
    // these to the second cell's neighbors; if a match, then we've found a
    // known location & can populate it
    //
    //
    // thought process: find the two anchors, find their common dimension & set
    // the direction perpendicular to their horizon & against the edge.
    // Algorithmically set the next two anchors along that perpendicular
    // trajectory. Repeat until next edge is hit. Rotate direction of
    // perpendicularity & attempt to continue filling side.
    for (int i = 0; i < NUM_ROWS * NUM_COLS; ++i) {
        for (n1 = c1->neighbors; *n1; ++n1) {
            if (*n1 == c2) {
                continue;
            }
            if (!(*n1)->dirty) {
                // skip if we've already processed this cell
                continue;
            }
            for (n11 = (*n1)->neighbors; *n11; ++n11) {
                if (*n11 == c1 || *n11 == c2) {
                    continue;
                }
                for (n2 = c2->neighbors; *n2; ++n2) {
                    if (*n2 == c1 || *n2 == c2) {
                        continue;
                }
/* I think it's okay if this is clean ... we just need another anchor */
#if 0
                    if (!(*n2)->dirty) {
                        // skip if we've already processed this cell
                        continue;
                    }
#endif
                    if (*n2 == *n11) {
                        // found a match; since this is a neighbor to c2, it must
                        // share the same perpendicular dimension
                        (*n2)->row = c2->row + dvector[0];
                        (*n2)->col = c2->col + dvector[1];
                        (*n2)->dirty = 0;

                        // we can also update (*n1)'s neighbor
                        (*n1)->row = c1->row + dvector[0];
                        (*n1)->col = c1->col + dvector[1];
                        (*n1)->dirty = 0;

                        // check if we've hit the boundary
                        if (dvector[0]) {
                            if (-1 == dvector[0]) {
                                if (0 == (*n1)->row) {
                                    // boundary; now need to shift direction
                                    // TODO: add dependency on input to this
                                    // function, that the input cells be in a
                                    // corner; i.e., their parallel-dimension
                                    // indicies should be either [0] & [1], or
                                    // [NUM_XXX - 1] & [NUM_XXX - 2]. This will
                                    // make direction-updating logistics much
                                    // easier.
                                    if (0 == (*n1)->col || 0 == (*n2)->col) {
                                        // cannot turn left, need to go right
                                        dvector[0] = 0;
                                        dvector[1] = 1;
                                    } else {
                                        // can turn left, so let's do it
                                        dvector[0] = 0;
                                        dvector[1] = -1;
                                    }
                                }
                            } else {
                                assert(1 == dvector[0]);
                                if (NUM_ROWS - 1 == (*n1)->row) {
                                    // boundary; now need to shift direction
                                    if (0 == (*n1)->col || 0 == (*n2)->col) {
                                        // cannot turn left, need to go right
                                        dvector[0] = 0;
                                        dvector[1] = 1;
                                    } else {
                                        // can turn left, so let's do it
                                        dvector[0] = 0;
                                        dvector[1] = -1;
                                    }
                                }
                            }
                        } else {
                            assert(dvector[1]);
                            if (-1 == dvector[1]) {
                                if (0 == (*n1)->col) {
                                    // boundary; now need to shift direction
                                    if (0 == (*n1)->row || 0 == (*n2)->row) {
                                        // cannot go up, need to go down
                                        dvector[0] = 1;
                                        dvector[1] = 0;
                                    } else {
                                        // can go up, so let's do it
                                        dvector[0] = -1;
                                        dvector[1] = 0;
                                    }
                                }
                            } else {
                                assert(1 == dvector[1]);
                                if (NUM_COLS - 1 == (*n1)->col) {
                                    // boundary; now need to shift direction
                                    if (0 == (*n1)->row || 0 == (*n2)->row) {
                                        // cannot go up, need to go down
                                        dvector[0] = 1;
                                        dvector[1] = 0;
                                    } else {
                                        // can go up, so let's do it
                                        dvector[0] = -1;
                                        dvector[1] = 0;
                                    }
                                }
                            }
                        }

                        // boundary not hit; update anchors & keep looping
                        c1 = *n1;
                        c2 = *n2;

                        // assert that we will restart with semi-sane input
                        // (this helps catch against update bugs)
                        assert(0 <= c1->row);
                        assert(NUM_ROWS - 1 >= c1->row);
                        assert(0 <= c1->col);
                        assert(NUM_COLS - 1 >= c1->col);

                        goto eloop0;
                    }
                }
            }
        }
eloop0:
        // no-op, to make C happy with my label placement
        /* no-op */;
    }

    return 0;
}
#endif /* 0 */

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
    syncside(cube, side, c[0][0], c[0][1]);

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
eloop1:
    syncside(cube, &cube->sides[1], *b1, *b2);

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
eloop2:
    syncside(cube, &cube->sides[2], *b1, *b2);

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
eloop3:
    syncside(cube, &cube->sides[3], *b1, *b2);

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
eloop4:
    syncside(cube, &cube->sides[4], *b1, *b2);

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
eloop5:
    syncside(cube, &cube->sides[5], *b1, *b2);

    return 0;
}

// vim: et ts=4 sw=4 :
