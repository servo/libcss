/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

static css_error parse_margin_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result);

css_error parse_margin_bottom(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, 
			CSS_PROP_MARGIN_BOTTOM, result);
}

css_error parse_margin_left(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, CSS_PROP_MARGIN_LEFT, result);
}

css_error parse_margin_right(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, CSS_PROP_MARGIN_RIGHT, result);
}

css_error parse_margin_top(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, CSS_PROP_MARGIN_TOP, result);
}

css_error parse_margin_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = MARGIN_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MARGIN_SET;
	}

	opv = buildOPV(op, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MARGIN_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MARGIN_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

