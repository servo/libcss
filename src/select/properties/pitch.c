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

css_error cascade_pitch(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	uint16_t value = 0;
	css_fixed freq = 0;
	uint32_t unit = UNIT_HZ;

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

	unit = to_css_unit(unit);

	if (outranks_existing(getOpcode(opv), isImportant(opv), state,
			isInherit(opv))) {
		/** \todo pitch */
	}

	return CSS_OK;
}

css_error set_pitch_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	UNUSED(hint);
	UNUSED(style);

	return CSS_OK;
}

css_error initial_pitch(css_select_state *state)
{
	UNUSED(state);

	return CSS_OK;
}

css_error compose_pitch(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

uint32_t destroy_pitch(void *bytecode)
{
	return generic_destroy_length(bytecode);
}
