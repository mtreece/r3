/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_H_
#define _R3_H_

#include <config.h>

// common structures
// NOTE: these are defined as incomplete types in order to decouple client
// implementations from the inner details
typedef struct r3cell r3cell;
typedef struct r3cube r3cube;
typedef struct r3side r3side;

// common defines
#include <r3/r3colors.h>
#include <r3/r3moves.h>

// API
#include <r3/r3_cell_get_color.h>
#include <r3/r3_cube_get_face.h>
#include <r3/r3_get_cell.h>
#include <r3/r3_init.h>
#include <r3/r3_is_solved.h>
#include <r3/r3_move.h>
#include <r3/r3_sprint.h>

#endif /* _R3_H_ */
// vim: ts=4 sts=4 sw=4 et :
