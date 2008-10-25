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
	*result = css_stylesheet_style_create(c->sheet, sizeof(opv));
	if (*result == NULL)
		return CSS_NOMEM;

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
			return CSS_INVALID;

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
	*result = css_stylesheet_style_create(c->sheet, required_size);
	if (*result == NULL)
		return CSS_NOMEM;

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
	*result = css_stylesheet_style_create(c->sheet, required_size);
	if (*result == NULL)
		return CSS_NOMEM;

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
	*result = css_stylesheet_style_create(c->sheet, sizeof(opv));
	if (*result == NULL)
		return CSS_NOMEM;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_border_bottom_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_bottom_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_bottom_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_collapse(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_left_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_left_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_left_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_right_color(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_right_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_right_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_spacing(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_top_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_border_top_width(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_bottom(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_caption_side(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	*result = css_stylesheet_style_create(c->sheet, sizeof(opv));
	if (*result == NULL)
		return CSS_NOMEM;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

css_error parse_clip(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_content(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_cue_before(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_cursor(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_display(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_elevation(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_empty_cells(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_float(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_font_family(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_font_style(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_font_variant(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_font_weight(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_height(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_left(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_letter_spacing(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_line_height(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

css_error parse_list_style_image(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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
	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

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

#endif
