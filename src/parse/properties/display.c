/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

/**
 * Parse display
 *
 * \param c       Parsing context
 * \param vector  Vector of tokens to process
 * \param ctx     Pointer to vector iteration context
 * \param result  Pointer to location to receive resulting style
 * \return CSS_OK on success,
 *         CSS_NOMEM on memory exhaustion,
 *         CSS_INVALID if the input is not valid
 *
 * Post condition: \a *ctx is updated with the next token to process
 *                 If the input is invalid, then \a *ctx remains unchanged.
 */
css_error parse_display(css_language *c, 
		const parserutils_vector *vector, int *ctx, 
		css_style **result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *ident;
	uint8_t flags = 0;
	uint16_t value = 0;
	uint32_t opv;
	bool match;

	/* IDENT (inline, block, list-item, run-in, inline-block, table,
	 * inline-table, table-row-group, table-header-group, 
	 * table-footer-group, table-row, table-column-group, table-column,
	 * table-cell, table-caption, none, inherit) */
	ident = parserutils_vector_iterate(vector, ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[INHERIT],
			&match) == lwc_error_ok && match)) {
		flags |= FLAG_INHERIT;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[INLINE],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_INLINE;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[BLOCK],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_BLOCK;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[LIST_ITEM],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_LIST_ITEM;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[RUN_IN],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_RUN_IN;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[INLINE_BLOCK],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_INLINE_BLOCK;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[INLINE_TABLE],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_INLINE_TABLE;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_ROW_GROUP],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_ROW_GROUP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_HEADER_GROUP],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_HEADER_GROUP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_FOOTER_GROUP],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_FOOTER_GROUP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_ROW],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_ROW;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_COLUMN_GROUP],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_COLUMN_GROUP;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_COLUMN],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_COLUMN;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_CELL],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_CELL;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[TABLE_CAPTION],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_TABLE_CAPTION;
	} else if ((lwc_string_caseless_isequal(
			ident->idata, c->strings[NONE],
			&match) == lwc_error_ok && match)) {
		value = DISPLAY_NONE;
	} else {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	opv = buildOPV(CSS_PROP_DISPLAY, flags, value);

	/* Allocate result */
	error = css_stylesheet_style_create(c->sheet, sizeof(opv), result);
	if (error != CSS_OK) {
		*ctx = orig_ctx;
		return error;
	}

	/* Copy the bytecode to it */
	memcpy((*result)->bytecode, &opv, sizeof(opv));

	return CSS_OK;
}
