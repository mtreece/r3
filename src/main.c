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
    r3_init(&cube);
    char buffer[1024];
    size_t buflen = sizeof(buffer);
    r3_sprint(buffer, &buflen, &cube);
    printf(buffer);
    r3_move(&cube, R3_UP, 0);

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
