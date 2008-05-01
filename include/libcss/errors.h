/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_errors_h_
#define css_errors_h_

#include <stddef.h>

typedef enum css_error {
	CSS_OK               = 0,

	CSS_NOMEM            = 1,
	CSS_BADPARM          = 2,
	CSS_INVALID          = 3,
	CSS_FILENOTFOUND     = 4,
	CSS_NEEDDATA         = 5,
	CSS_BADCHARSET       = 6,
	CSS_EOF              = 7,
} css_error;

/* Convert a libcss error value to a string */
const char *css_error_to_string(css_error error);
/* Convert a string to a libcss error value */
css_error css_error_from_string(const char *str, size_t len);

#endif

