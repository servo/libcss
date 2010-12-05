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

css_error cascade_pitch_range(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	/** \todo pitch-range */
	return cascade_number(opv, style, state, NULL);
}

css_error set_pitch_range_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	UNUSED(hint);
	UNUSED(style);

	return CSS_OK;
}

css_error initial_pitch_range(css_select_state *state)
{
	UNUSED(state);

	return CSS_OK;
}

css_error compose_pitch_range(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	UNUSED(parent);
	UNUSED(child);
	UNUSED(result);

	return CSS_OK;
}

uint32_t destroy_pitch_range(void *bytecode)
{
	return generic_destroy_number(bytecode);
}
