/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "select/propset.h"
#include "select/propget.h"
#include "utils/utils.h"

#include "select/properties/properties.h"
#include "select/properties/helpers.h"

css_error cascade_border_top_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_bg_border_color(opv, style, state, set_border_top_color);
}

css_error set_border_top_color_from_hint(const css_hint *hint, 
		css_computed_style *style)
{
	return set_border_top_color(style, hint->status, hint->data.color);
}

css_error initial_border_top_color(css_select_state *state)
{
	return set_border_top_color(state->result, CSS_BORDER_COLOR_INITIAL, 0);
}

css_error compose_border_top_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_color color;
	uint8_t type = get_border_top_color(child, &color);

	if (type == CSS_BORDER_COLOR_INHERIT) {
		type = get_border_top_color(parent, &color);
	}

	return set_border_top_color(result, type, color);
}

uint32_t destroy_border_top_color(void *bytecode)
{
	return generic_destroy_color(bytecode);
}
