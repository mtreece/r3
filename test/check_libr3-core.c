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

    return s;
}
EXPORT_SUITE(builder);

// vim: et ts=4 sw=4 :
