/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include <parserutils/utils/hash.h>

#include <libcss/computed.h>
#include <libcss/select.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "stylesheet.h"
#include "select/hash.h"
#include "select/propset.h"
#include "utils/parserutilserror.h"
#include "utils/utils.h"

/**
 * CSS selection context
 */
struct css_select_ctx {
	uint32_t n_sheets;		/**< Number of sheets */

	const css_stylesheet **sheets;	/**< Array of sheets */

	css_alloc alloc;		/**< Allocation routine */
	void *pw;			/**< Client-specific private data */
};

typedef struct prop_state {
	uint32_t specificity;		/* Specificity of property in result */
	uint32_t set       : 1,		/* Whether property is set in result */
	         origin    : 2,		/* Origin of property in result */
	         important : 1;		/* Importance of property in result */
} prop_state;

/**
 * Selection state
 */
typedef struct css_select_state {
	void *node;			/* Node we're selecting for */
	uint64_t pseudo_element;	/* Pseudo element to select for */
	uint64_t pseudo_classes;	/* Currently active pseudo classes */
	uint64_t media;			/* Currently active media types */
	css_computed_style *result;	/* Style to populate */

	css_select_handler *handler;	/* Handler functions */
	void *pw;			/* Client data for handlers */

	const css_stylesheet *sheet;	/* Current sheet being processed */

	css_origin current_origin;	/* Origin of current sheet */
	uint32_t current_specificity;	/* Specificity of current rule */

	/* Useful interned strings */
	const parserutils_hash_entry *universal;
	const parserutils_hash_entry *first_child;
	const parserutils_hash_entry *link;
	const parserutils_hash_entry *visited;
	const parserutils_hash_entry *hover;
	const parserutils_hash_entry *active;
	const parserutils_hash_entry *focus;
	const parserutils_hash_entry *left;
	const parserutils_hash_entry *right;
	const parserutils_hash_entry *first;
	const parserutils_hash_entry *first_line;
	const parserutils_hash_entry *first_letter;
	const parserutils_hash_entry *before;
	const parserutils_hash_entry *after;

	prop_state props[N_OPCODES];
} css_select_state;

static css_error select_from_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state);
static css_error intern_strings_for_sheet(css_select_ctx *ctx,
		const css_stylesheet *sheet, css_select_state *state);
static css_error match_selectors_in_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state);
static css_error match_selector_chain(css_select_ctx *ctx, 
		const css_selector *selector, css_select_state *state);
static css_error match_named_combinator(css_select_ctx *ctx, 
		css_combinator type, const parserutils_hash_entry *name, 
		css_select_state *state, void *node, void **next_node);
static css_error match_universal_combinator(css_select_ctx *ctx, 
		css_combinator type, const css_selector *selector, 
		css_select_state *state, void *node, void **next_node);
static css_error match_details(css_select_ctx *ctx, void *node, 
		const css_selector_detail *detail, css_select_state *state, 
		bool *match);
static css_error match_detail(css_select_ctx *ctx, void *node, 
		const css_selector_detail *detail, css_select_state *state, 
		bool *match);
static css_error cascade_style(const css_style *style, css_select_state *state);
static inline void advance_bytecode(css_style *style, uint32_t n_bytes);
static bool outranks_existing(uint16_t op, bool important, 
		css_select_state *state);

/* Property handlers */
#include "select/properties.c"

/**
 * Enumeration of property groups
 */
enum prop_group {
	GROUP_NORMAL	= 0x0,
	GROUP_UNCOMMON	= 0x1,
	GROUP_PAGE	= 0x2,
	GROUP_AURAL	= 0x3
};

/**
 * Dispatch table for properties, indexed by opcode
 */
static struct prop_table {
	css_error (*cascade)(uint32_t opv, css_style *style, 
			css_select_state *state);
	css_error (*initial)(css_computed_style *style);

	uint32_t inherited : 1,
	         group : 2;
} properties[N_OPCODES] = {
	{ cascade_azimuth,               initial_azimuth,               1, GROUP_AURAL},
	{ cascade_background_attachment, initial_background_attachment, 0, GROUP_NORMAL },
	{ cascade_background_color,      initial_background_color,      0, GROUP_NORMAL },
	{ cascade_background_image,      initial_background_image,      0, GROUP_NORMAL },
	{ cascade_background_position,   initial_background_position,   0, GROUP_NORMAL },
	{ cascade_background_repeat,     initial_background_repeat,     0, GROUP_NORMAL },
	{ cascade_border_collapse,       initial_border_collapse,       1, GROUP_NORMAL },
	{ cascade_border_spacing,        initial_border_spacing,        1, GROUP_UNCOMMON },
	{ cascade_border_top_color,      initial_border_top_color,      0, GROUP_NORMAL },
	{ cascade_border_right_color,    initial_border_right_color,    0, GROUP_NORMAL },
	{ cascade_border_bottom_color,   initial_border_bottom_color,   0, GROUP_NORMAL },
	{ cascade_border_left_color,     initial_border_left_color,     0, GROUP_NORMAL },
	{ cascade_border_top_style,      initial_border_top_style,      0, GROUP_NORMAL },
	{ cascade_border_right_style,    initial_border_right_style,    0, GROUP_NORMAL },
	{ cascade_border_bottom_style,   initial_border_bottom_style,   0, GROUP_NORMAL },
	{ cascade_border_left_style,     initial_border_left_style,     0, GROUP_NORMAL },
	{ cascade_border_top_width,      initial_border_top_width,      0, GROUP_NORMAL },
	{ cascade_border_right_width,    initial_border_right_width,    0, GROUP_NORMAL },
	{ cascade_border_bottom_width,   initial_border_bottom_width,   0, GROUP_NORMAL },
	{ cascade_border_left_width,     initial_border_left_width,     0, GROUP_NORMAL },
	{ cascade_bottom,                initial_bottom,                0, GROUP_NORMAL },
	{ cascade_caption_side,          initial_caption_side,          1, GROUP_NORMAL },
	{ cascade_clear,                 initial_clear,                 0, GROUP_NORMAL },
	{ cascade_clip,                  initial_clip,                  0, GROUP_UNCOMMON },
	{ cascade_color,                 initial_color,                 1, GROUP_NORMAL },
	{ cascade_content,               initial_content,               0, GROUP_UNCOMMON },
	{ cascade_counter_increment,     initial_counter_increment,     0, GROUP_UNCOMMON },
	{ cascade_counter_reset,         initial_counter_reset,         0, GROUP_UNCOMMON },
	{ cascade_cue_after,             initial_cue_after,             0, GROUP_AURAL },
	{ cascade_cue_before,            initial_cue_before,            0, GROUP_AURAL },
	{ cascade_cursor,                initial_cursor,                1, GROUP_UNCOMMON },
	{ cascade_direction,             initial_direction,             1, GROUP_NORMAL },
	{ cascade_display,               initial_display,               0, GROUP_NORMAL },
	{ cascade_elevation,             initial_elevation,             1, GROUP_AURAL },
	{ cascade_empty_cells,           initial_empty_cells,           1, GROUP_NORMAL },
	{ cascade_float,                 initial_float,                 0, GROUP_NORMAL },
	{ cascade_font_family,           initial_font_family,           1, GROUP_NORMAL },
	{ cascade_font_size,             initial_font_size,             1, GROUP_NORMAL },
	{ cascade_font_style,            initial_font_style,            1, GROUP_NORMAL },
	{ cascade_font_variant,          initial_font_variant,          1, GROUP_NORMAL },
	{ cascade_font_weight,           initial_font_weight,           1, GROUP_NORMAL },
	{ cascade_height,                initial_height,                0, GROUP_NORMAL },
	{ cascade_left,                  initial_left,                  0, GROUP_NORMAL },
	{ cascade_letter_spacing,        initial_letter_spacing,        1, GROUP_UNCOMMON },
	{ cascade_line_height,           initial_line_height,           1, GROUP_NORMAL },
	{ cascade_list_style_image,      initial_list_style_image,      1, GROUP_NORMAL },
	{ cascade_list_style_position,   initial_list_style_position,   1, GROUP_NORMAL },
	{ cascade_list_style_type,       initial_list_style_type,       1, GROUP_NORMAL },
	{ cascade_margin_top,            initial_margin_top,            0, GROUP_NORMAL },
	{ cascade_margin_right,          initial_margin_right,          0, GROUP_NORMAL },
	{ cascade_margin_bottom,         initial_margin_bottom,         0, GROUP_NORMAL },
	{ cascade_margin_left,           initial_margin_left,           0, GROUP_NORMAL },
	{ cascade_max_height,            initial_max_height,            0, GROUP_NORMAL },
	{ cascade_max_width,             initial_max_width,             0, GROUP_NORMAL },
	{ cascade_min_height,            initial_min_height,            0, GROUP_NORMAL },
	{ cascade_min_width,             initial_min_width,             0, GROUP_NORMAL },
	{ cascade_orphans,               initial_orphans,               1, GROUP_PAGE },
	{ cascade_outline_color,         initial_outline_color,         0, GROUP_UNCOMMON },
	{ cascade_outline_style,         initial_outline_style,         0, GROUP_NORMAL },
	{ cascade_outline_width,         initial_outline_width,         0, GROUP_UNCOMMON },
	{ cascade_overflow,              initial_overflow,              0, GROUP_NORMAL },
	{ cascade_padding_top,           initial_padding_top,           0, GROUP_NORMAL },
	{ cascade_padding_right,         initial_padding_right,         0, GROUP_NORMAL },
	{ cascade_padding_bottom,        initial_padding_bottom,        0, GROUP_NORMAL },
	{ cascade_padding_left,          initial_padding_left,          0, GROUP_NORMAL },
	{ cascade_page_break_after,      initial_page_break_after,      0, GROUP_PAGE },
	{ cascade_page_break_before,     initial_page_break_before,     0, GROUP_PAGE },
	{ cascade_page_break_inside,     initial_page_break_inside,     1, GROUP_PAGE },
	{ cascade_pause_after,           initial_pause_after,           0, GROUP_AURAL },
	{ cascade_pause_before,          initial_pause_before,          0, GROUP_AURAL },
	{ cascade_pitch_range,           initial_pitch_range,           1, GROUP_AURAL },
	{ cascade_pitch,                 initial_pitch,                 1, GROUP_AURAL },
	{ cascade_play_during,           initial_play_during,           0, GROUP_AURAL },
	{ cascade_position,              initial_position,              0, GROUP_NORMAL },
	{ cascade_quotes,                initial_quotes,                1, GROUP_UNCOMMON },
	{ cascade_richness,              initial_richness,              1, GROUP_AURAL },
	{ cascade_right,                 initial_right,                 0, GROUP_NORMAL },
	{ cascade_speak_header,          initial_speak_header,          1, GROUP_AURAL },
	{ cascade_speak_numeral,         initial_speak_numeral,         1, GROUP_AURAL },
	{ cascade_speak_punctuation,     initial_speak_punctuation,     1, GROUP_AURAL },
	{ cascade_speak,                 initial_speak,                 1, GROUP_AURAL },
	{ cascade_speech_rate,           initial_speech_rate,           1, GROUP_AURAL },
	{ cascade_stress,                initial_stress,                1, GROUP_AURAL },
	{ cascade_table_layout,          initial_table_layout,          0, GROUP_NORMAL },
	{ cascade_text_align,            initial_text_align,            1, GROUP_NORMAL },
	{ cascade_text_decoration,       initial_text_decoration,       0, GROUP_NORMAL },
	{ cascade_text_indent,           initial_text_indent,           1, GROUP_NORMAL },
	{ cascade_text_transform,        initial_text_transform,        1, GROUP_NORMAL },
	{ cascade_top,                   initial_top,                   0, GROUP_NORMAL },
	{ cascade_unicode_bidi,          initial_unicode_bidi,          0, GROUP_NORMAL },
	{ cascade_vertical_align,        initial_vertical_align,        0, GROUP_NORMAL },
	{ cascade_visibility,            initial_visibility,            1, GROUP_NORMAL },
	{ cascade_voice_family,          initial_voice_family,          1, GROUP_AURAL },
	{ cascade_volume,                initial_volume,                1, GROUP_AURAL },
	{ cascade_white_space,           initial_white_space,           1, GROUP_NORMAL },
	{ cascade_widows,                initial_widows,                1, GROUP_PAGE },
	{ cascade_width,                 initial_width,                 0, GROUP_NORMAL },
	{ cascade_word_spacing,          initial_word_spacing,          1, GROUP_UNCOMMON },
	{ cascade_z_index,               initial_z_index,               0, GROUP_NORMAL }
};

/**
 * Create a selection context
 *
 * \param alloc   Memory (de)allocation function
 * \param pw      Client-specific private data
 * \param result  Pointer to location to receive created context
 * \return CSS_OK on success, appropriate error otherwise.
 */
css_error css_select_ctx_create(css_alloc alloc, void *pw,
		css_select_ctx **result)
{
	css_select_ctx *c;

	if (alloc == NULL || result == NULL)
		return CSS_BADPARM;

	c = alloc(NULL, sizeof(css_select_ctx), pw);
	if (c == NULL)
		return CSS_NOMEM;

	c->n_sheets = 0;
	c->sheets = NULL;

	c->alloc = alloc;
	c->pw = pw;

	*result = c;

	return CSS_OK;
}

/**
 * Destroy a selection context
 *
 * \param ctx  The context to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_destroy(css_select_ctx *ctx)
{
	if (ctx == NULL)
		return CSS_BADPARM;

	if (ctx->sheets != NULL)
		ctx->alloc(ctx->sheets, 0, ctx->pw);

	ctx->alloc(ctx, 0, ctx->pw);

	return CSS_OK;
}

/**
 * Append a stylesheet to a selection context
 *
 * \param ctx    The context to append to
 * \param sheet  The sheet to append
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_append_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet)
{
	if (ctx == NULL || sheet == NULL)
		return CSS_BADPARM;

	return css_select_ctx_insert_sheet(ctx, sheet, ctx->n_sheets);
}

/**
 * Insert a stylesheet into a selection context
 * 
 * \param ctx    The context to insert into
 * \param sheet  Sheet to insert
 * \param index  Index in context to insert sheet
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_insert_sheet(css_select_ctx *ctx,
		const css_stylesheet *sheet, uint32_t index)
{
	const css_stylesheet **temp;

	if (ctx == NULL || sheet == NULL)
		return CSS_BADPARM;

	/* Index must be in the range [0, n_sheets]
	 * The latter being equivalent to append */
	if (index > ctx->n_sheets)
		return CSS_INVALID;

	temp = ctx->alloc(ctx->sheets, 
			(ctx->n_sheets + 1) * sizeof(css_stylesheet *),
			ctx->pw);
	if (temp == NULL)
		return CSS_NOMEM;

	ctx->sheets = temp;

	if (index < ctx->n_sheets) {
		memmove(&ctx->sheets[index + 1], &ctx->sheets[index],
			(ctx->n_sheets - index) * sizeof(css_stylesheet *));
	}

	ctx->sheets[index] = sheet;

	ctx->n_sheets++;

	return CSS_OK;
}

/**
 * Remove a sheet from a selection context
 *
 * \param ctx    The context to remove from
 * \param sheet  Sheet to remove
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_remove_sheet(css_select_ctx *ctx,
		const css_stylesheet *sheet)
{
	uint32_t index;

	if (ctx == NULL || sheet == NULL)
		return CSS_BADPARM;

	for (index = 0; index < ctx->n_sheets; index++) {
		if (ctx->sheets[index] == sheet)
			break;
	}

	if (index == ctx->n_sheets)
		return CSS_INVALID;

	memmove(&ctx->sheets[index], &ctx->sheets[index + 1],
			(ctx->n_sheets - index) * sizeof(css_stylesheet *));

	ctx->n_sheets--;

	return CSS_OK;

}

/**
 * Count the number of top-level sheets in a selection context
 *
 * \param ctx    Context to consider
 * \param count  Pointer to location to receive count of sheets
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_count_sheets(css_select_ctx *ctx, uint32_t *count)
{
	if (ctx == NULL || count == NULL)
		return CSS_BADPARM;

	*count = ctx->n_sheets;

	return CSS_OK;
}

/**
 * Retrieve a sheet from a selection context
 *
 * \param ctx    Context to look in
 * \param index  Index in context to look
 * \param sheet  Pointer to location to receive sheet
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_select_ctx_get_sheet(css_select_ctx *ctx, uint32_t index,
		const css_stylesheet **sheet)
{
	if (ctx == NULL || sheet == NULL)
		return CSS_BADPARM;

	if (index > ctx->n_sheets)
		return CSS_INVALID;

	*sheet = ctx->sheets[index];

	return CSS_OK;
}

/**
 * Select a style for the given node
 *
 * \param ctx             Selection context to use
 * \param node            Node to select style for
 * \param pseudo_element  Pseudo element to select for, instead
 * \param pseudo_classes  Currently active pseudo classes
 * \param media           Currently active media types
 * \param result          Pointer to style to populate (assumed clean)
 * \param handler         Dispatch table of handler functions
 * \param pw              Client-specific private data for handler functions
 * \return CSS_OK on success, appropriate error otherwise.
 *
 * In computing the style, no reference is made to the parent node's
 * style. Therefore, the resultant computed style is not ready for
 * immediate use, as some properties may be marked as inherited.
 * Use css_computed_style_compose() to obtain a fully computed style.
 *
 * This two-step approach to style computation is designed to allow
 * the client to store the partially computed style and efficiently
 * update the fully computed style for a node when layout changes.
 */
css_error css_select_style(css_select_ctx *ctx, void *node,
		uint64_t pseudo_element, uint64_t pseudo_classes,
		uint64_t media,	css_computed_style *result,
		css_select_handler *handler, void *pw)
{
	uint32_t i;
	css_error error;
	css_select_state state;

	if (ctx == NULL || node == NULL || result == NULL || handler == NULL)
		return CSS_BADPARM;

	/* Set up the selection state */
	memset(&state, 0, sizeof(css_select_state));
	state.node = node;
	state.pseudo_element = pseudo_element;
	state.pseudo_classes = pseudo_classes;
	state.media = media;
	state.result = result;
	state.handler = handler;
	state.pw = pw;

	/* Iterate through the top-level stylesheets, selecting styles
	 * from those which apply to our current media requirements */
	for (i = 0; i < ctx->n_sheets; i++) {
		if ((ctx->sheets[i]->media & media) != 0) {
			error = select_from_sheet(ctx, ctx->sheets[i], &state);
			if (error != CSS_OK)
				return error;
		}
	}

	/* Finally, fix up any remaining unset properties.
	 * Those properties which are inherited need to be set as inherit.
	 * Those which are not inherited need to be set to their default value.
	 */
	for (i = 0; i < N_OPCODES; i++) {
		/* Do nothing if this property is set */
		if (state.props[i].set)
			continue;

		/* Do nothing if this property is inherited (the default state 
		 * of a clean computed style is for everything to be set to 
		 * inherit) */
		if (properties[i].inherited)
			continue;

		/* Remaining properties are neither inherited nor already set.
		 * Thus, we set them to their initial values here. Except, 
		 * however, if the property in question resides in one of the 
		 * extension blocks and the extension block has yet to be 
		 * allocated. In that case, we do nothing and leave it to the 
		 * property accessors to return the initial values for the 
		 * property. */
		if (properties[i].group == GROUP_NORMAL) {
			error = properties[i].initial(result);
			if (error != CSS_OK)
				return error;
		} else if (properties[i].group == GROUP_UNCOMMON &&
				result->uncommon != NULL) {
			error = properties[i].initial(result);
			if (error != CSS_OK)
				return error;
		} else if (properties[i].group == GROUP_PAGE &&
				result->page != NULL) {
			error = properties[i].initial(result);
			if (error != CSS_OK)
				return error;
		} else if (properties[i].group == GROUP_AURAL &&
				result->aural != NULL) {
			error = properties[i].initial(result);
			if (error != CSS_OK)
				return error;
		}
	}

	return CSS_OK;
}

/******************************************************************************
 * Selection engine internals below here                                      *
 ******************************************************************************/

css_error select_from_sheet(css_select_ctx *ctx, const css_stylesheet *sheet, 
		css_select_state *state)
{
	const css_stylesheet *s = sheet;
	const css_rule *rule = s->rule_list;

	do {
		/* Find first non-charset rule, if we're at the list head */
		if (rule == s->rule_list) {
			for (; rule != NULL; rule = rule->next) {
				if (rule->type != CSS_RULE_CHARSET)
					break;
			}
		}

		if (rule != NULL && rule->type == CSS_RULE_IMPORT) {
			/* Current rule is an import */
			const css_rule_import *import = 
					(const css_rule_import *) rule;

			if (import->sheet != NULL &&
					(import->sheet->media & 
					state->media) != 0) {
				/* It's applicable, so process it */
				s = import->sheet;
				rule = s->rule_list;
			} else {
				/* Not applicable; skip over it */
				rule = rule->next;
			}
		} else {
			/* Gone past import rules in this sheet */
			css_error error;

			/* Process this sheet */
			state->sheet = s;
			state->current_origin = s->origin;

			error = intern_strings_for_sheet(ctx, s, state);
			if (error != CSS_OK)
				return error;

			error = match_selectors_in_sheet(ctx, s, state);
			if (error != CSS_OK)
				return error;

			/* Find next sheet to process */
			if (s->ownerRule != NULL) {
				s = s->ownerRule->parent;
				rule = s->ownerRule->next;
			} else {
				s = NULL;
			}
		}
	} while (s != NULL);

	return CSS_OK;
}

css_error intern_strings_for_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state)
{
	parserutils_error perror;

	UNUSED(ctx);

	/* Universal selector */
	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "*", SLEN("*"), &state->universal);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	/* Pseudo classes */
	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "first-child", SLEN("first-child"), 
			&state->first_child);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "link", SLEN("link"), 
			&state->link);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "visited", SLEN("visited"), 
			&state->visited);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "hover", SLEN("hover"), 
			&state->hover);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "active", SLEN("active"), 
			&state->active);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "focus", SLEN("focus"), 
			&state->focus);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "left", SLEN("left"), 
			&state->left);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "right", SLEN("right"), 
			&state->right);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "first", SLEN("first"), 
			&state->first);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	/* Pseudo elements */
	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "first-line", SLEN("first-line"), 
			&state->first_line);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "first_letter", SLEN("first-letter"),
			&state->first_letter);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "before", SLEN("before"), 
			&state->before);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "after", SLEN("after"), 
			&state->after);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	return CSS_OK;
}

css_error match_selectors_in_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state)
{
	const parserutils_hash_entry *element;
	const css_selector **selectors;
	const uint8_t *name;
	size_t len;
	parserutils_error perror;
	css_error error;

	/* Get node's name */
	error = state->handler->node_name(state->pw, state->node, &name, &len);
	if (error != CSS_OK)
		return error;

	/* Intern it */
	perror = parserutils_hash_insert(sheet->dictionary,
			name, len, &element);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	/* Find hash chain that applies to current node */
	error = css_selector_hash_find(sheet->selectors, element, &selectors);
	if (error != CSS_OK)
		return error;

	/* Process any matching selectors */
	while (*selectors != NULL) {
		error = match_selector_chain(ctx, *selectors, state);
		if (error != CSS_OK)
			return error;

		error = css_selector_hash_iterate(sheet->selectors, selectors,
				&selectors);
		if (error != CSS_OK)
			return error;
	}

	/* Find hash chain for universal selector */
	error = css_selector_hash_find(sheet->selectors, state->universal, 
			&selectors);
	if (error != CSS_OK)
		return error;

	/* Process any matching selectors */
	while (*selectors != NULL) {
		error = match_selector_chain(ctx, *selectors, state);
		if (error != CSS_OK)
			return error;

		error = css_selector_hash_iterate(sheet->selectors, selectors,
				&selectors);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

css_error match_selector_chain(css_select_ctx *ctx, 
		const css_selector *selector, css_select_state *state)
{
	const css_selector *s = selector;
	void *node = state->node;
	css_error error;

	do {
		void *next_node = NULL;
		const css_selector_detail *detail = &s->data;
		bool match = false;

		/* First, consider any named combinator on this selector */
		if (s->data.comb != CSS_COMBINATOR_NONE &&
				s->combinator->data.name != state->universal) {
			error = match_named_combinator(ctx, s->data.comb, 
					s->combinator->data.name, state, node, 
					&next_node);
			if (error != CSS_OK)
				return error;

			/* No match for combinator, so reject selector chain */
			if (next_node == NULL)
				return CSS_OK;
		}

		/* Now match details on this selector */
		error = match_details(ctx, node, detail, state, &match);
		if (error != CSS_OK)
			return error;

		/* Details don't match, so reject selector chain */
		if (match == false)
			return CSS_OK;

		/* If we had a universal combinator, then consider that */
		if (s->data.comb != CSS_COMBINATOR_NONE &&
				s->combinator->data.name == state->universal) {
			error = match_universal_combinator(ctx, s->data.comb, 
					s->combinator, state, node, &next_node);
			if (error != CSS_OK)
				return error;

			/* No match for combinator, so reject selector chain */
			if (next_node == NULL)
				return CSS_OK;
		}

		/* Details matched, so progress to combining selector */
		s = s->combinator;
		node = next_node;
	} while (s != NULL);

	/* If we got here, then the entire selector chain matched, so cascade */
	state->current_specificity = selector->specificity;

	return cascade_style(((css_rule_selector *) selector->rule)->style, 
			state);
}

css_error match_named_combinator(css_select_ctx *ctx, css_combinator type,
		const parserutils_hash_entry *name, css_select_state *state,
		void *node, void **next_node)
{
	css_error error;

	UNUSED(ctx);

	switch (type) {
	case CSS_COMBINATOR_ANCESTOR:
		error = state->handler->named_ancestor_node(state->pw, node, 
				name->data, name->len, next_node);
		if (error != CSS_OK)
			return error;
		break;
	case CSS_COMBINATOR_PARENT:
		error = state->handler->named_parent_node(state->pw, node, 
				name->data, name->len, next_node);
		if (error != CSS_OK)
			return error;
		break;
	case CSS_COMBINATOR_SIBLING:
		error = state->handler->named_sibling_node(state->pw, node, 
				name->data, name->len, next_node);
		if (error != CSS_OK)
			return error;
		break;
	case CSS_COMBINATOR_NONE:
		break;
	}

	return CSS_OK;
}

css_error match_universal_combinator(css_select_ctx *ctx, css_combinator type,
		const css_selector *selector, css_select_state *state,
		void *node, void **next_node)
{
	const css_selector_detail *detail = &selector->data;
	void *n = node;
	css_error error;

	/** \todo div * p */

	do {
		bool match = false;

		/* Find candidate node */
		switch (type) {
		case CSS_COMBINATOR_ANCESTOR:
		case CSS_COMBINATOR_PARENT:
			error = state->handler->parent_node(state->pw, n, &n);
			if (error != CSS_OK)
				return error;
			break;
		case CSS_COMBINATOR_SIBLING:
			error = state->handler->sibling_node(state->pw, n, &n);
			if (error != CSS_OK)
				return error;
			break;
		case CSS_COMBINATOR_NONE:
			break;
		}

		if (n != NULL) {
			/* Match its details */
			error = match_details(ctx, n, detail, state, &match);
			if (error != CSS_OK)
				return error;

			/* If we found a match, use it */
			if (match == true)
				break;

			/* For parent and sibling selectors, only adjacent
			 * nodes are valid. Thus, if we failed to match,
			 * give up. */
			if (type == CSS_COMBINATOR_PARENT ||
					type == CSS_COMBINATOR_SIBLING)
				n = NULL;
		}
	} while (n != NULL);

	*next_node = n;

	return CSS_OK;
}

css_error match_details(css_select_ctx *ctx, void *node, 
		const css_selector_detail *detail, css_select_state *state, 
		bool *match)
{
	css_error error;

	/* We match by default (if there are no details than the element
	 * selector, then we must match) */
	*match = true;

	/** \todo Some details are easier to test than others (e.g. dashmatch 
	 * actually requires looking at data rather than simply comparing 
	 * pointers). Should we consider sorting the detail list such that the 
	 * simpler details come first (and thus the expensive match routines 
	 * can be avoided unless absolutely necessary)? */

	while (detail->next != 0) {
		/* Don't bother with the first detail, as it's the 
		 * element selector */
		detail++;

		error = match_detail(ctx, node, detail, state, match);
		if (error != CSS_OK)
			return error;

		/* Detail doesn't match, so reject selector chain */
		if (*match == false)
			return CSS_OK;
	}

	return CSS_OK;
}

css_error match_detail(css_select_ctx *ctx, void *node, 
		const css_selector_detail *detail, css_select_state *state, 
		bool *match)
{
	css_error error = CSS_OK;

	UNUSED(ctx);

	switch (detail->type) {
	case CSS_SELECTOR_CLASS:
		error = state->handler->node_has_class(state->pw, node,
				detail->name->data, detail->name->len,
				match);
		break;
	case CSS_SELECTOR_ID:
		error = state->handler->node_has_id(state->pw, node,
				detail->name->data, detail->name->len,
				match);
		break;
	case CSS_SELECTOR_PSEUDO_CLASS:
		if (detail->name == state->first_child &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_FIRST_CHILD))
			*match = true;
		else if (detail->name == state->link &&
				(state->pseudo_classes & 
				CSS_PSEUDO_CLASS_LINK))
			*match = true;
		else if (detail->name == state->visited &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_VISITED))
			*match = true;
		else if (detail->name == state->hover &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_HOVER))
			*match = true;
		else if (detail->name == state->active &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_ACTIVE))
			*match = true;
		else if (detail->name == state->focus &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_FOCUS))
			*match = true;
		else if (detail->name == state->left &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_LEFT))
			*match = true;
		else if (detail->name == state->right &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_RIGHT))
			*match = true;
		else if (detail->name == state->first &&
				(state->pseudo_classes &
				CSS_PSEUDO_CLASS_FIRST))
			*match = true;
		else
			*match = false;
		break;
	case CSS_SELECTOR_PSEUDO_ELEMENT:
		if (detail->name == state->first_line && 
				(state->pseudo_element & 
				CSS_PSEUDO_ELEMENT_FIRST_LINE))
			*match = true;
		else if (detail->name == state->first_letter &&
				(state->pseudo_element &
				CSS_PSEUDO_ELEMENT_FIRST_LETTER))
			*match = true;
		else if (detail->name == state->before &&
				(state->pseudo_element &
				CSS_PSEUDO_ELEMENT_BEFORE))
			*match = true;
		else if (detail->name == state->after &&
				(state->pseudo_element &
				CSS_PSEUDO_ELEMENT_AFTER))
			*match = true;
		else
			*match = false;
		break;
	case CSS_SELECTOR_ATTRIBUTE:
		error = state->handler->node_has_attribute(state->pw, node,
				detail->name->data, detail->name->len,
				match);
		break;
	case CSS_SELECTOR_ATTRIBUTE_EQUAL:
		error = state->handler->node_has_attribute_equal(state->pw, 
				node, detail->name->data, detail->name->len,
				detail->value->data, detail->value->len,
				match);
		break;
	case CSS_SELECTOR_ATTRIBUTE_DASHMATCH:
		error = state->handler->node_has_attribute_dashmatch(state->pw,
				node, detail->name->data, detail->name->len,
				detail->value->data, detail->value->len,
				match);
		break;
	case CSS_SELECTOR_ATTRIBUTE_INCLUDES:
		error = state->handler->node_has_attribute_includes(state->pw, 
				node, detail->name->data, detail->name->len,
				detail->value->data, detail->value->len,
				match);
		break;
	}

	return error;
}

css_error cascade_style(const css_style *style, css_select_state *state)
{
	css_style s = *style;

	while (s.length > 0) {
		opcode op;
		css_error error;
		uint32_t opv = *((uint32_t *) s.bytecode);

		advance_bytecode(&s, sizeof(opv));

		op = getOpcode(opv);

		error = properties[op].cascade(opv, &s, state);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

void advance_bytecode(css_style *style, uint32_t n_bytes)
{
	style->length -= n_bytes;
	style->bytecode = ((uint8_t *) style->bytecode) + n_bytes;
}

bool outranks_existing(uint16_t op, bool important, css_select_state *state)
{
	prop_state *existing = &state->props[op];
	bool outranks = false;

	/* Sorting on origin & importance gives the following:
	 * 
	 *           | UA, - | UA, i | USER, - | USER, i | AUTHOR, - | AUTHOR, i
	 *           |----------------------------------------------------------
	 * UA    , - |   S       S       Y          Y         Y           Y
	 * UA    , i |   S       S       Y          Y         Y           Y
	 * USER  , - |   -       -       S          Y         Y           Y
	 * USER  , i |   -       -       -          S         -           -
	 * AUTHOR, - |   -       -       -          Y         S           Y
	 * AUTHOR, i |   -       -       -          Y         -           S
	 *
	 * Where the columns represent the origin/importance of the property 
	 * being considered and the rows represent the origin/importance of 
	 * the existing property.
	 *
	 * - means that the existing property must be preserved
	 * Y means that the new property must be applied
	 * S means that the specificities of the rules must be considered.
	 *
	 * If specificities are considered, the highest specificity wins.
	 * If specificities are equal, then the rule defined last wins.
	 *
	 * We have no need to explicitly consider the ordering of rules if
	 * the specificities are the same because:
	 *
	 * a) We process stylesheets in order
	 * b) The selector hash chains within a sheet are ordered such that 
	 *    more specific rules come after less specific ones and, when
	 *    specificities are identical, rules defined later occur after
	 *    those defined earlier.
	 *
	 * Therefore, where we consider specificity, below, the property 
	 * currently being considered will always be applied if its specificity
	 * is greater than or equal to that of the existing property.
	 */

	if (existing->set == 0) {
		/* Property hasn't been set before, new one wins */
		outranks = true;
	} else {
		assert(CSS_ORIGIN_UA < CSS_ORIGIN_USER);
		assert(CSS_ORIGIN_USER < CSS_ORIGIN_AUTHOR);

		if (existing->origin < state->current_origin) {
			/* New origin has more weight than existing one.
			 * Thus, new property wins, except when the existing 
			 * one is USER, i. */
			if (existing->important == 0 ||
					existing->origin != CSS_ORIGIN_USER) {
				outranks = true;
			}
		} else if (existing->origin == state->current_origin) {
			/* Origins are identical, consider importance, except 
			 * for UA stylesheets, when specificity is always 
			 * considered (as importance is meaningless) */
			if (existing->origin == CSS_ORIGIN_UA) {
				if (state->current_specificity >=
						existing->specificity) {
					outranks = true;
				}
			} else if (existing->important == 0 && important) {
				/* New is more important than old. */
				outranks = true;
			} else if (existing->important && important == false) {
				/* Old is more important than new */
			} else {
				/* Same importance, consider specificity */
				if (state->current_specificity >=
						existing->specificity) {
					outranks = true;
				}
			}
		} else {
			/* Existing origin has more weight than new one.
			 * Thus, existing property wins, except when the new
			 * one is USER, i. */
			if (state->current_origin == CSS_ORIGIN_USER &&
					important) {
				outranks = true;
			}
		}
	}

	if (outranks) {
		/* The new property is about to replace the old one.
		 * Update our state to reflect this. */
		existing->set = 1;
		existing->specificity = state->current_specificity;
		existing->origin = state->current_origin;
		existing->important = important;
	}

	return outranks;
}

