/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_dispatch_h_
#define css_select_dispatch_h_

#include <stdint.h>

#include <libcss/errors.h>
#include <libcss/computed.h>

#include "stylesheet.h"
#include "bytecode/bytecode.h"
#include "select/select.h"

/**
 * Enumeration of property groups
 */
enum prop_group {
	GROUP_NORMAL	= 0x0,
	GROUP_UNCOMMON	= 0x1,
	GROUP_PAGE	= 0x2,
	GROUP_AURAL	= 0x3
};

extern struct prop_table {
	css_error (*cascade)(uint32_t opv, css_style *style, 
			css_select_state *state);
	css_error (*initial)(css_computed_style *style);
	css_error (*compose)(const css_computed_style *parent,
			const css_computed_style *child,
			css_computed_style *result);

	uint32_t inherited : 1,
	         group : 2;
} prop_dispatch[N_OPCODES];

#endif
