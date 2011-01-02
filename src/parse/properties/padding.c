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
 * Parse padding shorthand
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
css_error parse_padding(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style **result)
{
	int orig_ctx = *ctx;
	int prev_ctx;
	const css_token *token;
	css_style *top = NULL;
	css_style *right = NULL;
	css_style *bottom = NULL;
	css_style *left = NULL;
	css_style *ret = NULL;
	uint32_t num_sides = 0;
	uint32_t required_size;
	bool match;
	css_error error;

	/* Firstly, handle inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		uint32_t *bytecode;

		error = css_stylesheet_style_create(c->sheet,
			4 * sizeof(uint32_t), &ret);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		bytecode = (uint32_t *) ret->bytecode;

		*(bytecode++) = buildOPV(CSS_PROP_PADDING_TOP,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_PADDING_RIGHT,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_PADDING_BOTTOM,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_PADDING_LEFT,
				FLAG_INHERIT, 0);

		parserutils_vector_iterate(vector, ctx);

		*result = ret;

		return CSS_OK;
	} else if (token == NULL) {
		/* No tokens -- clearly garbage */
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	/* Attempt to parse up to 4 widths */
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

		if (top == NULL &&
				(error = parse_padding_side(c, vector, ctx, 
				CSS_PROP_PADDING_TOP, &top)) == CSS_OK) {
			num_sides = 1;
		} else if (right == NULL &&
				(error = parse_padding_side(c, vector, ctx, 
				CSS_PROP_PADDING_RIGHT, &right)) == CSS_OK) {
			num_sides = 2;
		} else if (bottom == NULL &&
				(error = parse_padding_side(c, vector, ctx, 
				CSS_PROP_PADDING_BOTTOM, &bottom)) == CSS_OK) {
			num_sides = 3;
		} else if (left == NULL &&
				(error = parse_padding_side(c, vector, ctx, 
				CSS_PROP_PADDING_LEFT, &left)) == CSS_OK) {
			num_sides = 4;
		}

		if (error == CSS_OK) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
		} else {
			/* Forcibly cause loop to exit */
			token = NULL;
		}
	} while (*ctx != prev_ctx && token != NULL);

	if (num_sides == 0) {
		error = CSS_INVALID;
		goto cleanup;
	}

	/* Calculate size of resultant style */
	if (num_sides == 1) {
		required_size = 4 * top->length;
	} else if (num_sides == 2) {
		required_size = 2 * top->length + 2 * right->length;
	} else if (num_sides == 3) {
		required_size = top->length + 2 * right->length + 
				bottom->length;
	} else {
		required_size = top->length + right->length +
				bottom->length + left->length;
	}

	error = css_stylesheet_style_create(c->sheet, required_size, &ret);
	if (error != CSS_OK)
		goto cleanup;

	required_size = 0;

	if (num_sides == 1) {
		uint32_t *opv = ((uint32_t *) top->bytecode);
		uint8_t flags = getFlags(*opv);
		uint16_t value = getValue(*opv);

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		*opv = buildOPV(CSS_PROP_PADDING_RIGHT, flags, value);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		*opv = buildOPV(CSS_PROP_PADDING_BOTTOM, flags, value);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		*opv = buildOPV(CSS_PROP_PADDING_LEFT, flags, value);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;
	} else if (num_sides == 2) {
		uint32_t *vopv = ((uint32_t *) top->bytecode);
		uint32_t *hopv = ((uint32_t *) right->bytecode);
		uint8_t vflags = getFlags(*vopv);
		uint8_t hflags = getFlags(*hopv);
		uint16_t vvalue = getValue(*vopv);
		uint16_t hvalue = getValue(*hopv);

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				right->bytecode, right->length);
		required_size += right->length;

		*vopv = buildOPV(CSS_PROP_PADDING_BOTTOM, vflags, vvalue);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		*hopv = buildOPV(CSS_PROP_PADDING_LEFT, hflags, hvalue);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				right->bytecode, right->length);
		required_size += right->length;
	} else if (num_sides == 3) {
		uint32_t *opv = ((uint32_t *) right->bytecode);
		uint8_t flags = getFlags(*opv);
		uint16_t value = getValue(*opv);

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				right->bytecode, right->length);
		required_size += right->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				bottom->bytecode, bottom->length);
		required_size += bottom->length;

		*opv = buildOPV(CSS_PROP_PADDING_LEFT, flags, value);
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				right->bytecode, right->length);
		required_size += right->length;
	} else {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				top->bytecode, top->length);
		required_size += top->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				right->bytecode, right->length);
		required_size += right->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				bottom->bytecode, bottom->length);
		required_size += bottom->length;

		memcpy(((uint8_t *) ret->bytecode) + required_size,
				left->bytecode, left->length);
		required_size += left->length;
	}

	assert(required_size == ret->length);

	/* Write the result */
	*result = ret;
	/* Invalidate ret, so that cleanup doesn't destroy it */
	ret = NULL;

	/* Clean up after ourselves */
cleanup:
	if (top)
		css_stylesheet_style_destroy(c->sheet, top, error == CSS_OK);
	if (right)
		css_stylesheet_style_destroy(c->sheet, right, error == CSS_OK);
	if (bottom)
		css_stylesheet_style_destroy(c->sheet, bottom, error == CSS_OK);
	if (left)
		css_stylesheet_style_destroy(c->sheet, left, error == CSS_OK);
	if (ret)
		css_stylesheet_style_destroy(c->sheet, ret, error == CSS_OK);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}

