/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_GET_FACING_CELL_
#define _R3_GET_FACING_CELL_

#include <config.h>

#include "r3cube.h"
#include "r3sides.h"

/**
 * @brief
 * From the facing r3side of this r3cube, get the r3cell that is located at
 * specified row and column.
 *
 * @param cube The r3cube to operate on.
 * @param row The row of the facing side to pull from.
 * @param col The column of the facing side to pull from.
 *
 * @return The facing cell at the specified row, col; NULL on error.
 */
const r3cell *r3_get_facing_cell(r3cube *cube, int row, int col);

#endif /* _R3_GET_FACING_CELL_ */
// vim: et ts=4 sw=4 :
