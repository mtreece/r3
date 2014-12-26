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

static int syncside(r3cube *cube, r3side *side, r3cell *c1, r3cell *c2)
{
    if (!cube || !side || !c1 || !c2) {
        return -1;
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
