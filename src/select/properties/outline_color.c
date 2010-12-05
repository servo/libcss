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

css_error cascade_outline_color(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = CSS_OUTLINE_COLOR_INHERIT;
	css_color color = 0;

	if (isInherit(opv) == false) {
		switch (getValue(opv)) {
		case OUTLINE_COLOR_SET:
			value = CSS_OUTLINE_COLOR_COLOR;
			color = *((css_color *) style->bytecode);
			advance_bytecode(style, sizeof(color));
			break;
		case OUTLINE_COLOR_INVERT:
			value = CSS_OUTLINE_COLOR_INVERT;
			break;
		}
	}

	if (outranks_existing(getOpcode(opv), isImportant(opv), state,
			isInherit(opv))) {
		return set_outline_color(state->result, value, color);
	}

	return CSS_OK;
}

css_error set_outline_color_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_outline_color(style, hint->status, hint->data.color);
}

css_error initial_outline_color(css_select_state *state)
{
	return set_outline_color(state->result, CSS_OUTLINE_COLOR_INVERT, 0);
}

css_error compose_outline_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_color color = 0;
	uint8_t type = get_outline_color(child, &color);

	if ((child->uncommon == NULL && parent->uncommon != NULL) ||
			type == CSS_OUTLINE_COLOR_INHERIT ||
			(child->uncommon != NULL && result != child)) {
		if ((child->uncommon == NULL && parent->uncommon != NULL) ||
				type == CSS_OUTLINE_COLOR_INHERIT) {
			type = get_outline_color(parent, &color);
		}

		return set_outline_color(result, type, color);
	}

	return CSS_OK;
}

uint32_t destroy_outline_color(void *bytecode)
{
	return generic_destroy_color(bytecode);
}
