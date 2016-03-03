/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <config.h>

// TODO: generalize this one day...
#if NUM_SIDES != 6 \
 || NUM_ROWS  != 3 \
 || NUM_COLS  != 3
#error Test currently only supports 6x3x3
#endif /* if NUM_SIDES != ... */

#include <r3.h>

#include "check_libr3.h"

/**
 * @brief
 * Test that the "identity" property upholds: on a freshly init'ed cube, does
 * each cell report its proper color?
 */
START_TEST(test_identity)
{
    r3cube cube;
    r3cell *cell;

    const int colors[NUM_SIDES][NUM_ROWS][NUM_COLS] = {
        {/* s0: col 0     col 1      col 2  */
            {R3_WHITE,  R3_WHITE,  R3_WHITE},  /* row 0 */
            {R3_WHITE,  R3_WHITE,  R3_WHITE},  /* row 1 */
            {R3_WHITE,  R3_WHITE,  R3_WHITE},  /* row 2 */  },
        {/* s1: col 0     col 1      col 2  */
            {R3_RED,    R3_RED,    R3_RED},    /* row 0 */
            {R3_RED,    R3_RED,    R3_RED},    /* row 1 */
            {R3_RED,    R3_RED,    R3_RED},    /* row 2 */  },
        {/* s2: col 0     col 1      col 2  */
            {R3_BLUE,   R3_BLUE,   R3_BLUE},   /* row 0 */
            {R3_BLUE,   R3_BLUE,   R3_BLUE},   /* row 1 */
            {R3_BLUE,   R3_BLUE,   R3_BLUE},   /* row 2 */  },
        {/* s3: col 0     col 1      col 2  */
            {R3_ORANGE, R3_ORANGE, R3_ORANGE}, /* row 0 */
            {R3_ORANGE, R3_ORANGE, R3_ORANGE}, /* row 1 */
            {R3_ORANGE, R3_ORANGE, R3_ORANGE}, /* row 2 */  },
        {/* s4: col 0     col 1      col 2  */
            {R3_GREEN,  R3_GREEN,  R3_GREEN},  /* row 0 */
            {R3_GREEN,  R3_GREEN,  R3_GREEN},  /* row 1 */
            {R3_GREEN,  R3_GREEN,  R3_GREEN},  /* row 2 */  },
        {/* s5: col 0     col 1      col 2  */
            {R3_YELLOW, R3_YELLOW, R3_YELLOW}, /* row 0 */
            {R3_YELLOW, R3_YELLOW, R3_YELLOW}, /* row 1 */
            {R3_YELLOW, R3_YELLOW, R3_YELLOW}, /* row 2 */  },
    };

    // can I init a cube?
    ck_assert_int_eq(0, r3_init(&cube));

    for (unsigned side = 0; side < NUM_SIDES; ++side) {
        for (unsigned row = 0; row < NUM_ROWS; ++row) {
            for (unsigned col = 0; col < NUM_COLS; ++col) {
                ck_assert(NULL != (cell = r3_get_cell(&cube, side, row, col)));
                ck_assert_int_eq(r3_cell_get_color(cell),
                                 colors[side][row][col]);
            }
        }
    }
}
END_TEST

static Suite *builder()
{
    Suite *s;
    TCase *tc;

    s = suite_create("libr3::moves");

    tc = tcase_create("identity");
    tcase_add_test(tc, test_identity);
    suite_add_tcase(s, tc);

    return s;
}
EXPORT_SUITE(builder);

// vim: et ts=4 sw=4 :
