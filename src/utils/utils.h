/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007-8 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_utils_h_
#define css_utils_h_

#include <libcss/types.h>

#include "utils/fpmath.h"

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef SLEN
/* Calculate length of a string constant */
#define SLEN(s) (sizeof((s)) - 1) /* -1 for '\0' */
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

static inline fixed number_from_css_string(const css_string *string,
		size_t *consumed)
{
	size_t len;
	const uint8_t *ptr;
	int sign = 1;
	int32_t intpart = 0;
	int32_t fracpart = 0;
	int32_t pwr = 1;

	if (string == NULL || string->len == 0 || consumed == NULL)
		return 0;

	len = string->len;
	ptr = string->ptr;

	/* number = [+-]? ([0-9]+ | [0-9]* '.' [0-9]+) */

	/* Extract sign, if any */
	if (ptr[0] == '-') {
		sign = -1;
		len--;
		ptr++;
	} else if (ptr[0] == '+') {
		len--;
		ptr++;
	}

	/* Ensure we have either a digit or a '.' followed by a digit */
	if (len == 0) {
		*consumed = 0;
		return 0;
	} else {
		if (ptr[0] == '.') {
			if (len == 1 || ptr[1] < '0' || '9' < ptr[1]) {
				*consumed = 0;
				return 0;
			}
		} else if (ptr[0] < '0' || '9' < ptr[0]) {
			*consumed = 0;
			return 0;
		}
	}

	/* Now extract intpart, assuming base 10 */
	while (len > 0) {
		/* Stop on first non-digit */
		if (ptr[0] < '0' || '9' < ptr[0])
			break;

		/* Clamp to a max of 2^22 - 1 */
		if (intpart < (1 << 22)) {
			intpart *= 10;
			intpart += ptr[0] - '0';
		}
		ptr++;
		len--;
	}

	/* And fracpart, again, assuming base 10 */
	if (len > 1 && ptr[0] == '.' && ('0' <= ptr[1] && ptr[1] <= '9')) {
		ptr++;
		len--;

		while (len > 0) {
			if (ptr[0] < '0' || '9' < ptr[0])
				break;

			if (pwr < 1000000) {
				pwr *= 10;
				fracpart *= 10;
				fracpart += ptr[0] - '0';
			}
			ptr++;
			len--;
		}
		fracpart = ((1 << 10) * fracpart + pwr/2) / pwr;
		if (fracpart >= (1 << 10)) {
			intpart++;
			fracpart &= (1 << 10) - 1;
		}
	}

	/* If the intpart is larger than we can represent, 
	 * then clamp to the maximum value we can store. */
	if (intpart >= (1 << 21)) {
		fracpart = (1 << 10) - 1;
		intpart = (1 << 21) - 1;
	}

	*consumed = ptr - string->ptr;

	return FMULI(((intpart << 10) | fracpart), sign);
}

#endif
