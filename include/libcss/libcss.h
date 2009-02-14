/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_h_
#define libcss_h_

#include <libcss/errors.h>
#include <libcss/types.h>
#include <libcss/functypes.h>

/* Initialise the CSS library for use */
css_error css_initialise(const char *aliases_file,
		css_alloc alloc, void *pw);

/* Clean up after LibCSS */
css_error css_finalise(css_alloc alloc, void *pw);

#endif

