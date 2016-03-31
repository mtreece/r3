/*
 * Copyright (C) 2014-2015 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_INIT_H_
#define _R3_INIT_H_

#include <config.h>

#include <stdlib.h>

/**
 * @brief
 * Initialize an r3cube structure. This will reset/create state on the
 * structure, such as positioning/orientation and "identity" of color cells
 * (i.e. all colors grouped together on their own side). ... if only life were
 * this simple.
 *
 * Note, the API expects for the calling code to handle memory management. To
 * accommodate communicating the buffer size to the calling code, this function
 * allows for a `len` argument to be passed in. If the `len` argument is too
 * small to hold an r3cube structure, this function will return in error. This
 * function will populate the `len` field, if non-NULL, with the needed size.
 * This will allow the calling code to re-adjust their buffer length and try
 * again to initialize. If `len` is unspecified (i.e., NULL), no error checking
 * is performed on the buffer length, which could result in memory corruption
 * or application segfaults, etc. The `len` parameter is left optional, with
 * the use-case that calling-code will be able to assess the needed length only
 * once for the initialization of multiple cubes.
 *
 * All API calls on an r3cube expect the r3cube structure state to have been
 * initialized by this call.
 *
 * @param[in,out] cube The r3cube to initialize
 * @param[in,out] len On input, this is the length of the supplied buffer for
 *                    `cube`; on output, this is the length that is actually
 *                    required by this implementation.
 *
 * @retval 0 success
 * @retval 1 length of buffer too small
 */
int r3_init(r3cube *cube, size_t *len);

#endif /* _R3_INIT_H_ */
// vim: et ts=4 sw=4 :
