/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_stylesheet_h_
#define libcss_stylesheet_h_

#include <libcss/errors.h>
#include <libcss/types.h>

css_error css_stylesheet_create(css_language_level level,
		const char *charset, const char *url, const char *title,
		css_origin origin, uint64_t media, bool allow_quirks,
		lwc_context *dict, css_allocator_fn alloc, void *alloc_pw, 
		css_stylesheet **stylesheet);
css_error css_stylesheet_destroy(css_stylesheet *sheet);

css_error css_stylesheet_append_data(css_stylesheet *sheet,
		const uint8_t *data, size_t len);
css_error css_stylesheet_data_done(css_stylesheet *sheet);

css_error css_stylesheet_next_pending_import(css_stylesheet *parent,
		lwc_string **url, uint64_t *media);
css_error css_stylesheet_register_import(css_stylesheet *parent,
		css_stylesheet *child);

css_error css_stylesheet_get_language_level(css_stylesheet *sheet,
		css_language_level *level);
css_error css_stylesheet_get_url(css_stylesheet *sheet, const char **url);
css_error css_stylesheet_get_title(css_stylesheet *sheet, const char **title);
css_error css_stylesheet_get_origin(css_stylesheet *sheet, css_origin *origin);
css_error css_stylesheet_get_media(css_stylesheet *sheet, uint64_t *media);
css_error css_stylesheet_quirks_allowed(css_stylesheet *sheet, bool *allowed);
css_error css_stylesheet_used_quirks(css_stylesheet *sheet, bool *quirks);

css_error css_stylesheet_get_disabled(css_stylesheet *sheet, bool *disabled);
css_error css_stylesheet_set_disabled(css_stylesheet *sheet, bool disabled);

#endif

