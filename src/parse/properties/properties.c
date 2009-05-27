/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

/**
 * Dispatch table of property handlers, indexed by property enum
 */
const css_prop_handler property_handlers[LAST_PROP + 1 - FIRST_PROP] =
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

	opv = buildOPV(CSS_PROP_COLOR, flags, value);

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

	opv = buildOPV(CSS_PROP_DISPLAY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK)
		return error;

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}

