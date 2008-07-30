/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_types_h_
#define css_types_h_

#include <stdbool.h>
#include <inttypes.h>

/** Source of charset information, in order of importance
 * A client-dictated charset will override all others.
 * A document-specified charset will override autodetection or the default */
typedef enum css_charset_source {
	CSS_CHARSET_DEFAULT          = 0,	/**< Default setting */
	CSS_CHARSET_REFERRED         = 1,	/**< From referring document */
	CSS_CHARSET_METADATA         = 2,	/**< From linking metadata */
	CSS_CHARSET_DOCUMENT         = 3,	/**< Defined in document */
	CSS_CHARSET_DICTATED         = 4,	/**< Dictated by client */
} css_charset_source;

/**
 * String type
 */
typedef struct css_string {
	uint8_t *ptr;			/**< Pointer to data */
	size_t len;			/**< Byte length of string */
} css_string;

typedef struct css_stylesheet css_stylesheet;

#endif
