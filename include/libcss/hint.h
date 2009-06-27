/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_hint_h_
#define libcss_hint_h_

#include <libwapcaplet/libwapcaplet.h>

#include <libcss/computed.h>
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
	/* Objects pointed to by fields in this struct are copied for
	 * use in the computed style. Note that the copy occurs using
	 * memcpy, so if the object contains pointers itself, then the
	 * data pointed to by those pointers is *not* copied.
	 */

	/* Computed styles are transient objects which provide a window onto 
	 * the internal styling and do not reference the strings they point to.
	 * Thus, any strings passed in by the client through this struct will 
	 * not be referenced either. Therefore, such strings must be guaranteed
	 * to persist for the life of the document.
	 */
	union {
		css_computed_clip_rect *clip;
		css_color color;
		css_computed_content_item *content;
		css_computed_counter *counter;
		int32_t integer;
		css_hint_length length;
		struct {
			css_hint_length h;
			css_hint_length v;
		} position;
		lwc_string *string;
		lwc_string **strings;
	} data;

	uint8_t status;
} css_hint;

#endif
