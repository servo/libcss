/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "select/properties.h"
#include "select/propset.h"
#include "utils/utils.h"

static css_error cascade_bg_border_color(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_colour));
static css_error cascade_uri_none(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t, 
				lwc_string *));
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
static css_error cascade_page_break_after_before(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t));
static css_error cascade_counter_increment_reset(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t,
				css_computed_counter *));

css_error cascade_azimuth(uint32_t opv, css_style *style,
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

css_error initial_azimuth(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error compose_azimuth(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

css_error cascade_background_attachment(uint32_t opv, css_style *style, 
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

css_error initial_background_attachment(css_computed_style *style)
{
	return set_background_attachment(style, 
			CSS_BACKGROUND_ATTACHMENT_SCROLL);
}

css_error compose_background_attachment(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_background_attachment(child) == 
			CSS_BACKGROUND_ATTACHMENT_INHERIT) {
		return set_background_attachment(result, 
				css_computed_background_attachment(parent));
	}

	return CSS_OK;
}

css_error cascade_background_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, set_background_color);
}

css_error initial_background_color(css_computed_style *style)
{
	return set_background_color(style, CSS_BACKGROUND_COLOR_TRANSPARENT, 0);
}

css_error compose_background_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_background_color(child, &color) == 
			CSS_BACKGROUND_COLOR_INHERIT) {
		uint8_t p = css_computed_background_color(parent, &color);

		return set_background_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_background_image(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_uri_none(opv, style, state, set_background_image);
}

css_error initial_background_image(css_computed_style *style)
{
	return set_background_image(style, CSS_BACKGROUND_IMAGE_NONE, NULL);
}

css_error compose_background_image(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	lwc_string *url;

	if (css_computed_background_image(child, &url) == 
			CSS_BACKGROUND_IMAGE_INHERIT) {
		uint8_t p = css_computed_background_image(parent, &url);

		return set_background_image(result, p, url);
	}

	return CSS_OK;
}

css_error cascade_background_position(uint32_t opv, css_style *style, 
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

css_error initial_background_position(css_computed_style *style)
{
	return set_background_position(style, CSS_BACKGROUND_POSITION_SET, 
			0, CSS_UNIT_PCT, 0, CSS_UNIT_PCT);
}

css_error compose_background_position(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed hlength = 0, vlength = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;

	if (css_computed_background_position(child, &hlength, &hunit,
			&vlength, &vunit) == CSS_BACKGROUND_POSITION_INHERIT) {
		uint8_t p = css_computed_background_position(parent,
				&hlength, &hunit, &vlength, &vunit);

		return set_background_position(result, p, hlength, hunit,
				vlength, vunit);
	}

	return CSS_OK;
}


css_error cascade_background_repeat(uint32_t opv, css_style *style, 
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

css_error initial_background_repeat(css_computed_style *style)
{
	return set_background_repeat(style, CSS_BACKGROUND_REPEAT_REPEAT);
}

css_error compose_background_repeat(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_background_repeat(child) == 
			CSS_BACKGROUND_REPEAT_INHERIT) {
		return set_background_repeat(result,
				css_computed_background_repeat(parent));
	}

	return CSS_OK;
}

css_error cascade_border_collapse(uint32_t opv, css_style *style, 
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

css_error initial_border_collapse(css_computed_style *style)
{
	return set_border_collapse(style, CSS_BORDER_COLLAPSE_SEPARATE);
}

css_error compose_border_collapse(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_border_collapse(child) == 
			CSS_BORDER_COLLAPSE_INHERIT) {
		return set_border_collapse(result,
				css_computed_border_collapse(parent));
	}

	return CSS_OK;
}

css_error cascade_border_spacing(uint32_t opv, css_style *style, 
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

css_error initial_border_spacing(css_computed_style *style)
{
	return set_border_spacing(style, CSS_BORDER_SPACING_SET,
			0, CSS_UNIT_PX, 0, CSS_UNIT_PX);
}

css_error compose_border_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed hlength = 0, vlength = 0;
	css_unit hunit = CSS_UNIT_PX, vunit = CSS_UNIT_PX;

	if ((child->uncommon == NULL && parent->uncommon != NULL) || 
			css_computed_border_spacing(child, 
			&hlength, &hunit, &vlength, &vunit) == 
			CSS_BORDER_SPACING_INHERIT) {
		uint8_t p = css_computed_border_spacing(parent, 
				&hlength, &hunit, &vlength, &vunit);

		return set_border_spacing(result, p, hlength, hunit, 
				vlength, vunit);
	}

	return CSS_OK;
}

css_error cascade_border_top_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, set_border_top_color);
}

css_error initial_border_top_color(css_computed_style *style)
{
	return set_border_top_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

css_error compose_border_top_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_border_top_color(child, &color) == 
			CSS_BORDER_COLOR_INHERIT) {
		uint8_t p = css_computed_border_top_color(parent, &color);

		return set_border_top_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_border_right_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, 
			set_border_right_color);
}

css_error initial_border_right_color(css_computed_style *style)
{
	return set_border_right_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

css_error compose_border_right_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_border_right_color(child, &color) == 
			CSS_BORDER_COLOR_INHERIT) {
		uint8_t p = css_computed_border_right_color(parent, &color);

		return set_border_right_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_border_bottom_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state,
			set_border_bottom_color);
}

css_error initial_border_bottom_color(css_computed_style *style)
{
	return set_border_bottom_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

css_error compose_border_bottom_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_border_bottom_color(child, &color) == 
			CSS_BORDER_COLOR_INHERIT) {
		uint8_t p = css_computed_border_bottom_color(parent, &color);

		return set_border_bottom_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_border_left_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, 
			set_border_left_color);
}

css_error initial_border_left_color(css_computed_style *style)
{
	return set_border_left_color(style, CSS_BORDER_COLOR_COLOR, 0);
}

css_error compose_border_left_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_border_left_color(child, &color) == 
			CSS_BORDER_COLOR_INHERIT) {
		uint8_t p = css_computed_border_left_color(parent, &color);

		return set_border_left_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_border_top_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_top_style);
}

css_error initial_border_top_style(css_computed_style *style)
{
	return set_border_top_style(style, CSS_BORDER_STYLE_NONE);
}

css_error compose_border_top_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_border_top_style(child) == 
			CSS_BORDER_STYLE_INHERIT) {
		return set_border_top_style(result,
				css_computed_border_top_style(parent));
	}

	return CSS_OK;
}

css_error cascade_border_right_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_right_style);
}

css_error initial_border_right_style(css_computed_style *style)
{
	return set_border_right_style(style, CSS_BORDER_STYLE_NONE);
}

css_error compose_border_right_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_border_right_style(child) == 
			CSS_BORDER_STYLE_INHERIT) {
		return set_border_right_style(result,
				css_computed_border_right_style(parent));
	}

	return CSS_OK;
}

css_error cascade_border_bottom_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_bottom_style);
}

css_error initial_border_bottom_style(css_computed_style *style)
{
	return set_border_bottom_style(style, CSS_BORDER_STYLE_NONE);
}

css_error compose_border_bottom_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_border_bottom_style(child) == 
			CSS_BORDER_STYLE_INHERIT) {
		return set_border_bottom_style(result,
				css_computed_border_bottom_style(parent));
	}

	return CSS_OK;
}

css_error cascade_border_left_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_left_style);
}

css_error initial_border_left_style(css_computed_style *style)
{
	return set_border_left_style(style, CSS_BORDER_STYLE_NONE);
}

css_error compose_border_left_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_border_left_style(child) == 
			CSS_BORDER_STYLE_INHERIT) {
		return set_border_left_style(result,
				css_computed_border_left_style(parent));
	}

	return CSS_OK;
}

css_error cascade_border_top_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_top_width);
}

css_error initial_border_top_width(css_computed_style *style)
{
	return set_border_top_width(style, CSS_BORDER_WIDTH_MEDIUM, 
			0, CSS_UNIT_PX);
}

css_error compose_border_top_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_border_top_width(child, &length, &unit) == 
			CSS_BORDER_WIDTH_INHERIT) {
		uint8_t p = css_computed_border_top_width(parent, 
				&length, &unit);

		return set_border_top_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_border_right_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_right_width);
}

css_error initial_border_right_width(css_computed_style *style)
{
	return set_border_right_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

css_error compose_border_right_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_border_right_width(child, &length, &unit) == 
			CSS_BORDER_WIDTH_INHERIT) {
		uint8_t p = css_computed_border_right_width(parent, 
				&length, &unit);

		return set_border_right_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_border_bottom_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_bottom_width);
}

css_error initial_border_bottom_width(css_computed_style *style)
{
	return set_border_bottom_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

css_error compose_border_bottom_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_border_bottom_width(child, &length, &unit) == 
			CSS_BORDER_WIDTH_INHERIT) {
		uint8_t p = css_computed_border_bottom_width(parent, 
				&length, &unit);

		return set_border_bottom_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_border_left_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_border_left_width);
}

css_error initial_border_left_width(css_computed_style *style)
{
	return set_border_left_width(style, CSS_BORDER_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

css_error compose_border_left_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_border_left_width(child, &length, &unit) == 
			CSS_BORDER_WIDTH_INHERIT) {
		uint8_t p = css_computed_border_left_width(parent, 
				&length, &unit);

		return set_border_left_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_bottom);
}

css_error initial_bottom(css_computed_style *style)
{
	return set_bottom(style, CSS_BOTTOM_AUTO, 0, CSS_UNIT_PX);
}

css_error compose_bottom(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_bottom(child, &length, &unit) == 
			CSS_BOTTOM_INHERIT) {
		uint8_t p = css_computed_bottom(parent, &length, &unit);

		return set_bottom(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_caption_side(uint32_t opv, css_style *style, 
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

css_error initial_caption_side(css_computed_style *style)
{
	return set_caption_side(style, CSS_CAPTION_SIDE_TOP);
}

css_error compose_caption_side(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_caption_side(child) == 
			CSS_CAPTION_SIDE_INHERIT) {
		return set_caption_side(result, 
				css_computed_caption_side(parent));
	}

	return CSS_OK;
}

css_error cascade_clear(uint32_t opv, css_style *style, 
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

css_error initial_clear(css_computed_style *style)
{
	return set_clear(style, CSS_CLEAR_NONE);
}

css_error compose_clear(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_clear(child) == CSS_CLEAR_INHERIT) {
		return set_clear(result, css_computed_clear(parent));
	}

	return CSS_OK;
}

css_error cascade_clip(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_CLIP_INHERIT;
	css_computed_clip_rect rect = { 0, 0, 0, 0, 
			CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX,
			false, false, false, false };

	if (isInherit(opv) == false) {
		switch (getValue(opv) & CLIP_SHAPE_MASK) {
		case CLIP_SHAPE_RECT:
			if (getValue(opv) & CLIP_RECT_TOP_AUTO) {
				rect.top_auto = true;
			} else {
				rect.top = *((css_fixed *) style->bytecode);
				advance_bytecode(style, sizeof(css_fixed));
				rect.tunit = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(uint32_t));
			}
			if (getValue(opv) & CLIP_RECT_RIGHT_AUTO) {
				rect.right_auto = true;
			} else {
				rect.right = *((css_fixed *) style->bytecode);
				advance_bytecode(style, sizeof(css_fixed));
				rect.runit = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(uint32_t));
			}
			if (getValue(opv) & CLIP_RECT_BOTTOM_AUTO) {
				rect.bottom_auto = true;
			} else {
				rect.bottom = *((css_fixed *) style->bytecode);
				advance_bytecode(style, sizeof(css_fixed));
				rect.bunit = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(uint32_t));
			}
			if (getValue(opv) & CLIP_RECT_LEFT_AUTO) {
				rect.left_auto = true;
			} else {
				rect.left = *((css_fixed *) style->bytecode);
				advance_bytecode(style, sizeof(css_fixed));
				rect.lunit = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(uint32_t));
			}
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

css_error initial_clip(css_computed_style *style)
{
	css_computed_clip_rect rect = { 0, 0, 0, 0, 
			CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX,
			false, false, false, false };

	return set_clip(style, CSS_CLIP_AUTO, &rect);
}

css_error compose_clip(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_computed_clip_rect rect = { 0, 0, 0, 0, 
			CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX, CSS_UNIT_PX,
			false, false, false, false };

	if ((child->uncommon == NULL && parent->uncommon != NULL) || 
			css_computed_clip(child, &rect) ==
				CSS_CLIP_INHERIT) {
		uint8_t p = css_computed_clip(parent, &rect);
		return set_clip(result, p, &rect);
	}

	return CSS_OK;
}

css_error cascade_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_COLOR_INHERIT;
	css_colour color = 0;

	if (isInherit(opv) == false) {
		value = CSS_COLOR_COLOR;
		color = *((css_colour *) style->bytecode);
		advance_bytecode(style, sizeof(color));
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_color(state->result, value, color);
	}

	return CSS_OK;
}

css_error initial_color(css_computed_style *style)
{
	return set_color(style, CSS_COLOR_COLOR, 0);
}

css_error compose_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color;

	if (css_computed_color(child, &color) == CSS_COLOR_INHERIT) {
		uint8_t p = css_computed_color(parent, &color);

		return set_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_content(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_CONTENT_INHERIT;
	css_computed_content_item *content = NULL;
	uint32_t n_contents = 0;

	if (isInherit(opv) == false) {
		uint32_t v = getValue(opv);

		if (v == CONTENT_NORMAL) {
			value = CSS_CONTENT_NORMAL;
		} else if (v == CONTENT_NONE) {
			value = CSS_CONTENT_NONE;
		} else {
			value = CSS_CONTENT_SET;
                        
                        while (v != CONTENT_NORMAL) {
                                lwc_string *he = *((lwc_string **) style->bytecode);
                                css_computed_content_item *temp;
                                
                                temp = state->result->alloc(content,
						(n_contents + 1) *
						sizeof(css_computed_content_item),
						state->result->pw);
				if (temp == NULL) {
					if (content != NULL) {
						state->result->alloc(content,
							0, state->result->pw);
					}
					return CSS_NOMEM;
				}

				content = temp;

				switch (v & 0xff) {
				case CONTENT_COUNTER:
					advance_bytecode(style, sizeof(he));

					content[n_contents].type =
						CSS_COMPUTED_CONTENT_COUNTER;
					content[n_contents].data.counter.name = he;
					content[n_contents].data.counter.style = v >> CONTENT_COUNTER_STYLE_SHIFT;
					break;
				case CONTENT_COUNTERS:
				{
					lwc_string *sep;
	
					advance_bytecode(style, sizeof(he));

					sep = *((lwc_string **) 
							style->bytecode);
					advance_bytecode(style, sizeof(sep));

					content[n_contents].type =
						CSS_COMPUTED_CONTENT_COUNTERS;
					content[n_contents].data.counters.name = he;
					content[n_contents].data.counters.sep = sep;
					content[n_contents].data.counters.style = v >> CONTENT_COUNTERS_STYLE_SHIFT;
				}
					break;
				case CONTENT_URI:
					advance_bytecode(style, sizeof(he));

					content[n_contents].type =
						CSS_COMPUTED_CONTENT_URI;
					content[n_contents].data.uri = he;
					break;
				case CONTENT_ATTR:
					advance_bytecode(style, sizeof(he));

					content[n_contents].type =
						CSS_COMPUTED_CONTENT_ATTR;
					content[n_contents].data.attr = he;
					break;
				case CONTENT_STRING:
					advance_bytecode(style, sizeof(he));

					content[n_contents].type =
						CSS_COMPUTED_CONTENT_STRING;
					content[n_contents].data.string = he;
					break;
				case CONTENT_OPEN_QUOTE:
					content[n_contents].type =
						CSS_COMPUTED_CONTENT_OPEN_QUOTE;
					break;
				case CONTENT_CLOSE_QUOTE:
					content[n_contents].type =
						CSS_COMPUTED_CONTENT_CLOSE_QUOTE;
					break;
				case CONTENT_NO_OPEN_QUOTE:
					content[n_contents].type =
						CSS_COMPUTED_CONTENT_NO_OPEN_QUOTE;
					break;
				case CONTENT_NO_CLOSE_QUOTE:
					content[n_contents].type =
						CSS_COMPUTED_CONTENT_NO_CLOSE_QUOTE;
					break;
				}

				n_contents++;

				v = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(v));
			}
		}
	}

	/* If we have some content, terminate the array with a blank entry */
	if (n_contents > 0) {
		css_computed_content_item *temp;

		temp = state->result->alloc(content,
				(n_contents + 1) * sizeof(css_computed_content_item),
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(content, 0, state->result->pw);
			return CSS_NOMEM;
		}

		content = temp;

		content[n_contents].type = CSS_COMPUTED_CONTENT_NONE;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_content(state->result, value, content);
	} else if (content != NULL) {
		state->result->alloc(content, 0, state->result->pw);
	}

	return CSS_OK;
}

css_error initial_content(css_computed_style *style)
{
	return set_content(style, CSS_CONTENT_NORMAL, NULL);
}

css_error compose_content(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_error error;
	const css_computed_content_item *items = NULL;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_content(child, &items) ==
				CSS_CONTENT_INHERIT) {
		uint8_t p = css_computed_content(parent, &items);
		size_t n_items = 0;
		css_computed_content_item *copy = NULL;

		if (p == CSS_CONTENT_SET) {
			const css_computed_content_item *i;

			for (i = items; i->type != CSS_COMPUTED_CONTENT_NONE; 
					i++)
				n_items++;

			copy = result->alloc(NULL, (n_items + 1) * 
					sizeof(css_computed_content_item),
					result->pw);
			if (copy == NULL)
				return CSS_NOMEM;

			memcpy(copy, items, (n_items + 1) * 
					sizeof(css_computed_content_item));
		}

		error = set_content(result, p, copy);
		if (error != CSS_OK && copy != NULL)
			result->alloc(copy, 0, result->pw);

		return error;
	}

	return CSS_OK;
}

css_error cascade_counter_increment(uint32_t opv, css_style *style, 
		css_select_state *state)
{	
	return cascade_counter_increment_reset(opv, style, state, 
			set_counter_increment);
}

css_error initial_counter_increment(css_computed_style *style)
{
	return set_counter_increment(style, CSS_COUNTER_INCREMENT_NONE, NULL);
}

css_error compose_counter_increment(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_error error;
	const css_computed_counter *items = NULL;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_counter_increment(child, &items) ==
				CSS_COUNTER_INCREMENT_INHERIT) {
		uint8_t p = css_computed_counter_increment(parent, &items);
		size_t n_items = 0;
		css_computed_counter *copy = NULL;

		if (p == CSS_COUNTER_INCREMENT_NAMED && items != NULL) {
			const css_computed_counter *i;

			for (i = items; i->name != NULL; i++)
				n_items++;

			copy = result->alloc(NULL, (n_items + 1) * 
					sizeof(css_computed_counter),
					result->pw);
			if (copy == NULL)
				return CSS_NOMEM;

			memcpy(copy, items, (n_items + 1) * 
					sizeof(css_computed_counter));
		}

		error = set_counter_increment(result, p, copy);
		if (error != CSS_OK && copy != NULL)
			result->alloc(copy, 0, result->pw);

		return error;
	}

	return CSS_OK;
}

css_error cascade_counter_reset(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_counter_increment_reset(opv, style, state,
			set_counter_reset);
}

css_error initial_counter_reset(css_computed_style *style)
{
	return set_counter_reset(style, CSS_COUNTER_RESET_NONE, NULL);
}

css_error compose_counter_reset(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_error error;
	const css_computed_counter *items = NULL;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_counter_reset(child, &items) ==
				CSS_COUNTER_RESET_INHERIT) {
		uint8_t p = css_computed_counter_reset(parent, &items);
		size_t n_items = 0;
		css_computed_counter *copy = NULL;

		if (p == CSS_COUNTER_RESET_NAMED && items != NULL) {
			const css_computed_counter *i;

			for (i = items; i->name != NULL; i++)
				n_items++;

			copy = result->alloc(NULL, (n_items + 1) * 
					sizeof(css_computed_counter),
					result->pw);
			if (copy == NULL)
				return CSS_NOMEM;

			memcpy(copy, items, (n_items + 1) * 
					sizeof(css_computed_counter));
		}

		error = set_counter_reset(result, p, copy);
		if (error != CSS_OK && copy != NULL)
			result->alloc(copy, 0, result->pw);

		return error;
	}

	return CSS_OK;
}

css_error cascade_cue_after(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo cue-after */
	return cascade_uri_none(opv, style, state, NULL);
}

css_error initial_cue_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error compose_cue_after(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

css_error cascade_cue_before(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo cue-before */
	return cascade_uri_none(opv, style, state, NULL);
}

css_error initial_cue_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error compose_cue_before(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

css_error cascade_cursor(uint32_t opv, css_style *style, 
		css_select_state *state)
{	
	uint16_t value = CSS_CURSOR_INHERIT;
	lwc_string **uris = NULL;
	uint32_t n_uris = 0;

	if (isInherit(opv) == false) {
		uint32_t v = getValue(opv);

		while (v == CURSOR_URI) {
			lwc_string *uri;
			lwc_string **temp;

			uri = *((lwc_string **) style->bytecode);
			advance_bytecode(style, sizeof(uri));

			temp = state->result->alloc(uris, 
					(n_uris + 1) * sizeof(lwc_string *), 
					state->result->pw);
			if (temp == NULL) {
				if (uris != NULL) {
					state->result->alloc(uris, 0,
							state->result->pw);
				}
				return CSS_NOMEM;
			}

			uris = temp;

			uris[n_uris] = uri;

			n_uris++;

			v = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(v));
		}

		switch (v) {
		case CURSOR_AUTO:
			value = CSS_CURSOR_AUTO;
			break;
		case CURSOR_CROSSHAIR:
			value = CSS_CURSOR_CROSSHAIR;
			break;
		case CURSOR_DEFAULT:
			value = CSS_CURSOR_DEFAULT;
			break;
		case CURSOR_POINTER:
			value = CSS_CURSOR_POINTER;
			break;
		case CURSOR_MOVE:
			value = CSS_CURSOR_MOVE;
			break;
		case CURSOR_E_RESIZE:
			value = CSS_CURSOR_E_RESIZE;
			break;
		case CURSOR_NE_RESIZE:
			value = CSS_CURSOR_NE_RESIZE;
			break;
		case CURSOR_NW_RESIZE:
			value = CSS_CURSOR_NW_RESIZE;
			break;
		case CURSOR_N_RESIZE:
			value = CSS_CURSOR_N_RESIZE;
			break;
		case CURSOR_SE_RESIZE:
			value = CSS_CURSOR_SE_RESIZE;
			break;
		case CURSOR_SW_RESIZE:
			value = CSS_CURSOR_SW_RESIZE;
			break;
		case CURSOR_S_RESIZE:
			value = CSS_CURSOR_S_RESIZE;
			break;
		case CURSOR_W_RESIZE:
			value = CSS_CURSOR_W_RESIZE;
			break;
		case CURSOR_TEXT:
			value = CSS_CURSOR_TEXT;
			break;
		case CURSOR_WAIT:
			value = CSS_CURSOR_WAIT;
			break;
		case CURSOR_HELP:
			value = CSS_CURSOR_HELP;
			break;
		case CURSOR_PROGRESS:
			value = CSS_CURSOR_PROGRESS;
			break;
		}
	}

	/* Terminate array with blank entry, if needed */
	if (n_uris > 0) {
		lwc_string **temp;

		temp = state->result->alloc(uris, 
				(n_uris + 1) * sizeof(lwc_string *), 
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(uris, 0, state->result->pw);
			return CSS_NOMEM;
		}

		uris = temp;

		uris[n_uris] = NULL;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_cursor(state->result, value, uris);
	} else {
		if (n_uris > 0)
			state->result->alloc(uris, 0, state->result->pw);
	}

	return CSS_OK;
}

css_error initial_cursor(css_computed_style *style)
{
	return set_cursor(style, CSS_CURSOR_AUTO, NULL);
}

css_error compose_cursor(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_error error;
	lwc_string **urls = NULL;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_cursor(child, &urls) ==
				CSS_CURSOR_INHERIT) {
		uint8_t p = css_computed_cursor(parent, &urls);
		size_t n_urls = 0;
		lwc_string **copy = NULL;

		if (urls != NULL) {
			lwc_string **i;

			for (i = urls; (*i) != NULL; i++)
				n_urls++;

			copy = result->alloc(NULL, (n_urls + 1) * 
					sizeof(lwc_string *),
					result->pw);
			if (copy == NULL)
				return CSS_NOMEM;

			memcpy(copy, urls, (n_urls + 1) * 
					sizeof(lwc_string *));
		}

		error = set_cursor(result, p, copy);
		if (error != CSS_OK && copy != NULL)
			result->alloc(copy, 0, result->pw);

		return error;
	}

	return CSS_OK;
}

css_error cascade_direction(uint32_t opv, css_style *style, 
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

css_error initial_direction(css_computed_style *style)
{
	return set_direction(style, CSS_DIRECTION_LTR);
}

css_error compose_direction(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_direction(child) == CSS_DIRECTION_INHERIT) {
		return set_direction(result, css_computed_direction(parent));
	}

	return CSS_OK;
}

css_error cascade_display(uint32_t opv, css_style *style, 
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

css_error initial_display(css_computed_style *style)
{
	return set_display(style, CSS_DISPLAY_INLINE);
}

css_error compose_display(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_display(child) == CSS_DISPLAY_INHERIT) {
		return set_display(result, css_computed_display(parent));
	}

	return CSS_OK;
}

css_error cascade_elevation(uint32_t opv, css_style *style, 
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

css_error initial_elevation(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error compose_elevation(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

css_error cascade_empty_cells(uint32_t opv, css_style *style, 
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

css_error initial_empty_cells(css_computed_style *style)
{
	return set_empty_cells(style, CSS_EMPTY_CELLS_SHOW);
}

css_error compose_empty_cells(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_empty_cells(child) == CSS_EMPTY_CELLS_INHERIT) {
		return set_empty_cells(result, 
				css_computed_empty_cells(parent));
	}

	return CSS_OK;
}

css_error cascade_float(uint32_t opv, css_style *style, 
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

css_error initial_float(css_computed_style *style)
{
	return set_float(style, CSS_FLOAT_NONE);
}

css_error compose_float(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_float(child) == CSS_FLOAT_INHERIT) {
		return set_float(result, css_computed_float(parent));
	}

	return CSS_OK;
}

css_error cascade_font_family(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_FONT_FAMILY_INHERIT;
	lwc_string **fonts = NULL;
	uint32_t n_fonts = 0;

	if (isInherit(opv) == false) {
		uint32_t v = getValue(opv);

		while (v != FONT_FAMILY_END) {
			lwc_string *font = NULL;
			lwc_string **temp;

			switch (v) {
			case FONT_FAMILY_STRING:
			case FONT_FAMILY_IDENT_LIST:
				font = *((lwc_string **) 
						style->bytecode);
				advance_bytecode(style, sizeof(font));
				break;
			case FONT_FAMILY_SERIF:
				if (value == CSS_FONT_FAMILY_INHERIT)
					value = CSS_FONT_FAMILY_SERIF;
				break;
			case FONT_FAMILY_SANS_SERIF:
				if (value == CSS_FONT_FAMILY_INHERIT)
					value = CSS_FONT_FAMILY_SANS_SERIF;
				break;
			case FONT_FAMILY_CURSIVE:
				if (value == CSS_FONT_FAMILY_INHERIT)
					value = CSS_FONT_FAMILY_CURSIVE;
				break;
			case FONT_FAMILY_FANTASY:
				if (value == CSS_FONT_FAMILY_INHERIT)
					value = CSS_FONT_FAMILY_FANTASY;
				break;
			case FONT_FAMILY_MONOSPACE:
				if (value == CSS_FONT_FAMILY_INHERIT)
					value = CSS_FONT_FAMILY_MONOSPACE;
				break;
			}

			/* Only use family-names which occur before the first
			 * generic-family. Any values which occur after the
			 * first generic-family are ignored. */
			/** \todo Do this at bytecode generation time? */
			if (value == CSS_FONT_FAMILY_INHERIT && font != NULL) {
				temp = state->result->alloc(fonts, 
					(n_fonts + 1) * sizeof(lwc_string *), 
					state->result->pw);
				if (temp == NULL) {
					if (fonts != NULL) {
						state->result->alloc(fonts, 0,
							state->result->pw);
					}
					return CSS_NOMEM;
				}

				fonts = temp;

				fonts[n_fonts] = font;

				n_fonts++;
			}

			v = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(v));
		}
	}

	/* Terminate array with blank entry, if needed */
	if (n_fonts > 0) {
		lwc_string **temp;

		temp = state->result->alloc(fonts, 
				(n_fonts + 1) * sizeof(lwc_string *), 
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(fonts, 0, state->result->pw);
			return CSS_NOMEM;
		}

		fonts = temp;

		fonts[n_fonts] = NULL;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_font_family(state->result, value, fonts);
	} else {
		if (n_fonts > 0)
			state->result->alloc(fonts, 0, state->result->pw);
	}

	return CSS_OK;
}

css_error initial_font_family(css_computed_style *style)
{
	return set_font_family(style, CSS_FONT_FAMILY_DEFAULT, NULL);
}

css_error compose_font_family(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_error error;
	lwc_string **urls = NULL;

	if (css_computed_font_family(child, &urls) == CSS_FONT_FAMILY_INHERIT) {
		uint8_t p = css_computed_font_family(parent, &urls);
		size_t n_urls = 0;
		lwc_string **copy = NULL;

		if (urls != NULL) {
			lwc_string **i;

			for (i = urls; (*i) != NULL; i++)
				n_urls++;

			copy = result->alloc(NULL, (n_urls + 1) * 
					sizeof(lwc_string *),
					result->pw);
			if (copy == NULL)
				return CSS_NOMEM;

			memcpy(copy, urls, (n_urls + 1) * 
					sizeof(lwc_string *));
		}

		error = set_font_family(result, p, copy);
		if (error != CSS_OK && copy != NULL)
			result->alloc(copy, 0, result->pw);

		return error;
	}

	return CSS_OK;
}

css_error cascade_font_size(uint32_t opv, css_style *style, 
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

css_error initial_font_size(css_computed_style *style)
{
	return set_font_size(style, CSS_FONT_SIZE_MEDIUM, 0, CSS_UNIT_PX);
}

css_error compose_font_size(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed size = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_font_size(child, &size, &unit) ==
			CSS_FONT_SIZE_INHERIT) {
		uint8_t p = css_computed_font_size(parent, &size, &unit);
		return set_font_size(result, p, size, unit);
	}

	return CSS_OK;
}

css_error cascade_font_style(uint32_t opv, css_style *style, 
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

css_error initial_font_style(css_computed_style *style)
{
	return set_font_style(style, CSS_FONT_STYLE_NORMAL);
}

css_error compose_font_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_font_style(child) == CSS_FONT_STYLE_INHERIT) {
		return set_font_style(result, css_computed_font_style(parent));
	}

	return CSS_OK;
}

css_error cascade_font_variant(uint32_t opv, css_style *style, 
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

css_error initial_font_variant(css_computed_style *style)
{
	return set_font_variant(style, CSS_FONT_VARIANT_NORMAL);
}

css_error compose_font_variant(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_font_variant(child) == CSS_FONT_VARIANT_INHERIT) {
		return set_font_variant(result, 
				css_computed_font_variant(parent));
	}

	return CSS_OK;
}

css_error cascade_font_weight(uint32_t opv, css_style *style, 
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

css_error initial_font_weight(css_computed_style *style)
{
	return set_font_weight(style, CSS_FONT_WEIGHT_NORMAL);
}

css_error compose_font_weight(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_font_weight(child) == CSS_FONT_WEIGHT_INHERIT) {
		return set_font_weight(result, 
				css_computed_font_weight(parent));
	}

	return CSS_OK;
}

css_error cascade_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_height);
}

css_error initial_height(css_computed_style *style)
{
	return set_height(style, CSS_HEIGHT_AUTO, 0, CSS_UNIT_PX);
}

css_error compose_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_height(child, &length, &unit) == 
			CSS_HEIGHT_INHERIT) {
		uint8_t p = css_computed_height(parent, &length, &unit);

		return set_height(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_left);
}

css_error initial_left(css_computed_style *style)
{
	return set_left(style, CSS_LEFT_AUTO, 0, CSS_UNIT_PX);
}

css_error compose_left(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_left(child, &length, &unit) == 
			CSS_LEFT_INHERIT) {
		uint8_t p = css_computed_left(parent, &length, &unit);

		return set_left(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_letter_spacing(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_normal(opv, style, state, set_letter_spacing);
}

css_error initial_letter_spacing(css_computed_style *style)
{
	return set_letter_spacing(style, CSS_LETTER_SPACING_NORMAL, 
			0, CSS_UNIT_PX);
}

css_error compose_letter_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if ((child->uncommon == NULL && parent->uncommon != NULL) || 
			css_computed_letter_spacing(child, &length, &unit) == 
			CSS_LETTER_SPACING_INHERIT) {
		uint8_t p = css_computed_letter_spacing(parent, &length, &unit);

		return set_letter_spacing(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_line_height(uint32_t opv, css_style *style, 
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

css_error initial_line_height(css_computed_style *style)
{
	return set_line_height(style, CSS_LINE_HEIGHT_NORMAL, 0, CSS_UNIT_PX);
}

css_error compose_line_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_line_height(child, &length, &unit) == 
			CSS_LINE_HEIGHT_INHERIT) {
		uint8_t p = css_computed_line_height(parent, &length, &unit);

		return set_line_height(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_list_style_image(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_uri_none(opv, style, state, set_list_style_image);
}

css_error initial_list_style_image(css_computed_style *style)
{
	return set_list_style_image(style, CSS_LIST_STYLE_IMAGE_NONE, NULL);
}

css_error compose_list_style_image(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	lwc_string *url;

	if (css_computed_list_style_image(child, &url) == 
			CSS_LIST_STYLE_IMAGE_INHERIT) {
		uint8_t p = css_computed_list_style_image(parent, &url);

		return set_list_style_image(result, p, url);
	}

	return CSS_OK;
}

css_error cascade_list_style_position(uint32_t opv, css_style *style, 
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

css_error initial_list_style_position(css_computed_style *style)
{
	return set_list_style_position(style, CSS_LIST_STYLE_POSITION_OUTSIDE);
}

css_error compose_list_style_position(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_list_style_position(child) == 
			CSS_LIST_STYLE_POSITION_INHERIT) {
		return set_list_style_position(result,
				css_computed_list_style_position(parent));
	}

	return CSS_OK;
}

css_error cascade_list_style_type(uint32_t opv, css_style *style, 
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

css_error initial_list_style_type(css_computed_style *style)
{
	return set_list_style_type(style, CSS_LIST_STYLE_TYPE_DISC);
}

css_error compose_list_style_type(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_list_style_type(child) == 
			CSS_LIST_STYLE_TYPE_INHERIT) {
		return set_list_style_type(result,
				css_computed_list_style_type(parent));
	}

	return CSS_OK;
}

css_error cascade_margin_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_top);
}

css_error initial_margin_top(css_computed_style *style)
{
	return set_margin_top(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

css_error compose_margin_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_margin_top(child, &length, &unit) ==
			CSS_MARGIN_INHERIT) {
		uint8_t p = css_computed_margin_top(parent, &length, &unit);

		return set_margin_top(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_margin_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_right);
}

css_error initial_margin_right(css_computed_style *style)
{
	return set_margin_right(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

css_error compose_margin_right(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_margin_right(child, &length, &unit) ==
			CSS_MARGIN_INHERIT) {
		uint8_t p = css_computed_margin_right(parent, &length, &unit);

		return set_margin_right(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_margin_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_bottom);
}

css_error initial_margin_bottom(css_computed_style *style)
{
	return set_margin_bottom(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

css_error compose_margin_bottom(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_margin_bottom(child, &length, &unit) ==
			CSS_MARGIN_INHERIT) {
		uint8_t p = css_computed_margin_bottom(parent, &length, &unit);

		return set_margin_bottom(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_margin_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_margin_left);
}

css_error initial_margin_left(css_computed_style *style)
{
	return set_margin_left(style, CSS_MARGIN_SET, 0, CSS_UNIT_PX);
}

css_error compose_margin_left(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_margin_left(child, &length, &unit) ==
			CSS_MARGIN_INHERIT) {
		uint8_t p = css_computed_margin_left(parent, &length, &unit);

		return set_margin_left(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_max_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_none(opv, style, state, set_max_height);
}

css_error initial_max_height(css_computed_style *style)
{
	return set_max_height(style, CSS_MAX_HEIGHT_NONE, 0, CSS_UNIT_PX);
}

css_error compose_max_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_max_height(child, &length, &unit) ==
			CSS_MAX_HEIGHT_INHERIT) {
		uint8_t p = css_computed_max_height(parent, &length, &unit);

		return set_max_height(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_max_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_none(opv, style, state, set_max_width);;
}

css_error initial_max_width(css_computed_style *style)
{
	return set_max_width(style, CSS_MAX_WIDTH_NONE, 0, CSS_UNIT_PX);
}

css_error compose_max_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_max_width(child, &length, &unit) ==
			CSS_MAX_WIDTH_INHERIT) {
		uint8_t p = css_computed_max_width(parent, &length, &unit);

		return set_max_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_min_height(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_min_height);
}

css_error initial_min_height(css_computed_style *style)
{
	return set_min_height(style, CSS_MIN_HEIGHT_SET, 0, CSS_UNIT_PX);
}

css_error compose_min_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_min_height(child, &length, &unit) ==
			CSS_MIN_HEIGHT_INHERIT) {
		uint8_t p = css_computed_min_height(parent, &length, &unit);

		return set_min_height(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_min_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_min_width);
}

css_error initial_min_width(css_computed_style *style)
{
	return set_min_width(style, CSS_MIN_WIDTH_SET, 0, CSS_UNIT_PX);
}

css_error compose_min_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_min_width(child, &length, &unit) ==
			CSS_MIN_WIDTH_INHERIT) {
		uint8_t p = css_computed_min_width(parent, &length, &unit);

		return set_min_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_orphans(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo orphans */
	return cascade_number(opv, style, state, NULL);
}

css_error initial_orphans(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error compose_orphans(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

css_error cascade_outline_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_OUTLINE_COLOR_INHERIT;
	css_colour color = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case OUTLINE_COLOR_SET:
			value = CSS_OUTLINE_COLOR_COLOR;
			color = *((css_colour *) style->bytecode);
			advance_bytecode(style, sizeof(color));
			break;
		case OUTLINE_COLOR_INVERT:
			value = CSS_OUTLINE_COLOR_INVERT;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_outline_color(state->result, value, color);
	}

	return CSS_OK;
}

css_error initial_outline_color(css_computed_style *style)
{
	return set_outline_color(style, CSS_OUTLINE_COLOR_INVERT, 0);
}

css_error compose_outline_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_colour color = 0;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_outline_color(child, &color) ==
			CSS_OUTLINE_COLOR_INHERIT) {
		uint8_t p = css_computed_outline_color(parent, &color);

		return set_outline_color(result, p, color);
	}

	return CSS_OK;
}

css_error cascade_outline_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_outline_style);
}

css_error initial_outline_style(css_computed_style *style)
{
	return set_outline_style(style, CSS_OUTLINE_STYLE_NONE);
}

css_error compose_outline_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	if (css_computed_outline_style(child) == CSS_OUTLINE_STYLE_INHERIT) {
		return set_outline_style(result, 
				css_computed_outline_style(parent));
	}

	return CSS_OK;
}

css_error cascade_outline_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_width(opv, style, state, set_outline_width);
}

css_error initial_outline_width(css_computed_style *style)
{
	return set_outline_width(style, CSS_OUTLINE_WIDTH_MEDIUM,
			0, CSS_UNIT_PX);
}

css_error compose_outline_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			css_computed_outline_width(child, &length, &unit) ==
			CSS_OUTLINE_WIDTH_INHERIT) {
		uint8_t p = css_computed_outline_width(parent, &length, &unit);

		return set_outline_width(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_overflow(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_OVERFLOW_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case OVERFLOW_VISIBLE:
			value = CSS_OVERFLOW_VISIBLE;
			break;
		case OVERFLOW_HIDDEN:
			value = CSS_OVERFLOW_HIDDEN;
			break;
		case OVERFLOW_SCROLL:
			value = CSS_OVERFLOW_SCROLL;
			break;
		case OVERFLOW_AUTO:
			value = CSS_OVERFLOW_AUTO;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_overflow(state->result, value);
	}

	return CSS_OK;
}

css_error initial_overflow(css_computed_style *style)
{
	return set_overflow(style, CSS_OVERFLOW_VISIBLE);
}

css_error cascade_padding_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_top);
}

css_error initial_padding_top(css_computed_style *style)
{
	return set_padding_top(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

css_error cascade_padding_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_right);
}

css_error initial_padding_right(css_computed_style *style)
{
	return set_padding_right(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

css_error cascade_padding_bottom(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_bottom);
}

css_error initial_padding_bottom(css_computed_style *style)
{
	return set_padding_bottom(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

css_error cascade_padding_left(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_left);
}

css_error initial_padding_left(css_computed_style *style)
{
	return set_padding_left(style, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

css_error cascade_page_break_after(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo page-break-after */
	return cascade_page_break_after_before(opv, style, state, NULL);
}

css_error initial_page_break_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_page_break_before(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo page-break-before */
	return cascade_page_break_after_before(opv, style, state, NULL);
}

css_error initial_page_break_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_page_break_inside(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case PAGE_BREAK_INSIDE_AUTO:
		case PAGE_BREAK_INSIDE_AVOID:
			/** \todo convert to public values */
			value = 0;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo page-break-inside */
	}

	return CSS_OK;
}

css_error initial_page_break_inside(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_pause_after(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pause-after */
	return cascade_length(opv, style, state, NULL);
}

css_error initial_pause_after(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_pause_before(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pause-before */
	return cascade_length(opv, style, state, NULL);
}

css_error initial_pause_before(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_pitch_range(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pitch-range */
	return cascade_number(opv, style, state, NULL);
}

css_error initial_pitch_range(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_pitch(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	css_fixed freq = 0;
	uint32_t unit = CSS_UNIT_HZ;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case PITCH_FREQUENCY:
			value = 0;

			freq = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(freq));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case PITCH_X_LOW:
		case PITCH_LOW:
		case PITCH_MEDIUM:
		case PITCH_HIGH:
		case PITCH_X_HIGH:
			/** \todo convert to public values */
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo pitch */
	}

	return CSS_OK;
}

css_error initial_pitch(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_play_during(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	lwc_string *uri = NULL;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case PLAY_DURING_URI:
			value = 0;

			uri = *((lwc_string **) style->bytecode);
			advance_bytecode(style, sizeof(uri));
			break;
		case PLAY_DURING_AUTO:
		case PLAY_DURING_NONE:
			/** \todo convert to public values */
			break;
		}

		/** \todo mix & repeat */
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo play-during */
	}

	return CSS_OK;
}

css_error initial_play_during(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_position(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_POSITION_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case POSITION_STATIC:
			value = CSS_POSITION_STATIC;
			break;
		case POSITION_RELATIVE:
			value = CSS_POSITION_RELATIVE;
			break;
		case POSITION_ABSOLUTE:
			value = CSS_POSITION_ABSOLUTE;
			break;
		case POSITION_FIXED:
			value = CSS_POSITION_FIXED;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_position(state->result, value);
	}

	return CSS_OK;
}

css_error initial_position(css_computed_style *style)
{
	return set_position(style, CSS_POSITION_STATIC);
}

css_error cascade_quotes(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_QUOTES_INHERIT;
	lwc_string **quotes = NULL;
	uint32_t n_quotes = 0;

	if (isInherit(opv) == false) {
		uint32_t v = getValue(opv);

		value = CSS_QUOTES_STRING;

		while (v != QUOTES_NONE) {
			lwc_string *quote;
			lwc_string **temp;

			quote = *((lwc_string **) style->bytecode);
			advance_bytecode(style, sizeof(quote));

			temp = state->result->alloc(quotes, 
					(n_quotes + 1) * sizeof(lwc_string *), 
					state->result->pw);
			if (temp == NULL) {
				if (quotes != NULL) {
					state->result->alloc(quotes, 0,
							state->result->pw);
				}
				return CSS_NOMEM;
			}

			quotes = temp;

			quotes[n_quotes] = quote;

			n_quotes++;

			v = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(v));
		}
	}

	/* Terminate array, if required */
	if (n_quotes > 0) {
		lwc_string **temp;

		temp = state->result->alloc(quotes, 
				(n_quotes + 1) * sizeof(lwc_string *), 
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(quotes, 0, state->result->pw);
			return CSS_NOMEM;
		}

		quotes = temp;

		quotes[n_quotes] = NULL;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_quotes(state->result, value, quotes);
	} else {
		if (quotes != NULL)
			state->result->alloc(quotes, 0, state->result->pw);
	}

	return CSS_OK;
}

css_error initial_quotes(css_computed_style *style)
{
	return set_quotes(style, CSS_QUOTES_DEFAULT, NULL);
}

css_error cascade_richness(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo richness */
	return cascade_number(opv, style, state, NULL);
}

css_error initial_richness(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_right(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_right);
}

css_error initial_right(css_computed_style *style)
{
	return set_right(style, CSS_RIGHT_AUTO, 0, CSS_UNIT_PX);
}

css_error compose_right(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_right(child, &length, &unit) == 
			CSS_RIGHT_INHERIT) {
		uint8_t p = css_computed_right(parent, &length, &unit);

		return set_right(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_speak_header(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case SPEAK_HEADER_ONCE:
		case SPEAK_HEADER_ALWAYS:
			/** \todo convert to public values */
			value = 0;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo speak-header */
	}

	return CSS_OK;
}

css_error initial_speak_header(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_speak_numeral(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case SPEAK_NUMERAL_DIGITS:
		case SPEAK_NUMERAL_CONTINUOUS:
			/** \todo convert to public values */
			value = 0;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo speak-numeral */
	}

	return CSS_OK;
}

css_error initial_speak_numeral(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_speak_punctuation( 
		uint32_t opv, css_style *style, css_select_state *state)
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case SPEAK_PUNCTUATION_CODE:
		case SPEAK_PUNCTUATION_NONE:
			/** \todo convert to public values */
			value = 0;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo speak-punctuation */
	}

	return CSS_OK;
}

css_error initial_speak_punctuation(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_speak(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case SPEAK_NORMAL:
		case SPEAK_NONE:
		case SPEAK_SPELL_OUT:
			/** \todo convert to public values */
			value = 0;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo speak */
	}

	return CSS_OK;
}

css_error initial_speak(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_speech_rate(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	css_fixed rate = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case SPEECH_RATE_SET:
			value = 0;

			rate = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(rate));
			break;
		case SPEECH_RATE_X_SLOW:
		case SPEECH_RATE_SLOW:
		case SPEECH_RATE_MEDIUM:
		case SPEECH_RATE_FAST:
		case SPEECH_RATE_X_FAST:
		case SPEECH_RATE_FASTER:
		case SPEECH_RATE_SLOWER:
			/** \todo convert to public values */
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo speech-rate */
	}

	return CSS_OK;
}

css_error initial_speech_rate(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_stress(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo stress */
	return cascade_number(opv, style, state, NULL);
}

css_error initial_stress(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_table_layout(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_TABLE_LAYOUT_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case TABLE_LAYOUT_AUTO:
			value = CSS_TABLE_LAYOUT_AUTO;
			break;
		case TABLE_LAYOUT_FIXED:
			value = CSS_TABLE_LAYOUT_FIXED;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_table_layout(state->result, value);
	}

	return CSS_OK;
}

css_error initial_table_layout(css_computed_style *style)
{
	return set_table_layout(style, CSS_TABLE_LAYOUT_AUTO);
}

css_error cascade_text_align(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_TEXT_ALIGN_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case TEXT_ALIGN_LEFT:
			value = CSS_TEXT_ALIGN_LEFT;
			break;
		case TEXT_ALIGN_RIGHT:
			value = CSS_TEXT_ALIGN_RIGHT;
			break;
		case TEXT_ALIGN_CENTER:
			value = CSS_TEXT_ALIGN_CENTER;
			break;
		case TEXT_ALIGN_JUSTIFY:
			value = CSS_TEXT_ALIGN_JUSTIFY;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_text_align(state->result, value);
	}

	return CSS_OK;
}

css_error initial_text_align(css_computed_style *style)
{
	return set_text_align(style, CSS_TEXT_ALIGN_DEFAULT);
}

css_error cascade_text_decoration(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_TEXT_DECORATION_INHERIT;
	
	UNUSED(style);

	if (isInherit(opv) == false) {
		if (getValue(opv) == TEXT_DECORATION_NONE) {
			value = CSS_TEXT_DECORATION_NONE;
		} else {
			assert(value == 0);

			if (getValue(opv) & TEXT_DECORATION_UNDERLINE)
				value |= CSS_TEXT_DECORATION_UNDERLINE;
			if (getValue(opv) & TEXT_DECORATION_OVERLINE)
				value |= CSS_TEXT_DECORATION_OVERLINE;
			if (getValue(opv) & TEXT_DECORATION_LINE_THROUGH)
				value |= CSS_TEXT_DECORATION_LINE_THROUGH;
			if (getValue(opv) & TEXT_DECORATION_BLINK)
				value |= CSS_TEXT_DECORATION_BLINK;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_text_decoration(state->result, value);
	}

	return CSS_OK;
}

css_error initial_text_decoration(css_computed_style *style)
{
	return set_text_decoration(style, CSS_TEXT_DECORATION_NONE);
}

css_error cascade_text_indent(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_text_indent);
}

css_error initial_text_indent(css_computed_style *style)
{
	return set_text_indent(style, CSS_TEXT_INDENT_SET, 0, CSS_UNIT_PX);
}

css_error cascade_text_transform(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_TEXT_TRANSFORM_INHERIT;
	
	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case TEXT_TRANSFORM_CAPITALIZE:
			value = CSS_TEXT_TRANSFORM_CAPITALIZE;
			break;
		case TEXT_TRANSFORM_UPPERCASE:
			value = CSS_TEXT_TRANSFORM_UPPERCASE;
			break;
		case TEXT_TRANSFORM_LOWERCASE:
			value = CSS_TEXT_TRANSFORM_LOWERCASE;
			break;
		case TEXT_TRANSFORM_NONE:
			value = CSS_TEXT_TRANSFORM_NONE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_text_transform(state->result, value);
	}

	return CSS_OK;
}

css_error initial_text_transform(css_computed_style *style)
{
	return set_text_transform(style, CSS_TEXT_TRANSFORM_NONE);
}

css_error cascade_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_top);
}

css_error initial_top(css_computed_style *style)
{
	return set_top(style, CSS_TOP_AUTO, 0, CSS_UNIT_PX);
}

css_error compose_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;

	if (css_computed_top(child, &length, &unit) == 
			CSS_TOP_INHERIT) {
		uint8_t p = css_computed_top(parent, &length, &unit);

		return set_top(result, p, length, unit);
	}

	return CSS_OK;
}

css_error cascade_unicode_bidi(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_UNICODE_BIDI_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case UNICODE_BIDI_NORMAL:
			value = CSS_UNICODE_BIDI_NORMAL;
			break;
		case UNICODE_BIDI_EMBED:
			value = CSS_UNICODE_BIDI_EMBED;
			break;
		case UNICODE_BIDI_BIDI_OVERRIDE:
			value = CSS_UNICODE_BIDI_BIDI_OVERRIDE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_unicode_bidi(state->result, value);
	}

	return CSS_OK;
}

css_error initial_unicode_bidi(css_computed_style *style)
{
	return set_unicode_bidi(style, CSS_UNICODE_BIDI_NORMAL);
}

css_error cascade_vertical_align(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_VERTICAL_ALIGN_INHERIT;
	css_fixed length = 0;
	uint32_t unit = CSS_UNIT_PX;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case VERTICAL_ALIGN_SET:
			value = CSS_VERTICAL_ALIGN_SET;

			length = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(length));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case VERTICAL_ALIGN_BASELINE:
			value = CSS_VERTICAL_ALIGN_BASELINE;
			break;
		case VERTICAL_ALIGN_SUB:
			value = CSS_VERTICAL_ALIGN_SUB;
			break;
		case VERTICAL_ALIGN_SUPER:
			value = CSS_VERTICAL_ALIGN_SUPER;
			break;
		case VERTICAL_ALIGN_TOP:
			value = CSS_VERTICAL_ALIGN_TOP;
			break;
		case VERTICAL_ALIGN_TEXT_TOP:
			value = CSS_VERTICAL_ALIGN_TEXT_TOP;
			break;
		case VERTICAL_ALIGN_MIDDLE:
			value = CSS_VERTICAL_ALIGN_MIDDLE;
			break;
		case VERTICAL_ALIGN_BOTTOM:
			value = CSS_VERTICAL_ALIGN_BOTTOM;
			break;
		case VERTICAL_ALIGN_TEXT_BOTTOM:
			value = CSS_VERTICAL_ALIGN_TEXT_BOTTOM;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_vertical_align(state->result, value, length, unit);
	}

	return CSS_OK;
}

css_error initial_vertical_align(css_computed_style *style)
{
	return set_vertical_align(style, CSS_VERTICAL_ALIGN_BASELINE,
			0, CSS_UNIT_PX);
}

css_error cascade_visibility(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_VISIBILITY_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case VISIBILITY_VISIBLE:
			value = CSS_VISIBILITY_VISIBLE;
			break;
		case VISIBILITY_HIDDEN:
			value = CSS_VISIBILITY_HIDDEN;
			break;
		case VISIBILITY_COLLAPSE:
			value = CSS_VISIBILITY_COLLAPSE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_visibility(state->result, value);
	}

	return CSS_OK;
}

css_error initial_visibility(css_computed_style *style)
{
	return set_visibility(style, CSS_VISIBILITY_VISIBLE);
}

css_error cascade_voice_family(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	lwc_string **voices = NULL;
	uint32_t n_voices = 0;

	if (isInherit(opv) == false) {
		uint32_t v = getValue(opv);

		while (v != VOICE_FAMILY_END) {
			lwc_string *voice = NULL;
			lwc_string **temp;

			switch (v) {
			case VOICE_FAMILY_STRING:
			case VOICE_FAMILY_IDENT_LIST:
				voice = *((lwc_string **) 
						style->bytecode);
				advance_bytecode(style, sizeof(voice));
				break;
			case VOICE_FAMILY_MALE:
				if (value == 0)
					value = 1;
				break;
			case VOICE_FAMILY_FEMALE:
				if (value == 0)
					value = 1;
				break;
			case VOICE_FAMILY_CHILD:
				if (value == 0)
					value = 1;
				break;
			}

			/* Only use family-names which occur before the first
			 * generic-family. Any values which occur after the
			 * first generic-family are ignored. */
			/** \todo Do this at bytecode generation time? */
			if (value == 0 && voice != NULL) {
				temp = state->result->alloc(voices, 
					(n_voices + 1) * sizeof(lwc_string *), 
					state->result->pw);
				if (temp == NULL) {
					if (voices != NULL) {
						state->result->alloc(voices, 0,
							state->result->pw);
					}
					return CSS_NOMEM;
				}

				voices = temp;

				voices[n_voices] = voice;

				n_voices++;
			}

			v = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(v));
		}
	}

	/* Terminate array with blank entry, if needed */
	if (n_voices > 0) {
		lwc_string **temp;

		temp = state->result->alloc(voices, 
				(n_voices + 1) * sizeof(lwc_string *), 
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(voices, 0, state->result->pw);
			return CSS_NOMEM;
		}

		voices = temp;

		voices[n_voices] = NULL;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo voice-family */
		if (n_voices > 0)
			state->result->alloc(voices, 0, state->result->pw);
	} else {
		if (n_voices > 0)
			state->result->alloc(voices, 0, state->result->pw);
	}

	return CSS_OK;
}

css_error initial_voice_family(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_volume(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	css_fixed val = 0;
	uint32_t unit = CSS_UNIT_PCT;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case VOLUME_NUMBER:
			value = 0;

			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			break;
		case VOLUME_DIMENSION:
			value = 0;

			val = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(val));
			unit = *((uint32_t *) style->bytecode);
			advance_bytecode(style, sizeof(unit));
			break;
		case VOLUME_SILENT:
		case VOLUME_X_SOFT:
		case VOLUME_SOFT:
		case VOLUME_MEDIUM:
		case VOLUME_LOUD:
		case VOLUME_X_LOUD:
			/** \todo convert to public values */
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		/** \todo volume */
	}

	return CSS_OK;
}

css_error initial_volume(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_white_space(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_WHITE_SPACE_INHERIT;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case WHITE_SPACE_NORMAL:
			value = CSS_WHITE_SPACE_NORMAL;
			break;
		case WHITE_SPACE_PRE:
			value = CSS_WHITE_SPACE_PRE;
			break;
		case WHITE_SPACE_NOWRAP:
			value = CSS_WHITE_SPACE_NOWRAP;
			break;
		case WHITE_SPACE_PRE_WRAP:
			value = CSS_WHITE_SPACE_PRE_WRAP;
			break;
		case WHITE_SPACE_PRE_LINE:
			value = CSS_WHITE_SPACE_PRE_LINE;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_white_space(state->result, value);
	}

	return CSS_OK;
}

css_error initial_white_space(css_computed_style *style)
{
	return set_white_space(style, CSS_WHITE_SPACE_NORMAL);
}

css_error cascade_widows(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo widows */
	return cascade_number(opv, style, state, NULL);
}

css_error initial_widows(css_computed_style *style)
{
	UNUSED(style);

	return CSS_OK;
}

css_error cascade_width(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_auto(opv, style, state, set_width);
}

css_error initial_width(css_computed_style *style)
{
	return set_width(style, CSS_WIDTH_AUTO, 0, CSS_UNIT_PX);
}

css_error cascade_word_spacing(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length_normal(opv, style, state, set_word_spacing);
}

css_error initial_word_spacing(css_computed_style *style)
{
	return set_word_spacing(style, CSS_WORD_SPACING_NORMAL, 0, CSS_UNIT_PX);
}

css_error cascade_z_index(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_Z_INDEX_INHERIT;
	css_fixed index = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case Z_INDEX_SET:
			value = CSS_Z_INDEX_SET;

			index = *((css_fixed *) style->bytecode);
			advance_bytecode(style, sizeof(index));
			break;
		case Z_INDEX_AUTO:
			value = CSS_Z_INDEX_AUTO;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return set_z_index(state->result, value, index);
	}

	return CSS_OK;
}

css_error initial_z_index(css_computed_style *style)
{
	return set_z_index(style, CSS_Z_INDEX_AUTO, 0);
}

/******************************************************************************
 * Utilities below here                                                       *
 ******************************************************************************/
css_error cascade_bg_border_color(uint32_t opv, css_style *style,
		css_select_state *state, 
		css_error (*fun)(css_computed_style *, uint8_t, css_colour))
{
	uint16_t value = CSS_BACKGROUND_COLOR_INHERIT;
	css_colour color = 0;

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
			color = *((css_colour *) style->bytecode);
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
				lwc_string *))
{
	uint16_t value = CSS_BACKGROUND_IMAGE_INHERIT;
	lwc_string *uri = NULL;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case BACKGROUND_IMAGE_NONE:
			value = CSS_BACKGROUND_IMAGE_NONE;
			break;
		case BACKGROUND_IMAGE_URI:
			value = CSS_BACKGROUND_IMAGE_IMAGE;
			uri = *((lwc_string **) style->bytecode);
			advance_bytecode(style, sizeof(uri));
			break;
		}
	}

	/** \todo lose fun != NULL once all properties have set routines */
	if (fun != NULL && outranks_existing(getOpcode(opv), 
			isImportant(opv), state)) {
		return fun(state->result, value, uri);
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

css_error cascade_page_break_after_before(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t))
{
	uint16_t value = 0;

	UNUSED(style);

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case PAGE_BREAK_AFTER_AUTO:
		case PAGE_BREAK_AFTER_ALWAYS:
		case PAGE_BREAK_AFTER_AVOID:
		case PAGE_BREAK_AFTER_LEFT:
		case PAGE_BREAK_AFTER_RIGHT:
			/** \todo convert to public values */
			break;
		}
	}

	/** \todo lose fun != NULL */
	if (fun != NULL && outranks_existing(getOpcode(opv), 
			isImportant(opv), state)) {
		return fun(state->result, value);
	}

	return CSS_OK;
}

css_error cascade_counter_increment_reset(uint32_t opv, css_style *style,
		css_select_state *state,
		css_error (*fun)(css_computed_style *, uint8_t,
				css_computed_counter *))
{
	uint16_t value = CSS_COUNTER_INCREMENT_INHERIT;
	css_computed_counter *counters = NULL;
	uint32_t n_counters = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case COUNTER_INCREMENT_NAMED:
		{
			uint32_t v = getValue(opv);

			while (v != COUNTER_INCREMENT_NONE) {
				css_computed_counter *temp;
				lwc_string *name;
				css_fixed val = 0;

				name = *((lwc_string **)
						style->bytecode);
				advance_bytecode(style, sizeof(name));

				val = *((css_fixed *) style->bytecode);
				advance_bytecode(style, sizeof(val));

				temp = state->result->alloc(counters,
						(n_counters + 1) * 
						sizeof(css_computed_counter),
						state->result->pw);
				if (temp == NULL) {
					if (counters != NULL) {
						state->result->alloc(counters, 
							0, state->result->pw);
					}
					return CSS_NOMEM;
				}

				counters = temp;

				counters[n_counters].name = name;
				counters[n_counters].value = val;

				n_counters++;

				v = *((uint32_t *) style->bytecode);
				advance_bytecode(style, sizeof(v));
			}
		}
			break;
		case COUNTER_INCREMENT_NONE:
			value = CSS_COUNTER_INCREMENT_NONE;
			break;
		}
	}

	/* If we have some counters, terminate the array with a blank entry */
	if (n_counters > 0) {
		css_computed_counter *temp;

		temp = state->result->alloc(counters, 
				(n_counters + 1) * sizeof(css_computed_counter),
				state->result->pw);
		if (temp == NULL) {
			state->result->alloc(counters, 0, state->result->pw);
			return CSS_NOMEM;
		}

		counters = temp;

		counters[n_counters].name = NULL;
		counters[n_counters].value = 0;
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state)) {
		return fun(state->result, value, counters);
	} else if (n_counters > 0) {
		state->result->alloc(counters, 0, state->result->pw);
	}

	return CSS_OK;
}

