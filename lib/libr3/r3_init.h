/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_INIT_H_
#define _R3_INIT_H_

#include <config.h>

/**
 * @brief
 * Initialize an r3cube structure. This will reset/create state on the
 * structure, such as positioning/orientation and "identity" of color cells
 * (i.e. all colors grouped together on their own side). ... if only life were
 * this simple.
 *
 * All API calls on an r3cube expect the r3cube structure state to have been
 * initialized by this call.
 *
 * @param cube The r3cube to initialize
 *
 * @retval 0 success
 */
int r3_init(r3cube *cube);

#endif /* _R3_INIT_H_ */
// vim: et ts=4 sw=4 sts=-1 :
