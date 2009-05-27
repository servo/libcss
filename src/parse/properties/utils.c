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

css_error parse_important(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint8_t *result)
{
	const css_token *token;

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token != NULL && tokenIsChar(token, '!')) {
		consumeWhitespace(vector, ctx);

		token = parserutils_vector_iterate(vector, ctx);
		if (token == NULL || token->type != CSS_TOKEN_IDENT)
			return CSS_INVALID;

		if (token->ilower == c->strings[IMPORTANT])
			*result |= FLAG_IMPORTANT;
	} else if (token != NULL)
		return CSS_INVALID;

	return CSS_OK;
}

css_error parse_colour_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t *result)
{
	const css_token *token;
	uint8_t r = 0, g = 0, b = 0;
	css_error error;

	UNUSED(c);

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
				(token != NULL &&
				token->type != CSS_TOKEN_NUMBER && 
				token->type != CSS_TOKEN_DIMENSION))
			return CSS_INVALID;
	}

	if (token->type == CSS_TOKEN_IDENT) {
		error = parse_named_colour(c, token->idata, result);
		if (error != CSS_OK && c->sheet->quirks_allowed) {
			error = parse_hash_colour(token->idata, result);
			if (error == CSS_OK)
				c->sheet->quirks_used = true;
		}
			
		return error;
	} else if (token->type == CSS_TOKEN_HASH) {
		return parse_hash_colour(token->idata, result);
	} else if (c->sheet->quirks_allowed && 
			token->type == CSS_TOKEN_NUMBER) {
		error = parse_hash_colour(token->idata, result);
		if (error == CSS_OK)
			c->sheet->quirks_used = true;
		return error;
	} else if (c->sheet->quirks_allowed && 
			token->type == CSS_TOKEN_DIMENSION) {
		error = parse_hash_colour(token->idata, result);
		if (error == CSS_OK)
			c->sheet->quirks_used = true;
		return error;
	} else if (token->type == CSS_TOKEN_FUNCTION) {
		if (token->ilower == c->strings[RGB]) {
			int i;
			css_token_type valid = CSS_TOKEN_NUMBER;

			for (i = 0; i < 3; i++) {
				css_fixed num;
				size_t consumed = 0;
				uint8_t *component = i == 0 ? &r 
							    : i == 1 ? &b : &g;
				int32_t intval;

				consumeWhitespace(vector, ctx);

				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL || (token->type != 
						CSS_TOKEN_NUMBER && 
						token->type != 
						CSS_TOKEN_PERCENTAGE))
					return CSS_INVALID;

				if (i == 0)
					valid = token->type;
				else if (token->type != valid)
					return CSS_INVALID;

				num = number_from_lwc_string(token->idata,
						valid == CSS_TOKEN_NUMBER, 
						&consumed);
				if (consumed != lwc_string_length(token->idata))
					return CSS_INVALID;

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
					return CSS_INVALID;

				if (i != 2 && tokenIsChar(token, ','))
					parserutils_vector_iterate(vector, ctx);
				else if (i == 2 && tokenIsChar(token, ')'))
					parserutils_vector_iterate(vector, ctx);
				else
					return CSS_INVALID;
			}
		} else
			return CSS_INVALID;
	}

	*result = (r << 24) | (g << 16) | (b << 8);

	return CSS_OK;
}

css_error parse_named_colour(css_language *c, lwc_string *data, 
		uint32_t *result)
{
	static const uint32_t colourmap[LAST_COLOUR + 1 - FIRST_COLOUR] = {
		0xf0f8ff00, /* ALICEBLUE */
		0xfaebd700, /* ANTIQUEWHITE */
		0x00ffff00, /* AQUA */
		0x7fffd400, /* AQUAMARINE */
		0xf0ffff00, /* AZURE */
		0xf5f5dc00, /* BEIGE */
		0xffe4c400, /* BISQUE */
		0x00000000, /* BLACK */
		0xffebcd00, /* BLANCHEDALMOND */
		0x0000ff00, /* BLUE */
		0x8a2be200, /* BLUEVIOLET */
		0xa52a2a00, /* BROWN */
		0xdeb88700, /* BURLYWOOD */
		0x5f9ea000, /* CADETBLUE */
		0x7fff0000, /* CHARTREUSE */
		0xd2691e00, /* CHOCOLATE */
		0xff7f5000, /* CORAL */
		0x6495ed00, /* CORNFLOWERBLUE */
		0xfff8dc00, /* CORNSILK */
		0xdc143c00, /* CRIMSON */
		0x00ffff00, /* CYAN */
		0x00008b00, /* DARKBLUE */
		0x008b8b00, /* DARKCYAN */
		0xb8860b00, /* DARKGOLDENROD */
		0xa9a9a900, /* DARKGRAY */
		0x00640000, /* DARKGREEN */
		0xa9a9a900, /* DARKGREY */
		0xbdb76b00, /* DARKKHAKI */
		0x8b008b00, /* DARKMAGENTA */
		0x556b2f00, /* DARKOLIVEGREEN */
		0xff8c0000, /* DARKORANGE */
		0x9932cc00, /* DARKORCHID */
		0x8b000000, /* DARKRED */
		0xe9967a00, /* DARKSALMON */
		0x8fbc8f00, /* DARKSEAGREEN */
		0x483d8b00, /* DARKSLATEBLUE */
		0x2f4f4f00, /* DARKSLATEGRAY */
		0x2f4f4f00, /* DARKSLATEGREY */
		0x00ced100, /* DARKTURQUOISE */
		0x9400d300, /* DARKVIOLET */
		0xff149300, /* DEEPPINK */
		0x00bfff00, /* DEEPSKYBLUE */
		0x69696900, /* DIMGRAY */
		0x69696900, /* DIMGREY */
		0x1e90ff00, /* DODGERBLUE */
		0xd1927500, /* FELDSPAR */
		0xb2222200, /* FIREBRICK */
		0xfffaf000, /* FLORALWHITE */
		0x228b2200, /* FORESTGREEN */
		0xff00ff00, /* FUCHSIA */
		0xdcdcdc00, /* GAINSBORO */
		0xf8f8ff00, /* GHOSTWHITE */
		0xffd70000, /* GOLD */
		0xdaa52000, /* GOLDENROD */
		0x80808000, /* GRAY */
		0x00800000, /* GREEN */
		0xadff2f00, /* GREENYELLOW */
		0x80808000, /* GREY */
		0xf0fff000, /* HONEYDEW */
		0xff69b400, /* HOTPINK */
		0xcd5c5c00, /* INDIANRED */
		0x4b008200, /* INDIGO */
		0xfffff000, /* IVORY */
		0xf0e68c00, /* KHAKI */
		0xe6e6fa00, /* LAVENDER */
		0xfff0f500, /* LAVENDERBLUSH */
		0x7cfc0000, /* LAWNGREEN */
		0xfffacd00, /* LEMONCHIFFON */
		0xadd8e600, /* LIGHTBLUE */
		0xf0808000, /* LIGHTCORAL */
		0xe0ffff00, /* LIGHTCYAN */
		0xfafad200, /* LIGHTGOLDENRODYELLOW */
		0xd3d3d300, /* LIGHTGRAY */
		0x90ee9000, /* LIGHTGREEN */
		0xd3d3d300, /* LIGHTGREY */
		0xffb6c100, /* LIGHTPINK */
		0xffa07a00, /* LIGHTSALMON */
		0x20b2aa00, /* LIGHTSEAGREEN */
		0x87cefa00, /* LIGHTSKYBLUE */
		0x8470ff00, /* LIGHTSLATEBLUE */
		0x77889900, /* LIGHTSLATEGRAY */
		0x77889900, /* LIGHTSLATEGREY */
		0xb0c4de00, /* LIGHTSTEELBLUE */
		0xffffe000, /* LIGHTYELLOW */
		0x00ff0000, /* LIME */
		0x32cd3200, /* LIMEGREEN */
		0xfaf0e600, /* LINEN */
		0xff00ff00, /* MAGENTA */
		0x80000000, /* MAROON */
		0x66cdaa00, /* MEDIUMAQUAMARINE */
		0x0000cd00, /* MEDIUMBLUE */
		0xba55d300, /* MEDIUMORCHID */
		0x9370db00, /* MEDIUMPURPLE */
		0x3cb37100, /* MEDIUMSEAGREEN */
		0x7b68ee00, /* MEDIUMSLATEBLUE */
		0x00fa9a00, /* MEDIUMSPRINGGREEN */
		0x48d1cc00, /* MEDIUMTURQUOISE */
		0xc7158500, /* MEDIUMVIOLETRED */
		0x19197000, /* MIDNIGHTBLUE */
		0xf5fffa00, /* MINTCREAM */
		0xffe4e100, /* MISTYROSE */
		0xffe4b500, /* MOCCASIN */
		0xffdead00, /* NAVAJOWHITE */
		0x00008000, /* NAVY */
		0xfdf5e600, /* OLDLACE */
		0x80800000, /* OLIVE */
		0x6b8e2300, /* OLIVEDRAB */
		0xffa50000, /* ORANGE */
		0xff450000, /* ORANGERED */
		0xda70d600, /* ORCHID */
		0xeee8aa00, /* PALEGOLDENROD */
		0x98fb9800, /* PALEGREEN */
		0xafeeee00, /* PALETURQUOISE */
		0xdb709300, /* PALEVIOLETRED */
		0xffefd500, /* PAPAYAWHIP */
		0xffdab900, /* PEACHPUFF */
		0xcd853f00, /* PERU */
		0xffc0cb00, /* PINK */
		0xdda0dd00, /* PLUM */
		0xb0e0e600, /* POWDERBLUE */
		0x80008000, /* PURPLE */
		0xff000000, /* RED */
		0xbc8f8f00, /* ROSYBROWN */
		0x4169e100, /* ROYALBLUE */
		0x8b451300, /* SADDLEBROWN */
		0xfa807200, /* SALMON */
		0xf4a46000, /* SANDYBROWN */
		0x2e8b5700, /* SEAGREEN */
		0xfff5ee00, /* SEASHELL */
		0xa0522d00, /* SIENNA */
		0xc0c0c000, /* SILVER */
		0x87ceeb00, /* SKYBLUE */
		0x6a5acd00, /* SLATEBLUE */
		0x70809000, /* SLATEGRAY */
		0x70809000, /* SLATEGREY */
		0xfffafa00, /* SNOW */
		0x00ff7f00, /* SPRINGGREEN */
		0x4682b400, /* STEELBLUE */
		0xd2b48c00, /* TAN */
		0x00808000, /* TEAL */
		0xd8bfd800, /* THISTLE */
		0xff634700, /* TOMATO */
		0x40e0d000, /* TURQUOISE */
		0xee82ee00, /* VIOLET */
		0xd0209000, /* VIOLETRED */
		0xf5deb300, /* WHEAT */
		0xffffff00, /* WHITE */
		0xf5f5f500, /* WHITESMOKE */
		0xffff0000, /* YELLOW */
		0x9acd3200  /* YELLOWGREEN */
	};
	int i;

	for (i = FIRST_COLOUR; i <= LAST_COLOUR; i++) {
		if (data == c->strings[i])
			break;
	}
	if (i == LAST_COLOUR + 1)
		return CSS_INVALID;

	*result = colourmap[i - FIRST_COLOUR];

	return CSS_OK;
}

css_error parse_hash_colour(lwc_string *data, uint32_t *result)
{
	uint8_t r = 0, g = 0, b = 0;
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

	*result = (r << 24) | (g << 16) | (b << 8);

	return CSS_OK;
}

css_error parse_unit_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t default_unit,
		css_fixed *length, uint32_t *unit)
{
	const css_token *token;
	css_fixed num;
	size_t consumed = 0;
	css_error error;

	UNUSED(c);

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_DIMENSION &&
			token->type != CSS_TOKEN_NUMBER &&
			token->type != CSS_TOKEN_PERCENTAGE))
		return CSS_INVALID;

	num = number_from_lwc_string(token->idata, false, &consumed);

	if (token->type == CSS_TOKEN_DIMENSION) {
		size_t len = lwc_string_length(token->idata);
		const char *data = lwc_string_data(token->idata);

		error = parse_unit_keyword(data + consumed, len - consumed, 
				unit);
		if (error != CSS_OK)
			return error;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		/* Non-zero values are permitted in quirks mode */
		if (num != 0) {
			if (c->sheet->quirks_allowed)
				c->sheet->quirks_used = true;
			else
				return CSS_INVALID;
		}

		*unit = default_unit;

		if (c->sheet->quirks_allowed) {
			/* Also, in quirks mode, we need to cater for 
			 * dimensions separated from their units by whitespace
			 * (e.g. "0 px")
			 */
			int temp_ctx = *ctx;
			uint32_t temp_unit;

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_iterate(vector, &temp_ctx);
			if (token != NULL && token->type == CSS_TOKEN_IDENT) {
				error = parse_unit_keyword(
						lwc_string_data(token->idata),
						lwc_string_length(token->idata),
						&temp_unit);
				if (error == CSS_OK) {
					c->sheet->quirks_used = true;
					*ctx = temp_ctx;
					*unit = temp_unit;
				}
			}
		}

	} else {
		if (consumed != lwc_string_length(token->idata))
			return CSS_INVALID;
		*unit = UNIT_PCT;
	}

	*length = num;

	return CSS_OK;
}

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

