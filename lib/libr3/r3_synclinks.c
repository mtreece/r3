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

#include "r3cube.h"
#include "r3_synclinks.h"

#define abs(x) (x >= 0 ? x : -x)

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
            for (n11 = (*n1)->neighbors; *n11; ++n11) {
                for (n2 = c2->neighbors; *n2; ++n2) {
                    if (*n2 == *n11) {
                        // found a match; since this is a neighbor to c2, it must
                        // share the same perpendicular dimension
                        (*n2)->row = c2->row + dvector[0];
                        (*n2)->col = c2->col + dvector[1];

                        // we can also update (*n1)'s neighbor
                        (*n1)->row = c1->row + dvector[0];
                        (*n1)->col = c1->col + dvector[1];

                        // check if we've hit the boundary
                        if (dvector[0]) {
                            if (-1 == dvector[0]) {
                                if (0 == (*n1)->row) {
                                    // boundary; now need to shift direction
                                }
                            } else {
                                assert(1 == dvector[0]);
                                if (NUM_ROWS - 1 == (*n1)->row) {
                                    // boundary; now need to shift direction
                                }
                            }
                        } else {
                            assert(dvector[1]);
                            if (-1 == dvector[1]) {
                                if (0 == (*n1)->col) {
                                    // boundary; now need to shift direction
                                }
                            } else {
                                assert(1 == dvector[1]);
                                if (NUM_COLS - 1 == (*n1)->col) {
                                    // boundary; now need to shift direction
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

int r3_synclinks(r3cube *cube)
{
    r3side *side;
    r3cell *(*c)[NUM_COLS];
    r3cell **b1, **b2;
    r3cell /* **n1, */ **n2;

    if (!cube) {
        return -1;
    }

    // start with the anchors on sides[0]; these should be synced already on
    // any operation
    side = &cube->sides[0];
    c = side->cells;
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

// vim: et ts=4 sw=4 sts=-1 :
