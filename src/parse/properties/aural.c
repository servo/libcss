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
 * Parse azimuth
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
css_error parse_azimuth(css_language *c, 
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

	/* angle | [ IDENT(left-side, far-left, left, center-left, center, 
	 *                 center-right, right, far-right, right-side) || 
	 *           IDENT(behind) 
	 *         ] 
	 *       | IDENT(leftwards, rightwards, inherit)
	 */
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
			token->ilower == c->strings[LEFTWARDS]) {
		parserutils_vector_iterate(vector, ctx);
		value = AZIMUTH_LEFTWARDS;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[RIGHTWARDS]) {
		parserutils_vector_iterate(vector, ctx);
		value = AZIMUTH_RIGHTWARDS;
	} else if (token->type == CSS_TOKEN_IDENT) {
		parserutils_vector_iterate(vector, ctx);

		/* Now, we may have one of the other keywords or behind,
		 * potentially followed by behind or other keyword, 
		 * respectively */
		if (token->ilower == c->strings[LEFT_SIDE]) {
			value = AZIMUTH_LEFT_SIDE;
		} else if (token->ilower == c->strings[FAR_LEFT]) {
			value = AZIMUTH_FAR_LEFT;
		} else if (token->ilower == c->strings[LEFT]) {
			value = AZIMUTH_LEFT;
		} else if (token->ilower == c->strings[CENTER_LEFT]) {
			value = AZIMUTH_CENTER_LEFT;
		} else if (token->ilower == c->strings[CENTER]) {
			value = AZIMUTH_CENTER;
		} else if (token->ilower == c->strings[CENTER_RIGHT]) {
			value = AZIMUTH_CENTER_RIGHT;
		} else if (token->ilower == c->strings[RIGHT]) {
			value = AZIMUTH_RIGHT;
		} else if (token->ilower == c->strings[FAR_RIGHT]) {
			value = AZIMUTH_FAR_RIGHT;
		} else if (token->ilower == c->strings[RIGHT_SIDE]) {
			value = AZIMUTH_RIGHT_SIDE;
		} else if (token->ilower == c->strings[BEHIND]) {
			value = AZIMUTH_BEHIND;
		} else {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		consumeWhitespace(vector, ctx);

		/* Get potential following token */
		token = parserutils_vector_peek(vector, *ctx);

		if (token != NULL && token->type == CSS_TOKEN_IDENT &&
				value == AZIMUTH_BEHIND) {
			parserutils_vector_iterate(vector, ctx);

			if (token->ilower == c->strings[LEFT_SIDE]) {
				value |= AZIMUTH_LEFT_SIDE;
			} else if (token->ilower == c->strings[FAR_LEFT]) {
				value |= AZIMUTH_FAR_LEFT;
			} else if (token->ilower == c->strings[LEFT]) {
				value |= AZIMUTH_LEFT;
			} else if (token->ilower == c->strings[CENTER-LEFT]) {
				value |= AZIMUTH_CENTER_LEFT;
			} else if (token->ilower == c->strings[CENTER]) {
				value |= AZIMUTH_CENTER;
			} else if (token->ilower == c->strings[CENTER-RIGHT]) {
				value |= AZIMUTH_CENTER_RIGHT;
			} else if (token->ilower == c->strings[RIGHT]) {
				value |= AZIMUTH_RIGHT;
			} else if (token->ilower == c->strings[FAR_RIGHT]) {
				value |= AZIMUTH_FAR_RIGHT;
			} else if (token->ilower == c->strings[RIGHT_SIDE]) {
				value |= AZIMUTH_RIGHT_SIDE;
			} else {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if (token != NULL && token->type == CSS_TOKEN_IDENT &&
				value != AZIMUTH_BEHIND) {
			parserutils_vector_iterate(vector, ctx);

			if (token->ilower == c->strings[BEHIND]) {
				value |= AZIMUTH_BEHIND;
			} else {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if ((token == NULL || token->type != CSS_TOKEN_IDENT) &&
				value == AZIMUTH_BEHIND) {
			value |= AZIMUTH_CENTER;
		}
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_DEG, 
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_ANGLE) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Valid angles lie between -360 and 360 degrees */
		if (unit == UNIT_DEG) {
			if (length < FMULI(F_360, -1) || length > F_360) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if (unit == UNIT_GRAD) {
			if (length < FMULI(F_400, -1) || length > F_400) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if (unit == UNIT_RAD) {
			if (length < FMULI(F_2PI, -1) || length > F_2PI) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		}

		value = AZIMUTH_ANGLE;
	}

	opv = buildOPV(CSS_PROP_AZIMUTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == AZIMUTH_ANGLE)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == AZIMUTH_ANGLE) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse cue-after
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
css_error parse_cue_after(css_language *c, 
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
		value = CUE_AFTER_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_AFTER_URI;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_CUE_AFTER, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == CUE_AFTER_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_AFTER_URI) {
                lwc_context_string_ref(c->sheet->dictionary, token->idata);
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(lwc_string *));
	}

	return CSS_OK;
}

/**
 * Parse cue-before
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
css_error parse_cue_before(css_language *c, 
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
		value = CUE_BEFORE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_BEFORE_URI;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_CUE_BEFORE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == CUE_BEFORE_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_BEFORE_URI) {
                lwc_context_string_ref(c->sheet->dictionary, token->idata);
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(lwc_string *));
	}

	return CSS_OK;
}

/**
 * Parse elevation
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
css_error parse_elevation(css_language *c, 
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

	/* angle | IDENT(below, level, above, higher, lower, inherit) */
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
			token->ilower == c->strings[BELOW]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_BELOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LEVEL]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_LEVEL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[ABOVE]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_ABOVE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[HIGHER]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_HIGHER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LOWER]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_LOWER;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_DEG,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_ANGLE) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Valid angles lie between -90 and 90 degrees */
		if (unit == UNIT_DEG) {
			if (length < FMULI(F_90, -1) || length > F_90) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if (unit == UNIT_GRAD) {
			if (length < FMULI(F_100, -1) || length > F_100) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		} else if (unit == UNIT_RAD) {
			if (length < FMULI(F_PI_2, -1) || length > F_PI_2) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		}

		value = ELEVATION_ANGLE;
	}

	opv = buildOPV(CSS_PROP_ELEVATION, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == ELEVATION_ANGLE)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == ELEVATION_ANGLE) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse pause-after
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
css_error parse_pause_after(css_language *c, 
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

	/* time | percentage | IDENT(inherit) */
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
		error = parse_unit_specifier(c, vector, ctx, UNIT_S,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_TIME) == false && (unit & UNIT_PCT) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Negative values are illegal */
		if (length < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = PAUSE_AFTER_SET;
	}

	opv = buildOPV(CSS_PROP_PAUSE_AFTER, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_AFTER_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_AFTER_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse pause-before
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
css_error parse_pause_before(css_language *c, 
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

	/* time | percentage | IDENT(inherit) */
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
		error = parse_unit_specifier(c, vector, ctx, UNIT_S,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_TIME) == false && (unit & UNIT_PCT) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Negative values are illegal */
		if (length < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = PAUSE_BEFORE_SET;
	}

	opv = buildOPV(CSS_PROP_PAUSE_BEFORE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_BEFORE_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_BEFORE_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse pitch-range
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
css_error parse_pitch_range(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		num = number_from_lwc_string(token->ilower, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Must be between 0 and 100 */
		if (num < 0 || num > F_100) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = PITCH_RANGE_SET;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_PITCH_RANGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PITCH_RANGE_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PITCH_RANGE_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

/**
 * Parse pitch
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
css_error parse_pitch(css_language *c, 
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

	/* frequency | IDENT(x-low, low, medium, high, x-high, inherit) */
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
			token->ilower == c->strings[X_LOW]) {
		parserutils_vector_iterate(vector, ctx);
		value = PITCH_X_LOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LOW]) {
		parserutils_vector_iterate(vector, ctx);
		value = PITCH_LOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = PITCH_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[HIGH]) {
		parserutils_vector_iterate(vector, ctx);
		value = PITCH_HIGH;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_HIGH]) {
		parserutils_vector_iterate(vector, ctx);
		value = PITCH_X_HIGH;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_HZ,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_FREQ) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Negative values are invalid */
		if (length < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = PITCH_FREQUENCY;
	}

	opv = buildOPV(CSS_PROP_PITCH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PITCH_FREQUENCY)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PITCH_FREQUENCY) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

/**
 * Parse play-during
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
css_error parse_play_during(css_language *c, 
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
	lwc_string *uri;

	/* URI [ IDENT(mix) || IDENT(repeat) ]? | IDENT(auto,none,inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		if (token->ilower == c->strings[INHERIT]) {
			flags |= FLAG_INHERIT;
		} else if (token->ilower == c->strings[NONE]) {
			value = PLAY_DURING_NONE;
		} else if (token->ilower == c->strings[AUTO]) {
			value = PLAY_DURING_AUTO;
		} else {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}
	} else {
		int modifiers;

		value = PLAY_DURING_URI;
		uri = token->idata;

		for (modifiers = 0; modifiers < 2; modifiers++) {
			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && token->type == CSS_TOKEN_IDENT) {
				if (token->ilower == c->strings[MIX]) {
					if ((value & PLAY_DURING_MIX) == 0)
						value |= PLAY_DURING_MIX;
					else {
						*ctx = orig_ctx;
						return CSS_INVALID;
					}
				} else if (token->ilower == 
						c->strings[REPEAT]) {
					if ((value & PLAY_DURING_REPEAT) == 0)
						value |= PLAY_DURING_REPEAT;
					else {
						*ctx = orig_ctx;
						return CSS_INVALID;
					}
				} else {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}

				parserutils_vector_iterate(vector, ctx);
			}
		}
	}

	opv = buildOPV(CSS_PROP_PLAY_DURING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && 
			(value & PLAY_DURING_TYPE_MASK) == PLAY_DURING_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && 
			(value & PLAY_DURING_TYPE_MASK)  == PLAY_DURING_URI) {
		lwc_context_string_ref(c->sheet->dictionary, uri);
                memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&uri, sizeof(lwc_string *));
	}

	return CSS_OK;
}

/**
 * Parse richness
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
css_error parse_richness(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		num = number_from_lwc_string(token->ilower, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Must be between 0 and 100 */
		if (num < 0 || num > F_100) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = RICHNESS_SET;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_RICHNESS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == RICHNESS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == RICHNESS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

/**
 * Parse speak-header
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
css_error parse_speak_header(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (once, always, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[ONCE]) {
		value = SPEAK_HEADER_ONCE;
	} else if (ident->ilower == c->strings[ALWAYS]) {
		value = SPEAK_HEADER_ALWAYS;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_SPEAK_HEADER, flags, value);

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
 * Parse speak-numeral
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
css_error parse_speak_numeral(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (digits, continuous, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[DIGITS]) {
		value = SPEAK_NUMERAL_DIGITS;
	} else if (ident->ilower == c->strings[CONTINUOUS]) {
		value = SPEAK_NUMERAL_CONTINUOUS;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_SPEAK_NUMERAL, flags, value);

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
 * Parse speak-punctuation
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
css_error parse_speak_punctuation(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (code, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[CODE]) {
		value = SPEAK_PUNCTUATION_CODE;
	} else if (ident->ilower == c->strings[NONE]) {
		value = SPEAK_PUNCTUATION_NONE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_SPEAK_PUNCTUATION, flags, value);

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
 * Parse speak
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
css_error parse_speak(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, none, spell-out, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = SPEAK_NORMAL;
	} else if (ident->ilower == c->strings[NONE]) {
		value = SPEAK_NONE;
	} else if (ident->ilower == c->strings[SPELL_OUT]) {
		value = SPEAK_SPELL_OUT;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_SPEAK, flags, value);

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
 * Parse speech-rate
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
css_error parse_speech_rate(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (x-slow, slow, medium, fast, x-fast, faster, slower, 
	 *                 inherit)
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_SLOW]) {
		value = SPEECH_RATE_X_SLOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SLOW]) {
		value = SPEECH_RATE_SLOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		value = SPEECH_RATE_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[FAST]) {
		value = SPEECH_RATE_FAST;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_FAST]) {
		value = SPEECH_RATE_X_FAST;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[FASTER]) {
		value = SPEECH_RATE_FASTER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SLOWER]) {
		value = SPEECH_RATE_SLOWER;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		num = number_from_lwc_string(token->ilower, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Make negative values invalid */
		if (num < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = SPEECH_RATE_SET;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_SPEECH_RATE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == SPEECH_RATE_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == SPEECH_RATE_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

/**
 * Parse stress
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
css_error parse_stress(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		num = number_from_lwc_string(token->ilower, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		if (num < 0 || num > INTTOFIX(100)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = STRESS_SET;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_STRESS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == STRESS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == STRESS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

/**
 * Parse voice-family
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
css_error parse_voice_family(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size = sizeof(opv);
	int temp_ctx = *ctx;
	uint8_t *ptr;

	/* [ IDENT+ | STRING ] [ ',' [ IDENT+ | STRING ] ]* | IDENT(inherit)
	 * 
	 * In the case of IDENT+, any whitespace between tokens is collapsed to
	 * a single space
	 */

	/* Pass 1: validate input and calculate space */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else {
		bool first = true;

		while (token != NULL) {
			if (token->type == CSS_TOKEN_IDENT) {
				/* IDENT+ */
				if (first == false) {
					required_size += sizeof(opv);
				}

				if (token->ilower == c->strings[MALE]) {
					if (first) {
						value = VOICE_FAMILY_MALE;
					}
				} else if (token->ilower == 
						c->strings[FEMALE]) {
					if (first) {
						value = VOICE_FAMILY_FEMALE;
					}
				} else if (token->ilower == c->strings[CHILD]) {
					if (first) {
						value = VOICE_FAMILY_CHILD;
					}
				} else {
					if (first) {
						value = VOICE_FAMILY_IDENT_LIST;
					}

					required_size +=
						sizeof(lwc_string *);

					/* Skip past [ IDENT* S* ]* */
					while (token != NULL) {
						token = parserutils_vector_peek(
								vector, 
								temp_ctx);
						if (token != NULL && 
							token->type != 
							CSS_TOKEN_IDENT &&
								token->type != 
								CSS_TOKEN_S) {
							break;
						}

						/* idents must not match 
						 * generic families */
						if (token != NULL && token->type == CSS_TOKEN_IDENT && 
								(token->ilower == c->strings[MALE] || 
								token->ilower == c->strings[FEMALE] || 
								token->ilower == c->strings[CHILD])) {
							*ctx = orig_ctx;
							return CSS_INVALID;
						}
						token = parserutils_vector_iterate(
							vector, &temp_ctx);
					}
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				/* STRING */
				if (first == false) {
					required_size += sizeof(opv);
				} else {
					value = VOICE_FAMILY_STRING;
				}

				required_size += 
					sizeof(lwc_string *);
			} else {
				/* Invalid token */
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			consumeWhitespace(vector, &temp_ctx);

			/* Look for a comma */
			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, ',')) {
				/* Got one. Move past it */
				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);

				/* Ensure that a valid token follows */
				token = parserutils_vector_peek(vector, 
						temp_ctx);
				if (token == NULL || 
					(token->type != CSS_TOKEN_IDENT && 
						token->type != 
						CSS_TOKEN_STRING)) {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else {
				/* No comma, so we're done */
				break;
			}

			/* Flag that this is no longer the first pass */
			first = false;

			/* Iterate for next chunk */
			token = parserutils_vector_iterate(vector, &temp_ctx);
		}

		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_VOICE_FAMILY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy OPV to bytecode */
	ptr = (*result)->bytecode;
	memcpy(ptr, &opv, sizeof(opv));
	ptr += sizeof(opv);

	/* Pass 2: populate bytecode */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING)) {
		css_stylesheet_style_destroy(c->sheet, *result);
		*result = NULL;
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		/* Nothing to do */
	} else {
		bool first = true;

		while (token != NULL) {
			if (token->type == CSS_TOKEN_IDENT) {
                                lwc_string *tok_idata = token->idata;
				lwc_string *name = tok_idata;
                                lwc_string *newname;
			
				if (token->ilower == c->strings[MALE]) {
					opv = VOICE_FAMILY_MALE;
				} else if (token->ilower == 
						c->strings[FEMALE]) {
					opv = VOICE_FAMILY_FEMALE;
				} else if (token->ilower == c->strings[CHILD]) {
					opv = VOICE_FAMILY_CHILD;
				} else {
					uint16_t len = lwc_string_length(token->idata);
					const css_token *temp_token = token;
                                        lwc_error lerror;
					uint8_t *buf;
					uint8_t *p;

					temp_ctx = *ctx;

					opv = VOICE_FAMILY_IDENT_LIST;

					/* Build string from idents */
					while (temp_token != NULL) {
						temp_token = parserutils_vector_peek(
								vector, temp_ctx);
						if (temp_token != NULL && 
							temp_token->type != 
							CSS_TOKEN_IDENT &&
								temp_token->type != 
								CSS_TOKEN_S) {
							break;
						}

						if (temp_token != NULL && temp_token->type == CSS_TOKEN_IDENT) {
							len += lwc_string_length(temp_token->idata);
						} else if (temp_token != NULL) {
							len += 1;
						}

						temp_token = parserutils_vector_iterate(
								vector, &temp_ctx);
					}

					/** \todo Don't use alloca */
					buf = alloca(len);
					p = buf;

					memcpy(p, lwc_string_data(token->idata), lwc_string_length(token->idata));
					p += lwc_string_length(token->idata);

					while (token != NULL) {
						token = parserutils_vector_peek(
								vector, *ctx);
						if (token != NULL &&
							token->type != 
							CSS_TOKEN_IDENT &&
								token->type !=
								CSS_TOKEN_S) {
							break;
						}

						if (token != NULL && 
							token->type == 
								CSS_TOKEN_IDENT) {
							memcpy(p,
								lwc_string_data(token->idata),
								lwc_string_length(token->idata));
							p += lwc_string_length(token->idata);
						} else if (token != NULL) {
							*p++ = ' ';
						}

						token = parserutils_vector_iterate(
							vector, ctx);
					}

					/* Strip trailing whitespace */
					while (p > buf && p[-1] == ' ')
						p--;

					/* Insert into hash, if it's different
					 * from the name we already have */
                                        lerror = lwc_context_intern(c->sheet->dictionary,
                                                                    (char *)buf, len, &newname);
                                        if (lerror != lwc_error_ok) {
                                                css_stylesheet_style_destroy(c->sheet, *result);
                                                *result = NULL;
						*ctx = orig_ctx;
                                                return css_error_from_lwc_error(lerror);
                                        }
                                        
                                        if (newname == name)
                                                lwc_context_string_unref(c->sheet->dictionary,
                                                                         newname);
                                        
                                        name = newname;
                                }

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}

				if (opv == VOICE_FAMILY_IDENT_LIST) {
                                        /* Only ref 'name' again if the token owns it,
                                         * otherwise we already own the only ref to the
                                         * new name generated above.
                                         */
                                        if (name == tok_idata)
                                                lwc_context_string_ref(c->sheet->dictionary, name);
					memcpy(ptr, &name, sizeof(name));
					ptr += sizeof(name);
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				opv = VOICE_FAMILY_STRING;

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}
                                
                                lwc_context_string_ref(c->sheet->dictionary, token->idata);
				memcpy(ptr, &token->idata, 
						sizeof(token->idata));
				ptr += sizeof(token->idata);
			} else {
				css_stylesheet_style_destroy(c->sheet, *result);
				*result = NULL;
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, ',')) {
				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);

				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || 
					(token->type != CSS_TOKEN_IDENT &&
						token->type !=
						CSS_TOKEN_STRING)) {
					css_stylesheet_style_destroy(c->sheet,
							*result);
					*result = NULL;
					*ctx = orig_ctx;
					return CSS_INVALID;
				}
			} else {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
		}

		/* Write terminator */
		opv = VOICE_FAMILY_END;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}

/**
 * Parse volume
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
css_error parse_volume(css_language *c, 
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

	/* number | percentage | IDENT(silent, x-soft, soft, medium, loud, 
	 *                             x-loud, inherit)
	 */
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
			token->ilower == c->strings[SILENT]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_SILENT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_SOFT]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_X_SOFT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SOFT]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_SOFT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LOUD]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_LOUD;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_LOUD]) {
		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_X_LOUD;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		length = number_from_lwc_string(token->ilower, 
				false, &consumed);
		if (consumed != lwc_string_length(token->ilower)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Must be between 0 and 100 */
		if (length < 0 || length > F_100) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		parserutils_vector_iterate(vector, ctx);
		value = VOLUME_NUMBER;
	} else {
		/* Yes, really UNIT_PX -- percentages MUST have a % sign */
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if ((unit & UNIT_PCT) == false) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		/* Must be positive */
		if (length < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = VOLUME_DIMENSION;
	}

	opv = buildOPV(CSS_PROP_VOLUME, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == VOLUME_NUMBER)
		required_size += sizeof(length);
	else if ((flags & FLAG_INHERIT) == false && value == VOLUME_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && (value == VOLUME_NUMBER || 
			value == VOLUME_DIMENSION))
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
	if ((flags & FLAG_INHERIT) == false && value == VOLUME_DIMENSION)
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));

	return CSS_OK;
}

