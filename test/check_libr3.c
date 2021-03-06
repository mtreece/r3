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
#include <stdio.h>

#include <check.h>
#include <check_libr3.h>

int main(void)
{
    suite_getter *sg;
    unsigned int nfailed = 0;

    for (sg = (suite_getter *) &_suite_getters;
            sg < (suite_getter *) &_esuite_getters; ++sg) {
        Suite *s;
        SRunner *sr;

        printf("Launching suite_getter @ %p.\n", sg);

        s = (*sg)();
        sr = srunner_create(s);

#if defined(CHECK_SUPPORTS_TAP)
        srunner_set_tap(sr, "-");
#endif /* defined(CHECK_SUPPORTS_TAP) */

        srunner_run_all(sr, CK_ENV);
        nfailed += srunner_ntests_failed(sr);

        srunner_free(sr);
    }

    return (0 == nfailed) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// vim: ts=4 sts=4 sw=4 et :
