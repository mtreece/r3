/*
 * Copyright (C) 2014 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#ifndef _R3_SPRINT_H_
#define _R3_SPRINT_H_

#include <config.h>

#include <stddef.h>

/*
 * Side to sides-number:
 *
 *      1
 *   3  0  4  5
 *      2
 *
 */
#define R3_SPRINT_FORMAT \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"|%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|  |%c|%c|%c|\n" \
"+-+-+-+  +-+-+-+  +-+-+-+  +-+-+-+\n" \
"\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n" \
"         |%c|%c|%c|\n" \
"         +-+-+-+\n"

#define R3_SPRINT_MINLENGTH sizeof(R3_SPRINT_FORMAT)

/**
 * @brief
 * Print the state of the r3cube into `buffer`. This is similar to sprintf.
 *
 *
 * @param[in] buffer The buffer to output the r3cube to
 * @param[in,out] buflen On input, the maximum length supported; on output, the
 *                       length written
 * @param[in] cube The r3cube to sprint.
 *
 *
 * @retval 0 success
 */
int r3_sprint(char *buffer, size_t *buflen, r3cube *cube);

#endif /* _R3_SPRINT_H_ */
// vim: et ts=4 sw=4 sts=-1 :
