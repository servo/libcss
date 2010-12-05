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

	if (outranks_existing(getOpcode(opv), isImportant(opv), state,
			isInherit(opv))) {
		return set_table_layout(state->result, value);
	}

	return CSS_OK;
}

css_error set_table_layout_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_table_layout(style, hint->status);
}

css_error initial_table_layout(css_select_state *state)
{
	return set_table_layout(state->result, CSS_TABLE_LAYOUT_AUTO);
}

css_error compose_table_layout(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_table_layout(child);

	if (type == CSS_TABLE_LAYOUT_INHERIT) {
		type = get_table_layout(parent);
	}

	return set_table_layout(result, type);
}

uint32_t destroy_table_layout(void *bytecode)
{
	UNUSED(bytecode);
	
	return sizeof(uint32_t);
}
