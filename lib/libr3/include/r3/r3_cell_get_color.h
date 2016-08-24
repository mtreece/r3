/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_CELL_GET_COLOR_
#define _R3_CELL_GET_COLOR_

#include <config.h>

#include <r3/r3.h>

/**
 * @brief
 * Return the color of the given r3cell.
 *
 * @param cell The r3cell to get the color of.
 *
 * @return The color of this r3cell; negative on error.
 */
int r3_cell_get_color(const r3cell *cell);

#endif /* _R3_CELL_GET_COLOR_ */
// vim: ts=4 sts=4 sw=4 et :
