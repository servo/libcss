/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_parse_parse_h_
#define css_parse_parse_h_

#include <libcss/functypes.h>
#include <libcss/types.h>

typedef struct css_parser css_parser;

/**
 * Parser option types
 */
typedef enum css_parser_opttype {
	CSS_PARSER_QUIRKS,
} css_parser_opttype;

/**
 * Parser option parameters
 */
typedef union css_parser_optparams {
	bool quirks;
} css_parser_optparams;

css_parser *css_parser_create(css_stylesheet *sheet, const char *charset,
		css_charset_source cs_source, css_alloc alloc, void *pw);
void css_parser_destroy(css_parser *parser);

css_error css_parser_setopt(css_parser *parser, css_parser_opttype type,
		css_parser_optparams *params);

css_error css_parser_parse_chunk(css_parser *parser, const uint8_t *data, 
		size_t len);
css_error css_parser_completed(css_parser *parser);

const char *css_parser_read_charset(css_parser *parser, 
		css_charset_source *source);

#endif

