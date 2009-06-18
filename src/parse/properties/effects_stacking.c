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

css_error parse_clip(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	int num_lengths = 0;
	css_fixed length[4] = { 0 };
	uint32_t unit[4] = { 0 };
	uint32_t required_size;

	/* FUNCTION(rect) [ [ IDENT(auto) | length ] CHAR(,)? ]{3} 
	 *                [ IDENT(auto) | length ] CHAR{)} |
	 * IDENT(auto, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		value = CLIP_AUTO;
	} else if (token->type == CSS_TOKEN_FUNCTION &&
			token->ilower == c->strings[RECT]) {
		int i;
		value = CLIP_SHAPE_RECT;

		for (i = 0; i < 4; i++) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL)
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_IDENT) {
				/* Slightly magical way of generating the auto 
				 * values. These are bits 3-6 of the value. */
				if (token->ilower == c->strings[AUTO])
					value |= 1 << (i + 3);
				else
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);
			} else {
				error = parse_unit_specifier(c, vector, ctx, 
						UNIT_PX, 
						&length[num_lengths], 
						&unit[num_lengths]);
				if (error != CSS_OK)
					return error;

				if (unit[num_lengths] & UNIT_ANGLE || 
						unit[num_lengths] & UNIT_TIME ||
						unit[num_lengths] & UNIT_FREQ ||
						unit[num_lengths] & UNIT_PCT)
					return CSS_INVALID;

				num_lengths++;
			}

			consumeWhitespace(vector, ctx);

			/* Consume optional comma after first 3 parameters */
			if (i < 3) {
				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL)
					return CSS_INVALID;

				if (tokenIsChar(token, ','))
					parserutils_vector_iterate(vector, ctx);
			}
		}

		consumeWhitespace(vector, ctx);

		/* Finally, consume closing parenthesis */
		token = parserutils_vector_iterate(vector, ctx);
		if (token == NULL || tokenIsChar(token, ')') == false)
			return CSS_INVALID;
	} else {
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_CLIP, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && 
			(value & CLIP_SHAPE_MASK) == CLIP_SHAPE_RECT) {
		required_size += 
			num_lengths * (sizeof(length[0]) + sizeof(unit[0]));
	}

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && 
			(value & CLIP_SHAPE_MASK) == CLIP_SHAPE_RECT) {
		int i;
		uint8_t *ptr = ((uint8_t *) (*result)->bytecode) + sizeof(opv);

		for (i = 0; i < num_lengths; i++) {
			memcpy(ptr, &length[i], sizeof(length[i]));
			ptr += sizeof(length[i]);
			memcpy(ptr, &unit[i], sizeof(unit[i]));
			ptr += sizeof(unit[i]);
		}
	}

	return CSS_OK;
}

css_error parse_overflow(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (visible, hidden, scroll, auto, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[VISIBLE]) {
		value = OVERFLOW_VISIBLE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = OVERFLOW_HIDDEN;
	} else if (ident->ilower == c->strings[SCROLL]) {
		value = OVERFLOW_SCROLL;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = OVERFLOW_AUTO;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_OVERFLOW, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_visibility(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (visible, hidden, collapse, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[VISIBLE]) {
		value = VISIBILITY_VISIBLE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = VISIBILITY_HIDDEN;
	} else if (ident->ilower == c->strings[COLLAPSE]) {
		value = VISIBILITY_COLLAPSE;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_VISIBILITY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_z_index(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed num = 0;
	uint32_t required_size;

	/* <integer> | IDENT (auto, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		value = Z_INDEX_AUTO;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		num = number_from_lwc_string(token->ilower, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower))
			return CSS_INVALID;

		value = Z_INDEX_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_Z_INDEX, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == Z_INDEX_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == Z_INDEX_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

