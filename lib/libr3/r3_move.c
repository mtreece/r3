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

int r3_move(r3cube *cube, int direction, int selector)
{
    int vertical;                       // direction is vertical? else, horizontal

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
