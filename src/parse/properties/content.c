/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

/**
 * Parse content
 *
 * \param c	  Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx	  Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *	   CSS_NOMEM on memory exhaustion,
 *	   CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *		   If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_content(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size = sizeof(opv);
	int temp_ctx = *ctx;
	uint8_t *ptr;
	bool match;

	/* IDENT(normal, none, inherit) |
	 * [
	 *	IDENT(open-quote, close-quote, no-open-quote, no-close-quote) |
	 *	STRING | URI |
	 *	FUNCTION(attr) IDENT ')' |
	 *	FUNCTION(counter) IDENT IDENT? ')' |
	 *	FUNCTION(counters) IDENT STRING IDENT? ')'
	 * ]+
	 */

	/* Pass 1: Calculate required size & validate input */
	token = parserutils_vector_peek(vector, temp_ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		flags = FLAG_INHERIT;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else if (token->type == CSS_TOKEN_IDENT &&
			 (lwc_string_caseless_isequal(
			token->idata, c->strings[NORMAL],
			&match) == lwc_error_ok && match)) {
		value = CONTENT_NORMAL;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else if (token->type == CSS_TOKEN_IDENT &&
			 (lwc_string_caseless_isequal(
			token->idata, c->strings[NONE],
			&match) == lwc_error_ok && match)) {
		value = CONTENT_NONE;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else {
		uint32_t len;

		error = parse_content_list(c, vector, &temp_ctx, &value,
				NULL, &len);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		required_size += len;
	}

	opv = buildOPV(CSS_PROP_CONTENT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy OPV to bytecode */
	ptr = (*result)->bytecode;
	memcpy(ptr, &opv, sizeof(opv));
	ptr += sizeof(opv);

	/* Pass 2: construct bytecode */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			((lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match) ||
			 (lwc_string_caseless_isequal(
			token->idata, c->strings[NORMAL],
			&match) == lwc_error_ok && match) ||
			 (lwc_string_caseless_isequal(
			token->idata, c->strings[NONE],
			&match) == lwc_error_ok && match))) {
		parserutils_vector_iterate(vector, ctx);
	} else {
		error = parse_content_list(c, vector, ctx, NULL, ptr, NULL);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}
	}

	return CSS_OK;
}
