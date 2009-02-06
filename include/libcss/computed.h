/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_computed_h_
#define libcss_computed_h_

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/properties.h>
#include <libcss/types.h>

typedef struct css_computed_aural {
	/** \todo */
	uint8_t dummy;
} css_computed_aural;

typedef struct css_computed_page {
	/** \todo */
	uint8_t dummy;
} css_computed_page;

typedef struct css_computed_counter {
	const css_string *name;
	int32_t value;
} css_computed_counter;

typedef struct css_computed_uncommon {
/*
 * border_spacing		  1 + 2(4)	  2(4)
 * clip				  2 + 4(4)	  4(4)
 * letter_spacing		  2 + 4		  4
 * outline_color		  2		  4
 * outline_width		  3 + 4		  4
 * word_spacing			  2 + 4		  4
 * 				---		---
 * 				 48 bits	 40 bytes
 *
 * Encode counter_increment and _reset as a NULL-terminated array of
 * name, value pairs.
 *
 * counter_increment		  1		  sizeof(ptr)
 * counter_reset		  1		  sizeof(ptr)
 * 				---		---
 * 				  2 bits	  2sizeof(ptr) bytes
 *
 * Encode quotes as a NULL-terminated array of string pointers
 *
 * quotes			  1		  sizeof(ptr)
 * 				---		---
 * 				  1 bit		  sizeof(ptr) bytes
 *
 * Encode cursor uri(s) as a NULL-terminated array of string pointers
 *
 * cursor			  5		   sizeof(ptr)
 * 				---		---
 * 				  5 bits	   sizeof(ptr) bytes
 *
 * content			?
 *
 * 				___		___
 * 				 56 bits	 40 + 4sizeof(ptr) bytes
 *
 * 				  7 bytes	 40 + 4sizeof(ptr) bytes
 * 				===================
 * 				 47 + 4sizeof(ptr) bytes
 *
 * Bit allocations:
 *
 *    76543210
 *  1 llllllcc	letter-spacing | outline-color
 *  2 ooooooob	outline-width  | border-spacing
 *  3 bbbbbbbb	border-spacing
 *  4 wwwwwwir	word-spacing   | counter-increment | counter-reset
 *  5 uuuuuqcc	cursor         | quotes            | clip
 *  6 cccccccc	clip
 *  7 cccccccc	clip
 */
	uint8_t bits[7];

	uint8_t unused[1];

	css_fixed border_spacing[2];

	css_fixed clip[4];

	css_fixed letter_spacing;

	css_color outline_color;
	css_fixed outline_width;

	css_fixed word_spacing;

	css_computed_counter **counter_increment;
	css_computed_counter **counter_reset;

	const css_string **quotes;

	const css_string *cursor;
} css_computed_uncommon;

struct css_computed_style {
/*
 * background_attachment	  2
 * background_repeat		  3
 * border_collapse		  2
 * border_top_style		  4
 * border_right_style		  4
 * border_bottom_style		  4
 * border_left_style		  4
 * caption_side			  2
 * clear			  3
 * direction			  2
 * display			  5
 * empty_cells			  2
 * float			  2
 * font_style			  2
 * font_variant			  2
 * font_weight			  4
 * list_style_position		  2
 * list_style_type		  4
 * overflow			  3
 * outline_style		  4
 * position			  3
 * table_layout			  2
 * text_align			  3
 * text_decoration		  5
 * text_transform		  3
 * unicode_bidi			  2
 * visibility			  2
 * white_space			  3
 *				---
 *				 83 bits
 *
 * Colours are 32bits of RRGGBBAA
 * Dimensions are encoded as a fixed point value + 4 bits of unit data
 *
 * background_color		  2		  4
 * background_image		  1		  sizeof(ptr)
 * background_position		  1 + 2(4)	  2(4)
 * border_top_color		  2		  4
 * border_right_color		  2		  4
 * border_bottom_color		  2		  4
 * border_left_color		  2		  4
 * border_top_width		  3 + 4		  4
 * border_right_width		  3 + 4		  4
 * border_bottom_width		  3 + 4		  4
 * border_left_width		  3 + 4		  4
 * top				  2 + 4		  4
 * right			  2 + 4		  4
 * bottom			  2 + 4		  4
 * left				  2 + 4		  4
 * color			  1		  4
 * font_size			  4 + 4		  4
 * height			  2 + 4		  4
 * line_height			  2 + 4		  4
 * list_style_image		  1		  sizeof(ptr)
 * margin_top			  2 + 4		  4
 * margin_right			  2 + 4		  4
 * margin_bottom		  2 + 4		  4
 * margin_left			  2 + 4		  4
 * max_height			  2 + 4		  4
 * max_width			  2 + 4		  4
 * min_height			  1 + 4		  4
 * min_width			  1 + 4		  4
 * padding_top			  1 + 4		  4
 * padding_right		  1 + 4		  4
 * padding_bottom		  1 + 4		  4
 * padding_left			  1 + 4		  4
 * text_indent			  1 + 4		  4
 * vertical_align		  4 + 4		  4
 * width			  2 + 4		  4
 * z_index			  2		  4
 * 				---		---
 *				181 bits	140 + 2sizeof(ptr) bytes
 *
 * Encode font family as a NULL-terminated array of string pointers
 *
 * font_family			  1		  sizeof(ptr)
 * 				---		---
 * 				  1 bit		  sizeof(ptr)
 *
 * 				___		___
 *				265 bits	140 + 3sizeof(ptr) bytes
 *
 *				 34 bytes	140 + 3sizeof(ptr) bytes
 *				===================
 *				174 + 3sizeof(ptr) bytes
 *
 * Bit allocations:
 *
 *    76543210
 *  1 vvvvvvvv	vertical-align
 *  2 ffffffff	font-size
 *  3 ttttttti	border-top-width    | background-image
 *  4 rrrrrrrc	border-right-width  | color
 *  5 bbbbbbbl	border-bottom-width | list-style-image
 *  6 lllllllf	border-left-width   | font-family
 *  7 ttttttcc	top                 | border-top-color
 *  8 rrrrrrcc	right               | border-right-color
 *  9 bbbbbbcc	bottom              | border-bottom-color
 * 10 llllllcc	left                | border-left-color
 * 11 hhhhhhbb	height              | background-color
 * 12 llllllzz	line-height         | z-index
 * 13 ttttttbb	margin-top          | background-attachment
 * 14 rrrrrrbb	margin-right        | border-collapse
 * 15 bbbbbbcc	margin-bottom       | caption-side
 * 16 lllllldd	margin-left         | direction
 * 17 mmmmmmee	max-height          | empty-cells
 * 18 mmmmmmff	max-width           | float
 * 19 wwwwwwff	width               | font-style
 * 20 mmmmmbbb	min-height          | background-repeat
 * 21 mmmmmccc	min-width           | clear
 * 22 tttttooo	padding-top         | overflow
 * 23 rrrrrppp	padding-right       | position
 * 24 bbbbbttt	padding-bottom      | text-align
 * 25 lllllttt	padding-left        | text-transform
 * 26 tttttwww	text-indent         | white-space
 * 27 bbbbbbbb	background-position
 * 28 bdddddff	background-position | display               | font-variant
 * 29 tttttll.	text-decoration     | list-style-position   | <unused>
 * 30 ttttrrrr	border-top-style    | border-right-style
 * 31 bbbbllll	border-bottom-style | border-left-style
 * 32 ffffllll	font-weight         | list-style-type
 * 33 oooottuu	outline-style       | table-layout          | unicode-bidi
 * 34 vv......	visibility          | <unused>
 */
	uint8_t bits[34];

	uint8_t unused[2];

	css_color background_color;
	const css_string *background_image;
	css_fixed background_position[2];

	css_color border_color[4];
	css_fixed border_width[4];

	css_fixed top;
	css_fixed right;
	css_fixed bottom;
	css_fixed left;

	css_color color;

	css_fixed font_size;

	css_fixed height;

	css_fixed line_height;

	const css_string *list_style_image;

	css_fixed margin[4];

	css_fixed max_height;
	css_fixed max_width;

	css_fixed min_height;
	css_fixed min_width;

	css_fixed padding[4];

	css_fixed text_indent;

	css_fixed vertical_align;

	css_fixed width;

	int32_t z_index;

	const css_string **font_family;

	css_computed_uncommon *uncommon;/**< Uncommon properties */
	css_computed_aural *aural;	/**< Aural properties */
	css_computed_page *page;	/**< Page properties */
};

css_error css_computed_style_create(css_alloc alloc, void *pw,
		css_computed_style **result);
css_error css_computed_style_destroy(css_computed_style *style);

css_error css_computed_style_compose(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);

/******************************************************************************
 * Property accessors below here                                              *
 ******************************************************************************/

#define LETTER_SPACING_INDEX 0
#define LETTER_SPACING_SHIFT 2
#define LETTER_SPACING_MASK  0xfc
static inline uint8_t css_computed_letter_spacing(
		const css_computed_style *style, 
		css_fixed *length, css_unit *unit)
{
	if (style->uncommon != NULL) {
		uint8_t bits = style->uncommon->bits[LETTER_SPACING_INDEX];
		bits &= LETTER_SPACING_MASK;
		bits >>= LETTER_SPACING_SHIFT;

		/* 6bits: uuuutt : unit | type */

		if ((bits & 3) == CSS_LETTER_SPACING_SET) {
			*length = style->uncommon->letter_spacing;
			*unit = bits >> 2;
			return CSS_LETTER_SPACING_SET;
		}
	}

	return CSS_LETTER_SPACING_NORMAL;
}
#undef LETTER_SPACING_MASK
#undef LETTER_SPACING_SHIFT
#undef LETTER_SPACING_INDEX

#define OUTLINE_COLOR_INDEX 0
#define OUTLINE_COLOR_SHIFT 0
#define OUTLINE_COLOR_MASK  0x3
static inline uint8_t css_computed_outline_color(
		const css_computed_style *style, css_color *color)
{
	if (style->uncommon != NULL) {
		uint8_t bits = style->uncommon->bits[OUTLINE_COLOR_INDEX];
		bits &= OUTLINE_COLOR_MASK;
		bits >>= OUTLINE_COLOR_SHIFT;

		/* 2bits: tt : type */

		if ((bits & 3) == CSS_OUTLINE_COLOR_COLOR) {
			*color = style->uncommon->outline_color;
			return CSS_OUTLINE_COLOR_COLOR;
		}
	}

	return CSS_OUTLINE_COLOR_INVERT;
}
#undef OUTLINE_COLOR_MASK
#undef OUTLINE_COLOR_SHIFT
#undef OUTLINE_COLOR_INDEX

#define OUTLINE_WIDTH_INDEX 1
#define OUTLINE_WIDTH_SHIFT 1
#define OUTLINE_WIDTH_MASK  0xfe
static inline uint8_t css_computed_outline_width(
		const css_computed_style *style, 
		css_fixed *length, css_unit *unit)
{
	if (style->uncommon != NULL) {
		uint8_t bits = style->uncommon->bits[OUTLINE_WIDTH_INDEX];
		bits &= OUTLINE_WIDTH_MASK;
		bits >>= OUTLINE_WIDTH_SHIFT;

		/* 7bits: uuuuttt : unit | type */

		if ((bits & 7) == CSS_OUTLINE_WIDTH_WIDTH) {
			*length = style->uncommon->outline_width;
			*unit = bits >> 3;
			return CSS_OUTLINE_WIDTH_WIDTH;
		} else
			return (bits & 7);
	}

	return CSS_OUTLINE_WIDTH_MEDIUM;
}
#undef OUTLINE_WIDTH_MASK
#undef OUTLINE_WIDTH_SHIFT
#undef OUTLINE_WIDTH_INDEX

#define BORDER_SPACING_INDEX 2
#define BORDER_SPACING_SHIFT 0
#define BORDER_SPACING_MASK  0xff
static inline uint8_t css_computed_border_spacing(
		const css_computed_style *style, 
		css_fixed *hlength, css_unit *hunit,
		css_fixed *vlength, css_unit *vunit)
{
	if (style->uncommon != NULL) {
		uint8_t bits = style->uncommon->bits[BORDER_SPACING_INDEX];
		bits &= BORDER_SPACING_MASK;
		bits >>= BORDER_SPACING_SHIFT;

		/* There's actually a ninth bit (bit 0 of the previous byte)
		 * This encodes SET/INHERIT. Given that we must be SET here,
		 * we don't bother looking at it */

		/* 8bits: hhhhvvvv : hunit | vunit */

		*hlength = style->uncommon->border_spacing[0];
		*hunit = bits >> 4;

		*vlength = style->uncommon->border_spacing[1];
		*vunit = bits & 0xf;
	} else {
		*hlength = *vlength = 0;
		*hunit = *vunit = CSS_UNIT_PX;
	}

	return CSS_BORDER_SPACING_SET;
}
#undef BORDER_SPACING_MASK
#undef BORDER_SPACING_SHIFT
#undef BORDER_SPACING_INDEX


#endif
