/*
 * Copyright (C) 2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_IS_SOLVED_
#define _R3_IS_SOLVED_

#include <config.h>

//TODO create public-facing, incomplete declarations for these structs
typedef struct r3cube r3cube;

/**
 * @brief
 * Determine if a cube is in a solved state. A "solved" state is one where
 * cells on each side are all of matching color (for their side).
 *
 * @param cube The r3cube to check for solvedness.
 *
 * @retval 1 The cube is valid and in a solved state.
 * @retval 0 The cube is in an unsolved state or is invalid.
 */
int r3_is_solved(const r3cube *cube);

#endif /* _R3_IS_SOLVED_ */
// vim: et ts=4 sw=4 :
