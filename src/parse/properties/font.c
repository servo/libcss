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
 * Parse font
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
css_error parse_font(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style **result)
{
	int orig_ctx = *ctx;
	int prev_ctx;
	const css_token *token;
	css_style *style = NULL;
	css_style *variant = NULL;
	css_style *weight = NULL;
	css_style *size = NULL;
	css_style *line_height = NULL;
	css_style *family = NULL;
	css_style *ret = NULL;
	uint32_t required_size;
	bool match;
	int svw;
	css_error error;

	/* Firstly, handle inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		uint32_t *bytecode;

		error = css_stylesheet_style_create(c->sheet, 
				6 * sizeof(uint32_t), &ret);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		bytecode = (uint32_t *) ret->bytecode;

		*(bytecode++) = buildOPV(CSS_PROP_FONT_STYLE, 
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_FONT_VARIANT,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_FONT_WEIGHT,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_FONT_SIZE,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_LINE_HEIGHT,
				FLAG_INHERIT, 0);
		*(bytecode++) = buildOPV(CSS_PROP_FONT_FAMILY,
				FLAG_INHERIT, 0);

		parserutils_vector_iterate(vector, ctx);

		*result = ret;

		return CSS_OK;
	} else if (token == NULL) {
		/* No tokens -- clearly garbage */
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	/* Attempt to parse the optional style, variant, and weight */
	for (svw = 0; svw < 3; svw++) {
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

		if (style == NULL && 
				(error = parse_font_style(c, vector,
				ctx, &style)) == CSS_OK) {
		} else if (variant == NULL && 
				(error = parse_font_variant(c, vector, ctx,
				&variant)) == CSS_OK) {
		} else if (weight == NULL && 
				(error = parse_font_weight(c, vector, ctx,
				&weight)) == CSS_OK) {
		}

		if (error == CSS_OK) {
			consumeWhitespace(vector, ctx);
		} else {
			break;
		}

		if (*ctx == prev_ctx)
			break;
	}

	consumeWhitespace(vector, ctx);

	/* Ensure that we're not about to parse another inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		error = CSS_INVALID;
		goto cleanup;
	}

	/* Now expect a font-size */
	error = parse_font_size(c, vector, ctx, &size);
	if (error != CSS_OK)
		goto cleanup;

	consumeWhitespace(vector, ctx);

	/* Potential line-height */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && tokenIsChar(token, '/')) {
		parserutils_vector_iterate(vector, ctx);

		consumeWhitespace(vector, ctx);

		/* Ensure that we're not about to parse another inherit */
		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && token->type == CSS_TOKEN_IDENT &&
				(lwc_string_caseless_isequal(
				token->idata, c->strings[INHERIT],
				&match) == lwc_error_ok && match)) {
			error = CSS_INVALID;
			goto cleanup;
		}

		error = parse_line_height(c, vector, ctx, &line_height);
		if (error != CSS_OK)
			goto cleanup;
	}

	consumeWhitespace(vector, ctx);

	/* Ensure that we're not about to parse another inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token != NULL && token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		error = CSS_INVALID;
		goto cleanup;
	}

	/* Now expect a font-family */
	error = parse_font_family(c, vector, ctx, &family);
	if (error != CSS_OK)
		goto cleanup;

	/* Must have size and family */
	assert(size != NULL);
	assert(family != NULL);

	/* Calculate the required size of the resultant style,
	 * defaulting the unspecified properties to their initial values */
	required_size = 0;

	if (style)
		required_size += style->length;
	else
		required_size += sizeof(uint32_t);

	if (variant)
		required_size += variant->length;
	else
		required_size += sizeof(uint32_t);

	if (weight)
		required_size += weight->length;
	else
		required_size += sizeof(uint32_t);

	required_size += size->length;

	if (line_height)
		required_size += line_height->length;
	else
		required_size += sizeof(uint32_t);

	required_size += family->length;

	/* Create and populate it */
	error = css_stylesheet_style_create(c->sheet, required_size, &ret);
	if (error != CSS_OK)
		goto cleanup;

	required_size = 0;

	if (style) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				style->bytecode, style->length);
		required_size += style->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_FONT_STYLE,
				0, FONT_STYLE_NORMAL);
		required_size += sizeof(uint32_t);
	}

	if (variant) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				variant->bytecode, variant->length);
		required_size += variant->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_FONT_VARIANT,
				0, FONT_VARIANT_NORMAL);
		required_size += sizeof(uint32_t);
	}

	if (weight) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				weight->bytecode, weight->length);
		required_size += weight->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_FONT_WEIGHT,
				0, FONT_WEIGHT_NORMAL);
		required_size += sizeof(uint32_t);
	}

	memcpy(((uint8_t *) ret->bytecode) + required_size,
			size->bytecode, size->length);
	required_size += size->length;

	if (line_height) {
		memcpy(((uint8_t *) ret->bytecode) + required_size,
				line_height->bytecode, line_height->length);
		required_size += line_height->length;
	} else {
		void *bc = ((uint8_t *) ret->bytecode) + required_size;

		*((uint32_t *) bc) = buildOPV(CSS_PROP_LINE_HEIGHT,
				0, LINE_HEIGHT_NORMAL);
		required_size += sizeof(uint32_t);
	}

	memcpy(((uint8_t *) ret->bytecode) + required_size,
			family->bytecode, family->length);
	required_size += family->length;

	assert(required_size == ret->length);

	/* Write the result */
	*result = ret;
	/* Invalidate ret, so that cleanup doesn't destroy it */
	ret = NULL;

	/* Clean up after ourselves */
cleanup:
	if (style)
		css_stylesheet_style_destroy(c->sheet, style, error == CSS_OK);
	if (variant)
		css_stylesheet_style_destroy(c->sheet, variant, error == CSS_OK);
	if (weight)
		css_stylesheet_style_destroy(c->sheet, weight, error == CSS_OK);
	if (size)
		css_stylesheet_style_destroy(c->sheet, size, error == CSS_OK);
	if (line_height)
		css_stylesheet_style_destroy(c->sheet, line_height, error == CSS_OK);
	if (family)
		css_stylesheet_style_destroy(c->sheet, family, error == CSS_OK);
	if (ret)
		css_stylesheet_style_destroy(c->sheet, ret, error == CSS_OK);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}






