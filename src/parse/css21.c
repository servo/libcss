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
static inline bool handleStartStylesheet(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleEndStylesheet(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleStartRuleset(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleEndRuleset(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleStartAtRule(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleEndAtRule(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleStartBlock(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleEndBlock(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleBlockContent(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleSelector(css_css21 *c, 
		const parserutils_vector *vector);
static inline bool handleDeclaration(css_css21 *c, 
		const parserutils_vector *vector);

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
	css_css21 *css21 = (css_css21 *) pw;

	switch (type) {
	case CSS_PARSER_START_STYLESHEET:
		return handleStartStylesheet(css21, tokens);
	case CSS_PARSER_END_STYLESHEET:
		return handleEndStylesheet(css21, tokens);
	case CSS_PARSER_START_RULESET:
		return handleStartRuleset(css21, tokens);
	case CSS_PARSER_END_RULESET:
		return handleEndRuleset(css21, tokens);
	case CSS_PARSER_START_ATRULE:
		return handleStartAtRule(css21, tokens);
	case CSS_PARSER_END_ATRULE:
		return handleEndAtRule(css21, tokens);
	case CSS_PARSER_START_BLOCK:
		return handleStartBlock(css21, tokens);
	case CSS_PARSER_END_BLOCK:
		return handleEndBlock(css21, tokens);
	case CSS_PARSER_BLOCK_CONTENT:
		return handleBlockContent(css21, tokens);
	case CSS_PARSER_SELECTOR:
		return handleSelector(css21, tokens);
	case CSS_PARSER_DECLARATION:
		return handleDeclaration(css21, tokens);
	}

	return true;
}

/******************************************************************************
 * Parser stages                                                              *
 ******************************************************************************/

bool handleStartStylesheet(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleEndStylesheet(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleStartRuleset(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleEndRuleset(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleStartAtRule(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleEndAtRule(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleStartBlock(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleEndBlock(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleBlockContent(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleSelector(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

bool handleDeclaration(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return true;
}

