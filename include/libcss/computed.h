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
#include <libcss/types.h>

typedef struct css_computed_aural {
} css_computed_aural;

typedef struct css_computed_page {
} css_computed_page;

typedef struct css_computed_uncommon {
/*
 * border_spacing		  1 + 2(4)	  2(4)
 * clip				  2 + 4(4)	  4(4)
 * counter_increment		  1		  4 + sizeof(ptr)
 * counter_reset		  1		  4 + sizeof(ptr)
 * letter_spacing		  1 + 4		  4
 * outline_color		  1		  4
 * outline_width		  3 + 4		  4
 * word_spacing			  1 + 4		  4
 * 				---		---
 * 				 47 bits	 48 + 2sizeof(ptr) bytes
 *
 * quotes			  1		  sizeof(ptr)
 * 				---		---
 * 				  1 bit		  sizeof(ptr) bytes
 *
 * content			?
 * cursor			?
 *
 * 				___		___
 * 				 48 bits	 48 + 3sizeof(ptr) bytes
 *
 * 				  6 bytes	 48 + 3sizeof(ptr) bytes
 * 				===================
 * 				 54 + 3sizeof(ptr) bytes
 */
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
 * background_color		  1		  4
 * background_image		  1		  sizeof(ptr)
 * background_position		  1		  2sizeof(ptr)
 * border_top_color		  1		  4
 * border_right_color		  1		  4
 * border_bottom_color		  1		  4
 * border_left_color		  1		  4
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
 *				168 bits	132 + 4sizeof(ptr) bytes
 *
 * Encode font family as a (NULL-terminated?) array of string pointers
 *
 * font_family			  1		  sizeof(ptr)
 * 				---		---
 * 				  1 bit		sizeof(ptr)
 *
 * 				___		___
 *				252 bits	132 + 5sizeof(ptr) bytes
 *
 *				 32 bytes	132 + 5sizeof(ptr) bytes
 *				===================
 *				164 + 5sizeof(ptr) bytes
 */
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

#endif
