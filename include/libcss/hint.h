/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_hint_h_
#define libcss_hint_h_

#include <libwapcaplet/libwapcaplet.h>

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/types.h>

/**
 * Length object for use in presentational hints
 */
typedef struct css_hint_length {
	css_fixed value;
	css_unit unit;
} css_hint_length;

/**
 * Presentational hints
 */
typedef struct css_hint {
	uint8_t status;

	css_colour color;
	int32_t integer;
	css_hint_length length1;
	css_hint_length length2;
	lwc_string *string;

	/** \todo Support clip/content/counter-{increment,reset}? */
} css_hint;

#endif
