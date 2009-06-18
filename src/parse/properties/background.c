/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

/**
 * Parse background-attachment
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not a valid background-attachment
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_background_attachment(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (fixed, scroll, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[FIXED]) {
		value = BACKGROUND_ATTACHMENT_FIXED;
	} else if (ident->ilower == c->strings[SCROLL]) {
		value = BACKGROUND_ATTACHMENT_SCROLL;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_BACKGROUND_ATTACHMENT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

/**
 * Parse background-color
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not a valid background-attachment
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_background_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (transparent, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TRANSPARENT]) {
		parserutils_vector_iterate(vector, ctx);
		value = BACKGROUND_COLOR_TRANSPARENT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		value = BACKGROUND_COLOR_SET;
	}

	opv = buildOPV(CSS_PROP_BACKGROUND_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

/**
 * Parse background-image
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not a valid background-attachment
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_background_image(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size;

	/* URI | IDENT (none, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[NONE]) {
		value = BACKGROUND_IMAGE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = BACKGROUND_IMAGE_URI;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_BACKGROUND_IMAGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_IMAGE_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_IMAGE_URI) {
                lwc_context_string_ref(c->sheet->dictionary, token->idata);
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(lwc_string *));
	}

	return CSS_OK;
}

/**
 * Parse background-position
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not a valid background-attachment
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_background_position(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint32_t opv;
	uint16_t value[2] = { 0 };
	css_fixed length[2] = { 0 };
	uint32_t unit[2] = { 0 };
	uint32_t required_size;

	/* [length | percentage | IDENT(left, right, top, bottom, center)]{1,2}
	 * | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		int i;

		for (i = 0; i < 2; i++) {
			token = parserutils_vector_peek(vector, *ctx);
			/* This can only occur on the second attempt */
			/* Also detect start of !important on second attempt */
			if (token == NULL || 
					(i == 1 && tokenIsChar(token, '!')))
				break;

			if (token->type == CSS_TOKEN_IDENT) {
				parserutils_vector_iterate(vector, ctx);

				if (token->ilower == c->strings[LEFT]) {
					value[i] = 
						BACKGROUND_POSITION_HORZ_LEFT;
				} else if (token->ilower == c->strings[RIGHT]) {
					value[i] = 
						BACKGROUND_POSITION_HORZ_RIGHT;
				} else if (token->ilower == c->strings[TOP]) {
					value[i] = BACKGROUND_POSITION_VERT_TOP;
				} else if (token->ilower == 
						c->strings[BOTTOM]) {
					value[i] = 
						BACKGROUND_POSITION_VERT_BOTTOM;
				} else if (token->ilower == 
						c->strings[CENTER]) {
					/* We'll fix this up later */
					value[i] = 
						BACKGROUND_POSITION_VERT_CENTER;
				} else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else {
				error = parse_unit_specifier(c, vector, ctx, 
						UNIT_PX, &length[i], &unit[i]);
				if (error != CSS_OK) {
					*ctx = orig_ctx;
					return error;
				}

				if (unit[i] & UNIT_ANGLE || 
						unit[i] & UNIT_TIME || 
						unit[i] & UNIT_FREQ) {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}

				/* We'll fix this up later, too */
				value[i] = BACKGROUND_POSITION_VERT_SET;
			}

			consumeWhitespace(vector, ctx);
		}

		/* Now, sort out the mess we've got */
		if (i == 1) {
			assert(BACKGROUND_POSITION_VERT_CENTER ==
					BACKGROUND_POSITION_HORZ_CENTER);

			/* Only one value, so the other is center */
			switch (value[0]) {
			case BACKGROUND_POSITION_HORZ_LEFT:
			case BACKGROUND_POSITION_HORZ_RIGHT:
			case BACKGROUND_POSITION_VERT_CENTER:
			case BACKGROUND_POSITION_VERT_TOP:
			case BACKGROUND_POSITION_VERT_BOTTOM:
				break;
			case BACKGROUND_POSITION_VERT_SET:
				value[0] = BACKGROUND_POSITION_HORZ_SET;
				break;
			default:
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			value[1] = BACKGROUND_POSITION_VERT_CENTER;
		} else if (value[0] != BACKGROUND_POSITION_VERT_SET &&
				value[1] != BACKGROUND_POSITION_VERT_SET) {
			/* Two keywords. Verify the axes differ */
			if (((value[0] & 0xf) != 0 && (value[1] & 0xf) != 0) ||
					((value[0] & 0xf0) != 0 && 
						(value[1] & 0xf0) != 0)) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else {
			/* One or two non-keywords. First is horizontal */
			if (value[0] == BACKGROUND_POSITION_VERT_SET)
				value[0] = BACKGROUND_POSITION_HORZ_SET;

			/* Verify the axes differ */
			if (((value[0] & 0xf) != 0 && (value[1] & 0xf) != 0) ||
					((value[0] & 0xf0) != 0 && 
						(value[1] & 0xf0) != 0)) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		}
	}

	opv = buildOPV(CSS_PROP_BACKGROUND_POSITION, flags, value[0] | value[1]);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false) { 
		if (value[0] == BACKGROUND_POSITION_HORZ_SET)
			required_size += sizeof(length[0]) + sizeof(unit[0]);
		if (value[1] == BACKGROUND_POSITION_VERT_SET)
			required_size += sizeof(length[1]) + sizeof(unit[1]);
	}

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false) {
		uint8_t *ptr = ((uint8_t *) (*result)->bytecode) + sizeof(opv);
		if (value[0] == BACKGROUND_POSITION_HORZ_SET) {
			memcpy(ptr, &length[0], sizeof(length[0]));
			ptr += sizeof(length[0]);
			memcpy(ptr, &unit[0], sizeof(unit[0]));
			ptr += sizeof(unit[0]);
		}
		if (value[1] == BACKGROUND_POSITION_VERT_SET) {
			memcpy(ptr, &length[1], sizeof(length[1]));
			ptr += sizeof(length[1]);
			memcpy(ptr, &unit[1], sizeof(unit[1]));
		}
	}

	return CSS_OK;
}

/**
 * Parse background-repeat
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not a valid background-attachment
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_background_repeat(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (no-repeat, repeat-x, repeat-y, repeat, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NO_REPEAT]) {
		value = BACKGROUND_REPEAT_NO_REPEAT;
	} else if (ident->ilower == c->strings[REPEAT_X]) {
		value = BACKGROUND_REPEAT_REPEAT_X;
	} else if (ident->ilower == c->strings[REPEAT_Y]) {
		value = BACKGROUND_REPEAT_REPEAT_Y;
	} else if (ident->ilower == c->strings[REPEAT]) {
		value = BACKGROUND_REPEAT_REPEAT;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_BACKGROUND_REPEAT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

