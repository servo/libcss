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
 * Typed colour object for use in presentational hints
 */
typedef struct css_hint_typed_colour {
	uint8_t type;
	css_colour color;
} css_hint_typed_colour;

/**
 * Typed length object for use in presentational hints
 */
typedef struct css_hint_typed_length {
	uint8_t type;
	css_hint_length length;
} css_hint_typed_length;

/**
 * Typed string object for use in presentational hints
 */
typedef struct css_hint_typed_string {
	uint8_t type;
	lwc_string *string;
} css_hint_typed_string;

/**
 * Presentational hint border
 */
typedef struct css_hint_border {
	css_hint_typed_length width;
	css_hint_typed_colour color;
	uint8_t style;
} css_hint_border;

/**
 * Presentational hints
 *
 * \todo There's no way to flag that a property isn't set here.
 */
typedef struct css_hint {
	css_hint_typed_colour background_color;

	css_hint_typed_string background_image;

	uint8_t border_collapse;

	css_hint_border border_bottom;
	css_hint_border border_left;
	css_hint_border border_right;
	css_hint_border border_top;

	struct {
		uint8_t type;
		css_hint_length h;
		css_hint_length v;
	} border_spacing;

	uint8_t caption_side;

	uint8_t clear;

	css_hint_typed_colour color;

	uint8_t _float;

	css_hint_typed_string font_family;

	css_hint_typed_length font_size;

	css_hint_typed_length height;

	uint8_t list_style_type;

	css_hint_typed_length margin_bottom;
	css_hint_typed_length margin_left;
	css_hint_typed_length margin_right;
	css_hint_typed_length margin_top;

	uint8_t overflow;

	css_hint_typed_length padding_bottom;
	css_hint_typed_length padding_left;
	css_hint_typed_length padding_right;
	css_hint_typed_length padding_top;

	uint8_t text_align;

	css_hint_typed_length vertical_align;

	uint8_t white_space;

	css_hint_typed_length width;
} css_hint;

#endif
