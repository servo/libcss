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
 * Parse border-spacing
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
css_error parse_border_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	css_fixed length[2] = { 0 };
	uint32_t unit[2] = { 0 };
	uint32_t required_size;
	bool match;

	/* length length? | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT &&
			(lwc_string_caseless_isequal(
			token->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		int num_lengths = 0;

		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length[0], &unit[0]);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		if (unit[0] & UNIT_ANGLE || unit[0] & UNIT_TIME || 
				unit[0] & UNIT_FREQ || unit[0] & UNIT_PCT) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		num_lengths = 1;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL) {
			/* Attempt second length, ignoring errors.
			 * The core !important parser will ensure 
			 * any remaining junk is thrown out.
			 * Ctx will be preserved on error, as usual
			 */
			error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
					&length[1], &unit[1]);
			if (error == CSS_OK) {
				if (unit[1] & UNIT_ANGLE || 
						unit[1] & UNIT_TIME ||
						unit[1] & UNIT_FREQ || 
						unit[1] & UNIT_PCT) {
					*ctx = orig_ctx;
					return CSS_INVALID;
				}

				num_lengths = 2;
			}
		}

		if (num_lengths == 1) {
			/* Only one length specified. Use for both axes. */
			length[1] = length[0];
			unit[1] = unit[0];
		}

		/* Lengths must not be negative */
		if (length[0] < 0 || length[1] < 0) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		value = BORDER_SPACING_SET;
	}

	opv = buildOPV(CSS_PROP_BORDER_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_SPACING_SET)
		required_size += 2 * (sizeof(length[0]) + sizeof(unit[0]));

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

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
