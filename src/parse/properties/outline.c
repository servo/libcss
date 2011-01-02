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
 * Parse outline shorthand
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
css_error parse_outline(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style **result)
{
	int orig_ctx = *ctx;
	int prev_ctx;
	const css_token *token;
	css_style *color = NULL;
	css_style *style = NULL;
	css_style *width = NULL;
	css_style *ret = NULL;
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
			3 * sizeof(uint32_t), &ret);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		bytecode = (uint32_t *) ret->bytecode;

		*(bytecode++) = buildOPV(CSS_PROP_OUTLINE_COLOR,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_OUTLINE_STYLE,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_OUTLINE_WIDTH,
				FLAG_INHERIT, 0);

		parserutils_vector_iterate(vector, ctx);

		*result = ret;

		return CSS_OK;
	} else if (token == NULL) {
		/* No tokens -- clearly garbage */
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	/* Attempt to parse individual properties */
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

		if (color == NULL &&
				(error = parse_outline_color(c, vector, 
				ctx, &color)) == CSS_OK) {
		} else if (style == NULL &&
				(error = parse_outline_style(c, vector, 
				ctx, &style)) == CSS_OK) {
		} else if (width == NULL &&
				(error = parse_outline_width(c, vector, 
				ctx, &width)) == CSS_OK) {
		}

		if (error == CSS_OK) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
		} else {
			/* Forcibly cause loop to exit */
			token = NULL;
		}
	} while (*ctx != prev_ctx && token != NULL);

	/* Calculate size of resultant style */
	required_size = 0;
	if (color)
		required_size += color->length;
	else
		required_size += sizeof(uint32_t);

	if (style)
		required_size += style->length;
	else
		required_size += sizeof(uint32_t);

	if (width)
		required_size += width->length;
	else
		required_size += sizeof(uint32_t);

	error = css_stylesheet_style_create(c->sheet, required_size, &ret);
	if (error != CSS_OK)
		goto cleanup;

	required_size = 0;

	if (color) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				color->bytecode, color->length);
		required_size += color->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_OUTLINE_COLOR,
				0, OUTLINE_COLOR_INVERT);
		required_size += sizeof(uint32_t);
	}

	if (style) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				style->bytecode, style->length);
		required_size += style->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_OUTLINE_STYLE,
				0, OUTLINE_STYLE_NONE);
		required_size += sizeof(uint32_t);
	}

	if (width) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				width->bytecode, width->length);
		required_size += width->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_OUTLINE_WIDTH,
				0, OUTLINE_WIDTH_MEDIUM);
		required_size += sizeof(uint32_t);
	}

	assert(required_size == ret->length);

	/* Write the result */
	*result = ret;
	/* Invalidate ret, so that cleanup doesn't destroy it */
	ret = NULL;

	/* Clean up after ourselves */
cleanup:
	if (color)
		css_stylesheet_style_destroy(c->sheet, color, error == CSS_OK);
	if (style)
		css_stylesheet_style_destroy(c->sheet, style, error == CSS_OK);
	if (width)
		css_stylesheet_style_destroy(c->sheet, width, error == CSS_OK);
	if (ret)
		css_stylesheet_style_destroy(c->sheet, ret, error == CSS_OK);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}
