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

static css_error parse_azimuth(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_attachment(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_image(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_position(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_background_repeat(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_bottom_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_collapse(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_left_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_right_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_spacing(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_border_top_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_bottom(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_caption_side(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_clear(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_clip(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_content(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_counter_increment(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_counter_reset(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cue_after(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cue_before(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_cursor(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_direction(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_display(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_elevation(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_empty_cells(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_float(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_family(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_size(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_variant(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_font_weight(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_height(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_left(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_letter_spacing(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_line_height(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_image(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_position(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_list_style_type(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_bottom(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_left(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_right(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_margin_top(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_max_height(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_max_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_min_height(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_min_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_orphans(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_outline_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_overflow(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_bottom(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_left(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_right(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_padding_top(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_after(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_before(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_page_break_inside(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pause_after(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pause_before(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pitch_range(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_pitch(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_play_during(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_position(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_quotes(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_richness(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_right(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_header(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_numeral(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak_punctuation(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speak(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_speech_rate(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_stress(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_table_layout(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_align(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_decoration(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_indent(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_text_transform(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_top(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_unicode_bidi(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_vertical_align(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_visibility(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_voice_family(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_volume(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_white_space(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_widows(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_word_spacing(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);
static css_error parse_z_index(css_css21 *c,
		const parserutils_vector *vector, int *ctx, 
		css_style **result);

static inline css_error parse_important(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		uint8_t *result);
static inline css_error parse_colour_specifier(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		uint32_t *result);
static inline css_error parse_unit_specifier(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		fixed *length, uint32_t *unit);

static inline css_error parse_border_side_color(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_border_side_style(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);
static inline css_error parse_border_side_width(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		uint16_t side, css_style **result);

/**
 * Type of property handler function
 */
typedef css_error (*css_prop_handler)(css_css21 *c, 
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

css_error parse_azimuth(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	/** \todo azimuth */
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_background_attachment(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[FIXED]) {
		value = BACKGROUND_ATTACHMENT_FIXED;
	} else if (ident->lower.ptr == c->strings[SCROLL]) {
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

css_error parse_background_color(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[TRANSPARENT]) {
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
	if (value == BACKGROUND_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == BACKGROUND_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_background_image(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->lower.ptr == c->strings[NONE]) {
		value = BACKGROUND_IMAGE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = BACKGROUND_IMAGE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_BACKGROUND_IMAGE, flags, value);

	required_size = sizeof(opv);
	if (value == BACKGROUND_IMAGE_URI)
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == BACKGROUND_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.ptr, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_background_position(css_css21 *c, 
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

css_error parse_background_repeat(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[NO_REPEAT]) {
		value = BACKGROUND_REPEAT_NO_REPEAT;
	} else if (ident->lower.ptr == c->strings[REPEAT_X]) {
		value = BACKGROUND_REPEAT_REPEAT_X;
	} else if (ident->lower.ptr == c->strings[REPEAT_Y]) {
		value = BACKGROUND_REPEAT_REPEAT_Y;
	} else if (ident->lower.ptr == c->strings[REPEAT]) {
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

css_error parse_border_bottom_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_bottom_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_bottom_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_BOTTOM, result);
}

css_error parse_border_collapse(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[COLLAPSE]) {
		value = BORDER_COLLAPSE_COLLAPSE;
	} else if (ident->lower.ptr == c->strings[SEPARATE]) {
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

css_error parse_border_left_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_left_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_left_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_LEFT, result);
}

css_error parse_border_right_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_right_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_right_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_RIGHT, result);
}

css_error parse_border_spacing(css_css21 *c, 
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

css_error parse_border_top_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_color(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_border_top_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_style(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_border_top_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	return parse_border_side_width(c, vector, ctx, SIDE_TOP, result);
}

css_error parse_bottom(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[AUTO]) {
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
	if (value == BOTTOM_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == BOTTOM_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_caption_side(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[TOP]) {
		value = CAPTION_SIDE_TOP;
	} else if (ident->lower.ptr == c->strings[BOTTOM]) {
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

css_error parse_clear(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[RIGHT]) {
		value = CLEAR_RIGHT;
	} else if (ident->lower.ptr == c->strings[LEFT]) {
		value = CLEAR_LEFT;
	} else if (ident->lower.ptr == c->strings[BOTH]) {
		value = CLEAR_BOTH;
	} else if (ident->lower.ptr == c->strings[NONE]) {
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

css_error parse_clip(css_css21 *c, 
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

css_error parse_color(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
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
	if (value == COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_content(css_css21 *c, 
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

css_error parse_counter_increment(css_css21 *c, 
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

css_error parse_counter_reset(css_css21 *c, 
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

css_error parse_cue_after(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->lower.ptr == c->strings[NONE]) {
		value = CUE_AFTER_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_AFTER_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CUE_AFTER, flags, value);

	required_size = sizeof(opv);
	if (value == CUE_AFTER_URI)
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == CUE_AFTER_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.ptr, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_cue_before(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->lower.ptr == c->strings[NONE]) {
		value = CUE_BEFORE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = CUE_BEFORE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_CUE_BEFORE, flags, value);

	required_size = sizeof(opv);
	if (value == CUE_BEFORE_URI)
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == CUE_BEFORE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.ptr, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_cursor(css_css21 *c, 
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

css_error parse_direction(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[LTR]) {
		value = DIRECTION_LTR;
	} else if (ident->lower.ptr == c->strings[RTL]) {
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

css_error parse_display(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[INLINE]) {
		value = DISPLAY_INLINE;
	} else if (ident->lower.ptr == c->strings[BLOCK]) {
		value = DISPLAY_BLOCK;
	} else if (ident->lower.ptr == c->strings[LIST_ITEM]) {
		value = DISPLAY_LIST_ITEM;
	} else if (ident->lower.ptr == c->strings[RUN_IN]) {
		value = DISPLAY_RUN_IN;
	} else if (ident->lower.ptr == c->strings[INLINE_BLOCK]) {
		value = DISPLAY_INLINE_BLOCK;
	} else if (ident->lower.ptr == c->strings[TABLE]) {
		value = DISPLAY_TABLE;
	} else if (ident->lower.ptr == c->strings[INLINE_TABLE]) {
		value = DISPLAY_INLINE_TABLE;
	} else if (ident->lower.ptr == c->strings[TABLE_ROW_GROUP]) {
		value = DISPLAY_TABLE_ROW_GROUP;
	} else if (ident->lower.ptr == c->strings[TABLE_HEADER_GROUP]) {
		value = DISPLAY_TABLE_HEADER_GROUP;
	} else if (ident->lower.ptr == c->strings[TABLE_FOOTER_GROUP]) {
		value = DISPLAY_TABLE_FOOTER_GROUP;
	} else if (ident->lower.ptr == c->strings[TABLE_ROW]) {
		value = DISPLAY_TABLE_ROW;
	} else if (ident->lower.ptr == c->strings[TABLE_COLUMN_GROUP]) {
		value = DISPLAY_TABLE_COLUMN_GROUP;
	} else if (ident->lower.ptr == c->strings[TABLE_COLUMN]) {
		value = DISPLAY_TABLE_COLUMN;
	} else if (ident->lower.ptr == c->strings[TABLE_CELL]) {
		value = DISPLAY_TABLE_CELL;
	} else if (ident->lower.ptr == c->strings[TABLE_CAPTION]) {
		value = DISPLAY_TABLE_CAPTION;
	} else if (ident->lower.ptr == c->strings[NONE]) {
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

css_error parse_elevation(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[BELOW]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_BELOW;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[LEVEL]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_LEVEL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[ABOVE]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_ABOVE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[HIGHER]) {
		parserutils_vector_iterate(vector, ctx);
		value = ELEVATION_HIGHER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[LOWER]) {
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
	if (value == ELEVATION_ANGLE)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == ELEVATION_ANGLE) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_empty_cells(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[SHOW]) {
		value = EMPTY_CELLS_SHOW;
	} else if (ident->lower.ptr == c->strings[HIDE]) {
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

css_error parse_float(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[LEFT]) {
		value = FLOAT_LEFT;
	} else if (ident->lower.ptr == c->strings[RIGHT]) {
		value = FLOAT_RIGHT;
	} else if (ident->lower.ptr == c->strings[NONE]) {
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

css_error parse_font_family(css_css21 *c, 
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

css_error parse_font_size(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[XX_SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_XX_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[X_SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_X_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[SMALL]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_SMALL;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[X_LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_X_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[XX_LARGE]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_XX_LARGE;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[LARGER]) {
		parserutils_vector_iterate(vector, ctx);
		value = FONT_SIZE_LARGER;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[SMALLER]) {
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
	if (value == FONT_SIZE_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == FONT_SIZE_DIMENSION) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_font_style(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[NORMAL]) {
		value = FONT_STYLE_NORMAL;
	} else if (ident->lower.ptr == c->strings[ITALIC]) {
		value = FONT_STYLE_ITALIC;
	} else if (ident->lower.ptr == c->strings[OBLIQUE]) {
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

css_error parse_font_variant(css_css21 *c, 
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[NORMAL]) {
		value = FONT_VARIANT_NORMAL;
	} else if (ident->lower.ptr == c->strings[SMALL_CAPS]) {
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

css_error parse_font_weight(css_css21 *c, 
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

	if (token->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		fixed num = number_from_css_string(&token->lower, &consumed);
		int32_t intpart = FIXTOINT(num);
		/* Invalid if there are trailing characters or it was a float */
		if (consumed != token->lower.len || num != intpart)
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
	} else if (token->lower.ptr == c->strings[NORMAL]) {
		value = FONT_WEIGHT_NORMAL;
	} else if (token->lower.ptr == c->strings[BOLD]) {
		value = FONT_WEIGHT_BOLD;
	} else if (token->lower.ptr == c->strings[BOLDER]) {
		value = FONT_WEIGHT_BOLDER;
	} else if (token->lower.ptr == c->strings[LIGHTER]) {
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

css_error parse_height(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[AUTO]) {
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
	if (value == HEIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == HEIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_left(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[AUTO]) {
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
	if (value == LEFT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == LEFT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_letter_spacing(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[NORMAL]) {
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
	if (value == LETTER_SPACING_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == LETTER_SPACING_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_line_height(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[NORMAL]) {
		parserutils_vector_iterate(vector, ctx);
		value = LINE_HEIGHT_NORMAL;
	} else if (token->type == CSS_TOKEN_NUMBER) {
		size_t consumed = 0;
		length = number_from_css_string(&token->lower, &consumed);
		if (consumed != token->lower.len)
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
	if (value == LINE_HEIGHT_NUMBER)
		required_size += sizeof(length);
	else if (value == LINE_HEIGHT_DIMENSION)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == LINE_HEIGHT_NUMBER || value == LINE_HEIGHT_DIMENSION)
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
	if (value == LINE_HEIGHT_DIMENSION)
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));

	return CSS_OK;
}

css_error parse_list_style_image(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT && 
			token->lower.ptr == c->strings[NONE]) {
		value = LIST_STYLE_IMAGE_NONE;
	} else if (token->type == CSS_TOKEN_URI) {
		value = LIST_STYLE_IMAGE_URI;
	} else
		return CSS_INVALID;

	opv = buildOPV(OP_LIST_STYLE_IMAGE, flags, value);

	required_size = sizeof(opv);
	if (value == LIST_STYLE_IMAGE_URI)
		required_size += sizeof(uint8_t *) + sizeof(size_t);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == LIST_STYLE_IMAGE_URI) {
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv),
				&token->data.ptr, sizeof(uint8_t *));
		memcpy((uint8_t *) (*result)->bytecode + sizeof(opv) + 
					sizeof(uint8_t *),
				&token->data.len, sizeof(size_t));
	}

	return CSS_OK;
}

css_error parse_list_style_position(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_list_style_type(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_margin_bottom(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_margin_left(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_margin_right(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_margin_top(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_max_height(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_max_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_min_height(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_min_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_orphans(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_outline_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_outline_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_outline_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_overflow(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_padding_bottom(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_padding_left(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_padding_right(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_padding_top(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_page_break_after(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_page_break_before(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_page_break_inside(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_pause_after(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_pause_before(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_pitch_range(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_pitch(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_play_during(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_position(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_quotes(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_richness(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_right(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[AUTO]) {
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
	if (value == RIGHT_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == RIGHT_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_speak_header(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_speak_numeral(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_speak_punctuation(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_speak(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_speech_rate(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_stress(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_table_layout(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_text_align(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_text_decoration(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_text_indent(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_text_transform(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_top(css_css21 *c, 
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags = FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[AUTO]) {
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
	if (value == TOP_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == TOP_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;
}

css_error parse_unicode_bidi(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_vertical_align(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_visibility(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_voice_family(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_volume(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_white_space(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_widows(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_word_spacing(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_z_index(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_important(css_css21 *c,
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

		if (token->lower.ptr == c->strings[IMPORTANT])
			*result |= FLAG_IMPORTANT;
	} else if (token != NULL)
		return CSS_INVALID;

	return CSS_OK;
}

css_error parse_colour_specifier(css_css21 *c,
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

css_error parse_unit_specifier(css_css21 *c,
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

css_error parse_border_side_color(css_css21 *c,
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[TRANSPARENT]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_COLOR_TRANSPARENT;
	} else {
		error = parse_colour_specifier(c, vector, ctx, &colour);
		if (error != CSS_OK)
			return error;

		value = BORDER_COLOR_SET;
	}

	value |= side;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BORDER_TRBL_COLOR, flags, value);

	required_size = sizeof(opv);
	if (value == BORDER_COLOR_SET)
		required_size += sizeof(colour);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == BORDER_COLOR_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&colour, sizeof(colour));
	}

	return CSS_OK;
}

css_error parse_border_side_style(css_css21 *c,
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

	if (ident->lower.ptr == c->strings[INHERIT]) {
		flags |= FLAG_INHERIT;
	} else if (ident->lower.ptr == c->strings[NONE]) {
		value = BORDER_STYLE_NONE;
	} else if (ident->lower.ptr == c->strings[HIDDEN]) {
		value = BORDER_STYLE_HIDDEN;
	} else if (ident->lower.ptr == c->strings[DOTTED]) {
		value = BORDER_STYLE_DOTTED;
	} else if (ident->lower.ptr == c->strings[DASHED]) {
		value = BORDER_STYLE_DASHED;
	} else if (ident->lower.ptr == c->strings[SOLID]) {
		value = BORDER_STYLE_SOLID;
	} else if (ident->lower.ptr == c->strings[DOUBLE]) {
		value = BORDER_STYLE_DOUBLE;
	} else if (ident->lower.ptr == c->strings[GROOVE]) {
		value = BORDER_STYLE_GROOVE;
	} else if (ident->lower.ptr == c->strings[RIDGE]) {
		value = BORDER_STYLE_RIDGE;
	} else if (ident->lower.ptr == c->strings[INSET]) {
		value = BORDER_STYLE_INSET;
	} else if (ident->lower.ptr == c->strings[OUTSET]) {
		value = BORDER_STYLE_OUTSET;
	} else
		return CSS_INVALID;

	value |= side;

	opv = buildOPV(OP_BORDER_TRBL_STYLE, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_side_width(css_css21 *c,
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
			token->lower.ptr == c->strings[INHERIT]) {
		parserutils_vector_iterate(vector, ctx);
		flags |= FLAG_INHERIT;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[THIN]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_THIN;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[MEDIUM]) {
		parserutils_vector_iterate(vector, ctx);
		value = BORDER_WIDTH_MEDIUM;
	} else if (token->type == CSS_TOKEN_IDENT &&
			token->lower.ptr == c->strings[THICK]) {
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

	value |= side;

	error = parse_important(c, vector, ctx, &flags);
	if (error != CSS_OK)
		return error;

	opv = buildOPV(OP_BORDER_TRBL_WIDTH, flags, value);

	required_size = sizeof(opv);
	if (value == BORDER_WIDTH_SET)
		required_size += sizeof(length) + sizeof(unit);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, required_size, result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));
	if (value == BORDER_WIDTH_SET) {
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv),
				&length, sizeof(length));
		memcpy(((uint8_t *) (*result)->bytecode) + sizeof(opv) +
				sizeof(length), &unit, sizeof(unit));
	}

	return CSS_OK;

}

#endif
