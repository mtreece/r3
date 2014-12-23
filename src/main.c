/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <stdio.h>
#include <string.h>

#include <r3.h>

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    r3cube cube;
    int res;
    char buffer[R3_SPRINT_MINLENGTH];
    size_t buflen;

    if (0 != (res = r3_init(&cube))) {
        fprintf(stderr, "%s:%d- error, returned %d\n", __FUNCTION__, __LINE__, res);
    }

    buflen = sizeof(buffer);
    if (-1 == (res = r3_sprint(buffer, &buflen, &cube))) {
        fprintf(stderr, "%s:%d- error, returned %d\n", __FUNCTION__, __LINE__, res);
    }
    printf(buffer);

    printf("==========================================\n");

    r3_move(&cube, R3_UP, 0);
    buflen = sizeof(buffer);
    if (-1 == (res = r3_sprint(buffer, &buflen, &cube))) {
        fprintf(stderr, "%s:%d- error, returned %d\n", __FUNCTION__, __LINE__, res);
    }
    printf(buffer);

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
