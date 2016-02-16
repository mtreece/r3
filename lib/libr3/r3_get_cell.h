/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_GET_CELL_
#define _R3_GET_CELL_

#include <config.h>

#include "r3cube.h"
#include "r3sides.h"

/**
 * @brief
 * From the specified r3side of this r3cube, get the r3cell that is located at
 * the specified row and column.
 *
 * The number `side` should adhere to the numbering scheme specified in
 * /doc/cube-layout.txt. (e.g., the facing side will be on side = 0).
 *
 * @param cube The r3cube to operate on.
 * @param side The side-number of the side to return a cell from.
 * @param row The row of the side to pull from.
 * @param col The column of the side to pull from.
 *
 * @return The cell at the specified row, col, side; NULL on error.
 */
const r3cell *r3_get_cell(r3cube *cube, unsigned side, unsigned row, unsigned col);

#endif /* _R3_GET_CELL_ */
// vim: et ts=4 sw=4 :
