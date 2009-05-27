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

css_error parse_cursor(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size = sizeof(opv);
	int temp_ctx = *ctx;
	uint8_t *ptr;

	/* [ (URI ',')* IDENT(auto, crosshair, default, pointer, move, e-resize,
	 *              ne-resize, nw-resize, n-resize, se-resize, sw-resize,
	 *              s-resize, w-resize, text, wait, help, progress) ] 
	 * | IDENT(inherit) 
	 */

	/* Pass 1: validate input and calculate bytecode size */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else {
		bool first = true;

		/* URI* */
		while (token != NULL && token->type == CSS_TOKEN_URI) {
			lwc_string *uri = token->idata;

			if (first == false) {
				required_size += sizeof(opv);
			} else {
				value = CURSOR_URI;
			}
			required_size += sizeof(uri);

			consumeWhitespace(vector, &temp_ctx);

			/* Expect ',' */
			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token == NULL || tokenIsChar(token, ',') == false)
				return CSS_INVALID;

			consumeWhitespace(vector, &temp_ctx);

			/* Expect either URI or IDENT */
			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_URI))
				return CSS_INVALID;

			first = false;
		}

		/* IDENT */
		if (token != NULL && token->type == CSS_TOKEN_IDENT) {
			if (token->ilower == c->strings[AUTO]) {
				if (first) {
					value = CURSOR_AUTO;
				}
			} else if (token->ilower == c->strings[CROSSHAIR]) {
				if (first) {
					value = CURSOR_CROSSHAIR;
				}
			} else if (token->ilower == c->strings[DEFAULT]) {
				if (first) {
					value = CURSOR_DEFAULT;
				}
			} else if (token->ilower == c->strings[POINTER]) {
				if (first) {
					value = CURSOR_POINTER;
				}
			} else if (token->ilower == c->strings[MOVE]) {
				if (first) {
					value = CURSOR_MOVE;
				}
			} else if (token->ilower == c->strings[E_RESIZE]) {
				if (first) {
					value = CURSOR_E_RESIZE;
				}
			} else if (token->ilower == c->strings[NE_RESIZE]) {
				if (first) {
					value = CURSOR_NE_RESIZE;
				}
			} else if (token->ilower == c->strings[NW_RESIZE]) {
				if (first) {
					value = CURSOR_NW_RESIZE;
				}
			} else if (token->ilower == c->strings[N_RESIZE]) {
				if (first) {
					value = CURSOR_N_RESIZE;
				}
			} else if (token->ilower == c->strings[SE_RESIZE]) {
				if (first) {
					value = CURSOR_SE_RESIZE;
				}
			} else if (token->ilower == c->strings[SW_RESIZE]) {
				if (first) {
					value = CURSOR_SW_RESIZE;
				}
			} else if (token->ilower == c->strings[S_RESIZE]) {
				if (first) {
					value = CURSOR_S_RESIZE;
				}
			} else if (token->ilower == c->strings[W_RESIZE]) {
				if (first) {
					value = CURSOR_W_RESIZE;
				}
			} else if (token->ilower == c->strings[TEXT]) {
				if (first) {
					value = CURSOR_TEXT;
				}
			} else if (token->ilower == c->strings[WAIT]) {
				if (first) {
					value = CURSOR_WAIT;
				}
			} else if (token->ilower == c->strings[HELP]) {
				if (first) {
					value = CURSOR_HELP;
				}
			} else if (token->ilower == c->strings[PROGRESS]) {
				if (first) {
					value = CURSOR_PROGRESS;
				}
			} else {
				return CSS_INVALID;
			}

			if (first == false) {
				required_size += sizeof(opv);
			}
		}

		consumeWhitespace(vector, &temp_ctx);

		token = parserutils_vector_peek(vector, temp_ctx);
		if (token != NULL && tokenIsChar(token, '!') == false)
			return CSS_INVALID;
	}

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(CSS_PROP_CURSOR, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy OPV to bytecode */
	ptr = (*result)->bytecode;
	memcpy(ptr, &opv, sizeof(opv));
	ptr += sizeof(opv);

	/* Pass 2: construct bytecode */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		/* Nothing to do */
	} else {
		bool first = true;

		/* URI* */
		while (token != NULL && token->type == CSS_TOKEN_URI) {
			lwc_string *uri = token->idata;

			if (first == false) {
				opv = CURSOR_URI;
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
                        
                        lwc_context_string_ref(c->sheet->dictionary, uri);
			memcpy(ptr, &uri, sizeof(uri));
			ptr += sizeof(uri);

			consumeWhitespace(vector, ctx);

			/* Expect ',' */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || tokenIsChar(token, ',') == false)
				return CSS_INVALID;

			consumeWhitespace(vector, ctx);

			/* Expect either URI or IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_URI))
				return CSS_INVALID;

			first = false;
		}

		/* IDENT */
		if (token != NULL && token->type == CSS_TOKEN_IDENT) {
			if (token->ilower == c->strings[AUTO]) {
				opv = CURSOR_AUTO;
			} else if (token->ilower == c->strings[CROSSHAIR]) {
				opv = CURSOR_CROSSHAIR;
			} else if (token->ilower == c->strings[DEFAULT]) {
				opv = CURSOR_DEFAULT;
			} else if (token->ilower == c->strings[POINTER]) {
				opv = CURSOR_POINTER;
			} else if (token->ilower == c->strings[MOVE]) {
				opv = CURSOR_MOVE;
			} else if (token->ilower == c->strings[E_RESIZE]) {
				opv = CURSOR_E_RESIZE;
			} else if (token->ilower == c->strings[NE_RESIZE]) {
				opv = CURSOR_NE_RESIZE;
			} else if (token->ilower == c->strings[NW_RESIZE]) {
				opv = CURSOR_NW_RESIZE;
			} else if (token->ilower == c->strings[N_RESIZE]) {
				opv = CURSOR_N_RESIZE;
			} else if (token->ilower == c->strings[SE_RESIZE]) {
				opv = CURSOR_SE_RESIZE;
			} else if (token->ilower == c->strings[SW_RESIZE]) {
				opv = CURSOR_SW_RESIZE;
			} else if (token->ilower == c->strings[S_RESIZE]) {
				opv = CURSOR_S_RESIZE;
			} else if (token->ilower == c->strings[W_RESIZE]) {
				opv = CURSOR_W_RESIZE;
			} else if (token->ilower == c->strings[TEXT]) {
				opv = CURSOR_TEXT;
			} else if (token->ilower == c->strings[WAIT]) {
				opv = CURSOR_WAIT;
			} else if (token->ilower == c->strings[HELP]) {
				opv = CURSOR_HELP;
			} else if (token->ilower == c->strings[PROGRESS]) {
				opv = CURSOR_PROGRESS;
			} else {
				return CSS_INVALID;
			}

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
		}

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, '!') == false)
			return CSS_INVALID;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	return CSS_OK;
}

