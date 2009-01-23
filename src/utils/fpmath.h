/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_utils_fpmath_h_
#define css_utils_fpmath_h_

#include <stdint.h>

/* 22:10 fixed point math */
typedef int32_t fixed;

/* Add two fixed point values */
#define FADD(a, b) ((a) + (b))
/* Subtract two fixed point values */
#define FSUB(a, b) ((a) - (b))
/* Multiply two fixed point values */
#define FMUL(a, b) (((a) * (b)) >> 10)
/* Divide two fixed point values */
#define FDIV(a, b) (((a) << 10) / (b))

/* Add an integer to a fixed point value */
#define FADDI(a, b) ((a) + ((b) << 10))
/* Subtract an integer from a fixed point value */
#define FSUBI(a, b) ((a) - ((b) << 10))
/* Multiply a fixed point value by an integer */
#define FMULI(a, b) ((a) * (b))
/* Divide a fixed point value by an integer */
#define FDIVI(a, b) ((a) / (b))

/* Convert a floating point value to fixed point */
#define FLTTOFIX(a) ((fixed) ((a) * (float) (1 << 10)))
/* Convert a fixed point value to floating point */
#define FIXTOFLT(a) ((float) (a) / (float) (1 << 10))

/* Convert an integer to a fixed point value */
#define INTTOFIX(a) ((a) << 10)
/* Convert a fixed point value to an integer */
#define FIXTOINT(a) ((a) >> 10)

#include "utils/fpconstants.h"

#endif

