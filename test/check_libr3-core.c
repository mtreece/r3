/*
 * Copyright (C) 2015 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <config.h>

#include <stdlib.h>

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

static Suite *builder()
{
    Suite *s;
    TCase *tc;

    s = suite_create("libr3::core");

    tc = tcase_create("null");
    tcase_add_test(tc, test_null);
    suite_add_tcase(s, tc);

    return s;
}
EXPORT_SUITE(builder);

// vim: et ts=4 sw=4 :
