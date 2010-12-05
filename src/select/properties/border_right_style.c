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

css_error cascade_border_right_style(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_border_style(opv, style, state, set_border_right_style);
}

css_error set_border_right_style_from_hint(const css_hint *hint, 
		css_computed_style *style)
{
	return set_border_right_style(style, hint->status);
}

css_error initial_border_right_style(css_select_state *state)
{
	return set_border_right_style(state->result, CSS_BORDER_STYLE_NONE);
}

css_error compose_border_right_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_border_right_style(child);

	if (type == CSS_BORDER_STYLE_INHERIT) {
		type = get_border_right_style(parent);
	}

	return set_border_right_style(result, type);
}

uint32_t destroy_border_right_style(void *bytecode)
{
	UNUSED(bytecode);
	
	return sizeof(uint32_t);
}
