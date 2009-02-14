/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_parse_language_h_
#define css_parse_language_h_

#include <parserutils/utils/vector.h>

#include <libcss/functypes.h>
#include <libcss/types.h>

#include "parse/parse.h"

typedef struct css_language css_language;

css_error css_language_create(css_stylesheet *sheet, css_parser *parser,
		css_allocator_fn alloc, void *pw, void **language);
css_error css_language_destroy(css_language *language);

#endif

