/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "parse/css21.h"
#include "parse/parse.h"

#include "utils/utils.h"

/**
 * Context for a CSS 2.1 parser
 */
struct css_css21 {
	css_stylesheet *sheet;		/**< The stylesheet to parse for */
	css_parser *parser;		/**< The underlying core parser */

	css_alloc alloc;		/**< Memory (de)allocation function */
	void *pw;			/**< Client's private data */
};

static bool css21_handle_event(css_parser_event type, 
		const parserutils_vector *tokens, void *pw);

/**
 * Create a CSS 2.1 parser
 *
 * \param sheet   The stylesheet object to parse for
 * \param parser  The core parser object to use
 * \param alloc   Memory (de)allocation function
 * \param pw      Pointer to client-specific private data
 * \return Pointer to parser object, or NULL on failure
 */
css_css21 *css_css21_create(css_stylesheet *sheet, css_parser *parser,
		css_alloc alloc, void *pw)
{
	css_css21 *css21;
	css_parser_optparams params;
	css_error error;

	if (sheet == NULL || parser == NULL || alloc == NULL)
		return NULL;

	css21 = alloc(NULL, sizeof(css_css21), pw);
	if (css21 == NULL)
		return NULL;

	params.event_handler.handler = css21_handle_event;
	params.event_handler.pw = css21;
	error = css_parser_setopt(parser, CSS_PARSER_EVENT_HANDLER, &params);
	if (error != CSS_OK) {
		alloc(css21, 0, pw);
		return NULL;
	}

	css21->sheet = sheet;
	css21->parser = parser;
	css21->alloc = alloc;
	css21->pw = pw;

	return css21;
}

/**
 * Destroy a CSS 2.1 parser
 *
 * \param css21  The parser to destroy
 */
void css_css21_destroy(css_css21 *css21)
{
	if (css21 == NULL)
		return;

	css21->alloc(css21, 0, css21->pw);
}

/**
 * Handler for core parser events
 *
 * \param type    The event type
 * \param tokens  Vector of tokens read since last event, or NULL
 * \param pw      Pointer to handler context
 * \return False to signal parse error, true otherwise
 */
bool css21_handle_event(css_parser_event type, const parserutils_vector *tokens,
		void *pw)
{
	UNUSED(type);
	UNUSED(tokens);
	UNUSED(pw);

	return true;
}

