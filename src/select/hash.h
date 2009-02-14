/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_hash_h_
#define css_select_hash_h_

#include <parserutils/utils/hash.h>

#include <libcss/errors.h>
#include <libcss/functypes.h>

/* Ugh. We need this to avoid circular includes. Happy! */
struct css_selector;

typedef struct css_selector_hash css_selector_hash;

css_error css_selector_hash_create(css_allocator_fn alloc, void *pw, 
		css_selector_hash **hash);
css_error css_selector_hash_destroy(css_selector_hash *hash);

css_error css_selector_hash_insert(css_selector_hash *hash,
		const struct css_selector *selector);
css_error css_selector_hash_remove(css_selector_hash *hash,
		const struct css_selector *selector);

css_error css_selector_hash_find(css_selector_hash *hash,
		const parserutils_hash_entry *name,
		const struct css_selector ***matched);
css_error css_selector_hash_iterate(css_selector_hash *hash,
		const struct css_selector **current, 
		const struct css_selector ***next);

#endif

