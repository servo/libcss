/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007-8 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_functypes_h_
#define css_functypes_h_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <libcss/types.h>

/* Type of allocation function for libcss */
typedef void *(*css_alloc)(void *ptr, size_t size, void *pw);

/**
 * Type of parse error handling function
 */
typedef void (*css_error_handler)(uint32_t line, uint32_t col,
		const char *message, void *pw);

#endif

