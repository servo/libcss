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

static css_error parse_list_style_type_value(css_language *c,
		const css_token *token, uint16_t *value);
static css_error parse_content_list(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t *value, uint8_t *buffer, uint32_t *buflen);

css_error parse_content(css_language *c, 
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

	/* IDENT(normal, none, inherit) |
	 * [
	 *	IDENT(open-quote, close-quote, no-open-quote, no-close-quote) |
	 *	STRING | URI |
	 *	FUNCTION(attr) IDENT ')' |
	 *	FUNCTION(counter) IDENT IDENT? ')' |
	 *	FUNCTION(counters) IDENT STRING IDENT? ')'
	 * ]+
	 */

	/* Pass 1: Calculate required size & validate input */
	token = parserutils_vector_peek(vector, temp_ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else if (token->type == CSS_TOKEN_IDENT &&
			 token->ilower == c->strings[NORMAL]) {
		value = CONTENT_NORMAL;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else if (token->type == CSS_TOKEN_IDENT &&
			 token->ilower == c->strings[NONE]) {
		value = CONTENT_NONE;
		parserutils_vector_iterate(vector, &temp_ctx);
	} else {
		uint32_t len;

		error = parse_content_list(c, vector, &temp_ctx, &value,
				NULL, &len);
		if (error != CSS_OK)
			return error;

		required_size += len;
	}

	opv = buildOPV(CSS_PROP_CONTENT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy OPV to bytecode */
	ptr = (*result)->bytecode;
	memcpy(ptr, &opv, sizeof(opv));
	ptr += sizeof(opv);

	/* Pass 2: construct bytecode */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			(token->ilower == c->strings[INHERIT] ||
			 token->ilower == c->strings[NORMAL] ||
			 token->ilower == c->strings[NONE])) {
			parserutils_vector_iterate(vector, ctx);
	} else {
		error = parse_content_list(c, vector, ctx, NULL, ptr, NULL);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

css_error parse_counter_increment(css_language *c, 
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

	/* [IDENT <integer>? ]+ | IDENT(none, inherit) */

	/* Pass 1: validate input and calculate bytecode size */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else if (token->ilower == c->strings[NONE]) {
		value = COUNTER_INCREMENT_NONE;
	} else {
		bool first = true;

		value = COUNTER_INCREMENT_NAMED;

		while (token != NULL) {
			lwc_string *name = token->idata;
			css_fixed increment = INTTOFIX(1);

			consumeWhitespace(vector, &temp_ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER)
				return CSS_INVALID;

			if (token != NULL && token->type == CSS_TOKEN_NUMBER) {
				size_t consumed = 0;

				increment = number_from_lwc_string(token->ilower,
						true, &consumed);

				if (consumed != lwc_string_length(token->ilower))
					return CSS_INVALID;

				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);
			}

			if (first == false) {
				required_size += sizeof(opv);
			}
			required_size += sizeof(name) + sizeof(increment);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;
		}

		/* And for the terminator */
		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_COUNTER_INCREMENT, flags, value);

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
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (token->ilower == c->strings[INHERIT] ||
			token->ilower == c->strings[NONE]) {
		/* Nothing to do */
	} else {
		bool first = true;

		opv = COUNTER_INCREMENT_NAMED;

		while (token != NULL) {
			lwc_string *name = token->idata;
			css_fixed increment = INTTOFIX(1);

			consumeWhitespace(vector, ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER)
				return CSS_INVALID;

			if (token != NULL && token->type == CSS_TOKEN_NUMBER) {
				size_t consumed = 0;

				increment = number_from_lwc_string(token->ilower,
						true, &consumed);

				if (consumed != lwc_string_length(token->ilower))
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
                        
                        lwc_context_string_ref(c->sheet->dictionary, name);
			memcpy(ptr, &name, sizeof(name));
			ptr += sizeof(name);
                        
			memcpy(ptr, &increment, sizeof(increment));
			ptr += sizeof(increment);

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;
		}

		/* And for the terminator */
		opv = COUNTER_INCREMENT_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}

css_error parse_counter_reset(css_language *c, 
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

	/* [IDENT <integer>? ]+ | IDENT(none, inherit) */

	/* Pass 1: validate input and calculate bytecode size */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else if (token->ilower == c->strings[NONE]) {
		value = COUNTER_RESET_NONE;
	} else {
		bool first = true;

		value = COUNTER_RESET_NAMED;

		while (token != NULL) {
			lwc_string *name = token->idata;
			css_fixed increment = INTTOFIX(0);

			consumeWhitespace(vector, &temp_ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER)
				return CSS_INVALID;

			if (token != NULL && token->type == CSS_TOKEN_NUMBER) {
				size_t consumed = 0;

				increment = number_from_lwc_string(token->ilower,
						true, &consumed);

				if (consumed != lwc_string_length(token->ilower))
					return CSS_INVALID;

				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);
			}

			if (first == false) {
				required_size += sizeof(opv);
			}
			required_size += sizeof(name) + sizeof(increment);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;
		}

		/* And for the terminator */
		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_COUNTER_RESET, flags, value);

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
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (token->ilower == c->strings[INHERIT] ||
			token->ilower == c->strings[NONE]) {
		/* Nothing to do */
	} else {
		bool first = true;

		opv = COUNTER_RESET_NAMED;

		while (token != NULL) {
			lwc_string *name = token->idata;
			css_fixed increment = INTTOFIX(0);

			consumeWhitespace(vector, ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER)
				return CSS_INVALID;

			if (token != NULL && token->type == CSS_TOKEN_NUMBER) {
				size_t consumed = 0;

				increment = number_from_lwc_string(token->ilower,
						true, &consumed);

				if (consumed != lwc_string_length(token->ilower))
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
                        
                        lwc_context_string_ref(c->sheet->dictionary, name);
			memcpy(ptr, &name, sizeof(name));
			ptr += sizeof(name);
                        
			memcpy(ptr, &increment, sizeof(increment));
			ptr += sizeof(increment);

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
			if (token != NULL && token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;
		}

		/* And for the terminator */
		opv = COUNTER_RESET_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}

css_error parse_list_style_image(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size;

	/* URI | IDENT (none, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[NONE]) {
		value = LIST_STYLE_IMAGE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = LIST_STYLE_IMAGE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_LIST_STYLE_IMAGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LIST_STYLE_IMAGE_URI)
		required_size += sizeof(lwc_string *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == LIST_STYLE_IMAGE_URI) {
                lwc_context_string_ref(c->sheet->dictionary, token->idata);
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(lwc_string *));
	}

	return CSS_OK;
}

css_error parse_list_style_position(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (inside, outside, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[INSIDE]) {
		value = LIST_STYLE_POSITION_INSIDE;
	} else if (ident->ilower == c->strings[OUTSIDE]) {
		value = LIST_STYLE_POSITION_OUTSIDE;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_LIST_STYLE_POSITION, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_list_style_type(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (disc, circle, square, decimal, decimal-leading-zero,
	 *        lower-roman, upper-roman, lower-greek, lower-latin,
	 *        upper-latin, armenian, georgian, lower-alpha, upper-alpha,
	 *        none, inherit)
	 */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else {
		error = parse_list_style_type_value(c, ident, &value);
		if (error != CSS_OK)
			return error;
	}

	opv = buildOPV(CSS_PROP_LIST_STYLE_TYPE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_quotes(css_language *c, 
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

	/* [ STRING STRING ]+ | IDENT(none,inherit) */ 

	/* Pass 1: validate input and calculate bytecode size */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT) {
		if (token->ilower == c->strings[INHERIT]) {
			flags = FLAG_INHERIT;
		} else if (token->ilower == c->strings[NONE]) {
			value = QUOTES_NONE;
		} else
			return CSS_INVALID;
	} else {
		bool first = true;

		/* [ STRING STRING ] + */
		while (token != NULL && token->type == CSS_TOKEN_STRING) {
			lwc_string *open = token->idata;
			lwc_string *close;

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			close = token->idata;

			token = parserutils_vector_iterate(vector, &temp_ctx);

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

		consumeWhitespace(vector, &temp_ctx);

		token = parserutils_vector_peek(vector, temp_ctx);
		if (token != NULL && tokenIsChar(token, '!') == false)
			return CSS_INVALID;

		/* Terminator */
		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_QUOTES, flags, value);

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
			token->type != CSS_TOKEN_STRING))
		return CSS_INVALID;

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
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			close = token->idata;

			token = parserutils_vector_iterate(vector, ctx);

			consumeWhitespace(vector, ctx);

			if (first == false) {
				opv = QUOTES_STRING;
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
                        
                        lwc_context_string_ref(c->sheet->dictionary, open);
			memcpy(ptr, &open, sizeof(open));
			ptr += sizeof(open);
			
                        lwc_context_string_ref(c->sheet->dictionary, close);
                        memcpy(ptr, &close, sizeof(close));
			ptr += sizeof(close);

			first = false;

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				break;
			token = parserutils_vector_iterate(vector, ctx);
		}

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, '!') == false)
			return CSS_INVALID;

		/* Terminator */
		opv = QUOTES_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}

css_error parse_list_style_type_value(css_language *c, const css_token *ident,
		uint16_t *value)
{
	/* IDENT (disc, circle, square, decimal, decimal-leading-zero,
	 *        lower-roman, upper-roman, lower-greek, lower-latin,
	 *        upper-latin, armenian, georgian, lower-alpha, upper-alpha,
	 *        none)
	 */
	if (ident->ilower == c->strings[DISC]) {
		*value = LIST_STYLE_TYPE_DISC;
	} else if (ident->ilower == c->strings[CIRCLE]) {
		*value = LIST_STYLE_TYPE_CIRCLE;
	} else if (ident->ilower == c->strings[SQUARE]) {
		*value = LIST_STYLE_TYPE_SQUARE;
	} else if (ident->ilower == c->strings[DECIMAL]) {
		*value = LIST_STYLE_TYPE_DECIMAL;
	} else if (ident->ilower == c->strings[DECIMAL_LEADING_ZERO]) {
		*value = LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO;
	} else if (ident->ilower == c->strings[LOWER_ROMAN]) {
		*value = LIST_STYLE_TYPE_LOWER_ROMAN;
	} else if (ident->ilower == c->strings[UPPER_ROMAN]) {
		*value = LIST_STYLE_TYPE_UPPER_ROMAN;
	} else if (ident->ilower == c->strings[LOWER_GREEK]) {
		*value = LIST_STYLE_TYPE_LOWER_GREEK;
	} else if (ident->ilower == c->strings[LOWER_LATIN]) {
		*value = LIST_STYLE_TYPE_LOWER_LATIN;
	} else if (ident->ilower == c->strings[UPPER_LATIN]) {
		*value = LIST_STYLE_TYPE_UPPER_LATIN;
	} else if (ident->ilower == c->strings[ARMENIAN]) {
		*value = LIST_STYLE_TYPE_ARMENIAN;
	} else if (ident->ilower == c->strings[GEORGIAN]) {
		*value = LIST_STYLE_TYPE_GEORGIAN;
	} else if (ident->ilower == c->strings[LOWER_ALPHA]) {
		*value = LIST_STYLE_TYPE_LOWER_ALPHA;
	} else if (ident->ilower == c->strings[UPPER_ALPHA]) {
		*value = LIST_STYLE_TYPE_UPPER_ALPHA;
	} else if (ident->ilower == c->strings[NONE]) {
		*value = LIST_STYLE_TYPE_NONE;
	} else
		return CSS_INVALID;

	return CSS_OK;
}

css_error parse_content_list(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t *value, uint8_t *buffer, uint32_t *buflen)
{
	css_error error;
	const css_token *token;
	bool first = true;
	uint32_t offset = 0;
	uint32_t opv;

	/* [
	 *	IDENT(open-quote, close-quote, no-open-quote, no-close-quote) |
	 *	STRING | URI |
	 *	FUNCTION(attr) IDENT ')' |
	 *	FUNCTION(counter) IDENT (',' IDENT)? ')' |
	 *	FUNCTION(counters) IDENT ',' STRING (',' IDENT)? ')'
	 * ]+
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
		return CSS_INVALID;

	while (token != NULL) {
		if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[OPEN_QUOTE]) {
			opv = CONTENT_OPEN_QUOTE;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			} 
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[CLOSE_QUOTE]) {
			opv = CONTENT_CLOSE_QUOTE;

			if (first == false) {				
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[NO_OPEN_QUOTE]) {
			opv = CONTENT_NO_OPEN_QUOTE;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[NO_CLOSE_QUOTE]) {
			opv = CONTENT_NO_CLOSE_QUOTE;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_STRING) {
			opv = CONTENT_STRING;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						token->idata);
				memcpy(buffer + offset, &token->idata,
						sizeof(token->idata));
			}

			offset += sizeof(token->idata);
		} else if (token->type == CSS_TOKEN_URI) {
			opv = CONTENT_URI;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						token->idata);
				memcpy(buffer + offset, &token->idata,
						sizeof(token->idata));
			}

			offset += sizeof(token->idata);
		} else if (token->type == CSS_TOKEN_FUNCTION &&
				token->ilower == c->strings[ATTR]) {
			opv = CONTENT_ATTR;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			consumeWhitespace(vector, ctx);

			/* Expect IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						token->idata);
				memcpy(buffer + offset, &token->idata, 
						sizeof(token->idata));
			}

			offset += sizeof(token->idata);

			consumeWhitespace(vector, ctx);

			/* Expect ')' */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || tokenIsChar(token, ')') == false)
				return CSS_INVALID;
		} else if (token->type == CSS_TOKEN_FUNCTION &&
				token->ilower == c->strings[COUNTER]) {
			lwc_string *name;

			opv = CONTENT_COUNTER;

			consumeWhitespace(vector, ctx);

			/* Expect IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

			name = token->idata;

			consumeWhitespace(vector, ctx);

			/* Possible ',' */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || 
					(tokenIsChar(token, ',') == false &&
					tokenIsChar(token, ')') == false))
				return CSS_INVALID;

			if (tokenIsChar(token, ',')) {
				uint16_t v;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);

				/* Expect IDENT */
				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || 
						token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

				error = parse_list_style_type_value(c,
						token, &v);
				if (error != CSS_OK)
					return error;

				opv |= v << CONTENT_COUNTER_STYLE_SHIFT;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			} else {
				opv |= LIST_STYLE_TYPE_DECIMAL << 
						CONTENT_COUNTER_STYLE_SHIFT;
			}

			/* Expect ')' */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || tokenIsChar(token,	')') == false)
				return CSS_INVALID;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						name);
				memcpy(buffer + offset, &name, sizeof(name));
			}

			offset += sizeof(name);
		} else if (token->type == CSS_TOKEN_FUNCTION &&
				token->ilower == c->strings[COUNTERS]) {
			lwc_string *name;
			lwc_string *sep;

			opv = CONTENT_COUNTERS;

			consumeWhitespace(vector, ctx);

			/* Expect IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

			name = token->idata;

			consumeWhitespace(vector, ctx);

			/* Expect ',' */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || tokenIsChar(token, ',') == false)
				return CSS_INVALID;

			consumeWhitespace(vector, ctx);

			/* Expect STRING */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			sep = token->idata;

			consumeWhitespace(vector, ctx);

			/* Possible ',' */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || 
					(tokenIsChar(token, ',') == false && 
					tokenIsChar(token, ')') == false))
				return CSS_INVALID;

			if (tokenIsChar(token, ',')) {
				uint16_t v;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);

				/* Expect IDENT */
				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || 
						token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

				error = parse_list_style_type_value(c,
						token, &v);
				if (error != CSS_OK)
					return error;

				opv |= v << CONTENT_COUNTERS_STYLE_SHIFT;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			} else {
				opv |= LIST_STYLE_TYPE_DECIMAL <<
						CONTENT_COUNTERS_STYLE_SHIFT;
			}

			/* Expect ')' */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || tokenIsChar(token, ')') == false)
				return CSS_INVALID;

			if (first == false) {
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						name);
				memcpy(buffer + offset, &name, sizeof(name));
			}

			offset += sizeof(name);

			if (buffer != NULL) {
                                lwc_context_string_ref(c->sheet->dictionary, 
						sep);
				memcpy(buffer + offset, &sep, sizeof(sep));
			}

			offset += sizeof(sep);
		} else {
			return CSS_INVALID;
		}

		if (first && value != NULL) {
			*value = opv;
		}
		first = false;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, '!'))
			break;

		token = parserutils_vector_iterate(vector, ctx);
	}

	/* Write list terminator */
	opv = CONTENT_NORMAL;

	if (buffer != NULL) {
		memcpy(buffer + offset, &opv, sizeof(opv));
	}

	offset += sizeof(opv);

	if (buflen != NULL) {
		*buflen = offset;
	}

	return CSS_OK;
}

