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
 * Parse border shorthand
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
css_error parse_border(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style **result)
{
	int orig_ctx = *ctx;
	css_style *top = NULL;
	css_style *right = NULL;
	css_style *bottom = NULL;
	css_style *left = NULL;
	css_style *ret = NULL;
	uint32_t required_size;
	css_error error;

	error = parse_border_side(c, vector, ctx, BORDER_SIDE_TOP, &top);
	if (error != CSS_OK)
		goto cleanup;

	*ctx = orig_ctx;
	error = parse_border_side(c, vector, ctx, BORDER_SIDE_RIGHT, &right);
	if (error != CSS_OK)
		goto cleanup;

	*ctx = orig_ctx;
	error = parse_border_side(c, vector, ctx, BORDER_SIDE_BOTTOM, &bottom);
	if (error != CSS_OK)
		goto cleanup;

	*ctx = orig_ctx;
	error = parse_border_side(c, vector, ctx, BORDER_SIDE_LEFT, &left);
	if (error != CSS_OK)
		goto cleanup;

	required_size = top->length + right->length + 
			bottom->length + left->length;

	error = css_stylesheet_style_create(c->sheet, required_size, &ret);
	if (error != CSS_OK)
		goto cleanup;

	required_size = 0;

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

	assert(required_size == ret->length);

	*result = ret;
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
