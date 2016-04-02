/*
 * Copyright (C) 2014-2016 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_SYNCLINKS_H_
#define _R3_SYNCLINKS_H_

#include <config.h>

//TODO create public-facing, incomplete declarations for these structs
typedef struct r3cube r3cube;

/**
 * @brief
 * Synchronize internal r3cube state between neighbor-links and cube-layout.
 *
 * This function is used internally by libr3, and shouldn't need to be used by
 * end-users.
 *
 * @param[in] cube The r3cube to operate on.
 *
 * @retval 0 Success
 */
int r3_synclinks(r3cube *cube);

#endif /* _R3_SYNCLINKS_H_ */
// vim: et ts=4 sw=4 :
