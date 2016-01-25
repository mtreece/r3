/*
 * Copyright (C) 2015-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <config.h>

#include <stdlib.h>

#include <r3.h>

#include "check_libr3.h"

/**
 * @brief
 * Test that we can "test ourselves".
 *
 * @details
 * This test just ensures we can create a passing test. It is a simple, dummy
 * test, for sake of having a test.
 */
START_TEST(test_null)
{
    ck_assert(1);
}
END_TEST

/**
 * @brief
 * Test that we can exercise the basics of the API without crashing anything.
 *
 * @details
 * This test more or less ensures that we don't segfault or otherwise do
 * something silly while making basic API calls.
 */
START_TEST(test_basics)
{
    r3cube cube;
    ck_assert_int_eq(0, r3_init(&cube));
}
END_TEST

static Suite *builder()
{
    Suite *s;
    TCase *tc;

    s = suite_create("libr3::core");

    tc = tcase_create("null");
    tcase_add_test(tc, test_null);
    suite_add_tcase(s, tc);

    tc = tcase_create("basics");
    tcase_add_test(tc, test_basics);
    suite_add_tcase(s, tc);

    return s;
}
EXPORT_SUITE(builder);

// vim: et ts=4 sw=4 :
