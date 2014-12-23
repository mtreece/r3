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

#include <r3.h>

int main(int __attribute__((unused)) argc, char __attribute__((unused)) *argv[])
{
    r3cube cube;
    char buffer[R3_SPRINT_MINLENGTH];
    size_t buflen;

    r3_init(&cube);

    buflen = sizeof(buffer);
    r3_sprint(buffer, &buflen, &cube);
    printf(buffer);

    printf("==========================================\n");

    r3_move(&cube, R3_UP, 0);
    r3_sprint(buffer, &buflen, &cube);
    printf(buffer);

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
