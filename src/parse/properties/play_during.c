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
 * Parse play-during
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
css_error parse_play_during(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size;
	lwc_string *uri;
	bool match;

	/* URI [ IDENT(mix) || IDENT(repeat) ]? | IDENT(auto,none,inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		if ((lwc_string_caseless_isequal(
				token->idata, c->strings[INHERIT],
				&match) == lwc_error_ok && match)) {
			flags |= FLAG_INHERIT;
		} else if ((lwc_string_caseless_isequal(
				token->idata, c->strings[NONE],
				&match) == lwc_error_ok && match)) {
			value = PLAY_DURING_NONE;
		} else if ((lwc_string_caseless_isequal(
				token->idata, c->strings[AUTO],
				&match) == lwc_error_ok && match)) {
			value = PLAY_DURING_AUTO;
		} else {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}
	} else {
		int modifiers;

		value = PLAY_DURING_URI;

		error = c->sheet->resolve(c->sheet->resolve_pw,
				c->sheet->url,
				token->idata, &uri);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		for (modifiers = 0; modifiers < 2; modifiers++) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && token->type == CSS_TOKEN_IDENT) {
				if ((lwc_string_caseless_isequal(
						token->idata, c->strings[MIX],
						&match) == lwc_error_ok && 
						match)) {
					if ((value & PLAY_DURING_MIX) == 0)
						value |= PLAY_DURING_MIX;
					else {
						*ctx = orig_ctx;
						return CSS_INVALID;
					}
				} else if (lwc_string_caseless_isequal(
						token->idata, 
						c->strings[REPEAT],
						&match) == lwc_error_ok &&
						match) {
					if ((value & PLAY_DURING_REPEAT) == 0)
						value |= PLAY_DURING_REPEAT;
					else {
						*ctx = orig_ctx;
						return CSS_INVALID;
					}
				} else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}

				parserutils_vector_iterate(vector, ctx);
			}
		}
	}

	opv = buildOPV(CSS_PROP_PLAY_DURING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && 
			(value & PLAY_DURING_TYPE_MASK) == PLAY_DURING_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && 
			(value & PLAY_DURING_TYPE_MASK)	 == PLAY_DURING_URI) {
		/* Don't ref URI -- we want to pass ownership to the bytecode */
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&uri, sizeof(lwc_string *));
	}

	return CSS_OK;
}
