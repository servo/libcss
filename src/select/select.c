/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include <libcss/computed.h>
#include <libcss/select.h>

#include "stylesheet.h"
#include "select/hash.h"
#include "select/propset.h"
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

/** \todo We need a better way of knowing the number of properties
 * Bytecode opcodes cover 84 properties, then there's a 
 * further 15 generated from the side bits */
/* Stylesheet identity is a monotonically increasing number based at 1 and
 * increasing by 1 for every stylesheet encountered, including imports.
 * Imported sheets' identities are below that of the sheet that imported
 * them. */
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
		uint64_t media,	css_computed_style *result)
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
	const css_rule *rule;
	css_error error;

	/* Find first non-charset rule */
	for (rule = sheet->rule_list; rule != NULL; rule = rule->next) {
		if (rule->type != CSS_RULE_CHARSET)
			break;
	}

	/* Process imports, if we have any */
	while (rule != NULL && rule->type == CSS_RULE_IMPORT) {
		const css_rule_import *import = (const css_rule_import *) rule;

		if (import->sheet != NULL &&
				(import->sheet->media & state->media) != 0) {
			/** \todo We really don't want to recurse here */
			error = select_from_sheet(ctx, import->sheet, state);
			if (error != CSS_OK)
				return error;
		}

		rule = rule->next;
	}

	/** \todo Finally, process the selectors in this sheet */

	return CSS_OK;
}

