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

	if (outranks_existing(getOpcode(opv), isImportant(opv), state,
			isInherit(opv))) {
		return set_overflow(state->result, value);
	}

	return CSS_OK;
}

css_error set_overflow_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_overflow(style, hint->status);
}

css_error initial_overflow(css_select_state *state)
{
	return set_overflow(state->result, CSS_OVERFLOW_VISIBLE);
}

css_error compose_overflow(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	uint8_t type = get_overflow(child);

	if (type == CSS_OVERFLOW_INHERIT) {
		type = get_overflow(parent);
	}

	return set_overflow(result, type);
}

uint32_t destroy_overflow(void *bytecode)
{
	UNUSED(bytecode);
	
	return sizeof(uint32_t);
}
