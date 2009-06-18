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

css_error parse_font_family(css_language *c, 
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

	/* [ IDENT+ | STRING ] [ ',' [ IDENT+ | STRING ] ]* | IDENT(inherit)
	 * 
	 * In the case of IDENT+, any whitespace between tokens is collapsed to
	 * a single space
	 *
	 * \todo Mozilla makes the comma optional. 
	 * Perhaps this is a quirk we should inherit?
	 */

	/* Pass 1: validate input and calculate space */
	token = parserutils_vector_iterate(vector, &temp_ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_STRING))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else {
		bool first = true;

		while (token != NULL) {
			if (token->type == CSS_TOKEN_IDENT) {
				if (first == false) {
					required_size += sizeof(opv);
				}

				if (token->ilower == c->strings[SERIF]) {
					if (first) {
						value = FONT_FAMILY_SERIF;
					}
				} else if (token->ilower == 
						c->strings[SANS_SERIF]) {
					if (first) {
						value = FONT_FAMILY_SANS_SERIF;
					}
				} else if (token->ilower == 
						c->strings[CURSIVE]) {
					if (first) {
						value = FONT_FAMILY_CURSIVE;
					}
				} else if (token->ilower == 
						c->strings[FANTASY]) {
					if (first) {
						value = FONT_FAMILY_FANTASY;
					}
				} else if (token->ilower == 
						c->strings[MONOSPACE]) {
					if (first) {
						value = FONT_FAMILY_MONOSPACE;
					}
				} else {
					if (first) {
						value = FONT_FAMILY_IDENT_LIST;
					}

					required_size +=
						sizeof(lwc_string *);

					/* Skip past [ IDENT* S* ]* */
					while (token != NULL) {
						token = parserutils_vector_peek(
								vector, 
								temp_ctx);
						if (token == NULL ||
							(token->type != 
							CSS_TOKEN_IDENT &&
								token->type != 
								CSS_TOKEN_S)) {
							break;
						}

						/* idents must not match 
						 * generic families */
						if (token->type == CSS_TOKEN_IDENT && 
								(token->ilower == c->strings[SERIF] || 
								token->ilower == c->strings[SANS_SERIF] || 
								token->ilower == c->strings[CURSIVE] || 
								token->ilower == c->strings[FANTASY] || 
								token->ilower == c->strings[MONOSPACE]))
							return CSS_INVALID;
						token = parserutils_vector_iterate(
							vector, &temp_ctx);
					}
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				if (first == false) {
					required_size += sizeof(opv);
				} else {
					value = FONT_FAMILY_STRING;
				}

				required_size += 
					sizeof(lwc_string *);
			} else {
				return CSS_INVALID;
			}

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, ',') == false &&
					tokenIsChar(token, '!') == false) {
				return CSS_INVALID;
			}

			if (token != NULL && tokenIsChar(token, ',')) {
				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);

				token = parserutils_vector_peek(vector, 
						temp_ctx);
				if (token == NULL || tokenIsChar(token, '!'))
					return CSS_INVALID;
			}

			first = false;

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, '!'))
				break;

			token = parserutils_vector_iterate(vector, &temp_ctx);
		}

		required_size += sizeof(opv);
	}

	opv = buildOPV(CSS_PROP_FONT_FAMILY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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
			
				if (token->ilower == c->strings[SERIF]) {
					opv = FONT_FAMILY_SERIF;
				} else if (token->ilower == 
						c->strings[SANS_SERIF]) {
					opv = FONT_FAMILY_SANS_SERIF;
				} else if (token->ilower == 
						c->strings[CURSIVE]) {
					opv = FONT_FAMILY_CURSIVE;
				} else if (token->ilower == 
						c->strings[FANTASY]) {
					opv = FONT_FAMILY_FANTASY;
				} else if (token->ilower == 
						c->strings[MONOSPACE]) {
					opv = FONT_FAMILY_MONOSPACE;
				} else {
					uint16_t len = lwc_string_length(token->idata);
					const css_token *temp_token = token;
					lwc_error lerror;
					uint8_t *buf;
					uint8_t *p;

					temp_ctx = *ctx;

					opv = FONT_FAMILY_IDENT_LIST;

					/* Build string from idents */
					while (temp_token != NULL) {
						temp_token = parserutils_vector_peek(
								vector, temp_ctx);
						if (temp_token == NULL || 
							(temp_token->type != 
							CSS_TOKEN_IDENT &&
								temp_token->type != 
								CSS_TOKEN_S)) {
							break;
						}

						if (temp_token->type == 
							CSS_TOKEN_IDENT) {
							len += lwc_string_length(temp_token->idata);
						} else {
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
						if (token == NULL ||
							(token->type != 
							CSS_TOKEN_IDENT &&
								token->type !=
								CSS_TOKEN_S)) {
							break;
						}

						if (token->type == 
								CSS_TOKEN_IDENT) {
							memcpy(p,
								lwc_string_data(token->idata),
								lwc_string_length(token->idata));
							p += lwc_string_length(token->idata);
						} else {
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

				if (opv == FONT_FAMILY_IDENT_LIST) {
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
				opv = FONT_FAMILY_STRING;

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
				return CSS_INVALID;
			}

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, ',') == false &&
					tokenIsChar(token, '!') == false) {
				css_stylesheet_style_destroy(c->sheet, *result);
				*result = NULL;
				return CSS_INVALID;
			}

			if (token != NULL && tokenIsChar(token, ',')) {
				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);

				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || tokenIsChar(token, '!')) {
					css_stylesheet_style_destroy(c->sheet,
							*result);
					*result = NULL;
					return CSS_INVALID;
				}
			}

			first = false;

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, '!'))
				break;

			token = parserutils_vector_iterate(vector, ctx);
		}

		/* Write terminator */
		opv = FONT_FAMILY_END;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	return CSS_OK;
}

css_error parse_font_size(css_language *c, 
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

	/* length | percentage | IDENT(xx-small, x-small, small, medium,
	 * large, x-large, xx-large, larger, smaller, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[XX_SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_XX_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_X_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_X_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[XX_LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_XX_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LARGER]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_LARGER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SMALLER]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_SMALLER;
	} else {
		error = parse_unit_specifier(c, vector, ctx, UNIT_PX,
				&length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		/* Negative values are not permitted */
		if (length < 0)
			return CSS_INVALID;

		value = FONT_SIZE_DIMENSION;
	}

	opv = buildOPV(CSS_PROP_FONT_SIZE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == FONT_SIZE_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == FONT_SIZE_DIMENSION) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_font_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, italic, oblique, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = FONT_STYLE_NORMAL;
	} else if (ident->ilower == c->strings[ITALIC]) {
		value = FONT_STYLE_ITALIC;
	} else if (ident->ilower == c->strings[OBLIQUE]) {
		value = FONT_STYLE_OBLIQUE;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_FONT_STYLE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_font_variant(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, small-caps, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = FONT_VARIANT_NORMAL;
	} else if (ident->ilower == c->strings[SMALL_CAPS]) {
		value = FONT_VARIANT_SMALL_CAPS;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_FONT_VARIANT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_font_weight(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* NUMBER (100, 200, 300, 400, 500, 600, 700, 800, 900) | 
	 * IDENT (normal, bold, bolder, lighter, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_fixed num = number_from_lwc_string(token->ilower, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != lwc_string_length(token->ilower))
			return CSS_INVALID;
		switch (FIXTOINT(num)) {
		case 100: value = FONT_WEIGHT_100; break;
		case 200: value = FONT_WEIGHT_200; break;
		case 300: value = FONT_WEIGHT_300; break;
		case 400: value = FONT_WEIGHT_400; break;
		case 500: value = FONT_WEIGHT_500; break;
		case 600: value = FONT_WEIGHT_600; break;
		case 700: value = FONT_WEIGHT_700; break;
		case 800: value = FONT_WEIGHT_800; break;
		case 900: value = FONT_WEIGHT_900; break;
		default: return CSS_INVALID;
		}
	} else if (token->ilower == c->strings[NORMAL]) {
		value = FONT_WEIGHT_NORMAL;
	} else if (token->ilower == c->strings[BOLD]) {
		value = FONT_WEIGHT_BOLD;
	} else if (token->ilower == c->strings[BOLDER]) {
		value = FONT_WEIGHT_BOLDER;
	} else if (token->ilower == c->strings[LIGHTER]) {
		value = FONT_WEIGHT_LIGHTER;
	} else
		return CSS_INVALID;

	opv = buildOPV(CSS_PROP_FONT_WEIGHT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

