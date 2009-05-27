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

static css_error parse_border_side_color(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result);
static css_error parse_border_side_style(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result);
static css_error parse_border_side_width(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result);

css_error parse_border_bottom_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, 
			CSS_PROP_BORDER_BOTTOM_COLOR, result);
}

css_error parse_border_bottom_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, 
			CSS_PROP_BORDER_BOTTOM_STYLE, result);
}

css_error parse_border_bottom_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, 
			CSS_PROP_BORDER_BOTTOM_WIDTH, result);
}

css_error parse_border_collapse(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (collapse, separate, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[COLLAPSE]) {
		value = BORDER_COLLAPSE_COLLAPSE;
	} else if (ident->ilower == c->strings[SEPARATE]) {
		value = BORDER_COLLAPSE_SEPARATE;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_BORDER_COLLAPSE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_left_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, 
			CSS_PROP_BORDER_LEFT_COLOR, result);
}

css_error parse_border_left_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, 
			CSS_PROP_BORDER_LEFT_STYLE, result);
}

css_error parse_border_left_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, 
			CSS_PROP_BORDER_LEFT_WIDTH, result);
}

css_error parse_border_right_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, 
			CSS_PROP_BORDER_RIGHT_COLOR, result);
}

css_error parse_border_right_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, 
			CSS_PROP_BORDER_RIGHT_STYLE, result);
}

css_error parse_border_right_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, 
			CSS_PROP_BORDER_RIGHT_WIDTH, result);
}

css_error parse_border_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length[2] = { 0 };
	uint32_t unit[2] = { 0 };
	uint32_t required_size;

	/* length length? | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		int num_lengths = 0;

		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length[0], &unit[0]);
		if (error != CSS_OK)
			return error;

		if (unit[0] & UNIT_ANGLE || unit[0] & UNIT_TIME || 
				unit[0] & UNIT_FREQ || unit[0] & UNIT_PCT)
			return CSS_INVALID;

		num_lengths = 1;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, '!') == false) {
			error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
					&length[1], &unit[1]);
			if (error != CSS_OK)
				return error;

			if (unit[1] & UNIT_ANGLE || unit[1] & UNIT_TIME ||
					unit[1] & UNIT_FREQ || 
					unit[1] & UNIT_PCT)
				return CSS_INVALID;

			num_lengths = 2;
		}

		if (num_lengths == 1) {
			/* Only one length specified. Use for both axes. */
			length[1] = length[0];
			unit[1] = unit[0];
		}

		/* Lengths must not be negative */
		if (length[0] < 0 || length[1] < 0)
			return CSS_INVALID;

		value = BORDER_SPACING_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_BORDER_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_SPACING_SET)
		required_size += 2 * (sizeof(length[0]) + sizeof(unit[0]));

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BORDER_SPACING_SET) {
		uint8_t *ptr = ((uint8_t *) (*result)->bytecode) + sizeof(opv);

		memcpy(ptr, &length[0], sizeof(length[0]));
		ptr += sizeof(length[0]);
		memcpy(ptr, &unit[0], sizeof(unit[0]));
		ptr += sizeof(unit[0]);
		memcpy(ptr, &length[1], sizeof(length[1]));
		ptr += sizeof(length[1]);
		memcpy(ptr, &unit[1], sizeof(unit[1]));
	}

	return CSS_OK;
}

css_error parse_border_top_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, 
			CSS_PROP_BORDER_TOP_COLOR, result);
}

css_error parse_border_top_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, 
			CSS_PROP_BORDER_TOP_STYLE, result);
}

css_error parse_border_top_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, 
			CSS_PROP_BORDER_TOP_WIDTH, result);
}

css_error parse_outline_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (invert, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INVERT]) {
		parserutils_vector_iterate(vector, ctx);
		value = OUTLINE_COLOR_INVERT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = OUTLINE_COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_OUTLINE_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == OUTLINE_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == OUTLINE_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_outline_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	uint32_t opv;
	uint16_t value;

	/* Parse as a border style  */
	error = parse_border_side_style(c, vector, ctx, 
			CSS_PROP_OUTLINE_STYLE, result);
	if (error != CSS_OK)
		return error;

	opv = *((uint32_t *) (*result)->bytecode);

	value = getValue(opv);

	/* Hidden is invalid */
	if (value == BORDER_STYLE_HIDDEN)
		return CSS_INVALID;

	return CSS_OK;
}

css_error parse_outline_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/* Parse as border width */
	return parse_border_side_width(c, vector, ctx, 
			CSS_PROP_OUTLINE_WIDTH, result);
}

css_error parse_border_side_color(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result)
{
	css_error error;
	const css_token *token;
	uint32_t opv;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (transparent, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TRANSPARENT]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_COLOR_TRANSPARENT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = BORDER_COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(op, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BORDER_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_border_side_style(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t op, css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (none, hidden, dotted, dashed, solid, double, groove, 
	 * ridge, inset, outset, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NONE]) {
		value = BORDER_STYLE_NONE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = BORDER_STYLE_HIDDEN;
	} else if (ident->ilower == c->strings[DOTTED]) {
		value = BORDER_STYLE_DOTTED;
	} else if (ident->ilower == c->strings[DASHED]) {
		value = BORDER_STYLE_DASHED;
	} else if (ident->ilower == c->strings[SOLID]) {
		value = BORDER_STYLE_SOLID;
	} else if (ident->ilower == c->strings[DOUBLE]) {
		value = BORDER_STYLE_DOUBLE;
	} else if (ident->ilower == c->strings[GROOVE]) {
		value = BORDER_STYLE_GROOVE;
	} else if (ident->ilower == c->strings[RIDGE]) {
		value = BORDER_STYLE_RIDGE;
	} else if (ident->ilower == c->strings[INSET]) {
		value = BORDER_STYLE_INSET;
	} else if (ident->ilower == c->strings[OUTSET]) {
		value = BORDER_STYLE_OUTSET;
	} else
		return CSS_INVALID;

	opv = buildOPV(op, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_side_width(css_language *c,
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

	/* length | IDENT(thin, medium, thick, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[THIN]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_THIN;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[THICK]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_THICK;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit == UNIT_PCT || unit & UNIT_ANGLE ||
				unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Length must be positive */
		if (length < 0)
			return CSS_INVALID;

		value = BORDER_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(op, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BORDER_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

