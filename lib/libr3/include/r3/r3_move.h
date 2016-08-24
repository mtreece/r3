/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_MOVE_H_
#define _R3_MOVE_H_

#include <config.h>

#include <r3/r3.h>

/**
 * @brief
 * Update the r3cube with a move-operation on the `selector` in the direction
 * of `direction`. Since a move is dependent on the facing-direction and
 * orientation of the cube, this operation will rely on internal, existing
 * state of how the cube is oriented and facing the operator. The selector's
 * interpretation is dependent on `direction`; if `direction` is one of R3_{UP,
 * DOWN}, then `selector` will be a r3cube column index, and the column
 * `selector` will be moved appropriately; if `direction` is one of R3_{LEFT,
 * RIGHT}, then `selector` will be a r3cube row index, and the row `selector`
 * will be moved appropriately.
 *
 * @param cube The r3 cube to transform
 * @param direction R3_UP, R3_DOWN, R3_LEFT, R3_RIGHT
 * @param selector (Row or column) index (0-idx) to operate
 *
 * @retval 0 success
 */
int r3_move(r3cube *cube, unsigned direction, unsigned selector);

#endif /* _R3_MOVE_H_ */
// vim: ts=4 sts=4 sw=4 et :
