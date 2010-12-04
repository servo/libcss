/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include "stylesheet.h"
#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/utils.h"

/**
 * Parse a colour specifier
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive result (AARRGGBB)
 * \return CSS_OK      on success,
 *         CSS_INVALID if the input is invalid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_colour_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t *result)
{
	int orig_ctx = *ctx;
	const css_token *token;
	uint8_t r = 0, g = 0, b = 0, a = 0xff;
	bool match;
	css_error error;

	consumeWhitespace(vector, ctx);

	/* IDENT(<colour name>) | HASH(rgb | rrggbb) |
	 * FUNCTION(rgb) [ [ NUMBER | PERCENTAGE ] ',' ] {3} ')'
	 *
	 * For quirks, NUMBER | DIMENSION | IDENT, too
	 * I.E. "123456" -> NUMBER, "1234f0" -> DIMENSION, "f00000" -> IDENT
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_HASH &&
			token->type != CSS_TOKEN_FUNCTION)) {
		if (c->sheet->quirks_allowed == false ||
				token == NULL ||
				(token->type != CSS_TOKEN_NUMBER &&
				token->type != CSS_TOKEN_DIMENSION))
			goto invalid;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		error = parse_named_colour(c, token->idata, result);
		if (error != CSS_OK && c->sheet->quirks_allowed) {
			error = parse_hash_colour(token->idata, result);
			if (error == CSS_OK)
				c->sheet->quirks_used = true;
		}

		if (error != CSS_OK)
			*ctx = orig_ctx;

		return error;
	} else if (token->type == CSS_TOKEN_HASH) {
		error = parse_hash_colour(token->idata, result);
		if (error != CSS_OK)
			*ctx = orig_ctx;

		return error;
	} else if (c->sheet->quirks_allowed &&
			token->type == CSS_TOKEN_NUMBER) {
		error = parse_hash_colour(token->idata, result);
		if (error == CSS_OK)
			c->sheet->quirks_used = true;
		else
			*ctx = orig_ctx;

		return error;
	} else if (c->sheet->quirks_allowed &&
			token->type == CSS_TOKEN_DIMENSION) {
		error = parse_hash_colour(token->idata, result);
		if (error == CSS_OK)
			c->sheet->quirks_used = true;
		else
			*ctx = orig_ctx;

		return error;
	} else if (token->type == CSS_TOKEN_FUNCTION) {
		if ((lwc_string_caseless_isequal(
				token->idata, c->strings[RGB],
				&match) == lwc_error_ok && match)) {
			int i;
			css_token_type valid = CSS_TOKEN_NUMBER;

			for (i = 0; i < 3; i++) {
				css_fixed num;
				size_t consumed = 0;
				uint8_t *component = i == 0 ? &r
							    : i == 1 ? &g : &b;
				int32_t intval;

				consumeWhitespace(vector, ctx);

				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || (token->type !=
						CSS_TOKEN_NUMBER &&
						token->type !=
						CSS_TOKEN_PERCENTAGE))
					goto invalid;

				if (i == 0)
					valid = token->type;
				else if (token->type != valid)
					goto invalid;

				num = number_from_lwc_string(token->idata,
						valid == CSS_TOKEN_NUMBER,
						&consumed);
				if (consumed != lwc_string_length(token->idata))
					goto invalid;

				if (valid == CSS_TOKEN_NUMBER) {
					intval = FIXTOINT(num);
				} else {
					intval = FIXTOINT(
						FDIVI(FMULI(num, 255), 100));
				}

				if (intval > 255)
					*component = 255;
				else if (intval < 0)
					*component = 0;
				else
					*component = intval;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);

				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL)
					goto invalid;

				if (i != 2 && tokenIsChar(token, ','))
					parserutils_vector_iterate(vector, ctx);
				else if (i == 2 && tokenIsChar(token, ')'))
					parserutils_vector_iterate(vector, ctx);
				else
					goto invalid;
			}
		} else
			goto invalid;
	}

	*result = (a << 24) | (r << 16) | (g << 8) | b;

	return CSS_OK;

invalid:
	*ctx = orig_ctx;
	return CSS_INVALID;
}

/**
 * Parse a named colour
 *
 * \param c       Parsing context
 * \param data    Colour name string
 * \param result  Pointer to location to receive result
 * \return CSS_OK      on success,
 *         CSS_INVALID if the colour name is unknown
 */
css_error parse_named_colour(css_language *c, lwc_string *data,
		uint32_t *result)
{
	static const uint32_t colourmap[LAST_COLOUR + 1 - FIRST_COLOUR] = {
		0xfff0f8ff, /* ALICEBLUE */
		0xfffaebd7, /* ANTIQUEWHITE */
		0xff00ffff, /* AQUA */
		0xff7fffd4, /* AQUAMARINE */
		0xfff0ffff, /* AZURE */
		0xfff5f5dc, /* BEIGE */
		0xffffe4c4, /* BISQUE */
		0xff000000, /* BLACK */
		0xffffebcd, /* BLANCHEDALMOND */
		0xff0000ff, /* BLUE */
		0xff8a2be2, /* BLUEVIOLET */
		0xffa52a2a, /* BROWN */
		0xffdeb887, /* BURLYWOOD */
		0xff5f9ea0, /* CADETBLUE */
		0xff7fff00, /* CHARTREUSE */
		0xffd2691e, /* CHOCOLATE */
		0xffff7f50, /* CORAL */
		0xff6495ed, /* CORNFLOWERBLUE */
		0xfffff8dc, /* CORNSILK */
		0xffdc143c, /* CRIMSON */
		0xff00ffff, /* CYAN */
		0xff00008b, /* DARKBLUE */
		0xff008b8b, /* DARKCYAN */
		0xffb8860b, /* DARKGOLDENROD */
		0xffa9a9a9, /* DARKGRAY */
		0xff006400, /* DARKGREEN */
		0xffa9a9a9, /* DARKGREY */
		0xffbdb76b, /* DARKKHAKI */
		0xff8b008b, /* DARKMAGENTA */
		0xff556b2f, /* DARKOLIVEGREEN */
		0xffff8c00, /* DARKORANGE */
		0xff9932cc, /* DARKORCHID */
		0xff8b0000, /* DARKRED */
		0xffe9967a, /* DARKSALMON */
		0xff8fbc8f, /* DARKSEAGREEN */
		0xff483d8b, /* DARKSLATEBLUE */
		0xff2f4f4f, /* DARKSLATEGRAY */
		0xff2f4f4f, /* DARKSLATEGREY */
		0xff00ced1, /* DARKTURQUOISE */
		0xff9400d3, /* DARKVIOLET */
		0xffff1493, /* DEEPPINK */
		0xff00bfff, /* DEEPSKYBLUE */
		0xff696969, /* DIMGRAY */
		0xff696969, /* DIMGREY */
		0xff1e90ff, /* DODGERBLUE */
		0xffd19275, /* FELDSPAR */
		0xffb22222, /* FIREBRICK */
		0xfffffaf0, /* FLORALWHITE */
		0xff228b22, /* FORESTGREEN */
		0xffff00ff, /* FUCHSIA */
		0xffdcdcdc, /* GAINSBORO */
		0xfff8f8ff, /* GHOSTWHITE */
		0xffffd700, /* GOLD */
		0xffdaa520, /* GOLDENROD */
		0xff808080, /* GRAY */
		0xff008000, /* GREEN */
		0xffadff2f, /* GREENYELLOW */
		0xff808080, /* GREY */
		0xfff0fff0, /* HONEYDEW */
		0xffff69b4, /* HOTPINK */
		0xffcd5c5c, /* INDIANRED */
		0xff4b0082, /* INDIGO */
		0xfffffff0, /* IVORY */
		0xfff0e68c, /* KHAKI */
		0xffe6e6fa, /* LAVENDER */
		0xfffff0f5, /* LAVENDERBLUSH */
		0xff7cfc00, /* LAWNGREEN */
		0xfffffacd, /* LEMONCHIFFON */
		0xffadd8e6, /* LIGHTBLUE */
		0xfff08080, /* LIGHTCORAL */
		0xffe0ffff, /* LIGHTCYAN */
		0xfffafad2, /* LIGHTGOLDENRODYELLOW */
		0xffd3d3d3, /* LIGHTGRAY */
		0xff90ee90, /* LIGHTGREEN */
		0xffd3d3d3, /* LIGHTGREY */
		0xffffb6c1, /* LIGHTPINK */
		0xffffa07a, /* LIGHTSALMON */
		0xff20b2aa, /* LIGHTSEAGREEN */
		0xff87cefa, /* LIGHTSKYBLUE */
		0xff8470ff, /* LIGHTSLATEBLUE */
		0xff778899, /* LIGHTSLATEGRAY */
		0xff778899, /* LIGHTSLATEGREY */
		0xffb0c4de, /* LIGHTSTEELBLUE */
		0xffffffe0, /* LIGHTYELLOW */
		0xff00ff00, /* LIME */
		0xff32cd32, /* LIMEGREEN */
		0xfffaf0e6, /* LINEN */
		0xffff00ff, /* MAGENTA */
		0xff800000, /* MAROON */
		0xff66cdaa, /* MEDIUMAQUAMARINE */
		0xff0000cd, /* MEDIUMBLUE */
		0xffba55d3, /* MEDIUMORCHID */
		0xff9370db, /* MEDIUMPURPLE */
		0xff3cb371, /* MEDIUMSEAGREEN */
		0xff7b68ee, /* MEDIUMSLATEBLUE */
		0xff00fa9a, /* MEDIUMSPRINGGREEN */
		0xff48d1cc, /* MEDIUMTURQUOISE */
		0xffc71585, /* MEDIUMVIOLETRED */
		0xff191970, /* MIDNIGHTBLUE */
		0xfff5fffa, /* MINTCREAM */
		0xffffe4e1, /* MISTYROSE */
		0xffffe4b5, /* MOCCASIN */
		0xffffdead, /* NAVAJOWHITE */
		0xff000080, /* NAVY */
		0xfffdf5e6, /* OLDLACE */
		0xff808000, /* OLIVE */
		0xff6b8e23, /* OLIVEDRAB */
		0xffffa500, /* ORANGE */
		0xffff4500, /* ORANGERED */
		0xffda70d6, /* ORCHID */
		0xffeee8aa, /* PALEGOLDENROD */
		0xff98fb98, /* PALEGREEN */
		0xffafeeee, /* PALETURQUOISE */
		0xffdb7093, /* PALEVIOLETRED */
		0xffffefd5, /* PAPAYAWHIP */
		0xffffdab9, /* PEACHPUFF */
		0xffcd853f, /* PERU */
		0xffffc0cb, /* PINK */
		0xffdda0dd, /* PLUM */
		0xffb0e0e6, /* POWDERBLUE */
		0xff800080, /* PURPLE */
		0xffff0000, /* RED */
		0xffbc8f8f, /* ROSYBROWN */
		0xff4169e1, /* ROYALBLUE */
		0xff8b4513, /* SADDLEBROWN */
		0xfffa8072, /* SALMON */
		0xfff4a460, /* SANDYBROWN */
		0xff2e8b57, /* SEAGREEN */
		0xfffff5ee, /* SEASHELL */
		0xffa0522d, /* SIENNA */
		0xffc0c0c0, /* SILVER */
		0xff87ceeb, /* SKYBLUE */
		0xff6a5acd, /* SLATEBLUE */
		0xff708090, /* SLATEGRAY */
		0xff708090, /* SLATEGREY */
		0xfffffafa, /* SNOW */
		0xff00ff7f, /* SPRINGGREEN */
		0xff4682b4, /* STEELBLUE */
		0xffd2b48c, /* TAN */
		0xff008080, /* TEAL */
		0xffd8bfd8, /* THISTLE */
		0xffff6347, /* TOMATO */
		0xff40e0d0, /* TURQUOISE */
		0xffee82ee, /* VIOLET */
		0xffd02090, /* VIOLETRED */
		0xfff5deb3, /* WHEAT */
		0xffffffff, /* WHITE */
		0xfff5f5f5, /* WHITESMOKE */
		0xffffff00, /* YELLOW */
		0xff9acd32  /* YELLOWGREEN */
	};
	int i;
	bool match;

	for (i = FIRST_COLOUR; i <= LAST_COLOUR; i++) {
          if (lwc_string_caseless_isequal(data, c->strings[i],
                                          &match) == lwc_error_ok &&
				match)
			break;
	}
	if (i == LAST_COLOUR + 1)
		return CSS_INVALID;

	*result = colourmap[i - FIRST_COLOUR];

	return CSS_OK;
}

/**
 * Parse a hash colour (#rgb or #rrggbb)
 *
 * \param data    Pointer to colour string
 * \param result  Pointer to location to receive result (AARRGGBB)
 * \return CSS_OK      on success,
 *         CSS_INVALID if the input is invalid
 */
css_error parse_hash_colour(lwc_string *data, uint32_t *result)
{
	uint8_t r = 0, g = 0, b = 0, a = 0xff;
	size_t len = lwc_string_length(data);
	const char *input = lwc_string_data(data);

	if (len == 3 &&	isHex(input[0]) && isHex(input[1]) &&
			isHex(input[2])) {
		r = charToHex(input[0]);
		g = charToHex(input[1]);
		b = charToHex(input[2]);

		r |= (r << 4);
		g |= (g << 4);
		b |= (b << 4);
	} else if (len == 6 && isHex(input[0]) && isHex(input[1]) &&
			isHex(input[2]) && isHex(input[3]) &&
			isHex(input[4]) && isHex(input[5])) {
		r = (charToHex(input[0]) << 4);
		r |= charToHex(input[1]);
		g = (charToHex(input[2]) << 4);
		g |= charToHex(input[3]);
		b = (charToHex(input[4]) << 4);
		b |= charToHex(input[5]);
	} else
		return CSS_INVALID;

	*result = (a << 24) | (r << 16) | (g << 8) | b;

	return CSS_OK;
}

/**
 * Parse a unit specifier
 *
 * \param c             Parsing context
 * \param vector        Vector of tokens to process
 * \param ctx           Pointer to current vector iteration context
 * \param default_unit  The default unit to use if none specified
 * \param length        Pointer to location to receive length
 * \param unit          Pointer to location to receive unit
 * \return CSS_OK      on success,
 *         CSS_INVALID if the tokens do not form a valid unit
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_unit_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t default_unit,
		css_fixed *length, uint32_t *unit)
{
	int orig_ctx = *ctx;
	const css_token *token;
	css_fixed num;
	size_t consumed = 0;
	css_error error;

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_DIMENSION &&
			token->type != CSS_TOKEN_NUMBER &&
			token->type != CSS_TOKEN_PERCENTAGE)) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	num = number_from_lwc_string(token->idata, false, &consumed);

	if (token->type == CSS_TOKEN_DIMENSION) {
		size_t len = lwc_string_length(token->idata);
		const char *data = lwc_string_data(token->idata);
		css_unit temp_unit = CSS_UNIT_PX;

		error = parse_unit_keyword(data + consumed, len - consumed,
				&temp_unit);
		if (error != CSS_OK) {
			*ctx = orig_ctx;
			return error;
		}

		*unit = (uint32_t) temp_unit;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		/* Non-zero values are permitted in quirks mode */
		if (num != 0) {
			if (c->sheet->quirks_allowed) {
				c->sheet->quirks_used = true;
			} else {
				*ctx = orig_ctx;
				return CSS_INVALID;
			}
		}

		*unit = default_unit;

		if (c->sheet->quirks_allowed) {
			/* Also, in quirks mode, we need to cater for
			 * dimensions separated from their units by whitespace
			 * (e.g. "0 px")
			 */
			int temp_ctx = *ctx;
			css_unit temp_unit;

			consumeWhitespace(vector, &temp_ctx);

			/* Try to parse the unit keyword, ignoring errors */
			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token != NULL && token->type == CSS_TOKEN_IDENT) {
				error = parse_unit_keyword(
						lwc_string_data(token->idata),
						lwc_string_length(token->idata),
						&temp_unit);
				if (error == CSS_OK) {
					c->sheet->quirks_used = true;
					*ctx = temp_ctx;
					*unit = (uint32_t) temp_unit;
				}
			}
		}
	} else {
		/* Percentage -- number must be entire token data */
		if (consumed != lwc_string_length(token->idata)) {
			*ctx = orig_ctx;
			return CSS_INVALID;
		}
		*unit = UNIT_PCT;
	}

	*length = num;

	return CSS_OK;
}

/**
 * Parse a unit keyword
 *
 * \param ptr   Pointer to keyword string
 * \param len   Length, in bytes, of string
 * \param unit  Pointer to location to receive computed unit
 * \return CSS_OK      on success,
 *         CSS_INVALID on encountering an unknown keyword
 */
css_error parse_unit_keyword(const char *ptr, size_t len, css_unit *unit)
{
	if (len == 4) {
		if (strncasecmp(ptr, "grad", 4) == 0)
			*unit = UNIT_GRAD;
		else
			return CSS_INVALID;
	} else if (len == 3) {
		if (strncasecmp(ptr, "kHz", 3) == 0)
			*unit = UNIT_KHZ;
		else if (strncasecmp(ptr, "deg", 3) == 0)
			*unit = UNIT_DEG;
		else if (strncasecmp(ptr, "rad", 3) == 0)
			*unit = UNIT_RAD;
		else
			return CSS_INVALID;
	} else if (len == 2) {
		if (strncasecmp(ptr, "Hz", 2) == 0)
			*unit = UNIT_HZ;
		else if (strncasecmp(ptr, "ms", 2) == 0)
			*unit = UNIT_MS;
		else if (strncasecmp(ptr, "px", 2) == 0)
			*unit = UNIT_PX;
		else if (strncasecmp(ptr, "ex", 2) == 0)
			*unit = UNIT_EX;
		else if (strncasecmp(ptr, "em", 2) == 0)
			*unit = UNIT_EM;
		else if (strncasecmp(ptr, "in", 2) == 0)
			*unit = UNIT_IN;
		else if (strncasecmp(ptr, "cm", 2) == 0)
			*unit = UNIT_CM;
		else if (strncasecmp(ptr, "mm", 2) == 0)
			*unit = UNIT_MM;
		else if (strncasecmp(ptr, "pt", 2) == 0)
			*unit = UNIT_PT;
		else if (strncasecmp(ptr, "pc", 2) == 0)
			*unit = UNIT_PC;
		else
			return CSS_INVALID;
	} else if (len == 1) {
		if (strncasecmp(ptr, "s", 1) == 0)
			*unit = UNIT_S;
		else
			return CSS_INVALID;
	} else
		return CSS_INVALID;

	return CSS_OK;
}

/**
 * Parse a comma separated list, calculating the storage space required
 *
 * \param c         Parsing context
 * \param vector    Vector of tokens to process
 * \param ctx       Pointer to vector iteration context
 * \param token     Pointer to current token
 * \param reserved  Callback to determine if an identifier is reserved
 * \param size      Pointer to location to receive required storage
 * \return CSS_OK      on success,
 *         CSS_INVALID if the input is invalid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error comma_list_length(css_language *c,
		const parserutils_vector *vector, int *ctx,
		const css_token *token,
		bool (*reserved)(css_language *c, const css_token *ident),
		uint32_t *size)
{
	int orig_ctx = *ctx;
	uint32_t opv;
	uint32_t required_size = 0;
	bool first = true;

	while (token != NULL) {
		if (token->type == CSS_TOKEN_IDENT) {
			/* IDENT+ */
			required_size += first ? 0 : sizeof(opv);

			if (reserved(c, token) == false) {
				required_size += sizeof(lwc_string *);

				/* Skip past [ IDENT* S* ]* */
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

					/* Idents must not be reserved */
					if (token != NULL && token->type ==
							CSS_TOKEN_IDENT &&
							reserved(c, token)) {
						*ctx = orig_ctx;
						return CSS_INVALID;
					}

					token = parserutils_vector_iterate(
						vector, ctx);
				}
			}
		} else if (token->type == CSS_TOKEN_STRING) {
			/* STRING */
			required_size += first ? 0 : sizeof(opv);

			required_size += sizeof(lwc_string *);
		} else {
			/* Invalid token */
			*ctx = orig_ctx;
			return CSS_INVALID;
		}

		consumeWhitespace(vector, ctx);

		/* Look for a comma */
		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, ',')) {
			/* Got one. Move past it */
			parserutils_vector_iterate(vector, ctx);

			consumeWhitespace(vector, ctx);

			/* Ensure that a valid token follows */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_STRING)) {
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
		token = parserutils_vector_iterate(vector, ctx);
	}

	required_size += sizeof(opv);

	*size = required_size;

	return CSS_OK;
}

/**
 * Parse a comma separated list, converting to bytecode
 *
 * \param c          Parsing context
 * \param vector     Vector of tokens to process
 * \param ctx        Pointer to vector iteration context
 * \param token      Pointer to current token
 * \param reserved   Callback to determine if an identifier is reserved
 * \param get_value  Callback to retrieve bytecode value for a token
 * \param bytecode   Pointer to pointer to bytecode buffer. Updated on exit.
 * \return CSS_OK      on success,
 *         CSS_INVALID if the input is invalid
 *
 * \note The bytecode buffer must be at least comma_list_length() bytes long.
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error comma_list_to_bytecode(css_language *c,
		const parserutils_vector *vector, int *ctx,
		const css_token *token,
		bool (*reserved)(css_language *c, const css_token *ident),
		uint16_t (*get_value)(css_language *c, const css_token *token),
		uint8_t **bytecode)
{
	int orig_ctx = *ctx;
	uint8_t *ptr = *bytecode;
	bool first = true;
	uint32_t opv;
	uint8_t *buf = NULL;
	uint32_t buflen = 0;
	css_error error = CSS_OK;

	while (token != NULL) {
		if (token->type == CSS_TOKEN_IDENT) {
			lwc_string *tok_idata = token->idata;
			lwc_string *name = tok_idata;
			lwc_string *newname;

			opv = get_value(c, token);

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}

			if (reserved(c, token) == false) {
				uint32_t len = lwc_string_length(token->idata);
				const css_token *temp_token = token;
				int temp_ctx = *ctx;
				lwc_error lerror;
				uint8_t *p;

				/* Calculate required length of temp buffer */
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

					if (temp_token != NULL &&
						temp_token->type ==
							CSS_TOKEN_IDENT) {
						len += lwc_string_length(
							temp_token->idata);
					} else if (temp_token != NULL) {
						len += 1;
					}

					temp_token = parserutils_vector_iterate(
							vector, &temp_ctx);
				}

				if (len > buflen) {
					/* Allocate buffer */
					uint8_t *b = c->alloc(buf, len, c->pw);
					if (b == NULL) {
						error = CSS_NOMEM;
						goto cleanup;
					}

					buf = b;
					buflen = len;
				}

				p = buf;

				/* Populate buffer with string data */
				memcpy(p, lwc_string_data(token->idata),
					lwc_string_length(token->idata));
				p += lwc_string_length(token->idata);

				while (token != NULL) {
					token = parserutils_vector_peek(
							vector, *ctx);
					if (token != NULL && token->type !=
							CSS_TOKEN_IDENT &&
							token->type !=
							CSS_TOKEN_S) {
						break;
					}

					if (token != NULL && token->type ==
							CSS_TOKEN_IDENT) {
						memcpy(p, lwc_string_data(
								token->idata),
							lwc_string_length(
								token->idata));
						p += lwc_string_length(
								token->idata);
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
				lerror = lwc_intern_string(
						(char *) buf, p - buf,
						&newname);
				if (lerror != lwc_error_ok) {
					error = css_error_from_lwc_error(
							lerror);
					goto cleanup;
				}

				if (newname == name) {
					lwc_string_unref(newname);
				}

				name = newname;

				/* Only ref 'name' again if the token owns it,
				 * otherwise we already own the only ref to the
				 * new name generated above.
				 */
				if (name == tok_idata) {
					lwc_string_ref(name);
				}

				memcpy(ptr, &name, sizeof(name));
				ptr += sizeof(name);
			}
		} else if (token->type == CSS_TOKEN_STRING) {
			opv = get_value(c, token);

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}

			lwc_string_ref(token->idata);

			memcpy(ptr, &token->idata, sizeof(token->idata));
			ptr += sizeof(token->idata);
		} else {
			error = CSS_INVALID;
			goto cleanup;
		}

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, ',')) {
			parserutils_vector_iterate(vector, ctx);

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_STRING)) {
				error = CSS_INVALID;
				goto cleanup;
			}
		} else {
			break;
		}

		first = false;

		token = parserutils_vector_iterate(vector, ctx);
	}

	*bytecode = ptr;

cleanup:
	if (buf)
		c->alloc(buf, 0, c->pw);

	if (error != CSS_OK)
		*ctx = orig_ctx;

	return error;
}

