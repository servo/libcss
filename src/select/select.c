/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include <parserutils/utils/hash.h>

#include <libcss/computed.h>
#include <libcss/select.h>

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

/**
 * State for each property during selection
 */
typedef struct css_select_state {
	void *node;			/* Node we're selecting for */
	uint64_t pseudo_element;	/* Pseudo element to select for */
	uint64_t pseudo_classes;	/* Currently active pseudo classes */
	uint64_t media;			/* Currently active media types */
	css_computed_style *result;	/* Style to populate */

	css_select_handler *handler;	/* Handler functions */
	void *pw;			/* Client data for handlers */

	uint32_t current_sheet;		/* Identity of current sheet */

/** \todo We need a better way of knowing the number of properties
 * Bytecode opcodes cover 84 properties, then there's a 
 * further 15 generated from the side bits */
/* Stylesheet identity is a monotonically increasing number based at 1 and
 * increasing by 1 for every applicable stylesheet encountered, including 
 * imports. Imported sheets' identities are below that of the sheet that 
 * imported them. */
#define N_PROPS (99)
	struct {
		uint32_t specificity;	/* Specificity of property in result */
		uint32_t sheet;		/* Identity of applicable stylesheet */
		uint32_t set       : 1,	/* Whether property is set in result */
		         origin    : 2,	/* Origin of property in result */
		         important : 1,	/* Importance of property in result */
		         index     : 16;/* Index of corresponding rule */
	} props[N_PROPS];
#undef N_PROPS
} css_select_state;

static css_error select_from_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state);
static css_error match_selectors_in_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state);
static css_error match_selector_chain(css_select_ctx *ctx, 
		const css_selector *selector, css_origin origin,
		css_select_state *state, 
		const parserutils_hash_entry *universal);
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
static css_error cascade_style(css_select_ctx *ctx, css_style *style, 
		uint32_t specificity, css_origin origin, uint32_t rule_index,
		css_select_state *state);

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
 * \param result          Pointer to style to populate
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

	if (ctx == NULL || node == NULL || result == NULL)
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
	 * Those properties which are inherited, need to be set as inherit.
	 * Those which are not inherited need to be set to their default value.
	 */
	/** \todo fixup unset properties */

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
			state->current_sheet++;

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

css_error match_selectors_in_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet, css_select_state *state)
{
	const parserutils_hash_entry *universal;
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

	/* Intern universal selector string */
	perror = parserutils_hash_insert(sheet->dictionary, 
			(const uint8_t *) "*", 1, &universal);
	if (perror != PARSERUTILS_OK)
		return css_error_from_parserutils_error(perror);

	/* Find hash chain that applies to current node */
	error = css_selector_hash_find(sheet->selectors, element, &selectors);
	if (error != CSS_OK)
		return error;

	/* Process any matching selectors */
	while (*selectors != NULL) {
		error = match_selector_chain(ctx, *selectors, 
				sheet->origin, state, universal);
		if (error != CSS_OK)
			return error;

		error = css_selector_hash_iterate(sheet->selectors, selectors,
				&selectors);
		if (error != CSS_OK)
			return error;
	}

	/* Find hash chain for universal selector */
	error = css_selector_hash_find(sheet->selectors, universal, &selectors);
	if (error != CSS_OK)
		return error;

	/* Process any matching selectors */
	while (*selectors != NULL) {
		error = match_selector_chain(ctx, *selectors, 
				sheet->origin, state, universal);
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
		const css_selector *selector, css_origin origin, 
		css_select_state *state, 
		const parserutils_hash_entry *universal)
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
				s->combinator->data.name != universal) {
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
				s->combinator->data.name == universal) {
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
	return cascade_style(ctx,
			((css_rule_selector *) selector->rule)->style, 
			selector->specificity, origin, selector->rule->index,
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

		/* Match its details */
		error = match_details(ctx, n, detail, state, &match);
		if (error != CSS_OK)
			return error;

		/* If we found a match, use it */
		if (match == true)
			break;

		/* Wanted the parent, but it didn't match, so stop looking */
		if (type == CSS_COMBINATOR_PARENT)
			n = NULL;
	} while (n != NULL);

	*next_node = n;

	return CSS_OK;
}

css_error match_details(css_select_ctx *ctx, void *node, 
		const css_selector_detail *detail, css_select_state *state, 
		bool *match)
{
	css_error error;

	/** \todo Some details are easier to test than others (e.g. dashmatch 
	 * actually requires looking at data rather than simply comparing 
	 * pointers). Should we consider sorting the detail list such that the 
	 * simpler details come first (and thus the expensive match routines 
	 * can be avoided unless absolutely necessary). */

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
	case CSS_SELECTOR_PSEUDO:
		/** \todo pseudo classes/elements */
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

css_error cascade_style(css_select_ctx *ctx, css_style *style, 
		uint32_t specificity, css_origin origin, uint32_t rule_index,
		css_select_state *state)
{
	UNUSED(ctx);
	UNUSED(style);
	UNUSED(specificity);
	UNUSED(origin);
	UNUSED(rule_index);
	UNUSED(state);

	return CSS_OK;
}

