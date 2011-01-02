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
 * Parse pause shorthand
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
css_error parse_pause(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	const css_token *token;
	css_style *before = NULL;
	css_style *after = NULL;
	css_style *ret = NULL;
	int num_read = 0;
	int prev_ctx;
	uint32_t required_size;
	bool match;
	css_error error;

	/* Deal with inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && token->type == CSS_TOKEN_IDENT &&
		(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		uint32_t *bytecode;

		error = css_stylesheet_style_create(c->sheet,
				2 * sizeof(uint32_t), &ret);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		bytecode = (uint32_t *) ret->bytecode;

		*(bytecode++) = buildOPV(CSS_PROP_PAUSE_BEFORE, 
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_PAUSE_AFTER, 
				FLAG_INHERIT, 0);

		parserutils_vector_iterate(vector, ctx);

		*result = ret;

		return CSS_OK;
	} else if (token == NULL) {
		/* No tokens -- clearly garbage */
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	/* Attempt to read 1 or 2 pauses */
	do {
		prev_ctx = *ctx;
		error = CSS_OK;

		/* Ensure that we're not about to parse another inherit */
		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
				token->idata, c->strings[INHERIT],
				&match) == lwc_error_ok && match)) {
			error = CSS_INVALID;
			goto cleanup;
		}

		if (before == NULL && (error = parse_pause_before(c, vector, 
				ctx, &before)) == CSS_OK) {
			num_read = 1;
		} else if (after == NULL && 
				(error = parse_pause_after(c, vector, ctx, 
				&after)) == CSS_OK) {
			num_read = 2;
		}

		if (error == CSS_OK) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
		} else {
			token = NULL;
		}
	} while (*ctx != prev_ctx && token != NULL);

	if (num_read == 0) {
		error = CSS_INVALID;
		goto cleanup;
	}

	/* Calculate size of resultant style */
	if (num_read == 1)
		required_size = 2 * before->length;
	else
		required_size = before->length + after->length;

	error = css_stylesheet_style_create(c->sheet, required_size, &ret);
	if (error != CSS_OK)
		goto cleanup;

	required_size = 0;

	if (num_read == 1) {
		uint32_t *opv = ((uint32_t *) before->bytecode);
		uint8_t flags = getFlags(*opv);
		uint16_t value = getValue(*opv);

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				before->bytecode, before->length);
		required_size += before->length;

		*opv = buildOPV(CSS_PROP_PAUSE_AFTER, flags, value);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				before->bytecode, before->length);
		required_size += before->length;
	} else {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				before->bytecode, before->length);
		required_size += before->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				after->bytecode, after->length);
		required_size += after->length;
	}

	assert(required_size == ret->length);

	/* Write the result */
	*result = ret;
	/* Invalidate ret so that cleanup doesn't destroy it */
	ret = NULL;

	/* Clean up after ourselves */
cleanup:
	if (before)
		css_stylesheet_style_destroy(c->sheet, before, error == CSS_OK);
	if (after)
		css_stylesheet_style_destroy(c->sheet, after, error == CSS_OK);
	if (ret)
		css_stylesheet_style_destroy(c->sheet, ret, error == CSS_OK);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}

