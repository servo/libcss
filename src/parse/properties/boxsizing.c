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

css_error parse_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
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
		value = HEIGHT_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative height is illegal */
		if (length < 0)
			return CSS_INVALID;

		value = HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_line_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* number | length | percentage | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = LINE_HEIGHT_NORMAL;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		length = number_from_lwc_string(token->ilower, false, &consumed);
		if (consumed != lwc_string_length(token->ilower))
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		parserutils_vector_iterate(vector, ctx);
		value = LINE_HEIGHT_NUMBER;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		value = LINE_HEIGHT_DIMENSION;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_LINE_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LINE_HEIGHT_NUMBER)
		required_size += sizeof(length);
	else if ((flags & FLAG_INHERIT) == false && 
			value == LINE_HEIGHT_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && (value == LINE_HEIGHT_NUMBER || 
			value == LINE_HEIGHT_DIMENSION))
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
	if ((flags & FLAG_INHERIT) == false && value == LINE_HEIGHT_DIMENSION)
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));

	return CSS_OK;
}

css_error parse_max_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(none, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NONE]) {
		parserutils_vector_iterate(vector, ctx);
		value = MAX_HEIGHT_NONE;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		value = MAX_HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_MAX_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MAX_HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MAX_HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_max_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(none, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NONE]) {
		parserutils_vector_iterate(vector, ctx);
		value = MAX_WIDTH_NONE;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		value = MAX_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_MAX_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MAX_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags && FLAG_INHERIT) == false && value == MAX_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_min_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		value = MIN_HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_MIN_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MIN_HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MIN_HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_min_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are illegal */
		if (length < 0)
			return CSS_INVALID;

		value = MIN_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_MIN_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MIN_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MIN_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
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
		value = WIDTH_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Must be positive */
		if (length < 0)
			return CSS_INVALID;

		value = WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

