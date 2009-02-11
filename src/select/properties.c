/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

static css_error cascade_bg_border_color(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_color));
static css_error cascade_uri_none(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, 
				const css_string *));
static css_error cascade_border_style(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t));
static css_error cascade_border_width(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed, 
				css_unit));
static css_error cascade_length_auto(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit));
static css_error cascade_length_normal(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit));
static css_error cascade_length_none(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit));
static css_error cascade_length(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit));
static css_error cascade_number(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed));

static css_error cascade_azimuth(uint32_t opv, css_style *style,
		 css_select_state *state)
{
	uint16_t value = 0;
	css_fixed val = 0;
	uint32_t unit = CSS_UNIT_DEG;

	if (isInherit(opv) == false) {
		switch (getValue(opv) & ~AZIMUTH_BEHIND) {
		case AZIMUTH_ANGLE:
			value = 0;

			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case AZIMUTH_LEFTWARDS:
		case AZIMUTH_RIGHTWARDS:
		case AZIMUTH_LEFT_SIDE:
		case AZIMUTH_FAR_LEFT:
		case AZIMUTH_LEFT:
		case AZIMUTH_CENTER_LEFT:
		case AZIMUTH_CENTER:
		case AZIMUTH_CENTER_RIGHT:
		case AZIMUTH_RIGHT:
		case AZIMUTH_FAR_RIGHT:
		case AZIMUTH_RIGHT_SIDE:
			/** \todo azimuth values */
			break;
		}

		/** \todo azimuth behind */
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
	return cascade_bg_border_color(opv, style, state, set_background_color);
}

static css_error initial_background_color(css_computed_style *style)
{
	return set_background_color(style, CSS_BACKGROUND_COLOR_TRANSPARENT, 0);
}

static css_error cascade_background_image(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_uri_none(opv, style, state, set_background_image);
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

static css_error cascade_border_collapse(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BORDER_COLLAPSE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BORDER_COLLAPSE_SEPARATE:
			value = CSS_BORDER_COLLAPSE_SEPARATE;
			break;
		case BORDER_COLLAPSE_COLLAPSE:
			value = CSS_BORDER_COLLAPSE_COLLAPSE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_border_collapse(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_border_collapse(css_computed_style *style)
{
	return set_border_collapse(style, CSS_BORDER_COLLAPSE_SEPARATE);
}

static css_error cascade_border_spacing(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_BORDER_SPACING_INHERIT;
	css_fixed hlength = 0;
	css_fixed vlength = 0;
	uint32_t hunit = CSS_UNIT_PX;
	uint32_t vunit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		value = CSS_BORDER_SPACING_SET;
		hlength = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(hlength));
		hunit = *((uint32_t *) style->bytecode);
		advance_bytecode(style, sizeof(hunit));

		vlength = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(vlength));
		vunit = *((uint32_t *) style->bytecode);
		advance_bytecode(style, sizeof(vunit));
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_border_spacing(state->result, value,
				hlength, hunit, vlength, vunit);
	}

	return CSS_OK;
}

static css_error initial_border_spacing(css_computed_style *style)
{
	return set_border_spacing(style, CSS_BORDER_SPACING_SET,
			0, CSS_UNIT_PX, 0, CSS_UNIT_PX);
}

static css_error cascade_border_top_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, set_border_top_color);
}

static css_error initial_border_top_color(css_computed_style *style)
{
	return set_border_top_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

static css_error cascade_border_right_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, 
			set_border_right_color);
}

static css_error initial_border_right_color(css_computed_style *style)
{
	return set_border_right_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

static css_error cascade_border_bottom_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state,
			set_border_bottom_color);
}

static css_error initial_border_bottom_color(css_computed_style *style)
{
	return set_border_bottom_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

static css_error cascade_border_left_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, 
			set_border_left_color);
}

static css_error initial_border_left_color(css_computed_style *style)
{
	return set_border_left_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

static css_error cascade_border_top_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_top_style);
}

static css_error initial_border_top_style(css_computed_style *style)
{
	return set_border_top_style(style, CSS_BORDER_STYLE_NONE);
}

static css_error cascade_border_right_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_right_style);
}

static css_error initial_border_right_style(css_computed_style *style)
{
	return set_border_right_style(style, CSS_BORDER_STYLE_NONE);
}

static css_error cascade_border_bottom_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_bottom_style);
}

static css_error initial_border_bottom_style(css_computed_style *style)
{
	return set_border_bottom_style(style, CSS_BORDER_STYLE_NONE);
}

static css_error cascade_border_left_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_left_style);
}

static css_error initial_border_left_style(css_computed_style *style)
{
	return set_border_left_style(style, CSS_BORDER_STYLE_NONE);
}

static css_error cascade_border_top_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_top_width);
}

static css_error initial_border_top_width(css_computed_style *style)
{
	return set_border_top_width(style, CSS_BORDER_WIDTH_MEDIUM, 
			0, CSS_UNIT_PX);
}

static css_error cascade_border_right_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_right_width);
}

static css_error initial_border_right_width(css_computed_style *style)
{
	return set_border_right_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

static css_error cascade_border_bottom_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_bottom_width);
}

static css_error initial_border_bottom_width(css_computed_style *style)
{
	return set_border_bottom_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

static css_error cascade_border_left_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_left_width);
}

static css_error initial_border_left_width(css_computed_style *style)
{
	return set_border_left_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

static css_error cascade_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_bottom);
}

static css_error initial_bottom(css_computed_style *style)
{
	return set_bottom(style, CSS_BOTTOM_AUTO, 0, CSS_UNIT_PX);
}

static css_error cascade_caption_side(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_CAPTION_SIDE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case CAPTION_SIDE_TOP:
			value = CSS_CAPTION_SIDE_TOP;
			break;
		case CAPTION_SIDE_BOTTOM:
			value = CSS_CAPTION_SIDE_BOTTOM;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_caption_side(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_caption_side(css_computed_style *style)
{
	return set_caption_side(style, CSS_CAPTION_SIDE_TOP);
}

static css_error cascade_clear(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_CLEAR_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case CLEAR_NONE:
			value = CSS_CLEAR_NONE;
			break;
		case CLEAR_LEFT:
			value = CSS_CLEAR_LEFT;
			break;
		case CLEAR_RIGHT:
			value = CSS_CLEAR_RIGHT;
			break;
		case CLEAR_BOTH:
			value = CSS_CLEAR_BOTH;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_clear(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_clear(css_computed_style *style)
{
	return set_clear(style, CSS_CLEAR_NONE);
}

static css_error cascade_clip(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_CLIP_INHERIT;
	css_computed_clip_rect rect = { 0, 0, 0, 0, 
			CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX };

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv) & CLIP_SHAPE_MASK) {
		case CLIP_SHAPE_RECT:
			/** \todo clip rect can't store auto values */
			break;
		case CLIP_AUTO:
			value = CSS_CLIP_AUTO;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_clip(state->result, value, &rect);
	}

	return CSS_OK;
}

static css_error initial_clip(css_computed_style *style)
{
	css_computed_clip_rect rect = { 0, 0, 0, 0, 
			CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX };

	return set_clip(style, CSS_CLIP_AUTO, &rect);
}

static css_error cascade_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_COLOR_INHERIT;
	css_color color = 0;

	if (isInherit(opv) == false) {
		value = CSS_COLOR_COLOR;
		color = *((css_color *) style->bytecode);
		advance_bytecode(style, sizeof(color));
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_color(state->result, value, color);
	}

	return CSS_OK;
}

static css_error initial_color(css_computed_style *style)
{
	return set_color(style, CSS_COLOR_COLOR, 0);
}

static css_error cascade_content(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	/** \todo content */

	return CSS_OK;
}

static css_error initial_content(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_counter_increment(uint32_t opv, css_style *style, 
		css_select_state *state)
{	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	/** \todo counter-increment/reset */

	return CSS_OK;
}

static css_error initial_counter_increment(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_counter_reset(uint32_t opv, css_style *style, 
		css_select_state *state)
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

static css_error cascade_cue_after(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo cue-after */
	return cascade_uri_none(opv, style, state, NULL);
}

static css_error initial_cue_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_cue_before(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo cue-before */
	return cascade_uri_none(opv, style, state, NULL);
}

static css_error initial_cue_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_cursor(uint32_t opv, css_style *style, 
		css_select_state *state)
{	
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	/** \todo cursor */

	return CSS_OK;
}

static css_error initial_cursor(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_direction(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_DIRECTION_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case DIRECTION_LTR:
			value = CSS_DIRECTION_LTR;
			break;
		case DIRECTION_RTL:
			value = CSS_DIRECTION_RTL;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_direction(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_direction(css_computed_style *style)
{
	return set_direction(style, CSS_DIRECTION_LTR);
}

static css_error cascade_display(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_DISPLAY_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case DISPLAY_INLINE:
			value = CSS_DISPLAY_INLINE;
			break;
		case DISPLAY_BLOCK:
			value = CSS_DISPLAY_BLOCK;
			break;
		case DISPLAY_LIST_ITEM:
			value = CSS_DISPLAY_LIST_ITEM;
			break;
		case DISPLAY_RUN_IN:
			value = CSS_DISPLAY_RUN_IN;
			break;
		case DISPLAY_INLINE_BLOCK:
			value = CSS_DISPLAY_INLINE_BLOCK;
			break;
		case DISPLAY_TABLE:
			value = CSS_DISPLAY_TABLE;
			break;
		case DISPLAY_INLINE_TABLE:
			value = CSS_DISPLAY_INLINE_TABLE;
			break;
		case DISPLAY_TABLE_ROW_GROUP:
			value = CSS_DISPLAY_TABLE_ROW_GROUP;
			break;
		case DISPLAY_TABLE_HEADER_GROUP:
			value = CSS_DISPLAY_TABLE_HEADER_GROUP;
			break;
		case DISPLAY_TABLE_FOOTER_GROUP:
			value = CSS_DISPLAY_TABLE_FOOTER_GROUP;
			break;
		case DISPLAY_TABLE_ROW:
			value = CSS_DISPLAY_TABLE_ROW;
			break;
		case DISPLAY_TABLE_COLUMN_GROUP:
			value = CSS_DISPLAY_TABLE_COLUMN_GROUP;
			break;
		case DISPLAY_TABLE_COLUMN:
			value = CSS_DISPLAY_TABLE_COLUMN;
			break;
		case DISPLAY_TABLE_CELL:
			value = CSS_DISPLAY_TABLE_CELL;
			break;
		case DISPLAY_TABLE_CAPTION:
			value = CSS_DISPLAY_TABLE_CAPTION;
			break;
		case DISPLAY_NONE:
			value = DISPLAY_NONE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_display(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_display(css_computed_style *style)
{
	return set_display(style, CSS_DISPLAY_INLINE);
}

static css_error cascade_elevation(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	css_fixed val = 0;
	uint32_t unit = CSS_UNIT_DEG;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case ELEVATION_ANGLE: 
			value = 0;

			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));

			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case ELEVATION_BELOW:
		case ELEVATION_LEVEL:
		case ELEVATION_ABOVE:
		case ELEVATION_HIGHER:
		case ELEVATION_LOWER:
			/** \todo convert to public values */
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo set computed elevation */
	}

	return CSS_OK;
}

static css_error initial_elevation(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_empty_cells(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_EMPTY_CELLS_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case EMPTY_CELLS_SHOW:
			value = CSS_EMPTY_CELLS_SHOW;
			break;
		case EMPTY_CELLS_HIDE:
			value = CSS_EMPTY_CELLS_HIDE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_empty_cells(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_empty_cells(css_computed_style *style)
{
	return set_empty_cells(style, CSS_EMPTY_CELLS_SHOW);
}

static css_error cascade_float(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FLOAT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case FLOAT_LEFT:
			value = CSS_FLOAT_LEFT;
			break;
		case FLOAT_RIGHT:
			value = CSS_FLOAT_RIGHT;
			break;
		case FLOAT_NONE:
			value = CSS_FLOAT_NONE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_float(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_float(css_computed_style *style)
{
	return set_float(style, CSS_FLOAT_NONE);
}

static css_error cascade_font_family(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	UNUSED(opv);
	UNUSED(style);
	UNUSED(state);

	/** \todo font-family */

	return CSS_OK;
}

static css_error initial_font_family(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_font_size(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FONT_SIZE_INHERIT;
	css_fixed size = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case FONT_SIZE_DIMENSION: 
			value = CSS_FONT_SIZE_DIMENSION;

			size = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(size));

			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case FONT_SIZE_XX_SMALL:
			value = CSS_FONT_SIZE_XX_SMALL;
			break;
		case FONT_SIZE_X_SMALL:
			value = CSS_FONT_SIZE_X_SMALL;
			break;
		case FONT_SIZE_SMALL:
			value = CSS_FONT_SIZE_SMALL;
			break;
		case FONT_SIZE_MEDIUM:
			value = CSS_FONT_SIZE_MEDIUM;
			break;
		case FONT_SIZE_LARGE:
			value = CSS_FONT_SIZE_LARGE;
			break;
		case FONT_SIZE_X_LARGE:
			value = CSS_FONT_SIZE_X_LARGE;
			break;
		case FONT_SIZE_XX_LARGE:
			value = CSS_FONT_SIZE_XX_LARGE;
			break;
		case FONT_SIZE_LARGER:
			value = CSS_FONT_SIZE_LARGER;
			break;
		case FONT_SIZE_SMALLER:
			value = CSS_FONT_SIZE_SMALLER;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_font_size(state->result, value, size, unit);
	}

	return CSS_OK;
}

static css_error initial_font_size(css_computed_style *style)
{
	return set_font_size(style, CSS_FONT_SIZE_MEDIUM, 0, CSS_UNIT_PX);
}

static css_error cascade_font_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FONT_STYLE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case FONT_STYLE_NORMAL:
			value = CSS_FONT_STYLE_NORMAL;
			break;
		case FONT_STYLE_ITALIC:
			value = CSS_FONT_STYLE_ITALIC;
			break;
		case FONT_STYLE_OBLIQUE:
			value = CSS_FONT_STYLE_OBLIQUE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_font_style(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_font_style(css_computed_style *style)
{
	return set_font_style(style, CSS_FONT_STYLE_NORMAL);
}

static css_error cascade_font_variant(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FONT_VARIANT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case FONT_VARIANT_NORMAL:
			value = CSS_FONT_VARIANT_NORMAL;
			break;
		case FONT_VARIANT_SMALL_CAPS:
			value = CSS_FONT_VARIANT_SMALL_CAPS;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_font_variant(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_font_variant(css_computed_style *style)
{
	return set_font_variant(style, CSS_FONT_VARIANT_NORMAL);
}

static css_error cascade_font_weight(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FONT_WEIGHT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case FONT_WEIGHT_NORMAL:
			value = CSS_FONT_WEIGHT_NORMAL;
			break;
		case FONT_WEIGHT_BOLD:
			value = CSS_FONT_WEIGHT_BOLD;
			break;
		case FONT_WEIGHT_BOLDER:
			value = CSS_FONT_WEIGHT_BOLDER;
			break;
		case FONT_WEIGHT_LIGHTER:
			value = CSS_FONT_WEIGHT_LIGHTER;
			break;
		case FONT_WEIGHT_100:
			value = CSS_FONT_WEIGHT_100;
			break;
		case FONT_WEIGHT_200:
			value = CSS_FONT_WEIGHT_200;
			break;
		case FONT_WEIGHT_300:
			value = CSS_FONT_WEIGHT_300;
			break;
		case FONT_WEIGHT_400:
			value = CSS_FONT_WEIGHT_400;
			break;
		case FONT_WEIGHT_500:
			value = CSS_FONT_WEIGHT_500;
			break;
		case FONT_WEIGHT_600:
			value = CSS_FONT_WEIGHT_600;
			break;
		case FONT_WEIGHT_700:
			value = CSS_FONT_WEIGHT_700;
			break;
		case FONT_WEIGHT_800:
			value = CSS_FONT_WEIGHT_800;
			break;
		case FONT_WEIGHT_900:
			value = CSS_FONT_WEIGHT_900;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_font_weight(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_font_weight(css_computed_style *style)
{
	return set_font_weight(style, CSS_FONT_WEIGHT_NORMAL);
}

static css_error cascade_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_height);
}

static css_error initial_height(css_computed_style *style)
{
	return set_height(style, CSS_HEIGHT_AUTO, 0, CSS_UNIT_PX);
}

static css_error cascade_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_left);
}

static css_error initial_left(css_computed_style *style)
{
	return set_left(style, CSS_LEFT_AUTO, 0, CSS_UNIT_PX);
}

static css_error cascade_letter_spacing(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_normal(opv, style, state, set_letter_spacing);
}

static css_error initial_letter_spacing(css_computed_style *style)
{
	return set_letter_spacing(style, CSS_LETTER_SPACING_NORMAL, 
			0, CSS_UNIT_PX);
}

static css_error cascade_line_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_LINE_HEIGHT_INHERIT;
	css_fixed val = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case LINE_HEIGHT_NUMBER:
			value = CSS_LINE_HEIGHT_NUMBER;
			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			break;
		case LINE_HEIGHT_DIMENSION:
			value = CSS_LINE_HEIGHT_DIMENSION;
			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case LINE_HEIGHT_NORMAL:
			value = CSS_LINE_HEIGHT_NORMAL;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_line_height(state->result, value, val, unit);
	}

	return CSS_OK;
}

static css_error initial_line_height(css_computed_style *style)
{
	return set_line_height(style, CSS_LINE_HEIGHT_NORMAL, 0, CSS_UNIT_PX);
}

static css_error cascade_list_style_image(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_uri_none(opv, style, state, set_list_style_image);
}

static css_error initial_list_style_image(css_computed_style *style)
{
	return set_list_style_image(style, CSS_LIST_STYLE_IMAGE_NONE, NULL);
}

static css_error cascade_list_style_position(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_LIST_STYLE_POSITION_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case LIST_STYLE_POSITION_INSIDE:
			value = CSS_LIST_STYLE_POSITION_INSIDE;
			break;
		case LIST_STYLE_POSITION_OUTSIDE:
			value = CSS_LIST_STYLE_POSITION_OUTSIDE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_list_style_position(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_list_style_position(css_computed_style *style)
{
	return set_list_style_position(style, CSS_LIST_STYLE_POSITION_OUTSIDE);
}

static css_error cascade_list_style_type(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_LIST_STYLE_TYPE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case LIST_STYLE_TYPE_DISC:
			value = CSS_LIST_STYLE_TYPE_DISC;
			break;
		case LIST_STYLE_TYPE_CIRCLE:
			value = CSS_LIST_STYLE_TYPE_CIRCLE;
			break;
		case LIST_STYLE_TYPE_SQUARE:
			value = CSS_LIST_STYLE_TYPE_SQUARE;
			break;
		case LIST_STYLE_TYPE_DECIMAL:
			value = CSS_LIST_STYLE_TYPE_DECIMAL;
			break;
		case LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO:
			value = CSS_LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO;
			break;
		case LIST_STYLE_TYPE_LOWER_ROMAN:
			value = CSS_LIST_STYLE_TYPE_LOWER_ROMAN;
			break;
		case LIST_STYLE_TYPE_UPPER_ROMAN:
			value = CSS_LIST_STYLE_TYPE_UPPER_ROMAN;
			break;
		case LIST_STYLE_TYPE_LOWER_GREEK:
			value = CSS_LIST_STYLE_TYPE_LOWER_GREEK;
			break;
		case LIST_STYLE_TYPE_LOWER_LATIN:
			value = CSS_LIST_STYLE_TYPE_LOWER_LATIN;
			break;
		case LIST_STYLE_TYPE_UPPER_LATIN:
			value = CSS_LIST_STYLE_TYPE_UPPER_LATIN;
			break;
		case LIST_STYLE_TYPE_ARMENIAN:
			value = CSS_LIST_STYLE_TYPE_ARMENIAN;
			break;
		case LIST_STYLE_TYPE_GEORGIAN:
			value = CSS_LIST_STYLE_TYPE_GEORGIAN;
			break;
		case LIST_STYLE_TYPE_LOWER_ALPHA:
			value = CSS_LIST_STYLE_TYPE_LOWER_ALPHA;
			break;
		case LIST_STYLE_TYPE_UPPER_ALPHA:
			value = CSS_LIST_STYLE_TYPE_UPPER_ALPHA;
			break;
		case LIST_STYLE_TYPE_NONE:
			value = CSS_LIST_STYLE_TYPE_NONE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_list_style_type(state->result, value);
	}

	return CSS_OK;
}

static css_error initial_list_style_type(css_computed_style *style)
{
	return set_list_style_type(style, CSS_LIST_STYLE_TYPE_DISC);
}

static css_error cascade_margin_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_top);
}

static css_error initial_margin_top(css_computed_style *style)
{
	return set_margin_top(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_margin_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_right);
}

static css_error initial_margin_right(css_computed_style *style)
{
	return set_margin_right(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_margin_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_bottom);
}

static css_error initial_margin_bottom(css_computed_style *style)
{
	return set_margin_bottom(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_margin_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_left);
}

static css_error initial_margin_left(css_computed_style *style)
{
	return set_margin_left(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_max_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_none(opv, style, state, set_max_height);
}

static css_error initial_max_height(css_computed_style *style)
{
	return set_max_height(style, CSS_MAX_HEIGHT_NONE, 0, CSS_UNIT_PX);
}

static css_error cascade_max_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_none(opv, style, state, set_max_width);;
}

static css_error initial_max_width(css_computed_style *style)
{
	return set_max_width(style, CSS_MAX_WIDTH_NONE, 0, CSS_UNIT_PX);
}

static css_error cascade_min_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_min_height);
}

static css_error initial_min_height(css_computed_style *style)
{
	return set_min_height(style, CSS_MIN_HEIGHT_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_min_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_min_width);
}

static css_error initial_min_width(css_computed_style *style)
{
	return set_min_width(style, CSS_MIN_WIDTH_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_orphans(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo orphans */
	return cascade_number(opv, style, state, NULL);
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

static css_error cascade_outline_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_outline_style);
}

static css_error initial_outline_style(css_computed_style *style)
{
	return set_outline_style(style, CSS_OUTLINE_STYLE_NONE);
}

static css_error cascade_outline_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_outline_width);
}

static css_error initial_outline_width(css_computed_style *style)
{
	return set_outline_width(style, CSS_OUTLINE_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
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

static css_error cascade_padding_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_top);
}

static css_error initial_padding_top(css_computed_style *style)
{
	return set_padding_top(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_padding_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_right);
}

static css_error initial_padding_right(css_computed_style *style)
{
	return set_padding_right(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_padding_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_bottom);
}

static css_error initial_padding_bottom(css_computed_style *style)
{
	return set_padding_bottom(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

static css_error cascade_padding_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_left);
}

static css_error initial_padding_left(css_computed_style *style)
{
	return set_padding_left(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
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

static css_error cascade_pause_after(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pause-after */
	return cascade_length(opv, style, state, NULL);
}

static css_error initial_pause_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pause_before(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pause-before */
	return cascade_length(opv, style, state, NULL);
}

static css_error initial_pause_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_pitch_range(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pitch-range */
	return cascade_number(opv, style, state, NULL);
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

static css_error cascade_richness(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo richness */
	return cascade_number(opv, style, state, NULL);
}

static css_error initial_richness(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_right);
}

static css_error initial_right(css_computed_style *style)
{
	return set_right(style, CSS_RIGHT_AUTO, 0, CSS_UNIT_PX);
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

static css_error cascade_stress(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo stress */
	return cascade_number(opv, style, state, NULL);
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

static css_error cascade_text_indent(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_text_indent);
}

static css_error initial_text_indent(css_computed_style *style)
{
	return set_text_indent(style, CSS_TEXT_INDENT_SET, 0, CSS_UNIT_PX);
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

static css_error cascade_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_top);
}

static css_error initial_top(css_computed_style *style)
{
	return set_top(style, CSS_TOP_AUTO, 0, CSS_UNIT_PX);
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

static css_error cascade_widows(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo widows */
	return cascade_number(opv, style, state, NULL);
}

static css_error initial_widows(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

static css_error cascade_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_width);
}

static css_error initial_width(css_computed_style *style)
{
	return set_width(style, CSS_WIDTH_AUTO, 0, CSS_UNIT_PX);
}

static css_error cascade_word_spacing(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_normal(opv, style, state, set_word_spacing);
}

static css_error initial_word_spacing(css_computed_style *style)
{
	return set_word_spacing(style, CSS_WORD_SPACING_NORMAL, 0, CSS_UNIT_PX);
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

/******************************************************************************
 * Utilities below here                                                       *
 ******************************************************************************/
css_error cascade_bg_border_color(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_color))
{
	uint16_t value = CSS_BACKGROUND_COLOR_INHERIT;
	css_color color = 0;

	assert(CSS_BACKGROUND_COLOR_INHERIT == CSS_BORDER_COLOR_INHERIT);
	assert(CSS_BACKGROUND_COLOR_TRANSPARENT == 
			CSS_BORDER_COLOR_TRANSPARENT);
	assert(CSS_BACKGROUND_COLOR_COLOR == CSS_BORDER_COLOR_COLOR);

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
		return fun(state->result, value, color);
	}

	return CSS_OK;
}

css_error cascade_uri_none(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, 
				const css_string *))
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

	/** \todo lose fun != NULL once all properties have set routines */
	if (fun != NULL && outranks_existing(getOpcode(opv), 
			isImportant(opv), state)) {
		/** \todo fix this mess -- it's seriously unsafe */
		return fun(state->result, value, (css_string *) uri);
	}

	return CSS_OK;
}

css_error cascade_border_style(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t))
{
	uint16_t value = CSS_BORDER_STYLE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BORDER_STYLE_NONE:
			value = CSS_BORDER_STYLE_NONE;
			break;
		case BORDER_STYLE_HIDDEN:
			value = CSS_BORDER_STYLE_HIDDEN;
			break;
		case BORDER_STYLE_DOTTED:
			value = CSS_BORDER_STYLE_DOTTED;
			break;
		case BORDER_STYLE_DASHED:
			value = CSS_BORDER_STYLE_DASHED;
			break;
		case BORDER_STYLE_SOLID:
			value = CSS_BORDER_STYLE_SOLID;
			break;
		case BORDER_STYLE_DOUBLE:
			value = CSS_BORDER_STYLE_DOUBLE;
			break;
		case BORDER_STYLE_GROOVE:
			value = CSS_BORDER_STYLE_GROOVE;
			break;
		case BORDER_STYLE_RIDGE:
			value = CSS_BORDER_STYLE_RIDGE;
			break;
		case BORDER_STYLE_INSET:
			value = CSS_BORDER_STYLE_INSET;
			break;
		case BORDER_STYLE_OUTSET:
			value = CSS_BORDER_STYLE_OUTSET;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value);
	}

	return CSS_OK;
}

css_error cascade_border_width(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed, 
				css_unit))
{
	uint16_t value = CSS_BORDER_WIDTH_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BORDER_WIDTH_SET:
			value = CSS_BORDER_WIDTH_WIDTH;
			length = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(length));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case BORDER_WIDTH_THIN:
			value = CSS_BORDER_WIDTH_THIN;
			break;
		case BORDER_WIDTH_MEDIUM:
			value = CSS_BORDER_WIDTH_MEDIUM;
			break;
		case BORDER_WIDTH_THICK:
			value = CSS_BORDER_WIDTH_THICK;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error cascade_length_auto(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit))
{
	uint16_t value = CSS_BOTTOM_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BOTTOM_SET:
			value = CSS_BOTTOM_SET;
			length = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(length));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case BOTTOM_AUTO:
			value = CSS_BOTTOM_AUTO;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error cascade_length_normal(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit))
{
	uint16_t value = CSS_LETTER_SPACING_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case LETTER_SPACING_SET:
			value = CSS_LETTER_SPACING_SET;
			length = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(length));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case LETTER_SPACING_NORMAL:
			value = CSS_LETTER_SPACING_NORMAL;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error cascade_length_none(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit))
{
	uint16_t value = CSS_MAX_HEIGHT_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case MAX_HEIGHT_SET:
			value = CSS_MAX_HEIGHT_SET;
			length = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(length));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case MAX_HEIGHT_NONE:
			value = CSS_MAX_HEIGHT_NONE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error cascade_length(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed,
				css_unit))
{
	uint16_t value = CSS_MIN_HEIGHT_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		value = CSS_MIN_HEIGHT_SET;
		length = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(length));
		unit = *((uint32_t *) style->bytecode);
		advance_bytecode(style, sizeof(unit));
	}

	/** \todo lose fun != NULL once all properties have set routines */
	if (fun != NULL && outranks_existing(getOpcode(opv), 
			isImportant(opv), state)) {
		return fun(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error cascade_number(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, css_fixed))
{
	uint16_t value = 0;
	css_fixed length = 0;

	/** \todo values */

	if (isInherit(opv) == false) {
		value = 0;
		length = *((css_fixed *) style->bytecode);
		advance_bytecode(style, sizeof(length));
	}

	/** \todo lose fun != NULL once all properties have set routines */
	if (fun != NULL && outranks_existing(getOpcode(opv), 
			isImportant(opv), state)) {
		return fun(state->result, value, length);
	}

	return CSS_OK;
}

