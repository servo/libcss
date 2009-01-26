/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include <libcss/select.h>

#include "stylesheet.h"
#include "select/hash.h"
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

css_error css_select_style(css_select_ctx *ctx, void *node,
		uint32_t pseudo_classes, uint32_t media,
		css_computed_style **result)
{
	UNUSED(ctx);
	UNUSED(node);
	UNUSED(pseudo_classes);
	UNUSED(media);
	UNUSED(result);

	/** \todo implement */

	return CSS_OK;
}

