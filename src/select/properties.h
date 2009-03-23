/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_properties_h_
#define css_select_properties_h_

#include <libcss/errors.h>
#include <libcss/computed.h>

#include "stylesheet.h"
#include "select/select.h"

css_error cascade_azimuth(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_azimuth_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_azimuth(css_computed_style *style);
css_error compose_azimuth(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_background_attachment(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_background_attachment_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_background_attachment(css_computed_style *style);
css_error compose_background_attachment(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_background_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_background_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_background_color(css_computed_style *style);
css_error compose_background_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_background_image(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_background_image_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_background_image(css_computed_style *style);
css_error compose_background_image(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_background_position(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_background_position_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_background_position(css_computed_style *style);
css_error compose_background_position(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_background_repeat(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_background_repeat_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_background_repeat(css_computed_style *style);
css_error compose_background_repeat(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_collapse(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_collapse_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_collapse(css_computed_style *style);
css_error compose_border_collapse(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_spacing(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_spacing_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_spacing(css_computed_style *style);
css_error compose_border_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_top_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_top_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_top_color(css_computed_style *style);
css_error compose_border_top_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_right_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_right_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_right_color(css_computed_style *style);
css_error compose_border_right_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_bottom_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_bottom_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_bottom_color(css_computed_style *style);
css_error compose_border_bottom_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_left_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_left_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_left_color(css_computed_style *style);
css_error compose_border_left_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_top_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_top_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_top_style(css_computed_style *style);
css_error compose_border_top_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_right_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_right_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_right_style(css_computed_style *style);
css_error compose_border_right_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_bottom_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_bottom_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_bottom_style(css_computed_style *style);
css_error compose_border_bottom_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_left_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_left_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_left_style(css_computed_style *style);
css_error compose_border_left_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_top_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_top_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_top_width(css_computed_style *style);
css_error compose_border_top_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_right_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_right_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_right_width(css_computed_style *style);
css_error compose_border_right_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_bottom_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_bottom_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_bottom_width(css_computed_style *style);
css_error compose_border_bottom_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_border_left_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_border_left_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_border_left_width(css_computed_style *style);
css_error compose_border_left_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_bottom(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_bottom_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_bottom(css_computed_style *style);
css_error compose_bottom(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_caption_side(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_caption_side_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_caption_side(css_computed_style *style);
css_error compose_caption_side(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_clear(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_clear_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_clear(css_computed_style *style);
css_error compose_clear(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_clip(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_clip_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_clip(css_computed_style *style);
css_error compose_clip(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_color(css_computed_style *style);
css_error compose_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_content(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_content_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_content(css_computed_style *style);
css_error compose_content(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_counter_increment(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_counter_increment_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_counter_increment(css_computed_style *style);
css_error compose_counter_increment(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_counter_reset(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_counter_reset_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_counter_reset(css_computed_style *style);
css_error compose_counter_reset(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_cue_after(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_cue_after_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_cue_after(css_computed_style *style);
css_error compose_cue_after(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_cue_before(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_cue_before_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_cue_before(css_computed_style *style);
css_error compose_cue_before(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_cursor(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_cursor_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_cursor(css_computed_style *style);
css_error compose_cursor(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_direction(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_direction_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_direction(css_computed_style *style);
css_error compose_direction(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_display(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_display_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_display(css_computed_style *style);
css_error compose_display(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_elevation(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_elevation_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_elevation(css_computed_style *style);
css_error compose_elevation(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_empty_cells(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_empty_cells_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_empty_cells(css_computed_style *style);
css_error compose_empty_cells(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_float(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_float_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_float(css_computed_style *style);
css_error compose_float(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_font_family(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_font_family_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_font_family(css_computed_style *style);
css_error compose_font_family(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_font_size(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_font_size_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_font_size(css_computed_style *style);
css_error compose_font_size(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_font_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_font_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_font_style(css_computed_style *style);
css_error compose_font_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_font_variant(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_font_variant_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_font_variant(css_computed_style *style);
css_error compose_font_variant(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_font_weight(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_font_weight_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_font_weight(css_computed_style *style);
css_error compose_font_weight(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_height(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_height_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_height(css_computed_style *style);
css_error compose_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_left(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_left_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_left(css_computed_style *style);
css_error compose_left(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_letter_spacing(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_letter_spacing_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_letter_spacing(css_computed_style *style);
css_error compose_letter_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_line_height(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_line_height_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_line_height(css_computed_style *style);
css_error compose_line_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_list_style_image(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_list_style_image_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_list_style_image(css_computed_style *style);
css_error compose_list_style_image(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_list_style_position(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_list_style_position_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_list_style_position(css_computed_style *style);
css_error compose_list_style_position(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_list_style_type(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_list_style_type_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_list_style_type(css_computed_style *style);
css_error compose_list_style_type(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_margin_top(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_margin_top_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_margin_top(css_computed_style *style);
css_error compose_margin_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_margin_right(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_margin_right_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_margin_right(css_computed_style *style);
css_error compose_margin_right(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_margin_bottom(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_margin_bottom_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_margin_bottom(css_computed_style *style);
css_error compose_margin_bottom(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_margin_left(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_margin_left_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_margin_left(css_computed_style *style);
css_error compose_margin_left(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_max_height(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_max_height_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_max_height(css_computed_style *style);
css_error compose_max_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_max_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_max_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_max_width(css_computed_style *style);
css_error compose_max_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_min_height(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_min_height_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_min_height(css_computed_style *style);
css_error compose_min_height(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_min_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_min_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_min_width(css_computed_style *style);
css_error compose_min_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_orphans(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_orphans_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_orphans(css_computed_style *style);
css_error compose_orphans(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_outline_color(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_outline_color_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_outline_color(css_computed_style *style);
css_error compose_outline_color(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_outline_style(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_outline_style_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_outline_style(css_computed_style *style);
css_error compose_outline_style(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_outline_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_outline_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_outline_width(css_computed_style *style);
css_error compose_outline_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_overflow(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_overflow_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_overflow(css_computed_style *style);
css_error compose_overflow(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_padding_top(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_padding_top_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_padding_top(css_computed_style *style);
css_error compose_padding_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_padding_right(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_padding_right_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_padding_right(css_computed_style *style);
css_error compose_padding_right(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_padding_bottom(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_padding_bottom_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_padding_bottom(css_computed_style *style);
css_error compose_padding_bottom(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_padding_left(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_padding_left_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_padding_left(css_computed_style *style);
css_error compose_padding_left(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_page_break_after(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_page_break_after_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_page_break_after(css_computed_style *style);
css_error compose_page_break_after(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_page_break_before(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_page_break_before_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_page_break_before(css_computed_style *style);
css_error compose_page_break_before(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_page_break_inside(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_page_break_inside_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_page_break_inside(css_computed_style *style);
css_error compose_page_break_inside(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_pause_after(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_pause_after_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_pause_after(css_computed_style *style);
css_error compose_pause_after(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_pause_before(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_pause_before_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_pause_before(css_computed_style *style);
css_error compose_pause_before(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_pitch_range(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_pitch_range_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_pitch_range(css_computed_style *style);
css_error compose_pitch_range(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_pitch(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_pitch_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_pitch(css_computed_style *style);
css_error compose_pitch(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_play_during(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_play_during_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_play_during(css_computed_style *style);
css_error compose_play_during(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_position(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_position_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_position(css_computed_style *style);
css_error compose_position(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_quotes(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_quotes_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_quotes(css_computed_style *style);
css_error compose_quotes(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_richness(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_richness_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_richness(css_computed_style *style);
css_error compose_richness(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_right(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_right_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_right(css_computed_style *style);
css_error compose_right(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_speak_header(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_speak_header_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_speak_header(css_computed_style *style);
css_error compose_speak_header(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_speak_numeral(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_speak_numeral_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_speak_numeral(css_computed_style *style);
css_error compose_speak_numeral(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_speak_punctuation(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_speak_punctuation_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_speak_punctuation(css_computed_style *style);
css_error compose_speak_punctuation(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_speak(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_speak_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_speak(css_computed_style *style);
css_error compose_speak(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_speech_rate(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_speech_rate_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_speech_rate(css_computed_style *style);
css_error compose_speech_rate(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_stress(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_stress_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_stress(css_computed_style *style);
css_error compose_stress(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_table_layout(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_table_layout_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_table_layout(css_computed_style *style);
css_error compose_table_layout(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_text_align(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_text_align_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_text_align(css_computed_style *style);
css_error compose_text_align(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_text_decoration(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_text_decoration_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_text_decoration(css_computed_style *style);
css_error compose_text_decoration(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_text_indent(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_text_indent_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_text_indent(css_computed_style *style);
css_error compose_text_indent(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_text_transform(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_text_transform_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_text_transform(css_computed_style *style);
css_error compose_text_transform(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_top(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_top_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_top(css_computed_style *style);
css_error compose_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_unicode_bidi(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_unicode_bidi_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_unicode_bidi(css_computed_style *style);
css_error compose_unicode_bidi(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_vertical_align(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_vertical_align_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_vertical_align(css_computed_style *style);
css_error compose_vertical_align(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_visibility(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_visibility_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_visibility(css_computed_style *style);
css_error compose_visibility(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_voice_family(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_voice_family_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_voice_family(css_computed_style *style);
css_error compose_voice_family(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_volume(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_volume_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_volume(css_computed_style *style);
css_error compose_volume(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_white_space(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_white_space_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_white_space(css_computed_style *style);
css_error compose_white_space(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_widows(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_widows_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_widows(css_computed_style *style);
css_error compose_widows(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_width(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_width_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_width(css_computed_style *style);
css_error compose_width(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_word_spacing(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_word_spacing_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_word_spacing(css_computed_style *style);
css_error compose_word_spacing(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);
css_error cascade_z_index(uint32_t opv, css_style *style,
		css_select_state *state);
css_error set_z_index_from_hint(const css_hint *hint,
		css_computed_style *style);
css_error initial_z_index(css_computed_style *style);
css_error compose_z_index(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result);

#endif

