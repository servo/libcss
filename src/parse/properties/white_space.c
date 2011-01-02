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
 * Parse white-space
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_white_space(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	bool match;

	/* IDENT (normal, pre, nowrap, pre-wrap, pre-line, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		flags |= FLAG_INHERIT;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[NORMAL],
			&match) == lwc_error_ok && match)) {
		value = WHITE_SPACE_NORMAL;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[PRE],
			&match) == lwc_error_ok && match)) {
		value = WHITE_SPACE_PRE;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[NOWRAP],
			&match) == lwc_error_ok && match)) {
		value = WHITE_SPACE_NOWRAP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[PRE_WRAP],
			&match) == lwc_error_ok && match)) {
		value = WHITE_SPACE_PRE_WRAP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[PRE_LINE],
			&match) == lwc_error_ok && match)) {
		value = WHITE_SPACE_PRE_LINE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_WHITE_SPACE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}
