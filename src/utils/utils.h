/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007-8 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_utils_h_
#define css_utils_h_

#include <libcss/types.h>

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

static inline int32_t integer_from_css_string(const css_string *string)
{
	size_t len;
	const uint8_t *ptr;
	int sign = 1;
	int32_t val = 0;

	if (string == NULL || string->len == 0)
		return 0;

	len = string->len;
	ptr = string->ptr;

	/* Extract sign, if any */
	if (ptr[0] == '-') {
		sign = -1;
		len--;
		ptr++;
	} else if (ptr[0] == '+') {
		len--;
		ptr++;
	}

	/** \todo check for overflow */

	/* Now extract value, assuming base 10 */
	while (len > 0) {
		/* Stop on first non-digit */
		if (ptr[0] <= '0' || '9' <= ptr[0])
			break;

		val *= 10;
		val += ptr[0] - '0';
		ptr++;
		len--;
	}

	return val * sign;
}

#endif
