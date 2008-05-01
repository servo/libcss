/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_h_
#define css_h_

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/types.h>

/* Initialise the CSS library for use */
css_error css_initialise(const char *aliases_file,
		css_alloc alloc, void *pw);

/* Clean up after LibCSS */
css_error css_finalise(css_alloc alloc, void *pw);

#endif

