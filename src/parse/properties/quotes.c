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
 * Parse quotes
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
css_error parse_quotes(css_language *c, 
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
	bool match;

	/* [ STRING STRING ]+ | IDENT(none,inherit) */ 

	/* Pass 1: validate input and calculate bytecode size */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		if ((lwc_string_caseless_isequal(
				token->idata, c->strings[INHERIT],
				&match) == lwc_error_ok && match)) {
			flags = FLAG_INHERIT;
		} else if ((lwc_string_caseless_isequal(
				token->idata, c->strings[NONE],
				&match) == lwc_error_ok && match)) {
			value = QUOTES_NONE;
		} else {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}
	} else {
		bool first = true;

		/* [ STRING STRING ] + */
		while (token != NULL && token->type == CSS_TOKEN_STRING) {
			lwc_string *open = token->idata;
			lwc_string *close;

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			close = token->idata;

			parserutils_vector_iterate(vector, &temp_ctx);

			consumeWhitespace(vector, &temp_ctx);

			if (first == false) {
				required_size += sizeof(opv);
			} else {
				value = QUOTES_STRING;
			}
			required_size += sizeof(open) + sizeof(close);

			first = false;

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				break;
			token = parserutils_vector_iterate(vector, &temp_ctx);
		}

		/* Terminator */
		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_QUOTES, flags, value);

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

	/* Pass 2: construct bytecode */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		/* Nothing to do */
	} else {
		bool first = true;

		/* [ STRING STRING ]+ */
		while (token != NULL && token->type == CSS_TOKEN_STRING) {
			lwc_string *open = token->idata;
			lwc_string *close;

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING) {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}

			close = token->idata;

			parserutils_vector_iterate(vector, ctx);

			consumeWhitespace(vector, ctx);

			if (first == false) {
				opv = QUOTES_STRING;
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
			
			lwc_string_ref(open);
			memcpy(ptr, &open, sizeof(open));
			ptr += sizeof(open);
			
			lwc_string_ref(close);
			memcpy(ptr, &close, sizeof(close));
			ptr += sizeof(close);

			first = false;

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				break;
			token = parserutils_vector_iterate(vector, ctx);
		}

		/* Terminator */
		opv = QUOTES_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}
