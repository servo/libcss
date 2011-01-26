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
css_error css__parse_font(css_language *c,
		const parserutils_vector *vector, int *ctx,
		css_style *result)
{
	const css_token *token;
	css_error error;
	int orig_ctx = *ctx;
	int prev_ctx;
	bool style = true;
	bool variant = true;
	bool weight = true;
	bool size = true;
	bool line_height = true;
	bool family = true;
	css_style *style_style;
	css_style *variant_style;
	css_style *weight_style;
	css_style *size_style;
	css_style *line_height_style;
	css_style *family_style;
	int svw;

	/* Firstly, handle inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) 
		return CSS_INVALID;
		
	if (is_css_inherit(c, token)) {
		error = css_stylesheet_style_inherit(result, CSS_PROP_FONT_STYLE);
		if (error != CSS_OK) 
			return error;

		error = css_stylesheet_style_inherit(result, CSS_PROP_FONT_VARIANT);
		if (error != CSS_OK) 
			return error;		

		error = css_stylesheet_style_inherit(result, CSS_PROP_FONT_WEIGHT);
		if (error != CSS_OK) 
			return error;

		error = css_stylesheet_style_inherit(result, CSS_PROP_FONT_SIZE);
		if (error != CSS_OK) 
			return error;

		error = css_stylesheet_style_inherit(result, CSS_PROP_LINE_HEIGHT);
		if (error != CSS_OK) 
			return error;

		error = css_stylesheet_style_inherit(result, CSS_PROP_FONT_FAMILY);
		if (error == CSS_OK) 
			parserutils_vector_iterate(vector, ctx);

		return error;
	} 


	/* allocate styles */
	error = css__stylesheet_style_create(c->sheet, &style_style);
	if (error != CSS_OK) 
		return error;

	error = css__stylesheet_style_create(c->sheet, &variant_style);
	if (error != CSS_OK) {
		css__stylesheet_style_destroy(style_style);
		return error;
	}

	error = css__stylesheet_style_create(c->sheet, &weight_style);
	if (error != CSS_OK) {
		css__stylesheet_style_destroy(style_style);
		css__stylesheet_style_destroy(variant_style);
		return error;
	}

	error = css__stylesheet_style_create(c->sheet, &size_style);
	if (error != CSS_OK) {
		css__stylesheet_style_destroy(style_style);
		css__stylesheet_style_destroy(variant_style);
		css__stylesheet_style_destroy(weight_style);
		return error;
	}

	error = css__stylesheet_style_create(c->sheet, &line_height_style);
	if (error != CSS_OK) {
		css__stylesheet_style_destroy(style_style);
		css__stylesheet_style_destroy(variant_style);
		css__stylesheet_style_destroy(weight_style);
		css__stylesheet_style_destroy(size_style);
		return error;
	}

	error = css__stylesheet_style_create(c->sheet, &family_style);
	if (error != CSS_OK) {
		css__stylesheet_style_destroy(style_style);
		css__stylesheet_style_destroy(variant_style);
		css__stylesheet_style_destroy(weight_style);
		css__stylesheet_style_destroy(size_style);
		css__stylesheet_style_destroy(line_height_style);
		return error;
	}


	/* Attempt to parse the optional style, variant, and weight */
	for (svw = 0; svw < 3; svw++) {
		prev_ctx = *ctx;
		error = CSS_OK;

		/* Ensure that we're not about to parse another inherit */
		token = parserutils_vector_peek(vector, *ctx);
		if ((token != NULL) && is_css_inherit(c, token)) {
			error = CSS_INVALID;
			goto css__parse_font_cleanup;
		}

		if ((style) && 
		    (error = css__parse_font_style(c, vector,
					ctx, style_style)) == CSS_OK) {
			style = false;
		} else if ((variant) && 
			   (error = css__parse_font_variant(c, vector, ctx,
					variant_style)) == CSS_OK) {
			variant = false;
		} else if ((weight) && 
			   (error = css__parse_font_weight(c, vector, ctx,
				weight_style)) == CSS_OK) {
			weight = false;
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
	if ((token != NULL) && is_css_inherit(c, token)) {
		error = CSS_INVALID;
		goto css__parse_font_cleanup;
	}

	/* Now expect a font-size */
	error = css__parse_font_size(c, vector, ctx, size_style);
	if (error != CSS_OK)
		goto css__parse_font_cleanup;
	size = false;

	consumeWhitespace(vector, ctx);

	/* Potential line-height */
	token = parserutils_vector_peek(vector, *ctx);
	if ((token != NULL) && tokenIsChar(token, '/')) {
		parserutils_vector_iterate(vector, ctx);

		consumeWhitespace(vector, ctx);

		/* Ensure that we're not about to parse another inherit */
		token = parserutils_vector_peek(vector, *ctx);
		if ((token != NULL) && is_css_inherit(c, token)) {
			error = CSS_INVALID;
			goto css__parse_font_cleanup;
		}

		error = css__parse_line_height(c, vector, ctx, line_height_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

		line_height = false;
	}

	consumeWhitespace(vector, ctx);

	/* Ensure that we're not about to parse another inherit */
	token = parserutils_vector_peek(vector, *ctx);
	if ((token != NULL) && is_css_inherit(c, token)) {
		error = CSS_INVALID;
		goto css__parse_font_cleanup;
	}

	/* Now expect a font-family */
	error = css__parse_font_family(c, vector, ctx, family_style);
	if (error != CSS_OK)
		goto css__parse_font_cleanup;
	family = false;

	/* Must have size and family */
	assert(size != true);
	assert(family != true);


	/* defaults */
	if (style) {
		error = css__stylesheet_style_appendOPV(style_style, 
				CSS_PROP_FONT_STYLE, 0, 
				FONT_STYLE_NORMAL);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;
	}

	if (variant) {
		error = css__stylesheet_style_appendOPV(variant_style, 
				CSS_PROP_FONT_VARIANT, 0, 
				FONT_VARIANT_NORMAL);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;
	}

	if (weight) {
		error = css__stylesheet_style_appendOPV(weight_style, 
				CSS_PROP_FONT_WEIGHT,
				0, FONT_WEIGHT_NORMAL);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;
	}

	if (line_height) {
		error = css__stylesheet_style_appendOPV(line_height_style, 
				CSS_PROP_LINE_HEIGHT,
				0, LINE_HEIGHT_NORMAL);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;
	}

	/* merge final output */
	error = css__stylesheet_merge_style(result, style_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

	error = css__stylesheet_merge_style(result, variant_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

	error = css__stylesheet_merge_style(result, weight_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

	error = css__stylesheet_merge_style(result, size_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

	error = css__stylesheet_merge_style(result, line_height_style);
		if (error != CSS_OK)
			goto css__parse_font_cleanup;

	error = css__stylesheet_merge_style(result, family_style);



css__parse_font_cleanup:
	css__stylesheet_style_destroy(style_style);
	css__stylesheet_style_destroy(variant_style);
	css__stylesheet_style_destroy(weight_style);
	css__stylesheet_style_destroy(size_style);
	css__stylesheet_style_destroy(line_height_style);
	css__stylesheet_style_destroy(family_style);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}






