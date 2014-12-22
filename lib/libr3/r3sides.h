/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3SIDES_H_
#define _R3SIDES_H_

#include <config.h>

#include "r3colors.h"

// TODO: put in build system & generalize
#define NUM_SIDES 6
#define NUM_COLS 3
#define NUM_ROWS 3

typedef struct {
    int color;
} r3cell;

typedef struct {
    r3cell cells[NUM_COLS][NUM_ROWS];
} r3side;

#endif /* _R3SIDES_H_ */
// vim: et ts=4 sw=4 sts=-1 :
