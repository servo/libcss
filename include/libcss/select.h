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
