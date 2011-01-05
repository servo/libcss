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

	if (outranks_existing(getOpcode(opv), isImportant(opv), state,
			isInherit(opv))) {
		return set_border_collapse(state->computed, value);
	}

	return CSS_OK;
}

css_error set_border_collapse_from_hint(const css_hint *hint, 
		css_computed_style *style)
{
	return set_border_collapse(style, hint->status);
}

css_error initial_border_collapse(css_select_state *state)
{
	return set_border_collapse(state->computed, CSS_BORDER_COLLAPSE_SEPARATE);
}

css_error compose_border_collapse(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_border_collapse(child);

	if (type == CSS_BORDER_COLLAPSE_INHERIT) {
		type = get_border_collapse(parent);
	}
	
	return set_border_collapse(result, type);
}

uint32_t destroy_border_collapse(void *bytecode)
{
	UNUSED(bytecode);
	
	return sizeof(uint32_t);
}
