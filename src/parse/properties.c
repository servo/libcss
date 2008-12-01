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
		/** \todo the rest. behind on its own isn't defined afaics */
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
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == BACKGROUND_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.data, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_background_position(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/** \todo background-position */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo border-spacing */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo clip */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo content */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_counter_increment(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/** \todo counter-increment */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_counter_reset(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/** \todo counter-reset */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_AFTER_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.data, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
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
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == CUE_BEFORE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.data, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_cursor(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/** \todo cursor */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo font-family */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
		fixed num = number_from_css_string(&tmp, &consumed);
		int32_t intpart = FIXTOINT(num);
		/* Invalid if there are trailing characters or it was a float */
		if (consumed != token->ilower->len || num != intpart)
			return CSS_INVALID;
		switch (intpart) {
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
		length = number_from_css_string(&tmp, &consumed);
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
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if ((flags & FLAG_INHERIT) == false && value == LIST_STYLE_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.data, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
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
	} else if (ident->ilower == c->strings[DISC]) {
		value = LIST_STYLE_TYPE_DISC;
	} else if (ident->ilower == c->strings[CIRCLE]) {
		value = LIST_STYLE_TYPE_CIRCLE;
	} else if (ident->ilower == c->strings[SQUARE]) {
		value = LIST_STYLE_TYPE_SQUARE;
	} else if (ident->ilower == c->strings[DECIMAL]) {
		value = LIST_STYLE_TYPE_DECIMAL;
	} else if (ident->ilower == c->strings[DECIMAL_LEADING_ZERO]) {
		value = LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO;
	} else if (ident->ilower == c->strings[LOWER_ROMAN]) {
		value = LIST_STYLE_TYPE_LOWER_ROMAN;
	} else if (ident->ilower == c->strings[UPPER_ROMAN]) {
		value = LIST_STYLE_TYPE_UPPER_ROMAN;
	} else if (ident->ilower == c->strings[LOWER_GREEK]) {
		value = LIST_STYLE_TYPE_LOWER_GREEK;
	} else if (ident->ilower == c->strings[LOWER_LATIN]) {
		value = LIST_STYLE_TYPE_LOWER_LATIN;
	} else if (ident->ilower == c->strings[UPPER_LATIN]) {
		value = LIST_STYLE_TYPE_UPPER_LATIN;
	} else if (ident->ilower == c->strings[ARMENIAN]) {
		value = LIST_STYLE_TYPE_ARMENIAN;
	} else if (ident->ilower == c->strings[GEORGIAN]) {
		value = LIST_STYLE_TYPE_GEORGIAN;
	} else if (ident->ilower == c->strings[LOWER_ALPHA]) {
		value = LIST_STYLE_TYPE_LOWER_ALPHA;
	} else if (ident->ilower == c->strings[UPPER_ALPHA]) {
		value = LIST_STYLE_TYPE_UPPER_ALPHA;
	} else if (ident->ilower == c->strings[NONE]) {
		value = LIST_STYLE_TYPE_NONE;
	} else
		return CSS_INVALID;

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
		num = number_from_css_string(&tmp, &consumed);
		int32_t intpart = FIXTOINT(num);
		/* Invalid if there are trailing characters or it was a float */
		if (consumed != token->ilower->len || num != intpart)
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
		num = number_from_css_string(&tmp, &consumed);
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
	/** \todo play-during */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo quotes */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
		num = number_from_css_string(&tmp, &consumed);
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
		num = number_from_css_string(&tmp, &consumed);
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
		num = number_from_css_string(&tmp, &consumed);
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
	/** \todo text-decoration */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	/** \todo voice-family */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
		length = number_from_css_string(&tmp, &consumed);
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
		num = number_from_css_string(&tmp, &consumed);
		int32_t intpart = FIXTOINT(num);
		/* Invalid if there are trailing characters or it was a float */
		if (consumed != token->ilower->len || num != intpart)
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
		num = number_from_css_string(&tmp, &consumed);
		int32_t intpart = FIXTOINT(num);
		/* Invalid if there are trailing characters or it was a float */
		if (consumed != token->ilower->len || num != intpart)
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

	UNUSED(c);
	UNUSED(result);

	/** \todo Parse colours */

	/* For now, consume everything up to the end of the declaration or !, 
 	 * whichever comes first */
	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL &&
			tokenIsChar(token, '!') == false)
		parserutils_vector_iterate(vector, ctx);

	return CSS_OK;
}

css_error parse_unit_specifier(css_language *c,
		const parserutils_vector *vector, int *ctx,
		fixed *length, uint32_t *unit)
{
	const css_token *token;

	UNUSED(c);
	UNUSED(length);
	UNUSED(unit);

	/** \todo Parse units */

	/* For now, consume everything up to the end of the declaration or !, 
 	 * whichever comes first */
	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL &&
			tokenIsChar(token, '!') == false)
		parserutils_vector_iterate(vector, ctx);

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


#endif
