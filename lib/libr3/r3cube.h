/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3CUBE_H_
#define _R3CUBE_H_

#include <config.h>

#include "r3sides.h"

typedef struct {
    /// position/rotation when viewing and operating
    /// (not yet supported)
    int position;

    /// the sides of this cube
    r3side sides[NUM_SIDES];
} r3cube;

#endif /* _R3CUBE_H_ */
// vim: et ts=4 sw=4 sts=-1 :
