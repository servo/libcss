/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <stdbool.h>
#include <string.h>

#include <parserutils/charset/mibenum.h>

#include "charset/detect.h"
#include "utils/utils.h"

static parserutils_error css_charset_read_bom_or_charset(const uint8_t *data, 
		size_t len, uint16_t *mibenum);

/**
 * Extract a charset from a chunk of data
 *
 * \param data     Pointer to buffer containing data
 * \param len      Buffer length
 * \param mibenum  Pointer to location containing current MIB enum
 * \param source   Pointer to location containing current charset source
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 *
 * ::mibenum and ::source will be updated on exit
 *
 * CSS 2.1 $4.4
 */
parserutils_error css_charset_extract(const uint8_t *data, size_t len,
		uint16_t *mibenum, uint32_t *source)
{
	css_error error;
	uint16_t charset = 0;

	if (data == NULL || mibenum == NULL || source == NULL)
		return PARSERUTILS_BADPARM;

	/* If the charset was dictated by the client, we've nothing to detect */
	if (*source == CSS_CHARSET_DICTATED)
		return PARSERUTILS_OK;

	/* We need at least 4 bytes of data */
	if (len < 4)
		goto default_encoding;

	/* Look for a BOM and/or @charset */
	error = css_charset_read_bom_or_charset(data, len, &charset);
	if (error != PARSERUTILS_OK)
		return error;

	if (charset != 0) {
		*mibenum = charset;
		*source = CSS_CHARSET_DOCUMENT;

		return PARSERUTILS_OK;
	}

	/* If we've already got a charset from the linking mechanism or 
	 * referring document, then we've nothing further to do */
	if (*source != CSS_CHARSET_DEFAULT)
		return PARSERUTILS_OK;

	/* We've not yet found a charset, so use the default fallback */
default_encoding:

	charset = parserutils_charset_mibenum_from_name("UTF-8", SLEN("UTF-8"));

	*mibenum = charset;
	*source = CSS_CHARSET_DEFAULT;

	return PARSERUTILS_OK;
}


/**
 * Inspect the beginning of a buffer of data for the presence of a
 * UTF Byte Order Mark and/or an @charset rule
 *
 * \param data     Pointer to buffer containing data
 * \param len      Buffer length
 * \param mibenum  Pointer to location to receive MIB enum
 * \return PARSERUTILS_OK on success, appropriate error otherwise
 */
parserutils_error css_charset_read_bom_or_charset(const uint8_t *data, 
		size_t len, uint16_t *mibenum)
{
	uint16_t charset = 0;

	if (data == NULL)
		return PARSERUTILS_BADPARM;

	/* We require at least 4 bytes of data */
	if (len < 4)
		return PARSERUTILS_NEEDDATA;


	/* Look for BOM */
	if (data[0] == 0x00 && data[1] == 0x00 && 
			data[2] == 0xFE && data[3] == 0xFF) {
		charset = parserutils_charset_mibenum_from_name("UTF-32BE", 
				SLEN("UTF-32BE"));
	} else if (data[0] == 0xFF && data[1] == 0xFE &&
			data[2] == 0x00 && data[3] == 0x00) {
		charset = parserutils_charset_mibenum_from_name("UTF-32LE", 
				SLEN("UTF-32LE"));
	} else if (data[0] == 0xFE && data[1] == 0xFF) {
		charset = parserutils_charset_mibenum_from_name("UTF-16BE", 
				SLEN("UTF-16BE"));
	} else if (data[0] == 0xFF && data[1] == 0xFE) {
		charset = parserutils_charset_mibenum_from_name("UTF-16LE", 
				SLEN("UTF-16LE"));
	} else if (data[0] == 0xEF && data[1] == 0xBB && data[2] == 0xBF) {
		charset = parserutils_charset_mibenum_from_name("UTF-8", 
				SLEN("UTF-8"));
	}

	/* BOM beats @charset.
	 * UAs differ here, but none appear to match the spec. 
	 * The spec indicates that any @charset present in conjunction with a 
	 * BOM, should match the BOM. In reality, it appears UAs just take the 
	 * BOM as gospel and ignore any @charset rule. The w3c CSS validator 
	 * appears to do the same (at the least, it doesn't complain about a
	 * mismatch).
	 */
	if (charset != 0) {
		*mibenum = charset;
		return PARSERUTILS_OK;
	}

	/** \todo UTF-32 and UTF-16 @charset support */

	/* Look for @charset, assuming ASCII-compatible source data */
	if (len > 10 &&	strncmp((const char *) data, "@charset \"", 
			SLEN("@charset \"")) == 0) {
		const uint8_t *end;

		/* Look for "; at end of charset declaration */
		for (end = data + 10; end < data + len; end++) {
			if (*end == '"' && end < data + len - 1 && 
					*(end + 1) == ';')
				break;
		}

		if (end == data + len) {
			/* Ran out of input */
			return PARSERUTILS_NEEDDATA;
		}

		/* Convert to MIB enum */
		charset = parserutils_charset_mibenum_from_name(
				(char *) data + 10,  end - data - 10);
	}

	*mibenum = charset;

	return PARSERUTILS_OK;
}

