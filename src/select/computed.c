/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include "select/computed.h"
#include "select/dispatch.h"
#include "select/propset.h"
#include "utils/utils.h"

static css_error compute_border_colors(css_computed_style *style);
static css_error compute_display(const css_computed_style *parent,
		css_computed_style *style);
static css_error compute_float(css_computed_style *style);

static css_error compute_absolute_border_width(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_border_side_width(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit));
static css_error compute_absolute_sides(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_clip(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_line_height(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_margins(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_padding(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_vertical_align(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size);
static css_error compute_absolute_length(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit));
static css_error compute_absolute_length_auto(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit));
static css_error compute_absolute_length_none(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit));
static css_error compute_absolute_length_normal(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit));
static css_error compute_absolute_length_pair(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len1, css_unit *unit1,
				css_fixed *len2, css_unit *unit2),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len1, css_unit unit1,
				css_fixed len2, css_unit unit2));


/**
 * Create a computed style
 *
 * \param alloc   Memory (de)allocation function
 * \param pw      Pointer to client-specific data
 * \param result  Pointer to location to receive result
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_BADPARM on bad parameters.
 */
css_error css_computed_style_create(css_allocator_fn alloc, void *pw,
		css_computed_style **result)
{
	css_computed_style *s;

	if (alloc == NULL || result == NULL)
		return CSS_BADPARM;

	s = alloc(NULL, sizeof(css_computed_style), pw);
	if (s == NULL)
		return CSS_NOMEM;

	memset(s, 0, sizeof(css_computed_style));

	s->alloc = alloc;
	s->pw = pw;

	*result = s;

	return CSS_OK;
}

/**
 * Destroy a computed style
 *
 * \param style  Style to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_computed_style_destroy(css_computed_style *style)
{
	if (style == NULL)
		return CSS_BADPARM;

	if (style->uncommon != NULL) {
		if (style->uncommon->counter_increment != NULL) {
			style->alloc(style->uncommon->counter_increment, 0,
					style->pw);
		}

		if (style->uncommon->counter_reset != NULL) {
			style->alloc(style->uncommon->counter_reset, 0,
					style->pw);
		}
	
		if (style->uncommon->cursor != NULL)
			style->alloc(style->uncommon->cursor, 0, style->pw);

		style->alloc(style->uncommon, 0, style->pw);
	}

	if (style->page != NULL) {
		style->alloc(style->page, 0, style->pw);
	}

	if (style->aural != NULL) {
		style->alloc(style->aural, 0, style->pw);
	}

	if (style->font_family != NULL)
		style->alloc(style->font_family, 0, style->pw);

	if (style->quotes != NULL)
		style->alloc(style->quotes, 0, style->pw);

	style->alloc(style, 0, style->pw);

	return CSS_OK;
}

/**
 * Compose two computed styles
 *
 * \param parent             Parent style
 * \param child              Child style
 * \param compute_font_size  Function to compute an absolute font size
 * \param pw                 Client data for compute_font_size
 * \param result             Pointer to style to compose into
 * \return CSS_OK on success, appropriate error otherwise.
 *
 * Precondition: Parent is a fully composed style (thus has no properties
 * marked as inherited)
 */
css_error css_computed_style_compose(const css_computed_style *parent,
		const css_computed_style *child,
		css_error (*compute_font_size)(void *pw,
				const css_hint *parent, css_hint *size),
		void *pw,
		css_computed_style *result)
{
	css_error error = CSS_OK;
	size_t i;

	/* Iterate through the properties */
	for (i = 0; i < CSS_N_PROPERTIES; i++) {
		/* Skip any in extension blocks if the block does not exist */	
		if (prop_dispatch[i].group == GROUP_UNCOMMON &&
				parent->uncommon == NULL && 
				child->uncommon == NULL)
			continue;

		if (prop_dispatch[i].group == GROUP_PAGE &&
				parent->page == NULL && child->page == NULL)
			continue;

		if (prop_dispatch[i].group == GROUP_AURAL &&
				parent->aural == NULL && child->aural == NULL)
			continue;

		/* Compose the property */
		error = prop_dispatch[i].compose(parent, child, result);
		if (error != CSS_OK)
			break;
	}

	/* Finally, compute absolute values for everything */
	return compute_absolute_values(parent, result, compute_font_size, pw);
}

/******************************************************************************
 * Library internals                                                          *
 ******************************************************************************/

/**
 * Compute the absolute values of a style
 *
 * \param parent             Parent style, or NULL for tree root
 * \param style              Computed style to process
 * \param compute_font_size  Callback to calculate an absolute font-size
 * \param pw                 Private word for callback
 * \return CSS_OK on success.
 */
css_error compute_absolute_values(const css_computed_style *parent,
		css_computed_style *style,
		css_error (*compute_font_size)(void *pw, 
			const css_hint *parent, css_hint *size),
		void *pw)
{
	css_hint psize, size, ex_size;
	css_error error;

	/* Ensure font-size is absolute */
	if (parent != NULL) {
		psize.status = css_computed_font_size(parent, 
				&psize.data.length.value, 
				&psize.data.length.unit);
	}

	size.status = css_computed_font_size(style, 
			&size.data.length.value, 
			&size.data.length.unit);

	error = compute_font_size(pw, parent != NULL ? &psize : NULL, &size);
	if (error != CSS_OK)
		return error;

	error = set_font_size(style, size.status, 
			size.data.length.value, size.data.length.unit);
	if (error != CSS_OK)
		return error;

	/* Compute the size of an ex unit */
	ex_size.status = CSS_FONT_SIZE_DIMENSION;
	ex_size.data.length.value = INTTOFIX(1);
	ex_size.data.length.unit = CSS_UNIT_EX;
	error = compute_font_size(pw, &size, &ex_size);
	if (error != CSS_OK)
		return error;

	/* Fix up background-position */
	error = compute_absolute_length_pair(style, &size.data.length,
			&ex_size.data.length, css_computed_background_position,
			set_background_position);
	if (error != CSS_OK)
		return error;

	/* Fix up border-{top,right,bottom,left}-color */
	error = compute_border_colors(style);
	if (error != CSS_OK)
		return error;

	/* Fix up border-{top,right,bottom,left}-width */
	error = compute_absolute_border_width(style, &size.data.length, 
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up sides */
	error = compute_absolute_sides(style, &size.data.length, 
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up display */
	error = compute_display(parent, style);
	if (error != CSS_OK)
		return error;

	/* Fix up float */
	error = compute_float(style);
	if (error != CSS_OK)
		return error;

	/* Fix up height */
	error = compute_absolute_length_auto(style, &size.data.length,
			&ex_size.data.length, css_computed_height, set_height);
	if (error != CSS_OK)
		return error;

	/* Fix up line-height (must be before vertical-align) */
	error = compute_absolute_line_height(style, &size.data.length,
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up margins */
	error = compute_absolute_margins(style, &size.data.length,
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up max-height */
	error = compute_absolute_length_none(style, &size.data.length,
			&ex_size.data.length, css_computed_max_height,
			set_max_height);
	if (error != CSS_OK)
		return error;

	/* Fix up max-width */
	error = compute_absolute_length_none(style, &size.data.length,
			&ex_size.data.length, css_computed_max_width,
			set_max_width);
	if (error != CSS_OK)
		return error;

	/* Fix up min-height */
	error = compute_absolute_length(style, &size.data.length,
			&ex_size.data.length, css_computed_min_height,
			set_min_height);
	if (error != CSS_OK)
		return error;

	/* Fix up min-width */
	error = compute_absolute_length(style, &size.data.length,
			&ex_size.data.length, css_computed_min_width,
			set_min_width);
	if (error != CSS_OK)
		return error;

	/* Fix up padding */
	error = compute_absolute_padding(style, &size.data.length,
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up text-indent */
	error = compute_absolute_length(style, &size.data.length,
			&ex_size.data.length, css_computed_text_indent,
			set_text_indent);
	if (error != CSS_OK)
		return error;

	/* Fix up vertical-align */
	error = compute_absolute_vertical_align(style, &size.data.length,
			&ex_size.data.length);
	if (error != CSS_OK)
		return error;

	/* Fix up width */
	error = compute_absolute_length_auto(style, &size.data.length,
			&ex_size.data.length, css_computed_width, set_width);
	if (error != CSS_OK)
		return error;

	/* Uncommon properties */
	if (style->uncommon != NULL) {
		/* Fix up border-spacing */
		error = compute_absolute_length_pair(style,
				&size.data.length, &ex_size.data.length,
				css_computed_border_spacing,
				set_border_spacing);
		if (error != CSS_OK)
			return error;

		/* Fix up clip */
		error = compute_absolute_clip(style, &size.data.length,
				&ex_size.data.length);
		if (error != CSS_OK)
			return error;

		/* Fix up letter-spacing */
		error = compute_absolute_length_normal(style,
				&size.data.length, &ex_size.data.length,
				css_computed_letter_spacing, 
				set_letter_spacing);
		if (error != CSS_OK)
			return error;

		/* Fix up outline-width */
		error = compute_absolute_border_side_width(style, 
				&size.data.length, &ex_size.data.length, 
				css_computed_outline_width, set_outline_width);
		if (error != CSS_OK)
			return error;

		/* Fix up word spacing */
		error = compute_absolute_length_normal(style,
				&size.data.length, &ex_size.data.length,
				css_computed_word_spacing, set_word_spacing);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/******************************************************************************
 * Absolute value calculators
 ******************************************************************************/

/**
 * Compute border colours, replacing any set to initial with 
 * the computed value of color.
 *
 * \param style  The style to process
 * \return CSS_OK on success
 */
css_error compute_border_colors(css_computed_style *style)
{
	css_color color, bcol;
	css_error error;

	css_computed_color(style, &color);

	if (css_computed_border_top_color(style, &bcol) == 
			CSS_BORDER_COLOR_INITIAL) {
		error = set_border_top_color(style, 
				CSS_BORDER_COLOR_COLOR, color);
		if (error != CSS_OK)
			return error;
	}

	if (css_computed_border_right_color(style, &bcol) == 
			CSS_BORDER_COLOR_INITIAL) {
		error = set_border_right_color(style, 
				CSS_BORDER_COLOR_COLOR, color);
		if (error != CSS_OK)
			return error;
	}

	if (css_computed_border_bottom_color(style, &bcol) == 
			CSS_BORDER_COLOR_INITIAL) {
		error = set_border_bottom_color(style, 
				CSS_BORDER_COLOR_COLOR, color);
		if (error != CSS_OK)
			return error;
	}

	if (css_computed_border_left_color(style, &bcol) == 
			CSS_BORDER_COLOR_INITIAL) {
		error = set_border_left_color(style, 
				CSS_BORDER_COLOR_COLOR, color);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/**
 * Compute display, considering position and float ($9.7)
 *
 * \param parent  Parent style
 * \param style   Style to process
 * \return CSS_OK on success
 */
css_error compute_display(const css_computed_style *parent,
		css_computed_style *style)
{
	uint8_t display;
	uint8_t pos;
	css_error error;

	display = css_computed_display(style);
	if (display == CSS_DISPLAY_NONE)
		return CSS_OK; /*1*/

	pos = css_computed_position(style);

	if ((pos == CSS_POSITION_ABSOLUTE || pos == CSS_POSITION_FIXED)/*2*/ ||
			css_computed_float(style) != CSS_FLOAT_NONE /*3*/ ||
			parent == NULL/*4*/) {
		if (display == CSS_DISPLAY_INLINE_TABLE) {
			display = CSS_DISPLAY_TABLE;
		} else if (display == CSS_DISPLAY_INLINE || 
				display == CSS_DISPLAY_RUN_IN ||
				display == CSS_DISPLAY_TABLE_ROW_GROUP ||
				display == CSS_DISPLAY_TABLE_COLUMN ||
				display == CSS_DISPLAY_TABLE_COLUMN_GROUP ||
				display == CSS_DISPLAY_TABLE_HEADER_GROUP ||
				display == CSS_DISPLAY_TABLE_FOOTER_GROUP ||
				display == CSS_DISPLAY_TABLE_ROW ||
				display == CSS_DISPLAY_TABLE_CELL ||
				display == CSS_DISPLAY_TABLE_CAPTION ||
				display == CSS_DISPLAY_INLINE_BLOCK) {
			display = CSS_DISPLAY_BLOCK;
		}

		error = set_display(style, display);
		if (error != CSS_OK)
			return error;
	}

	/*5*/

	return CSS_OK;
}

/**
 * Compute float, considering position ($9.7)
 *
 * \param style  Style to process
 * \return CSS_OK on success
 */
css_error compute_float(css_computed_style *style)
{
	uint8_t pos;
	css_error error;

	pos = css_computed_position(style);

	if (pos == CSS_POSITION_ABSOLUTE || pos == CSS_POSITION_FIXED) /*2*/ {
		error = set_float(style, CSS_FLOAT_NONE);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/**
 * Compute absolute border widths
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_border_width(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_error error;

	error = compute_absolute_border_side_width(style, font_size, ex_size,
			css_computed_border_top_width, 
			set_border_top_width);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_border_side_width(style, font_size, ex_size,
			css_computed_border_right_width, 
			set_border_right_width);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_border_side_width(style, font_size, ex_size,
			css_computed_border_bottom_width, 
			set_border_bottom_width);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_border_side_width(style, font_size, ex_size,
			css_computed_border_left_width, 
			set_border_left_width);
	if (error != CSS_OK)
		return error;

	return CSS_OK;
}

/**
 * Compute an absolute border side width
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_border_side_width(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit))
{
	css_fixed length;
	css_unit unit;
	uint8_t type;

	type = get(style, &length, &unit);
	if (type == CSS_BORDER_WIDTH_THIN) {
		length = INTTOFIX(1);
		unit = CSS_UNIT_PX;
	} else if (type == CSS_BORDER_WIDTH_MEDIUM) {
		length = INTTOFIX(2);
		unit = CSS_UNIT_PX;
	} else if (type == CSS_BORDER_WIDTH_THICK) {
		length = INTTOFIX(4);
		unit = CSS_UNIT_PX;
	}

	if (unit == CSS_UNIT_EM) {
		length = FMUL(length, font_size->value);
		unit = font_size->unit;
	} else if (unit == CSS_UNIT_EX) {
		length = FMUL(length, ex_size->value);
		unit = ex_size->unit;
	}

	return set(style, CSS_BORDER_WIDTH_WIDTH, length, unit);
}

/**
 * Compute absolute clip
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_clip(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_computed_clip_rect rect = { 0, 0, 0, 0, CSS_UNIT_PX, CSS_UNIT_PX,
			CSS_UNIT_PX, CSS_UNIT_PX, false, false, false, false };
	css_error error;

	if (css_computed_clip(style, &rect) == CSS_CLIP_RECT) {
		if (rect.top_auto == false) {
			if (rect.tunit == CSS_UNIT_EM) {
				rect.top = FMUL(rect.top, font_size->value);
				rect.tunit = font_size->unit;
			} else if (rect.tunit == CSS_UNIT_EX) {
				rect.top = FMUL(rect.top, ex_size->value);
				rect.tunit = ex_size->unit;
			}
		}

		if (rect.right_auto == false) {
			if (rect.runit == CSS_UNIT_EM) {
				rect.right = FMUL(rect.right, font_size->value);
				rect.runit = font_size->unit;
			} else if (rect.runit == CSS_UNIT_EX) {
				rect.right = FMUL(rect.right, ex_size->value);
				rect.runit = ex_size->unit;
			}
		}

		if (rect.bottom_auto == false) {
			if (rect.bunit == CSS_UNIT_EM) {
				rect.bottom = 
					FMUL(rect.bottom, font_size->value);
				rect.bunit = font_size->unit;
			} else if (rect.bunit == CSS_UNIT_EX) {
				rect.bottom = FMUL(rect.bottom, ex_size->value);
				rect.bunit = ex_size->unit;
			}
		}

		if (rect.left_auto == false) {
			if (rect.lunit == CSS_UNIT_EM) {
				rect.left = FMUL(rect.left, font_size->value);
				rect.lunit = font_size->unit;
			} else if (rect.lunit == CSS_UNIT_EX) {
				rect.left = FMUL(rect.left, ex_size->value);
				rect.lunit = ex_size->unit;
			}
		}

		error = set_clip(style, CSS_CLIP_RECT, &rect);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/**
 * Compute absolute line-height
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_line_height(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;
	uint8_t type;
	css_error error;

	type = css_computed_line_height(style, &length, &unit);

	if (type == CSS_LINE_HEIGHT_DIMENSION) {
		if (unit == CSS_UNIT_EM) {
			length = FMUL(length, font_size->value);
			unit = font_size->unit;
		} else if (unit == CSS_UNIT_EX) {
			length = FMUL(length, ex_size->value);
			unit = ex_size->unit;
		} else if (unit == CSS_UNIT_PCT) {
			length = FDIV(FMUL(length, font_size->value), 
					INTTOFIX(100));
			unit = font_size->unit;
		}

		error = set_line_height(style, type, length, unit);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/**
 * Compute the absolute values of {top,right,bottom,left}
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_sides(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	enum css_position pos;
	css_error error;

	/* Calculate absolute lengths for sides */
	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_top, set_top);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_right, set_right);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_bottom, set_bottom);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_left, set_left);
	if (error != CSS_OK)
		return error;

	/* Now, complete computation based upon computed position */
	pos = css_computed_position(style);
	if (pos == CSS_POSITION_STATIC) {
		/* Static => everything is auto */
		error = set_top(style, CSS_TOP_AUTO, 0, CSS_UNIT_PX);
		if (error != CSS_OK)
			return error;

		error = set_right(style, CSS_RIGHT_AUTO, 0, CSS_UNIT_PX);
		if (error != CSS_OK)
			return error;

		error = set_bottom(style, CSS_BOTTOM_AUTO, 0, CSS_UNIT_PX);
		if (error != CSS_OK)
			return error;

		error = set_left(style, CSS_LEFT_AUTO, 0, CSS_UNIT_PX);
		if (error != CSS_OK)
			return error;
	} else if (pos == CSS_POSITION_RELATIVE) {
		/* Relative => follow $9.4.3 */
		css_fixed len1 = 0, len2 = 0;
		css_unit unit1 = CSS_UNIT_PX, unit2 = CSS_UNIT_PX;
		uint8_t type1, type2;

		/* First, left and right */
		type1 = css_computed_left(style, &len1, &unit1);
		type2 = css_computed_right(style, &len2, &unit2);

		if (type1 == CSS_LEFT_AUTO && type2 == CSS_RIGHT_AUTO) {
			/* Both auto => 0px */
			error = set_left(style, CSS_LEFT_SET, 0, CSS_UNIT_PX);
			if (error != CSS_OK)
				return error;

			error = set_right(style, CSS_RIGHT_SET, 0, CSS_UNIT_PX);
			if (error != CSS_OK)
				return error;
		} else if (type1 == CSS_LEFT_AUTO) {
			/* Left is auto => set to -right */
			error = set_left(style, CSS_LEFT_SET, -len2, unit2);
			if (error != CSS_OK)
				return error;
		} else if (type2 == CSS_RIGHT_AUTO) {
			/* Right is auto => set to -left */
			error = set_right(style, CSS_RIGHT_SET, -len1, unit1);
			if (error != CSS_OK)
				return error;
		} else {
			/* Overconstrained => consult direction */
			if (css_computed_direction(style) == 
					CSS_DIRECTION_LTR) {
				/* Right = -left */
				error = set_right(style, CSS_RIGHT_SET, 
						-len1, unit1);
				if (error != CSS_OK)
					return error;
			} else {
				/* Left = -right */
				error = set_left(style, CSS_LEFT_SET,
						-len2, unit2);
				if (error != CSS_OK)
					return error;
			}
		}

		/* Second, top and bottom */
		type1 = css_computed_top(style, &len1, &unit1);
		type2 = css_computed_bottom(style, &len2, &unit2);

		if (type1 == CSS_TOP_AUTO && type2 == CSS_BOTTOM_AUTO) {
			/* Both auto => 0px */
			error = set_top(style, CSS_TOP_SET, 0, CSS_UNIT_PX);
			if (error != CSS_OK)
				return error;

			error = set_bottom(style, CSS_BOTTOM_SET, 
					0, CSS_UNIT_PX);
			if (error != CSS_OK)
				return error;
		} else if (type1 == CSS_TOP_AUTO) {
			/* Top is auto => set to -bottom */
			error = set_top(style, CSS_TOP_SET, -len2, unit2);
			if (error != CSS_OK)
				return error;
		} else if (type2 == CSS_BOTTOM_AUTO) {
			/* Bottom is auto => set to -top */
			error = set_bottom(style, CSS_BOTTOM_SET, -len1, unit1);
			if (error != CSS_OK)
				return error;
		} else {
			/* Overconstrained => bottom = -top */
			error = set_bottom(style, CSS_BOTTOM_SET, -len1, unit1);
			if (error != CSS_OK)
				return error;
		}
	}

	return CSS_OK;
}

/**
 * Compute absolute margins
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_margins(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_error error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_margin_top, set_margin_top);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_margin_right, set_margin_right);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_margin_bottom, set_margin_bottom);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length_auto(style, font_size, ex_size,
			css_computed_margin_left, set_margin_left);
	if (error != CSS_OK)
		return error;
	
	return CSS_OK;
}

/**
 * Compute absolute padding
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_padding(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_error error;

	error = compute_absolute_length(style, font_size, ex_size,
			css_computed_padding_top, set_padding_top);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length(style, font_size, ex_size,
			css_computed_padding_right, set_padding_right);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length(style, font_size, ex_size,
			css_computed_padding_bottom, set_padding_bottom);
	if (error != CSS_OK)
		return error;

	error = compute_absolute_length(style, font_size, ex_size,
			css_computed_padding_left, set_padding_left);
	if (error != CSS_OK)
		return error;
	
	return CSS_OK;
}

/**
 * Compute absolute vertical-align
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \return CSS_OK on success
 */
css_error compute_absolute_vertical_align(css_computed_style *style,
		const css_hint_length *font_size,
		const css_hint_length *ex_size)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;
	uint8_t type;
	css_error error;

	type = css_computed_vertical_align(style, &length, &unit);

	if (type == CSS_VERTICAL_ALIGN_SET) {
		if (unit == CSS_UNIT_EM) {
			length = FMUL(length, font_size->value);
			unit = font_size->unit;
		} else if (unit == CSS_UNIT_EX) {
			length = FMUL(length, ex_size->value);
			unit = ex_size->unit;
		} else if (unit == CSS_UNIT_PCT) {
			css_fixed line_height = 0;
			css_unit lhunit = CSS_UNIT_PX;

			/* Relative to line-height */
			css_computed_line_height(style, &line_height, &lhunit);

			length = FDIV(FMUL(length, line_height), INTTOFIX(100));
			unit = lhunit;
		}

		error = set_vertical_align(style, type, length, unit);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

/**
 * Compute the absolute value of length
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_length(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit))
{
	css_fixed length;
	css_unit unit;
	uint8_t type;

	type = get(style, &length, &unit);

	if (unit == CSS_UNIT_EM) {
		length = FMUL(length, font_size->value);
		unit = font_size->unit;
	} else if (unit == CSS_UNIT_EX) {
		length = FMUL(length, ex_size->value);
		unit = ex_size->unit;
	}

	return set(style, type, length, unit);
}

/**
 * Compute the absolute value of length or auto
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_length_auto(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit))
{
	css_fixed length;
	css_unit unit;
	uint8_t type;

	type = get(style, &length, &unit);
	if (type != CSS_BOTTOM_AUTO) {
		if (unit == CSS_UNIT_EM) {
			length = FMUL(length, font_size->value);
			unit = font_size->unit;
		} else if (unit == CSS_UNIT_EX) {
			length = FMUL(length, ex_size->value);
			unit = ex_size->unit;
		}

		return set(style, CSS_BOTTOM_SET, length, unit);
	}

	return set(style, CSS_BOTTOM_AUTO, 0, CSS_UNIT_PX);
}

/**
 * Compute the absolute value of length or none
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_length_none(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit))
{
	css_fixed length;
	css_unit unit;
	uint8_t type;

	type = get(style, &length, &unit);
	if (type != CSS_MAX_HEIGHT_NONE) {
		if (unit == CSS_UNIT_EM) {
			length = FMUL(length, font_size->value);
			unit = font_size->unit;
		} else if (unit == CSS_UNIT_EX) {
			length = FMUL(length, ex_size->value);
			unit = ex_size->unit;
		}

		return set(style, CSS_MAX_HEIGHT_SET, length, unit);
	}

	return set(style, CSS_MAX_HEIGHT_NONE, 0, CSS_UNIT_PX);
}

/**
 * Compute the absolute value of length or normal
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_length_normal(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len, css_unit *unit),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len, css_unit unit))
{
	css_fixed length;
	css_unit unit;
	uint8_t type;

	type = get(style, &length, &unit);
	if (type != CSS_LETTER_SPACING_NORMAL) {
		if (unit == CSS_UNIT_EM) {
			length = FMUL(length, font_size->value);
			unit = font_size->unit;
		} else if (unit == CSS_UNIT_EX) {
			length = FMUL(length, ex_size->value);
			unit = ex_size->unit;
		}

		return set(style, CSS_LETTER_SPACING_SET, length, unit);
	}

	return set(style, CSS_LETTER_SPACING_NORMAL, 0, CSS_UNIT_PX);
}

/**
 * Compute the absolute value of length pair
 *
 * \param style      Style to process
 * \param font_size  Absolute font size
 * \param ex_size    Absolute ex size
 * \param get        Function to read length
 * \param set        Function to write length
 * \return CSS_OK on success
 */
css_error compute_absolute_length_pair(css_computed_style *style,
		const css_hint_length *font_size, 
		const css_hint_length *ex_size,
		uint8_t (*get)(const css_computed_style *style, 
				css_fixed *len1, css_unit *unit1,
				css_fixed *len2, css_unit *unit2),
		css_error (*set)(css_computed_style *style, uint8_t type,
				css_fixed len1, css_unit unit1,
				css_fixed len2, css_unit unit2))
{
	css_fixed length1, length2;
	css_unit unit1, unit2;
	uint8_t type;

	type = get(style, &length1, &unit1, &length2, &unit2);

	if (unit1 == CSS_UNIT_EM) {
		length1 = FMUL(length1, font_size->value);
		unit1 = font_size->unit;
	} else if (unit1 == CSS_UNIT_EX) {
		length1 = FMUL(length1, ex_size->value);
		unit1 = ex_size->unit;
	}

	if (unit2 == CSS_UNIT_EM) {
		length2 = FMUL(length2, font_size->value);
		unit2 = font_size->unit;
	} else if (unit2 == CSS_UNIT_EX) {
		length2 = FMUL(length2, ex_size->value);
		unit2 = ex_size->unit;
	}

	return set(style, type, length1, unit1, length2, unit2);
}

