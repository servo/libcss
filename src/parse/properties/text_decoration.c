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
 * Parse text-decoration
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
css_error parse_text_decoration(css_language *c, 
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

	/* IDENT([ underline || overline || line-through || blink ])
	 * | IDENT (none, inherit) */
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
			ident->idata, c->strings[NONE],
			&match) == lwc_error_ok && match)) {
		value = TEXT_DECORATION_NONE;
	} else {
		while (ident != NULL) {
			if ((lwc_string_caseless_isequal(
					ident->idata, c->strings[UNDERLINE],
					&match) == lwc_error_ok && match)) {
				if ((value & TEXT_DECORATION_UNDERLINE) == 0)
					value |= TEXT_DECORATION_UNDERLINE;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if ((lwc_string_caseless_isequal(
					ident->idata, c->strings[OVERLINE],
					&match) == lwc_error_ok && match)) {
				if ((value & TEXT_DECORATION_OVERLINE) == 0)
					value |= TEXT_DECORATION_OVERLINE;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if ((lwc_string_caseless_isequal(
					ident->idata, c->strings[LINE_THROUGH],
					&match) == lwc_error_ok && match)) {
				if ((value & TEXT_DECORATION_LINE_THROUGH) == 0)
					value |= TEXT_DECORATION_LINE_THROUGH;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if ((lwc_string_caseless_isequal(
					ident->idata, c->strings[BLINK],
					&match) == lwc_error_ok && match)) {
				if ((value & TEXT_DECORATION_BLINK) == 0)
					value |= TEXT_DECORATION_BLINK;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			consumeWhitespace(vector, ctx);

			ident = parserutils_vector_peek(vector, *ctx);
			if (ident != NULL && ident->type != CSS_TOKEN_IDENT)
				break;
			ident = parserutils_vector_iterate(vector, ctx);
		}
	}

	opv = buildOPV(CSS_PROP_TEXT_DECORATION, flags, value);

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
