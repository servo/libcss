/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include <libcss/errors.h>

/**
 * Convert a LibCSS error code to a string
 *
 * \param error  The error code to convert
 * \return Pointer to string representation of error, or NULL if unknown.
 */
const char *css_error_to_string(css_error error)
{
	const char *result = NULL;

	switch (error) {
	case CSS_OK:
		result = "No error";
		break;
	case CSS_NOMEM:
		result = "Insufficient memory";
		break;
	case CSS_BADPARM:
		result = "Bad parameter";
		break;
	case CSS_INVALID:
		result = "Invalid input";
		break;
	case CSS_FILENOTFOUND:
		result = "File not found";
		break;
	case CSS_NEEDDATA:
		result = "Insufficient data";
		break;
	case CSS_BADCHARSET:
		result = "BOM and @charset mismatch";
		break;
	case CSS_EOF:
		result = "EOF encountered";
		break;
	}

	return result;
}

/**
 * Convert a string representation of an error name to a LibCSS error code
 *
 * \param str  String containing error name
 * \param len  Length of string (bytes)
 * \return LibCSS error code, or CSS_OK if unknown
 */
css_error css_error_from_string(const char *str, size_t len)
{
	if (strncmp(str, "CSS_OK", len) == 0) {
		return CSS_OK;
	} else if (strncmp(str, "CSS_NOMEM", len) == 0) {
		return CSS_NOMEM;
	} else if (strncmp(str, "CSS_BADPARM", len) == 0) {
		return CSS_BADPARM;
	} else if (strncmp(str, "CSS_INVALID", len) == 0) {
		return CSS_INVALID;
	} else if (strncmp(str, "CSS_FILENOTFOUND", len) == 0) {
		return CSS_FILENOTFOUND;
	} else if (strncmp(str, "CSS_NEEDDATA", len) == 0) {
		return CSS_NEEDDATA;
	} else if (strncmp(str, "CSS_BADCHARSET", len) == 0) {
		return CSS_BADCHARSET;
	} else if (strncmp(str, "CSS_EOF", len) == 0) {
		return CSS_EOF;
	}

	return CSS_OK;
}
