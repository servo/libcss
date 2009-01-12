/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_parse_css21props_c_
#define css_parse_css21props_c_

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"

static css_error parse_azimuth(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_attachment(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_image(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_position(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_repeat(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_collapse(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_spacing(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_bottom(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_caption_side(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_clear(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_clip(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_content(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_counter_increment(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_counter_reset(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cue_after(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cue_before(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cursor(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_direction(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_display(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_elevation(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_empty_cells(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_float(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_family(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_size(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_variant(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_weight(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_height(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_left(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_letter_spacing(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_line_height(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_image(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_position(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_type(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_bottom(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_left(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_right(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_top(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_max_height(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_max_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_min_height(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_min_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_orphans(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_color(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_style(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_overflow(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_bottom(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_left(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_right(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_top(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_after(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_before(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_inside(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pause_after(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pause_before(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pitch_range(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pitch(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_play_during(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_position(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_quotes(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_richness(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_right(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_header(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_numeral(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_punctuation(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speech_rate(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_stress(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_table_layout(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_align(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_decoration(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_indent(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_transform(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_top(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_unicode_bidi(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_vertical_align(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_visibility(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_voice_family(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_volume(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_white_space(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_widows(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_width(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_word_spacing(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_z_index(css_language *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);

static inline css_error parse_important(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint8_t *result);
static inline css_error parse_colour_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t *result);
static inline css_error parse_unit_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		fixed *length, uint32_t *unit);

static inline css_error parse_border_side_color(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_border_side_style(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_border_side_width(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_margin_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_padding_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_list_style_type_value(css_language *c,
		const css_token *token, uint16_t *value);
static inline css_error parse_content_list(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint8_t *buffer, uint32_t *buflen);

/**
 * Type of property handler function
 */
typedef css_error (*css_prop_handler)(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result);

/**
 * Dispatch table of property handlers, indexed by property enum
 */
static const css_prop_handler property_handlers[LAST_PROP + 1 - FIRST_PROP] =
{
	parse_azimuth,
	parse_background_attachment,
	parse_background_color,
	parse_background_image,
	parse_background_position,
	parse_background_repeat,
	parse_border_bottom_color,
	parse_border_bottom_style,
	parse_border_bottom_width,
	parse_border_collapse,
	parse_border_left_color,
	parse_border_left_style,
	parse_border_left_width,
	parse_border_right_color,
	parse_border_right_style,
	parse_border_right_width,
	parse_border_spacing,
	parse_border_top_color,
	parse_border_top_style,
	parse_border_top_width,
	parse_bottom,
	parse_caption_side,
	parse_clear,
	parse_clip,
	parse_color,
	parse_content,
	parse_counter_increment,
	parse_counter_reset,
	parse_cue_after,
	parse_cue_before,
	parse_cursor,
	parse_direction,
	parse_display,
	parse_elevation,
	parse_empty_cells,
	parse_float,
	parse_font_family,
	parse_font_size,
	parse_font_style,
	parse_font_variant,
	parse_font_weight,
	parse_height,
	parse_left,
	parse_letter_spacing,
	parse_line_height,
	parse_list_style_image,
	parse_list_style_position,
	parse_list_style_type,
	parse_margin_bottom,
	parse_margin_left,
	parse_margin_right,
	parse_margin_top,
	parse_max_height,
	parse_max_width,
	parse_min_height,
	parse_min_width,
	parse_orphans,
	parse_outline_color,
	parse_outline_style,
	parse_outline_width,
	parse_overflow,
	parse_padding_bottom,
	parse_padding_left,
	parse_padding_right,
	parse_padding_top,
	parse_page_break_after,
	parse_page_break_before,
	parse_page_break_inside,
	parse_pause_after,
	parse_pause_before,
	parse_pitch_range,
	parse_pitch,
	parse_play_during,
	parse_position,
	parse_quotes,
	parse_richness,
	parse_right,
	parse_speak_header,
	parse_speak_numeral,
	parse_speak_punctuation,
	parse_speak,
	parse_speech_rate,
	parse_stress,
	parse_table_layout,
	parse_text_align,
	parse_text_decoration,
	parse_text_indent,
	parse_text_transform,
	parse_top,
	parse_unicode_bidi,
	parse_vertical_align,
	parse_visibility,
	parse_voice_family,
	parse_volume,
	parse_white_space,
	parse_widows,
	parse_width,
	parse_word_spacing,
	parse_z_index,
};

css_error parse_azimuth(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* angle | [ IDENT(left-side, far-left, left, center-left, center, 
	 *                 center-right, right, far-right, right-side) || 
	 *           IDENT(behind) 
	 *         ] 
	 *       | IDENT(leftwards, rightwards, inherit)
	 */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

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
		} else if (token->ilower == c->strings[CENTER-LEFT]) {
			value = AZIMUTH_CENTER_LEFT;
		} else if (token->ilower == c->strings[CENTER]) {
			value = AZIMUTH_CENTER;
		} else if (token->ilower == c->strings[CENTER-RIGHT]) {
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
			return CSS_INVALID;
		}

		consumeWhitespace(vector, ctx);

		/* Get potential following token */
		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && token->type != CSS_TOKEN_IDENT &&
				tokenIsChar(token, '!') == false)
			return CSS_INVALID;

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
				return CSS_INVALID;
			}
		} else if (token != NULL && token->type == CSS_TOKEN_IDENT &&
				value != AZIMUTH_BEHIND) {
			parserutils_vector_iterate(vector, ctx);

			if (token->ilower == c->strings[BEHIND]) {
				value |= AZIMUTH_BEHIND;
			} else {
				return CSS_INVALID;
			}
		}
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_ANGLE) == false)
			return CSS_INVALID;

		value = AZIMUTH_ANGLE;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_AZIMUTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == AZIMUTH_ANGLE)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_background_attachment(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (fixed, scroll, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[FIXED]) {
		value = BACKGROUND_ATTACHMENT_FIXED;
	} else if (ident->ilower == c->strings[SCROLL]) {
		value = BACKGROUND_ATTACHMENT_SCROLL;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BACKGROUND_ATTACHMENT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_background_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (transparent, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

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
		if (error != CSS_OK)
			return error;

		value = BACKGROUND_COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BACKGROUND_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_background_image(css_language *c, 
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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[NONE]) {
		value = BACKGROUND_IMAGE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = BACKGROUND_IMAGE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BACKGROUND_IMAGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_IMAGE_URI)
		required_size += sizeof(parserutils_hash_entry *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(parserutils_hash_entry *));
	}

	return CSS_OK;
}

css_error parse_background_position(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint32_t opv;
	uint16_t value[2] = { 0 };
	fixed length[2] = { 0 };
	uint32_t unit[2] = { 0 };
	uint32_t required_size;

	/* [length | percentage | IDENT(left, right, top, bottom, center)]{1,2}
	 * | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

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
					return CSS_INVALID;
				}
			} else {
				error = parse_unit_specifier(c, vector, ctx, 
						&length[i], &unit[i]);
				if (error != CSS_OK)
					return error;

				if (unit[i] & UNIT_ANGLE || 
						unit[i] & UNIT_TIME || 
						unit[i] & UNIT_FREQ)
					return CSS_INVALID;

				/* We'll fix this up later, too */
				value[i] = BACKGROUND_POSITION_VERT_SET;
			}

			consumeWhitespace(vector, ctx);
		}

		/* Now, sort out the mess we've got */
		if (i == 1) {
			/* Only one (horizontal) value, so vertical is center */
			switch (value[0]) {
			case BACKGROUND_POSITION_HORZ_LEFT:
			case BACKGROUND_POSITION_HORZ_RIGHT:
			case BACKGROUND_POSITION_VERT_CENTER:
				break;
			case BACKGROUND_POSITION_VERT_SET:
				value[0] = BACKGROUND_POSITION_HORZ_SET;
				break;
			default:
				return CSS_INVALID;
			}

			value[1] = BACKGROUND_POSITION_VERT_CENTER;
		} else if (value[0] != BACKGROUND_POSITION_VERT_SET &&
				value[1] != BACKGROUND_POSITION_VERT_SET) {
			/* Two keywords. Verify the axes differ */
			if (((value[0] & 0xf) != 0 && (value[1] & 0xf) != 0) ||
					((value[0] & 0xf0) != 0 && 
						(value[1] & 0xf0) != 0))
				return CSS_INVALID;
		} else {
			/* One or two non-keywords. First is horizontal */
			if (value[0] == BACKGROUND_POSITION_VERT_SET)
				value[0] = BACKGROUND_POSITION_HORZ_SET;

			/* Verify the axes differ */
			if (((value[0] & 0xf) != 0 && (value[1] & 0xf) != 0) ||
					((value[0] & 0xf0) != 0 && 
						(value[1] & 0xf0) != 0))
				return CSS_INVALID;
		}
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BOTTOM, flags, value[0] | value[1]);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false) { 
		if (value[0] == BACKGROUND_POSITION_HORZ_SET)
			required_size += sizeof(length[0]) + sizeof(unit[0]);
		if (value[1] == BACKGROUND_POSITION_VERT_SET)
			required_size += sizeof(length[1]) + sizeof(unit[1]);
	}

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_background_repeat(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (no-repeat, repeat-x, repeat-y, repeat, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BACKGROUND_REPEAT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_bottom_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_bottom_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_bottom_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_collapse(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (collapse, separate, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[COLLAPSE]) {
		value = BORDER_COLLAPSE_COLLAPSE;
	} else if (ident->ilower == c->strings[SEPARATE]) {
		value = BORDER_COLLAPSE_SEPARATE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BORDER_COLLAPSE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_left_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_left_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_left_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_right_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_right_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_right_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length[2] = { 0 };
	uint32_t unit[2] = { 0 };
	uint32_t required_size;

	/* length length? | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		int num_lengths = 0;

		error = parse_unit_specifier(c, vector, ctx, 
				&length[0], &unit[0]);
		if (error != CSS_OK)
			return error;

		if (unit[0] & UNIT_ANGLE || unit[0] & UNIT_TIME || 
				unit[0] & UNIT_FREQ || unit[0] & UNIT_PCT)
			return CSS_INVALID;

		num_lengths = 1;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_peek(vector, *ctx);
		if (token != NULL && tokenIsChar(token, '!') == false) {
			error = parse_unit_specifier(c, vector, ctx, 
					&length[1], &unit[1]);
			if (error != CSS_OK)
				return error;

			if (unit[1] & UNIT_ANGLE || unit[1] & UNIT_TIME ||
					unit[1] & UNIT_FREQ || 
					unit[1] & UNIT_PCT)
				return CSS_INVALID;

			num_lengths = 2;
		}

		if (num_lengths == 1) {
			/* Only one length specified. Use for both axes. */
			length[1] = length[0];
			unit[1] = unit[0];
		}

		/* Lengths must not be negative */
		if (length[0] < 0 || length[1] < 0)
			return CSS_INVALID;

		value = BORDER_SPACING_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BORDER_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_SPACING_SET)
		required_size += 2 * (sizeof(length[0]) + sizeof(unit[0]));

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_border_top_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_border_top_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_border_top_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_bottom(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = BOTTOM_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = BOTTOM_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BOTTOM, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BOTTOM_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BOTTOM_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_caption_side(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (top, bottom, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[TOP]) {
		value = CAPTION_SIDE_TOP;
	} else if (ident->ilower == c->strings[BOTTOM]) {
		value = CAPTION_SIDE_BOTTOM;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CAPTION_SIDE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_clear(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (left, right, both, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[RIGHT]) {
		value = CLEAR_RIGHT;
	} else if (ident->ilower == c->strings[LEFT]) {
		value = CLEAR_LEFT;
	} else if (ident->ilower == c->strings[BOTH]) {
		value = CLEAR_BOTH;
	} else if (ident->ilower == c->strings[NONE]) {
		value = CLEAR_NONE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CLEAR, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_clip(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	int num_lengths = 0;
	fixed length[4] = { 0 };
	uint32_t unit[4] = { 0 };
	uint32_t required_size;

	/* FUNCTION(rect) [ [ IDENT(auto) | length ] CHAR(,)? ]{3} 
	 *                [ IDENT(auto) | length ] CHAR{)} |
	 * IDENT(auto, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		value = CLIP_AUTO;
	} else if (token->type == CSS_TOKEN_FUNCTION &&
			token->ilower == c->strings[RECT]) {
		value = CLIP_SHAPE_RECT;

		consumeWhitespace(vector, ctx);

		for (int i = 0; i < 4; i++) {
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL)
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_IDENT) {
				/* Slightly magical way of generating the auto 
				 * values. These are bits 3-6 of the value. */
				if (token->ilower == c->strings[AUTO])
					value |= (i+1) << 3;
				else
					return CSS_INVALID;
			} else {
				error = parse_unit_specifier(c, vector, ctx, 
						&length[i], &unit[i]);
				if (error != CSS_OK)
					return error;

				if (unit[i] & UNIT_ANGLE || 
						unit[i] & UNIT_TIME || 
						unit[i] & UNIT_FREQ || 
						unit[i] & UNIT_PCT)
					return CSS_INVALID;

				num_lengths++;
			}

			consumeWhitespace(vector, ctx);

			/* Consume optional comma after first 3 parameters */
			if (i < 3) {
				token = parserutils_vector_peek(vector, *ctx);
				if (token == NULL)
					return CSS_INVALID;

				if (tokenIsChar(token, ','))
					parserutils_vector_iterate(vector, ctx);
			}
		}

		consumeWhitespace(vector, ctx);

		/* Finally, consume closing parenthesis */
		token = parserutils_vector_iterate(vector, ctx);
		if (token == NULL || tokenIsChar(token, ')') == false)
			return CSS_INVALID;
	} else {
		return CSS_INVALID;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_CLIP, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == CLIP_SHAPE_RECT) {
		required_size += 
			num_lengths * (sizeof(length[0]) + sizeof(unit[0]));
	}

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CLIP_SHAPE_RECT) {
		uint8_t *ptr = ((uint8_t *) (*result)->bytecode) + sizeof(opv);

		for (int i = 0; i < num_lengths; i++) {
			memcpy(ptr, &length[i], sizeof(length[i]));
			ptr += sizeof(length[i]);
			memcpy(ptr, &unit[i], sizeof(unit[i]));
			ptr += sizeof(unit[i]);
		}
	}

	return CSS_OK;
}

css_error parse_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

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
	} else if (token->type == CSS_TOKEN_IDENT &&
			 token->ilower == c->strings[NORMAL]) {
		value = CONTENT_NORMAL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			 token->ilower == c->strings[NONE]) {
		value = CONTENT_NONE;
	} else {
		uint32_t len;

		error = parse_content_list(c, vector, &temp_ctx, NULL, &len);
		if (error != CSS_OK)
			return error;

		required_size += len;
	}

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_CONTENT, flags, value);

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
		/* Nothing to do */
	} else {
		error = parse_content_list(c, vector, ctx, ptr, NULL);
		if (error != CSS_OK)
			return error;
	}

	/* Ensure we skip past !important */
	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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

	/* [IDENT <integer> ]+ | IDENT(none, inherit) */

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
			const parserutils_hash_entry *name = token->idata;
			fixed increment = 1;

			consumeWhitespace(vector, &temp_ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_NUMBER) {
				const css_string temp = { token->ilower->len,
					(uint8_t *) token->ilower->data };
				size_t consumed = 0;

				increment = number_from_css_string(&temp,
						true, &consumed);

				if (consumed != token->ilower->len)
					return CSS_INVALID;

				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);
			}

			if (first == false) {
				required_size += sizeof(opv);
			}
			required_size += sizeof(name) + sizeof(increment);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, &temp_ctx);
		}

		/* And for the terminator */
		required_size += sizeof(opv);
	}

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_COUNTER_INCREMENT, flags, value);

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
			const parserutils_hash_entry *name = token->idata;
			fixed increment = 1;

			consumeWhitespace(vector, ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_NUMBER) {
				const css_string temp = { token->ilower->len,
					(uint8_t *) token->ilower->data };
				size_t consumed = 0;

				increment = number_from_css_string(&temp,
						true, &consumed);

				if (consumed != token->ilower->len)
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
			memcpy(ptr, &name, sizeof(name));
			ptr += sizeof(name);
			memcpy(ptr, &increment, sizeof(increment));
			ptr += sizeof(increment);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
		}

		/* And for the terminator */
		opv = COUNTER_INCREMENT_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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

	/* [IDENT <integer> ]+ | IDENT(none, inherit) */

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
			const parserutils_hash_entry *name = token->idata;
			fixed increment = 1;

			consumeWhitespace(vector, &temp_ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, temp_ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_NUMBER) {
				const css_string temp = { token->ilower->len,
					(uint8_t *) token->ilower->data };
				size_t consumed = 0;

				increment = number_from_css_string(&temp,
						true, &consumed);

				if (consumed != token->ilower->len)
					return CSS_INVALID;

				parserutils_vector_iterate(vector, &temp_ctx);

				consumeWhitespace(vector, &temp_ctx);
			}

			if (first == false) {
				required_size += sizeof(opv);
			}
			required_size += sizeof(name) + sizeof(increment);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, &temp_ctx);
		}

		/* And for the terminator */
		required_size += sizeof(opv);
	}

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_COUNTER_RESET, flags, value);

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
			const parserutils_hash_entry *name = token->idata;
			fixed increment = 1;

			consumeWhitespace(vector, ctx);

			/* Optional integer */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
					token->type != CSS_TOKEN_NUMBER))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_NUMBER) {
				const css_string temp = { token->ilower->len,
					(uint8_t *) token->ilower->data };
				size_t consumed = 0;

				increment = number_from_css_string(&temp,
						true, &consumed);

				if (consumed != token->ilower->len)
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

			if (first == false) {
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}
			memcpy(ptr, &name, sizeof(name));
			ptr += sizeof(name);
			memcpy(ptr, &increment, sizeof(increment));
			ptr += sizeof(increment);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
		}

		/* And for the terminator */
		opv = COUNTER_RESET_NONE;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	return CSS_OK;
}

css_error parse_cue_after(css_language *c, 
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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[NONE]) {
		value = CUE_AFTER_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_AFTER_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CUE_AFTER, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == CUE_AFTER_URI)
		required_size += sizeof(parserutils_hash_entry *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_AFTER_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(parserutils_hash_entry *));
	}

	return CSS_OK;
}

css_error parse_cue_before(css_language *c, 
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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[NONE]) {
		value = CUE_BEFORE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_BEFORE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CUE_BEFORE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == CUE_BEFORE_URI)
		required_size += sizeof(parserutils_hash_entry *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_BEFORE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(parserutils_hash_entry *));
	}

	return CSS_OK;
}

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

	/* [ URI* IDENT(auto, crosshair, default, pointer, move, e-resize,
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
			const parserutils_hash_entry *uri = token->idata;

			if (first == false) {
				required_size += sizeof(opv);
			} else {
				value = CURSOR_URI;
			}
			required_size += sizeof(uri);

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, &temp_ctx);
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

	opv = buildOPV(OP_CURSOR, flags, value);

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
			const parserutils_hash_entry *uri = token->idata;

			if (first == false) {
				opv = CURSOR_URI;
				memcpy(ptr, &opv, sizeof(opv));
				ptr += sizeof(opv);
			}

			memcpy(ptr, &uri, sizeof(uri));
			ptr += sizeof(uri);

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && tokenIsChar(token, '!')) {
				break;
			}

			first = false;

			token = parserutils_vector_iterate(vector, ctx);
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

css_error parse_direction(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (ltr, rtl, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[LTR]) {
		value = DIRECTION_LTR;
	} else if (ident->ilower == c->strings[RTL]) {
		value = DIRECTION_RTL;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_DIRECTION, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_display(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (inline, block, list-item, run-in, inline-block, table,
	 * inline-table, table-row-group, table-header-group, 
	 * table-footer-group, table-row, table-column-group, table-column,
	 * table-cell, table-caption, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[INLINE]) {
		value = DISPLAY_INLINE;
	} else if (ident->ilower == c->strings[BLOCK]) {
		value = DISPLAY_BLOCK;
	} else if (ident->ilower == c->strings[LIST_ITEM]) {
		value = DISPLAY_LIST_ITEM;
	} else if (ident->ilower == c->strings[RUN_IN]) {
		value = DISPLAY_RUN_IN;
	} else if (ident->ilower == c->strings[INLINE_BLOCK]) {
		value = DISPLAY_INLINE_BLOCK;
	} else if (ident->ilower == c->strings[TABLE]) {
		value = DISPLAY_TABLE;
	} else if (ident->ilower == c->strings[INLINE_TABLE]) {
		value = DISPLAY_INLINE_TABLE;
	} else if (ident->ilower == c->strings[TABLE_ROW_GROUP]) {
		value = DISPLAY_TABLE_ROW_GROUP;
	} else if (ident->ilower == c->strings[TABLE_HEADER_GROUP]) {
		value = DISPLAY_TABLE_HEADER_GROUP;
	} else if (ident->ilower == c->strings[TABLE_FOOTER_GROUP]) {
		value = DISPLAY_TABLE_FOOTER_GROUP;
	} else if (ident->ilower == c->strings[TABLE_ROW]) {
		value = DISPLAY_TABLE_ROW;
	} else if (ident->ilower == c->strings[TABLE_COLUMN_GROUP]) {
		value = DISPLAY_TABLE_COLUMN_GROUP;
	} else if (ident->ilower == c->strings[TABLE_COLUMN]) {
		value = DISPLAY_TABLE_COLUMN;
	} else if (ident->ilower == c->strings[TABLE_CELL]) {
		value = DISPLAY_TABLE_CELL;
	} else if (ident->ilower == c->strings[TABLE_CAPTION]) {
		value = DISPLAY_TABLE_CAPTION;
	} else if (ident->ilower == c->strings[NONE]) {
		value = DISPLAY_NONE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_DISPLAY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_elevation(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* angle | IDENT(below, level, above, higher, lower, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

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
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_ANGLE) == false)
			return CSS_INVALID;

		value = ELEVATION_ANGLE;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_ELEVATION, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == ELEVATION_ANGLE)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_empty_cells(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (show, hide, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[SHOW]) {
		value = EMPTY_CELLS_SHOW;
	} else if (ident->ilower == c->strings[HIDE]) {
		value = EMPTY_CELLS_HIDE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_EMPTY_CELLS, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_float(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (left, right, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[LEFT]) {
		value = FLOAT_LEFT;
	} else if (ident->ilower == c->strings[RIGHT]) {
		value = FLOAT_RIGHT;
	} else if (ident->ilower == c->strings[NONE]) {
		value = FLOAT_NONE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_FLOAT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

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

	/* [ IDENT+ | STRING ] [ ','? [ IDENT+ | STRING ] ]* | IDENT(inherit)
	 * 
	 * In the case of IDENT+, any whitespace between tokens is collapsed to
	 * a single space
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
						sizeof(parserutils_hash_entry *);

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
					sizeof(parserutils_hash_entry *);
			} else {
				return CSS_INVALID;
			}

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
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

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_FONT_FAMILY, flags, value);

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
				const parserutils_hash_entry *name = 
						token->idata;
			
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
					uint16_t len = token->idata->len;
					const css_token *temp_token = token;
					parserutils_error perror;

					temp_ctx = *ctx;

					opv = FONT_FAMILY_IDENT_LIST;

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
							len += temp_token->idata->len;
						} else if (temp_token != NULL) {
							len += 1;
						}

						temp_token = parserutils_vector_iterate(
								vector, &temp_ctx);
					}

					uint8_t buf[len];
					uint8_t *p = buf;

					memcpy(p, token->idata->data, token->idata->len);
					p += token->idata->len;

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
								token->idata->data,
								token->idata->len);
							p += token->idata->len;
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
					if (p - buf != name->len || 
						memcmp(buf, name->data, 
							name->len) != 0) {
						perror = parserutils_hash_insert(
							c->sheet->dictionary, 
							buf, len, &name);
						if (perror != PARSERUTILS_OK) {
							css_stylesheet_style_destroy(c->sheet, *result);
							*result = NULL;
							return css_error_from_parserutils_error(perror);
						}
					}
				}

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}

				if (opv == FONT_FAMILY_IDENT_LIST) {
					memcpy(ptr, &name, sizeof(name));
					ptr += sizeof(name);
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				opv = FONT_FAMILY_STRING;

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK) {
		css_stylesheet_style_destroy(c->sheet, *result);
		*result = NULL;
		return error;
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
	fixed length = 0;
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
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = FONT_SIZE_DIMENSION;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_FONT_SIZE, flags, value);

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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

	opv = buildOPV(OP_FONT_STYLE, flags, value);

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = FONT_VARIANT_NORMAL;
	} else if (ident->ilower == c->strings[SMALL_CAPS]) {
		value = FONT_VARIANT_SMALL_CAPS;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_FONT_VARIANT, flags, value);

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len, 
				(uint8_t *) token->ilower };
		fixed num = number_from_css_string(&tmp, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
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

	opv = buildOPV(OP_FONT_WEIGHT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = HEIGHT_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_left(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = LEFT_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = LEFT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_LEFT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LEFT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == LEFT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_letter_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = LETTER_SPACING_NORMAL;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ ||
				unit & UNIT_PCT)
			return CSS_INVALID;

		value = LETTER_SPACING_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_LETTER_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LETTER_SPACING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_line_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* number | length | percentage | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = LINE_HEIGHT_NORMAL;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len, 
				(uint8_t *) token->ilower };
		length = number_from_css_string(&tmp, false, &consumed);
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = LINE_HEIGHT_NUMBER;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = LINE_HEIGHT_DIMENSION;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_LINE_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LINE_HEIGHT_NUMBER)
		required_size += sizeof(length);
	else if ((flags & FLAG_INHERIT) == false && 
			value == LINE_HEIGHT_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && (value == LINE_HEIGHT_NUMBER || 
			value == LINE_HEIGHT_DIMENSION))
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
	if ((flags & FLAG_INHERIT) == false && value == LINE_HEIGHT_DIMENSION)
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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

	opv = buildOPV(OP_LIST_STYLE_IMAGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == LIST_STYLE_IMAGE_URI)
		required_size += sizeof(parserutils_hash_entry *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == LIST_STYLE_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->idata, 
				sizeof(parserutils_hash_entry *));
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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[INSIDE]) {
		value = LIST_STYLE_POSITION_INSIDE;
	} else if (ident->ilower == c->strings[OUTSIDE]) {
		value = LIST_STYLE_POSITION_OUTSIDE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_LIST_STYLE_POSITION, flags, value);

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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else {
		error = parse_list_style_type_value(c, ident, &value);
		if (error != CSS_OK)
			return error;
	}

	opv = buildOPV(OP_LIST_STYLE_TYPE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_margin_bottom(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_margin_left(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_margin_right(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_margin_top(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_margin_side(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_max_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(none, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NONE]) {
		parserutils_vector_iterate(vector, ctx);
		value = MAX_HEIGHT_NONE;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MAX_HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_MAX_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MAX_HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MAX_HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_max_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(none, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NONE]) {
		parserutils_vector_iterate(vector, ctx);
		value = MAX_WIDTH_NONE;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MAX_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_MAX_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MAX_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags && FLAG_INHERIT) == false && value == MAX_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_min_height(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MIN_HEIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_MIN_HEIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MIN_HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MIN_HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_min_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MIN_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_MIN_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MIN_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MIN_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_orphans(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* <integer> | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = ORPHANS_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_ORPHANS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == ORPHANS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == ORPHANS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_outline_color(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (invert, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INVERT]) {
		parserutils_vector_iterate(vector, ctx);
		value = OUTLINE_COLOR_INVERT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = OUTLINE_COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_OUTLINE_COLOR, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == OUTLINE_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == OUTLINE_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_outline_style(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	uint32_t opv;
	uint8_t flags;
	uint16_t value;

	/* Fake as border-left-style */
	error = parse_border_side_style(c, vector, ctx, SIDE_LEFT, result);
	if (error != CSS_OK)
		return error;

	/* Then change the opcode to outline-style, and clear the side bits */
	opv = *((uint32_t *) (*result)->bytecode);

	flags = getFlags(opv);
	value = getValue(opv) & ~SIDE_LEFT;

	opv = buildOPV(OP_OUTLINE_STYLE, flags, value);

	*((uint32_t *) (*result)->bytecode) = opv;

	return CSS_OK;
}

css_error parse_outline_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	uint32_t opv;
	uint8_t flags;
	uint16_t value;

	/* Fake as border-left-width */
	error = parse_border_side_width(c, vector, ctx, SIDE_LEFT, result);
	if (error != CSS_OK)
		return error;

	/* Then change the opcode to outline-width, and clear the side bits */
	opv = *((uint32_t *) (*result)->bytecode);

	flags = getFlags(opv);
	value = getValue(opv) & ~SIDE_LEFT;

	opv = buildOPV(OP_OUTLINE_WIDTH, flags, value);

	*((uint32_t *) (*result)->bytecode) = opv;

	return CSS_OK;
}

css_error parse_overflow(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (visible, hidden, scroll, auto, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[VISIBLE]) {
		value = OVERFLOW_VISIBLE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = OVERFLOW_HIDDEN;
	} else if (ident->ilower == c->strings[SCROLL]) {
		value = OVERFLOW_SCROLL;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = OVERFLOW_AUTO;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_OVERFLOW, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_padding_bottom(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_padding_side(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_padding_left(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_padding_side(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_padding_right(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_padding_side(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_padding_top(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_padding_side(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_page_break_after(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (auto, always, avoid, left, right, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = PAGE_BREAK_AFTER_AUTO;
	} else if (ident->ilower == c->strings[ALWAYS]) {
		value = PAGE_BREAK_AFTER_ALWAYS;
	} else if (ident->ilower == c->strings[AVOID]) {
		value = PAGE_BREAK_AFTER_AVOID;
	} else if (ident->ilower == c->strings[LEFT]) {
		value = PAGE_BREAK_AFTER_LEFT;
	} else if (ident->ilower == c->strings[RIGHT]) {
		value = PAGE_BREAK_AFTER_RIGHT;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_PAGE_BREAK_AFTER, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_page_break_before(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (auto, always, avoid, left, right, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = PAGE_BREAK_BEFORE_AUTO;
	} else if (ident->ilower == c->strings[ALWAYS]) {
		value = PAGE_BREAK_BEFORE_ALWAYS;
	} else if (ident->ilower == c->strings[AVOID]) {
		value = PAGE_BREAK_BEFORE_AVOID;
	} else if (ident->ilower == c->strings[LEFT]) {
		value = PAGE_BREAK_BEFORE_LEFT;
	} else if (ident->ilower == c->strings[RIGHT]) {
		value = PAGE_BREAK_BEFORE_RIGHT;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_PAGE_BREAK_BEFORE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_page_break_inside(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (auto, avoid, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = PAGE_BREAK_INSIDE_AUTO;
	} else if (ident->ilower == c->strings[AVOID]) {
		value = PAGE_BREAK_INSIDE_AVOID;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_PAGE_BREAK_INSIDE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_pause_after(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* time | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_TIME) == false && (unit & UNIT_PCT) == false)
			return CSS_INVALID;

		value = PAUSE_AFTER_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_PAUSE_AFTER, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_AFTER_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_pause_before(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* time | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_TIME) == false && (unit & UNIT_PCT) == false)
			return CSS_INVALID;

		value = PAUSE_BEFORE_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_PAUSE_BEFORE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PAUSE_BEFORE_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_pitch_range(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = PITCH_RANGE_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_PITCH_RANGE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PITCH_RANGE_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PITCH_RANGE_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_pitch(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* frequency | IDENT(x-low, low, medium, high, x-high, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_LOW]) {
		value = PITCH_X_LOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[LOW]) {
		value = PITCH_LOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		value = PITCH_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[HIGH]) {
		value = PITCH_HIGH;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[X_HIGH]) {
		value = PITCH_X_HIGH;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_FREQ) == false)
			return CSS_INVALID;

		value = PITCH_FREQUENCY;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_PITCH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PITCH_FREQUENCY)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_play_during(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t required_size;
	const parserutils_hash_entry *uri;

	/* URI [ IDENT(mix) || IDENT(repeat) ]? | IDENT(auto,none,inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_URI))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT) {
		if (token->ilower == c->strings[INHERIT]) {
			flags |= FLAG_INHERIT;
		} else if (token->ilower == c->strings[NONE]) {
			value = PLAY_DURING_NONE;
		} else if (token->ilower == c->strings[AUTO]) {
			value = PLAY_DURING_AUTO;
		} else
			return CSS_INVALID;
	} else {
		int flags;

		value = PLAY_DURING_URI;
		uri = token->idata;

		for (flags = 0; flags < 2; flags++) {
			token = parserutils_vector_peek(vector, *ctx);
			if (token != NULL && token->type == CSS_TOKEN_IDENT) {
				if (token->ilower == c->strings[MIX]) {
					if ((value & PLAY_DURING_MIX) == 0)
						value |= PLAY_DURING_MIX;
					else
						return CSS_INVALID;
				} else if (token->ilower == 
						c->strings[REPEAT]) {
					if ((value & PLAY_DURING_REPEAT) == 0)
						value |= PLAY_DURING_REPEAT;
					else
						return CSS_INVALID;
				} else
					return CSS_INVALID;

				parserutils_vector_iterate(vector, ctx);
			}
		}
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_PLAY_DURING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PLAY_DURING_URI)
		required_size += sizeof(parserutils_hash_entry *);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PLAY_DURING_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&uri, sizeof(parserutils_hash_entry *));
	}

	return CSS_OK;
}

css_error parse_position(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (static, relative, absolute, fixed, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[STATIC]) {
		value = POSITION_STATIC;
	} else if (ident->ilower == c->strings[RELATIVE]) {
		value = POSITION_RELATIVE;
	} else if (ident->ilower == c->strings[ABSOLUTE]) {
		value = POSITION_ABSOLUTE;
	} else if (ident->ilower == c->strings[FIXED]) {
		value = POSITION_FIXED;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_POSITION, flags, value);

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
			const parserutils_hash_entry *open = token->idata;
			const parserutils_hash_entry *close;

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
		}

		consumeWhitespace(vector, &temp_ctx);

		token = parserutils_vector_peek(vector, temp_ctx);
		if (token != NULL && tokenIsChar(token, '!') == false)
			return CSS_INVALID;

		/* Terminator */
		required_size += sizeof(opv);
	}

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_QUOTES, flags, value);

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
			const parserutils_hash_entry *open = token->idata;
			const parserutils_hash_entry *close;

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

			memcpy(ptr, &open, sizeof(open));
			ptr += sizeof(open);
			memcpy(ptr, &close, sizeof(close));
			ptr += sizeof(close);

			first = false;
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

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	return CSS_OK;
}

css_error parse_richness(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = RICHNESS_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_RICHNESS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == RICHNESS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == RICHNESS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_right(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = RIGHT_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = RIGHT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_RIGHT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == RIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == RIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_speak_header(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (once, always, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[ONCE]) {
		value = SPEAK_HEADER_ONCE;
	} else if (ident->ilower == c->strings[ALWAYS]) {
		value = SPEAK_HEADER_ALWAYS;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_SPEAK_HEADER, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_speak_numeral(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (digits, continuous, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[DIGITS]) {
		value = SPEAK_NUMERAL_DIGITS;
	} else if (ident->ilower == c->strings[CONTINUOUS]) {
		value = SPEAK_NUMERAL_CONTINUOUS;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_SPEAK_NUMERAL, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_speak_punctuation(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (code, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[CODE]) {
		value = SPEAK_PUNCTUATION_CODE;
	} else if (ident->ilower == c->strings[NONE]) {
		value = SPEAK_PUNCTUATION_NONE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_SPEAK_PUNCTUATION, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_speak(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, none, spell-out, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = SPEAK_NORMAL;
	} else if (ident->ilower == c->strings[NONE]) {
		value = SPEAK_NONE;
	} else if (ident->ilower == c->strings[SPELL_OUT]) {
		value = SPEAK_SPELL_OUT;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_SPEAK, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_speech_rate(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (x-slow, slow, medium, fast, x-fast, faster, slower, 
	 *                 inherit)
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = SPEECH_RATE_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_SPEECH_RATE, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == SPEECH_RATE_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == SPEECH_RATE_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_stress(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* number | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, false, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = STRESS_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_STRESS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == STRESS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == STRESS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_table_layout(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (auto, fixed, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[AUTO]) {
		value = TABLE_LAYOUT_AUTO;
	} else if (ident->ilower == c->strings[FIXED]) {
		value = TABLE_LAYOUT_FIXED;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_TABLE_LAYOUT, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_text_align(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (left, right, center, justify, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_TEXT_ALIGN, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_text_decoration(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT([ underline || overline || line-through || blink ])
	 * | IDENT (none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NONE]) {
		value = TEXT_DECORATION_NONE;
	} else {
		while (ident != NULL) {
			if (ident->ilower == c->strings[UNDERLINE]) {
				if ((value & TEXT_DECORATION_UNDERLINE) == 0)
					value |= TEXT_DECORATION_UNDERLINE;
				else
					return CSS_INVALID;
			} else if (ident->ilower == c->strings[OVERLINE]) {
				if ((value & TEXT_DECORATION_OVERLINE) == 0)
					value |= TEXT_DECORATION_OVERLINE;
				else
					return CSS_INVALID;
			} else if (ident->ilower == c->strings[LINE_THROUGH]) {
				if ((value & TEXT_DECORATION_LINE_THROUGH) == 0)
					value |= TEXT_DECORATION_LINE_THROUGH;
				else
					return CSS_INVALID;
			} else if (ident->ilower == c->strings[BLINK]) {
				if ((value & TEXT_DECORATION_BLINK) == 0)
					value |= TEXT_DECORATION_BLINK;
				else
					return CSS_INVALID;
			} else
				return CSS_INVALID;

			ident = parserutils_vector_peek(vector, *ctx);
			if (ident != NULL && ident->type != CSS_TOKEN_IDENT)
				break;
		}
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_TEXT_DECORATION, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_text_indent(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = TEXT_INDENT_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_TEXT_INDENT, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == TEXT_INDENT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_text_transform(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (capitalize, uppercase, lowercase, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_TEXT_TRANSFORM, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_top(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = TOP_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = TOP_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_TOP, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == TOP_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == TOP_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_unicode_bidi(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, embed, bidi-override, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NORMAL]) {
		value = UNICODE_BIDI_NORMAL;
	} else if (ident->ilower == c->strings[EMBED]) {
		value = UNICODE_BIDI_EMBED;
	} else if (ident->ilower == c->strings[BIDI_OVERRIDE]) {
		value = UNICODE_BIDI_BIDI_OVERRIDE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_UNICODE_BIDI, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_vertical_align(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(baseline, sub, super, top, text-top,
	 *                             middle, bottom, text-bottom, inherit)
	 */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[BASELINE]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_BASELINE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SUB]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_SUB;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[SUPER]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_SUPER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TOP]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_TOP;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TEXT_TOP]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_TEXT_TOP;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MIDDLE]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_MIDDLE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[BOTTOM]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_BOTTOM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TEXT_BOTTOM]) {
		parserutils_vector_iterate(vector, ctx);
		value = VERTICAL_ALIGN_TEXT_BOTTOM;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = VERTICAL_ALIGN_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_VERTICAL_ALIGN, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == VERTICAL_ALIGN_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == VERTICAL_ALIGN_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_visibility(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (visible, hidden, collapse, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[VISIBLE]) {
		value = VISIBILITY_VISIBLE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = VISIBILITY_HIDDEN;
	} else if (ident->ilower == c->strings[COLLAPSE]) {
		value = VISIBILITY_COLLAPSE;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_VISIBILITY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_voice_family(css_language *c, 
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

	/* [ IDENT+ | STRING ] [ ','? [ IDENT+ | STRING ] ]* | IDENT(inherit)
	 * 
	 * In the case of IDENT+, any whitespace between tokens is collapsed to
	 * a single space
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
						sizeof(parserutils_hash_entry *);

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
								token->ilower == c->strings[CHILD]))
							return CSS_INVALID;
						token = parserutils_vector_iterate(
							vector, &temp_ctx);
					}
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				if (first == false) {
					required_size += sizeof(opv);
				} else {
					value = VOICE_FAMILY_STRING;
				}

				required_size += 
					sizeof(parserutils_hash_entry *);
			} else {
				return CSS_INVALID;
			}

			consumeWhitespace(vector, &temp_ctx);

			token = parserutils_vector_peek(vector, temp_ctx);
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

	error = parse_important(c, vector, &temp_ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_VOICE_FAMILY, flags, value);

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
				const parserutils_hash_entry *name = 
						token->idata;
			
				if (token->ilower == c->strings[MALE]) {
					opv = VOICE_FAMILY_MALE;
				} else if (token->ilower == 
						c->strings[FEMALE]) {
					opv = VOICE_FAMILY_FEMALE;
				} else if (token->ilower == c->strings[CHILD]) {
					opv = VOICE_FAMILY_CHILD;
				} else {
					uint16_t len = token->idata->len;
					const css_token *temp_token = token;
					parserutils_error perror;

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
							len += temp_token->idata->len;
						} else if (temp_token != NULL) {
							len += 1;
						}

						temp_token = parserutils_vector_iterate(
								vector, &temp_ctx);
					}

					uint8_t buf[len];
					uint8_t *p = buf;

					memcpy(p, token->idata->data, token->idata->len);
					p += token->idata->len;

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
								token->idata->data,
								token->idata->len);
							p += token->idata->len;
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
					if (p - buf != name->len || 
						memcmp(buf, name->data, 
							name->len) != 0) {
						perror = parserutils_hash_insert(
							c->sheet->dictionary, 
							buf, len, &name);
						if (perror != PARSERUTILS_OK) {
							css_stylesheet_style_destroy(c->sheet, *result);
							*result = NULL;
							return css_error_from_parserutils_error(perror);
						}
					}
				}

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}

				if (opv == VOICE_FAMILY_IDENT_LIST) {
					memcpy(ptr, &name, sizeof(name));
					ptr += sizeof(name);
				}
			} else if (token->type == CSS_TOKEN_STRING) {
				opv = VOICE_FAMILY_STRING;

				if (first == false) {
					memcpy(ptr, &opv, sizeof(opv));
					ptr += sizeof(opv);
				}

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
		opv = VOICE_FAMILY_END;
		memcpy(ptr, &opv, sizeof(opv));
		ptr += sizeof(opv);
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK) {
		css_stylesheet_style_destroy(c->sheet, *result);
		*result = NULL;
		return error;
	}

	return CSS_OK;
}

css_error parse_volume(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* number | percentage | IDENT(silent, x-soft, soft, medium, loud, 
	 *                             x-loud, inherit)
	 */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

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
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		length = number_from_css_string(&tmp, false, &consumed);
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = VOLUME_NUMBER;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if ((unit & UNIT_PCT) == false)
			return CSS_INVALID;

		value = VOLUME_DIMENSION;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_VOLUME, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == VOLUME_NUMBER)
		required_size += sizeof(length);
	else if ((flags & FLAG_INHERIT) == false && value == VOLUME_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_white_space(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (normal, pre, nowrap, pre-wrap, pre-line, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

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
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_WHITE_SPACE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_widows(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* <integer> | IDENT (inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = WIDOWS_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_WIDOWS, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == WIDOWS_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == WIDOWS_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

css_error parse_width(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = WIDTH_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_WIDTH, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_word_spacing(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | IDENT(normal, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = WORD_SPACING_NORMAL;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ ||
				unit & UNIT_PCT)
			return CSS_INVALID;

		value = WORD_SPACING_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_WORD_SPACING, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == WORD_SPACING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

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

css_error parse_z_index(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed num = 0;
	uint32_t required_size;

	/* <integer> | IDENT (auto, inherit) */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
			token->type != CSS_TOKEN_NUMBER))
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		value = Z_INDEX_AUTO;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		css_string tmp = { token->ilower->len,
				(uint8_t *) token->ilower };
		num = number_from_css_string(&tmp, true, &consumed);
		/* Invalid if there are trailing characters */
		if (consumed != token->ilower->len)
			return CSS_INVALID;

		value = Z_INDEX_SET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_Z_INDEX, flags, value);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == Z_INDEX_SET)
		required_size += sizeof(num);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == Z_INDEX_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv), 
				&num, sizeof(num));
	}

	return CSS_OK;
}

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
			token->type != CSS_TOKEN_FUNCTION))
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT) {
		/** \todo Parse colour names */
	} else if (token->type == CSS_TOKEN_HASH) {
		if (token->idata->len == 3) {
			r = charToHex(token->idata->data[0]);
			g = charToHex(token->idata->data[1]);
			b = charToHex(token->idata->data[2]);

			r |= (r << 4);
			g |= (g << 4);
			b |= (b << 4);
		} else if (token->idata->len == 6) {
			r = (charToHex(token->idata->data[0]) << 4);
			r |= charToHex(token->idata->data[1]);
			g = (charToHex(token->idata->data[2]) << 4);
			g |= charToHex(token->idata->data[3]);
			b = (charToHex(token->idata->data[4]) << 4);
			b |= charToHex(token->idata->data[5]);
		} else
			return CSS_INVALID;
	} else if (token->type == CSS_TOKEN_FUNCTION) {
		if (token->ilower == c->strings[RGB]) {
			css_token_type valid = CSS_TOKEN_NUMBER;

			for (int i = 0; i < 3; i++) {
				css_string tmp;
				fixed num;
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

				tmp.len = token->idata->len;
				tmp.data = (uint8_t *) token->idata->data;
				num = number_from_css_string(&tmp, 
						valid == CSS_TOKEN_NUMBER, 
						&consumed);
				if (consumed != token->idata->len)
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

css_error parse_unit_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		fixed *length, uint32_t *unit)
{
	const css_token *token;
	fixed num;
	size_t consumed = 0;
	css_string tmp;

	UNUSED(c);

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_DIMENSION &&
			token->type != CSS_TOKEN_PERCENTAGE))
		return CSS_INVALID;

	tmp.len = token->idata->len;
	tmp.data = (uint8_t *) token->idata->data;
	num = number_from_css_string(&tmp, false, &consumed);

	if (token->type == CSS_TOKEN_DIMENSION) {
		if (consumed == token->idata->len) {
			/** \todo In quirks mode, non-zero units should be 
			 * treated as px too */
			if (num != 0)
				return CSS_INVALID;
			*unit = UNIT_PX;
		} else if (token->idata->len - consumed == 4) {
			if (strncasecmp((char *) token->idata->data + consumed, 
					"grad", 4) == 0)
				*unit = UNIT_GRAD;
			else
				return CSS_INVALID;
		} else if (token->idata->len - consumed == 3) {
			if (strncasecmp((char *) token->idata->data + consumed,
					"kHz", 3) == 0)
				*unit = UNIT_KHZ;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "deg", 3) == 0)
				*unit = UNIT_DEG;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "rad", 3) == 0)
				*unit = UNIT_RAD;
			else
				return CSS_INVALID;
		} else if (token->idata->len - consumed == 2) {
			if (strncasecmp((char *) token->idata->data + consumed,
					"Hz", 2) == 0)
				*unit = UNIT_HZ;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "ms", 2) == 0)
				*unit = UNIT_MS;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "px", 2) == 0)
				*unit = UNIT_PX;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "ex", 2) == 0)
				*unit = UNIT_EX;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "em", 2) == 0)
				*unit = UNIT_EM;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "in", 2) == 0)
				*unit = UNIT_IN;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "cm", 2) == 0)
				*unit = UNIT_CM;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "mm", 2) == 0)
				*unit = UNIT_MM;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "pt", 2) == 0)
				*unit = UNIT_PT;
			else if (strncasecmp((char *) token->idata->data + 
					consumed, "pc", 2) == 0)
				*unit = UNIT_PC;
			else
				return CSS_INVALID;
		} else if (token->idata->len - consumed == 1) {
			if (strncasecmp((char *) token->idata->data + consumed,
					"s", 1) == 0)
				*unit = UNIT_S;
			else
				return CSS_INVALID;
		} else
			return CSS_INVALID;
	} else {
		if (consumed != token->idata->len)
			return CSS_INVALID;
		*unit = UNIT_PCT;
	}

	*length = num;

	return CSS_OK;
}

css_error parse_border_side_color(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	uint32_t colour = 0;
	uint32_t required_size;

	/* colour | IDENT (transparent, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[TRANSPARENT]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_COLOR_TRANSPARENT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = BORDER_COLOR_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BORDER_TRBL_COLOR, flags, value | side);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BORDER_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_border_side_style(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result)
{
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;

	/* IDENT (none, hidden, dotted, dashed, solid, double, groove, 
	 * ridge, inset, outset, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	if (ident->ilower == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->ilower == c->strings[NONE]) {
		value = BORDER_STYLE_NONE;
	} else if (ident->ilower == c->strings[HIDDEN]) {
		value = BORDER_STYLE_HIDDEN;
	} else if (ident->ilower == c->strings[DOTTED]) {
		value = BORDER_STYLE_DOTTED;
	} else if (ident->ilower == c->strings[DASHED]) {
		value = BORDER_STYLE_DASHED;
	} else if (ident->ilower == c->strings[SOLID]) {
		value = BORDER_STYLE_SOLID;
	} else if (ident->ilower == c->strings[DOUBLE]) {
		value = BORDER_STYLE_DOUBLE;
	} else if (ident->ilower == c->strings[GROOVE]) {
		value = BORDER_STYLE_GROOVE;
	} else if (ident->ilower == c->strings[RIDGE]) {
		value = BORDER_STYLE_RIDGE;
	} else if (ident->ilower == c->strings[INSET]) {
		value = BORDER_STYLE_INSET;
	} else if (ident->ilower == c->strings[OUTSET]) {
		value = BORDER_STYLE_OUTSET;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BORDER_TRBL_STYLE, flags, value | side);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_side_width(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | IDENT(thin, medium, thick, inherit) */
	token= parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT && 
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[THIN]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_THIN;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[THICK]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_THICK;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit == UNIT_PCT || unit & UNIT_ANGLE ||
				unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = BORDER_WIDTH_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BORDER_TRBL_WIDTH, flags, value | side);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == BORDER_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BORDER_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_margin_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(auto, inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[AUTO]) {
		parserutils_vector_iterate(vector, ctx);
		value = MARGIN_AUTO;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = MARGIN_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_MARGIN_TRBL, flags, value | side);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == MARGIN_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == MARGIN_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_padding_side(css_language *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result)
{
	css_error error;
	const css_token *token;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	fixed length = 0;
	uint32_t unit = 0;
	uint32_t required_size;

	/* length | percentage | IDENT(inherit) */
	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT &&
			token->ilower == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else {
		error = parse_unit_specifier(c, vector, ctx, &length, &unit);
		if (error != CSS_OK)
			return error;

		if (unit & UNIT_ANGLE || unit & UNIT_TIME || unit & UNIT_FREQ)
			return CSS_INVALID;

		value = PADDING_SET;
	}

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_PADDING_TRBL, flags, value | side);

	required_size = sizeof(opv);
	if ((flags & FLAG_INHERIT) == false && value == PADDING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == PADDING_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
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
		uint8_t *buffer, uint32_t *buflen)
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
	 *	FUNCTION(counter) IDENT IDENT? ')' |
	 *	FUNCTION(counters) IDENT STRING IDENT? ')'
	 * ]+
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
		return CSS_INVALID;

	while (token != NULL) {
		if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[OPEN_QUOTE]) {
			if (first == false) {
				opv = CONTENT_OPEN_QUOTE;

				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[CLOSE_QUOTE]) {
			if (first == false) {
				opv = CONTENT_CLOSE_QUOTE;
				
				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[NO_OPEN_QUOTE]) {
			if (first == false) {
				opv = CONTENT_NO_OPEN_QUOTE;

				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_IDENT &&
				token->ilower == c->strings[NO_CLOSE_QUOTE]) {
			if (first == false) {
				opv = CONTENT_NO_CLOSE_QUOTE;

				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}
		} else if (token->type == CSS_TOKEN_STRING) {
			if (first == false) {
				opv = CONTENT_STRING;

				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
				memcpy(buffer + offset, &token->idata,
						sizeof(token->idata));
			}

			offset += sizeof(token->idata);
		} else if (token->type == CSS_TOKEN_URI) {
			if (first == false) {
				opv = CONTENT_URI;

				if (buffer != NULL) {
					memcpy(buffer + offset, 
							&opv, sizeof(opv));
				}

				offset += sizeof(opv);
			}

			if (buffer != NULL) {
				memcpy(buffer + offset, &token->idata,
						sizeof(token->idata));
			}

			offset += sizeof(token->idata);
		} else if (token->type == CSS_TOKEN_FUNCTION &&
				token->ilower == c->strings[ATTR]) {
			if (first == false) {
				opv = CONTENT_ATTR;

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
			const parserutils_hash_entry *name;

			opv = CONTENT_COUNTER;

			consumeWhitespace(vector, ctx);

			/* Expect IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

			name = token->idata;

			consumeWhitespace(vector, ctx);

			/* Possible IDENT */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT && 
					tokenIsChar(token, ')') == false))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_IDENT) {
				uint16_t v;

				error = parse_list_style_type_value(c,
						token, &v);
				if (error != CSS_OK)
					return error;

				opv |= v << CONTENT_COUNTER_STYLE_SHIFT;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

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
				memcpy(buffer + offset, &name, sizeof(name));
			}

			offset += sizeof(name);
		} else if (token->type == CSS_TOKEN_FUNCTION &&
				token->ilower == c->strings[COUNTERS]) {
			const parserutils_hash_entry *name;
			const parserutils_hash_entry *sep;

			opv = CONTENT_COUNTERS;

			consumeWhitespace(vector, ctx);

			/* Expect IDENT */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_IDENT)
				return CSS_INVALID;

			name = token->idata;

			consumeWhitespace(vector, ctx);

			/* Expect STRING */
			token = parserutils_vector_iterate(vector, ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			sep = token->idata;

			consumeWhitespace(vector, ctx);

			/* Possible IDENT */
			token = parserutils_vector_peek(vector, *ctx);
			if (token == NULL || (token->type != CSS_TOKEN_IDENT && 
					tokenIsChar(token, ')') == false))
				return CSS_INVALID;

			if (token->type == CSS_TOKEN_IDENT) {
				uint16_t v;

				error = parse_list_style_type_value(c,
						token, &v);
				if (error != CSS_OK)
					return error;

				opv |= v << CONTENT_COUNTERS_STYLE_SHIFT;

				parserutils_vector_iterate(vector, ctx);

				consumeWhitespace(vector, ctx);
			}

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
				memcpy(buffer + offset, &name, sizeof(name));
			}

			offset += sizeof(name);

			if (buffer != NULL) {
				memcpy(buffer + offset, &sep, sizeof(sep));
			}

			offset += sizeof(sep);
		} else {
			return CSS_INVALID;
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

#endif
