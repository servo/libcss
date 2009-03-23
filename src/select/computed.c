/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include <libcss/computed.h>

#include "select/dispatch.h"
#include "utils/utils.h"

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

		if (style->uncommon->quotes != NULL)
			style->alloc(style->uncommon->quotes, 0, style->pw);

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

	style->alloc(style, 0, style->pw);

	return CSS_OK;
}

/**
 * Compose two computed styles
 *
 * \param parent  Parent style
 * \param child   Child style
 * \param result  Pointer to style to compose into
 * \return CSS_OK on success, appropriate error otherwise.
 *
 * Precondition: Parent is a fully composed style (thus has no properties
 * marked as inherited)
 */
css_error css_computed_style_compose(const css_computed_style *parent,
		const css_computed_style *child,
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

	return error;
}

