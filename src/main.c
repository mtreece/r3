/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

#include <r3.h>

int main(int argc, char *argv[])
{
    r3cube cube;
    r3_init(&cube);
    r3_move(&cube, 0, R3_UP);

    return 0;
}

// vim: et ts=4 sw=4 sts=-1 :
