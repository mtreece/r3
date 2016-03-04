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
 * The projection of cube colors from the identity cube.
 */
static const int identity[NUM_SIDES][NUM_ROWS][NUM_COLS] = {
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

/**
 * @brief
 * Check if a cube is set to identity.
 *
 * @param[in] cube The cube to check for identity
 *
 * @retval 0 The cube is NOT set to identity.
 * @retval 1 The cube IS set to identity.
 */
__attribute__((used))
static int is_identity(const r3cube *cube)
{
    r3cell *cell;
    for (unsigned side = 0; side < NUM_SIDES; ++side) {
        for (unsigned row = 0; row < NUM_ROWS; ++row) {
            for (unsigned col = 0; col < NUM_COLS; ++col) {
                ck_assert(NULL != (cell = r3_get_cell(cube, side, row, col)));
                if (r3_cell_get_color(cell) != identity[side][row][col]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

/**
 * @brief
 * Assert that an r3cube is at identity state.
 *
 * This function calls into check to assert that every color matches the
 * identity projection. This has the advantage of, on failure, printing the
 * discrepancy.
 *
 * @param[in] cube The cube to check for identity
 */
__attribute__((used))
static void assert_identity(const r3cube *cube)
{
    r3cell *cell;
    for (unsigned side = 0; side < NUM_SIDES; ++side) {
        for (unsigned row = 0; row < NUM_ROWS; ++row) {
            for (unsigned col = 0; col < NUM_COLS; ++col) {
                ck_assert(NULL != (cell = r3_get_cell(cube, side, row, col)));
                ck_assert_int_eq(r3_cell_get_color(cell),
                                 identity[side][row][col]);
            }
        }
    }
}

/**
 * @brief
 * Assert that an r3cube is NOT at identity state.
 *
 * This function calls into check to assert that at least one color fails to
 * match the identity projection.
 *
 * @param[in] cube The cube to check for non-identity
 */
__attribute__((used))
static void assert_non_identity(const r3cube *cube)
{
    r3cell *cell;
    for (unsigned side = 0; side < NUM_SIDES; ++side) {
        for (unsigned row = 0; row < NUM_ROWS; ++row) {
            for (unsigned col = 0; col < NUM_COLS; ++col) {
                ck_assert(NULL != (cell = r3_get_cell(cube, side, row, col)));
                if (r3_cell_get_color(cell) != identity[side][row][col]) {
                    /* cube fails an identity requirement; return w/o abort */
                    return;
                }
            }
        }
    }
    ck_abort_msg("cube is set to identity when it ought not be");
}

/**
 * @brief
 * Test that the "identity" property upholds: on a freshly init'ed cube, does
 * each cell report its proper color?
 */
START_TEST(test_identity)
{
    r3cube cube;

    // can I init a cube?
    ck_assert_int_eq(0, r3_init(&cube));

    // assert that a freshly init'ed cube is set to identity
    assert_identity(&cube);
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
