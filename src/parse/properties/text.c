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

/**
 * Parse color
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_color(css_language *c, 
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

	/* colour | IDENT (inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		value = COLOR_SET;
	}

	opv = buildOPV(CSS_PROP_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

/**
 * Parse direction
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_direction(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (ltr, rtl, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[LTR]) {
		value = DIRECTION_LTR;
	} else if (ident->ilower == c->strings[RTL]) {
		value = DIRECTION_RTL;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_DIRECTION, flags, value);

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
 * Parse letter-spacing
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_letter_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = LETTER_SPACING_NORMAL;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ ||
				unit & UNIT_PCT) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = LETTER_SPACING_SET;
	}

	opv = buildOPV(CSS_PROP_LETTER_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LETTER_SPACING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == LETTER_SPACING_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse text-align
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_text_align(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (left, right, center, justify, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[LEFT]) {
		value = TEXT_ALIGN_LEFT;
	} else if (ident->ilower == c->strings[RIGHT]) {
		value = TEXT_ALIGN_RIGHT;
	} else if (ident->ilower == c->strings[CENTER]) {
		value = TEXT_ALIGN_CENTER;
	} else if (ident->ilower == c->strings[JUSTIFY]) {
		value = TEXT_ALIGN_JUSTIFY;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_TEXT_ALIGN, flags, value);

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
 * Parse text-decoration
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_text_decoration(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT([ underline || overline || line-through || blink ])
	 * | IDENT (none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NONE]) {
		value = TEXT_DECORATION_NONE;
	} else {
		while (ident != NULL) {
			if (ident->ilower == c->strings[UNDERLINE]) {
				if ((value & TEXT_DECORATION_UNDERLINE) == 0)
					value |= TEXT_DECORATION_UNDERLINE;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if (ident->ilower == c->strings[OVERLINE]) {
				if ((value & TEXT_DECORATION_OVERLINE) == 0)
					value |= TEXT_DECORATION_OVERLINE;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if (ident->ilower == c->strings[LINE_THROUGH]) {
				if ((value & TEXT_DECORATION_LINE_THROUGH) == 0)
					value |= TEXT_DECORATION_LINE_THROUGH;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else if (ident->ilower == c->strings[BLINK]) {
				if ((value & TEXT_DECORATION_BLINK) == 0)
					value |= TEXT_DECORATION_BLINK;
				else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			consumeWhitespace(vector, ctx);

			ident = parserutils_vector_peek(vector, *ctx);
			if (ident != NULL && ident->type != CSS_TOKEN_IDENT)
				break;
			ident = parserutils_vector_iterate(vector, ctx);
		}
	}

	opv = buildOPV(CSS_PROP_TEXT_DECORATION, flags, value);

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
 * Parse text-indent
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_text_indent(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
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
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = TEXT_INDENT_SET;
	}

	opv = buildOPV(CSS_PROP_TEXT_INDENT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == TEXT_INDENT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == TEXT_INDENT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse text-transform
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_text_transform(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (capitalize, uppercase, lowercase, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[CAPITALIZE]) {
		value = TEXT_TRANSFORM_CAPITALIZE;
	} else if (ident->ilower == c->strings[UPPERCASE]) {
		value = TEXT_TRANSFORM_UPPERCASE;
	} else if (ident->ilower == c->strings[LOWERCASE]) {
		value = TEXT_TRANSFORM_LOWERCASE;
	} else if (ident->ilower == c->strings[NONE]) {
		value = TEXT_TRANSFORM_NONE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_TEXT_TRANSFORM, flags, value);

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
 * Parse unicode-bidi
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_unicode_bidi(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, embed, bidi-override, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = UNICODE_BIDI_NORMAL;
	} else if (ident->ilower == c->strings[EMBED]) {
		value = UNICODE_BIDI_EMBED;
	} else if (ident->ilower == c->strings[BIDI_OVERRIDE]) {
		value = UNICODE_BIDI_BIDI_OVERRIDE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_UNICODE_BIDI, flags, value);

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
 * Parse white-space
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_white_space(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, pre, nowrap, pre-wrap, pre-line, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = WHITE_SPACE_NORMAL;
	} else if (ident->ilower == c->strings[PRE]) {
		value = WHITE_SPACE_PRE;
	} else if (ident->ilower == c->strings[NOWRAP]) {
		value = WHITE_SPACE_NOWRAP;
	} else if (ident->ilower == c->strings[PRE_WRAP]) {
		value = WHITE_SPACE_PRE_WRAP;
	} else if (ident->ilower == c->strings[PRE_LINE]) {
		value = WHITE_SPACE_PRE_LINE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_WHITE_SPACE, flags, value);

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
 * Parse word-spacing
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_word_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = WORD_SPACING_NORMAL;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ ||
				unit & UNIT_PCT) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = WORD_SPACING_SET;
	}

	opv = buildOPV(CSS_PROP_WORD_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == WORD_SPACING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == WORD_SPACING_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

