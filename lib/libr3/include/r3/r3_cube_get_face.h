/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <config.h>

//TODO create public-facing, incomplete declarations for these structs
typedef struct r3cube r3cube;
typedef struct r3side r3side;

/**
 * @brief
 * Return the specified r3side, `side`, of `cube`. The number `side` should
 * adhere to the numbering scheme specified in /doc/cube-layout.txt. (e.g., the
 * facing side will be on side = 0).
 *
 * @param cube The r3cube to perform this operation on.
 * @param side The side-number of the side to return.
 *
 * @return The specified r3side of this r3cube, or NULL on error.
 */
const r3side *r3_cube_get_face(const r3cube *cube, unsigned side);

// vim: et ts=4 sw=4 :
