/*
 * Copyright (C) 2015 Michael T. Reece.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef _CHECK_LIBR3_H_
#define _CHECK_LIBR3_H_


#include <config.h>

#include <stdlib.h>

#include <check.h>

typedef Suite *(*suite_getter)(void);

extern suite_getter *_suite_getters;
extern suite_getter *_esuite_getters;

#define EXPORT_SUITE(name) \
   static Suite __attribute__((unused, section(".suite_getters"))) \
   * (*__suite_getters__##name)(void) = name;


#endif /* _CHECK_LIBR3_H_ */

// vim: et ts=4 sw=4 :
