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

css_fixed number_from_css_string(const css_string *string, bool int_only,
		size_t *consumed);

static inline bool isDigit(uint8_t c)
{
	return '0' <= c && c <= '9';
}

static inline bool isHex(uint8_t c)
{
	return isDigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

static inline uint32_t charToHex(uint8_t c)
{
	switch (c) {
	case 'a': case 'A':
		return 0xa;
	case 'b': case 'B':
		return 0xb;
	case 'c': case 'C':
		return 0xc;
	case 'd': case 'D':
		return 0xd;
	case 'e': case 'E':
		return 0xe;
	case 'f': case 'F':
		return 0xf;
	case '0':
		return 0x0;
	case '1':
		return 0x1;
	case '2':
		return 0x2;
	case '3':
		return 0x3;
	case '4':
		return 0x4;
	case '5':
		return 0x5;
	case '6':
		return 0x6;
	case '7':
		return 0x7;
	case '8':
		return 0x8;
	case '9':
		return 0x9;
	}

	return 0;
}

#endif
