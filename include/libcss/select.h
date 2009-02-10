/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_select_h_
#define libcss_select_h_

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/types.h>

enum css_pseudo_class {
	CSS_PSEUDO_CLASS_FIRST_CHILD    = (1<<0),
	CSS_PSEUDO_CLASS_LINK           = (1<<1),
	CSS_PSEUDO_CLASS_VISITED        = (1<<2),
	CSS_PSEUDO_CLASS_HOVER          = (1<<3),
	CSS_PSEUDO_CLASS_ACTIVE         = (1<<4),
	CSS_PSEUDO_CLASS_FOCUS          = (1<<5),
	/** \todo CSS_PSEUDO_CLASS_LANG = (1<<6), */
	CSS_PSEUDO_CLASS_LEFT           = (1<<7),
	CSS_PSEUDO_CLASS_RIGHT          = (1<<8),
	CSS_PSEUDO_CLASS_FIRST          = (1<<9)
};

enum css_pseudo_element {
	CSS_PSEUDO_ELEMENT_FIRST_LINE   = (1<<0),
	CSS_PSEUDO_ELEMENT_FIRST_LETTER = (1<<1),
	CSS_PSEUDO_ELEMENT_BEFORE       = (1<<2),
	CSS_PSEUDO_ELEMENT_AFTER        = (1<<3)
};

typedef struct css_select_handler {
	css_error (*node_name)(void *pw, void *node, const uint8_t **name, 
			size_t *len);

	css_error (*named_ancestor_node)(void *pw, void *node,
			const uint8_t *name, size_t len,
			void **ancestor);
	css_error (*named_parent_node)(void *pw, void *node,
			const uint8_t *name, size_t len,
			void **parent);
	css_error (*named_sibling_node)(void *pw, void *node,
			const uint8_t *name, size_t len,
			void **sibling);

	css_error (*parent_node)(void *pw, void *node, void **parent);
	css_error (*sibling_node)(void *pw, void *node, void **sibling);

	css_error (*node_has_class)(void *pw, void *node,
			const uint8_t *name, size_t len,
			bool *match);
	css_error (*node_has_id)(void *pw, void *node,
			const uint8_t *name, size_t len,
			bool *match);
	css_error (*node_has_attribute)(void *pw, void *node,
			const uint8_t *name, size_t len,
			bool *match);
	css_error (*node_has_attribute_equal)(void *pw, void *node,
			const uint8_t *name, size_t nlen,
			const uint8_t *value, size_t vlen,
			bool *match);
	css_error (*node_has_attribute_dashmatch)(void *pw, void *node,
			const uint8_t *name, size_t nlen,
			const uint8_t *value, size_t vlen,
			bool *match);
	css_error (*node_has_attribute_includes)(void *pw, void *node,
			const uint8_t *name, size_t nlen,
			const uint8_t *value, size_t vlen,
			bool *match);
} css_select_handler;

css_error css_select_ctx_create(css_alloc alloc, void *pw,
		css_select_ctx **result);
css_error css_select_ctx_destroy(css_select_ctx *ctx);

css_error css_select_ctx_append_sheet(css_select_ctx *ctx, 
		const css_stylesheet *sheet);
css_error css_select_ctx_insert_sheet(css_select_ctx *ctx,
		const css_stylesheet *sheet, uint32_t index);
css_error css_select_ctx_remove_sheet(css_select_ctx *ctx,
		const css_stylesheet *sheet);

css_error css_select_ctx_count_sheets(css_select_ctx *ctx, uint32_t *count);
css_error css_select_ctx_get_sheet(css_select_ctx *ctx, uint32_t index,
		const css_stylesheet **sheet);

css_error css_select_style(css_select_ctx *ctx, void *node,
		uint64_t pseudo_element, uint64_t pseudo_classes,
		uint64_t media, css_computed_style *result,
		css_select_handler *handler, void *pw);

#endif
