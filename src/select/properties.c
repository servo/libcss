/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

static css_error cascade_azimuth(uint32_t opv, css_style *style,
		 css_select_state *state)
{
	uint16_t value = 0;;
	css_fixed val = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		value = getValue(opv);

		if (value == AZIMUTH_ANGLE) {
			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo set computed azimuth */
	}

	return CSS_OK;
}

static css_error initial_azimuth(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_background_attachment(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_ATTACHMENT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_ATTACHMENT_FIXED:
			value = CSS_BACKGROUND_ATTACHMENT_FIXED;
			break;
		case BACKGROUND_ATTACHMENT_SCROLL:
			value = CSS_BACKGROUND_ATTACHMENT_SCROLL;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_background_attachment(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_background_attachment(css_computed_style *style)
{
	return set_background_attachment(style, 
			CSS_BACKGROUND_ATTACHMENT_SCROLL);
}

static css_error cascade_background_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_COLOR_INHERIT;
	css_color color = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_COLOR_TRANSPARENT:
			value = CSS_BACKGROUND_COLOR_TRANSPARENT;
			break;
		case BACKGROUND_COLOR_SET:
			value = CSS_BACKGROUND_COLOR_COLOR;
			color = *((css_color *) style->bytecode);
			advance_bytecode(style, sizeof(color));
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_background_color(state->result, value, color);
	}

	return CSS_OK;
}

static css_error initial_background_color(css_computed_style *style)
{
	return set_background_color(style, CSS_BACKGROUND_COLOR_TRANSPARENT, 0);
}

static css_error cascade_background_image(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_IMAGE_INHERIT;
	parserutils_hash_entry *uri = NULL;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_IMAGE_NONE:
			value = CSS_BACKGROUND_IMAGE_NONE;
			break;
		case BACKGROUND_IMAGE_URI:
			value = CSS_BACKGROUND_IMAGE_IMAGE;
			uri = *((parserutils_hash_entry **) style->bytecode);
			advance_bytecode(style, sizeof(uri));
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo fix this mess -- it's seriously unsafe */
		return set_background_image(state->result, value, 
				(css_string *) uri);
	}

	return CSS_OK;
}

static css_error initial_background_image(css_computed_style *style)
{
	return set_background_image(style, CSS_BACKGROUND_IMAGE_NONE, NULL);
}

static css_error cascade_background_position(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_POSITION_INHERIT;
	css_fixed hlength = 0;
	css_fixed vlength = 0;
	uint32_t hunit = CSS_UNIT_PX;
	uint32_t vunit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		value = CSS_BACKGROUND_POSITION_SET;

		switch (getValue(opv) & 0xf0) {
		case BACKGROUND_POSITION_HORZ_SET:
			hlength = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(hlength));
			hunit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(hunit));
			break;
		case BACKGROUND_POSITION_HORZ_CENTER:
			hlength = INTTOFIX(50);
			hunit = CSS_UNIT_PCT;
			break;
		case BACKGROUND_POSITION_HORZ_RIGHT:
			hlength = INTTOFIX(100);
			hunit = CSS_UNIT_PCT;
			break;
		case BACKGROUND_POSITION_HORZ_LEFT:
			hlength = INTTOFIX(0);
			hunit = CSS_UNIT_PCT;
			break;
		}

		switch (getValue(opv) & 0x0f) {
		case BACKGROUND_POSITION_VERT_SET:
			vlength = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(vlength));
			vunit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(vunit));
			break;
		case BACKGROUND_POSITION_VERT_CENTER:
			vlength = INTTOFIX(50);
			vunit = CSS_UNIT_PCT;
			break;
		case BACKGROUND_POSITION_VERT_BOTTOM:
			vlength = INTTOFIX(100);
			vunit = CSS_UNIT_PCT;
			break;
		case BACKGROUND_POSITION_VERT_TOP:
			vlength = INTTOFIX(0);
			vunit = CSS_UNIT_PCT;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_background_position(state->result, value,
				hlength, hunit, vlength, vunit);
	}

	return CSS_OK;
}

static css_error initial_background_position(css_computed_style *style)
{
	return set_background_position(style, CSS_BACKGROUND_POSITION_SET, 
			0, CSS_UNIT_PCT, 0, CSS_UNIT_PCT);
}

static css_error cascade_background_repeat(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BACKGROUND_REPEAT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_REPEAT_NO_REPEAT:
			value = CSS_BACKGROUND_REPEAT_NO_REPEAT;
			break;
		case BACKGROUND_REPEAT_REPEAT_X:
			value = CSS_BACKGROUND_REPEAT_REPEAT_X;
			break;
		case BACKGROUND_REPEAT_REPEAT_Y:
			value = CSS_BACKGROUND_REPEAT_REPEAT_Y;
			break;
		case BACKGROUND_REPEAT_REPEAT:
			value = CSS_BACKGROUND_REPEAT_REPEAT;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_background_repeat(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_background_repeat(css_computed_style *style)
{
	return set_background_repeat(style, CSS_BACKGROUND_REPEAT_REPEAT);
}

static css_error cascade_border_collapse( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_collapse(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_spacing( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_spacing(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_top_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_top_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_right_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_right_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_bottom_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_bottom_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_left_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_left_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_top_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_top_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_right_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_right_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_bottom_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_bottom_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_left_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_left_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_top_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_top_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_right_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_right_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_bottom_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_bottom_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_border_left_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_border_left_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_bottom( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_bottom(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_caption_side( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_caption_side(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_clear( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_clear(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_clip( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_clip(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_content( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_content(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_counter_increment( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_counter_increment(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_counter_reset( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_counter_reset(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_cue_after( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_cue_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_cue_before( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_cue_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_cursor( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_cursor(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_direction( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_direction(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_display( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_display(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_elevation( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_elevation(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_empty_cells( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_empty_cells(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_float( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_float(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_family( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_font_family(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_size( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_font_size(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_font_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_variant( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_font_variant(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_weight( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_font_weight(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_height( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_height(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_left( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_left(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_letter_spacing( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_letter_spacing(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_line_height( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_line_height(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_list_style_image( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_list_style_image(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_list_style_position( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_list_style_position(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_list_style_type( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_list_style_type(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_margin_top( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_margin_top(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_margin_right( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_margin_right(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_margin_bottom( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_margin_bottom(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_margin_left( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_margin_left(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_max_height( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_max_height(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_max_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_max_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_min_height( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_min_height(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_min_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_min_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_orphans( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_orphans(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_outline_color( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_outline_color(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_outline_style( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_outline_style(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_outline_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_outline_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_overflow( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_overflow(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_padding_top( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_padding_top(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_padding_right( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_padding_right(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_padding_bottom( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_padding_bottom(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_padding_left( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_padding_left(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_page_break_after( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_page_break_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_page_break_before( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_page_break_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_page_break_inside( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_page_break_inside(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pause_after( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_pause_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pause_before( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_pause_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pitch_range( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_pitch_range(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pitch( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_pitch(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_play_during( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_play_during(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_position( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_position(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_quotes( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_quotes(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_richness( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_richness(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_right( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_right(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_speak_header( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_speak_header(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_speak_numeral( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_speak_numeral(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_speak_punctuation( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_speak_punctuation(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_speak( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_speak(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_speech_rate( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_speech_rate(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_stress( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_stress(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_table_layout( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_table_layout(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_text_align( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_text_align(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_text_decoration( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_text_decoration(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_text_indent( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_text_indent(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_text_transform( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_text_transform(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_top( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_top(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_unicode_bidi( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_unicode_bidi(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_vertical_align( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_vertical_align(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_visibility( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_visibility(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_voice_family( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_voice_family(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_volume( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_volume(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_white_space( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_white_space(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_widows( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_widows(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_width( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_width(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_word_spacing( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_word_spacing(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_z_index( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	return CSS_OK;
}

static css_error initial_z_index(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

