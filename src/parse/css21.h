/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_parse_css21_h_
#define css_parse_css21_h_

#include <parserutils/utils/vector.h>

#include <libcss/functypes.h>
#include <libcss/types.h>

#include "parse/parse.h"

typedef struct css_css21 css_css21;

css_error css_css21_create(css_stylesheet *sheet, css_parser *parser,
		css_alloc alloc, void *pw, void **css21);
css_error css_css21_destroy(css_css21 *css21);

#endif

