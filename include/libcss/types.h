/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_types_h_
#define libcss_types_h_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <parserutils/utils/hash.h>

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
 * Stylesheet language level -- defines parsing rules and supported properties
 */
typedef enum css_language_level {
	CSS_LEVEL_1                 = 0,	/**< CSS 1 */
	CSS_LEVEL_2                 = 1,	/**< CSS 2 */
	CSS_LEVEL_21                = 2,	/**< CSS 2.1 */
	CSS_LEVEL_3                 = 3,	/**< CSS 3 */
	CSS_LEVEL_DEFAULT           = CSS_LEVEL_21	/**< Default level */
} css_language_level;

/**
 * Stylesheet media types
 */
typedef enum css_media_type {
	CSS_MEDIA_SCREEN            = (1<<0),
	CSS_MEDIA_ALL               = CSS_MEDIA_SCREEN
} css_media_type;

/**
 * Stylesheet origin
 */
typedef enum css_origin {
	CSS_ORIGIN_UA                = 0,	/**< User agent stylesheet */
	CSS_ORIGIN_USER              = 1,	/**< User stylesheet */
	CSS_ORIGIN_AUTHOR            = 2	/**< Author stylesheet */
} css_origin;

/**
 * String type
 *
 * \todo It might be better to define parserutils_string, and use that.
 *       (where parserutils_string is identical to parserutils_hash_entry)
 */
typedef parserutils_hash_entry css_string;

typedef struct css_stylesheet css_stylesheet;

#endif
