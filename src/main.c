/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <r3/r3.h>

/*
 * NOTE: this file is NOT the real "main" of this software! This is currently
 * only a very basic "program" for exercising some of the functionality being
 * developed in the library. Later, I will try to document long-term plans for
 * what real applications will be developed alongside the r3 library.
 *
 */

#define DO_CHECK_REPORT_END(check, xdo) \
    do { \
        typeof(xdo) res; \
        if (check (res = xdo)) { \
            fprintf(stderr, "%s:%d- error, returned %d\n", __FUNCTION__, __LINE__, res); \
            exit(-1); \
        } \
    } while (0)

#define DO_MOVE_AND_PRINT(cube, direction, selector) \
    do { \
        buflen = sizeof(buffer); \
        printf("Move %s(%d)\n", #direction, selector); \
        DO_CHECK_REPORT_END(0 !=, r3_move(cube, direction, selector)); \
        DO_CHECK_REPORT_END(0 !=, r3_sprint(buffer, &buflen, cube)); \
        printf("%s\n", buffer); \
        printf("==========================================\n"); \
    } while (0)

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    r3cube *cube;
    size_t cubelen = 0;
    char buffer[R3_SPRINT_MINLENGTH];
    size_t buflen;

    DO_CHECK_REPORT_END(-2 !=, r3_init(NULL, &cubelen));
    printf("r3cube length reported to be %zu bytes\n", cubelen);

    cube = malloc(cubelen);
    DO_CHECK_REPORT_END(0 !=, r3_init(cube, NULL));

    buflen = sizeof(buffer);
    DO_CHECK_REPORT_END(0 !=, r3_sprint(buffer, &buflen, cube));
    printf("Identity:\n");
    printf("%s\n", buffer);
    printf("==========================================\n");

    DO_MOVE_AND_PRINT(cube, R3_UP, 0);
    DO_MOVE_AND_PRINT(cube, R3_UP, 0);
    DO_MOVE_AND_PRINT(cube, R3_UP, 0);

    DO_MOVE_AND_PRINT(cube, R3_UP, 1);
    DO_MOVE_AND_PRINT(cube, R3_UP, 1);
    DO_MOVE_AND_PRINT(cube, R3_UP, 1);

    DO_MOVE_AND_PRINT(cube, R3_UP, 2);
    DO_MOVE_AND_PRINT(cube, R3_UP, 2);
    DO_MOVE_AND_PRINT(cube, R3_UP, 2);

    DO_MOVE_AND_PRINT(cube, R3_DOWN, 0);
    DO_MOVE_AND_PRINT(cube, R3_DOWN, 0);
    DO_MOVE_AND_PRINT(cube, R3_DOWN, 0);

    DO_MOVE_AND_PRINT(cube, R3_RIGHT, 0);

    return 0;
}

// vim: et ts=4 sw=4 :
