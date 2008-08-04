/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include <parserutils/utils/stack.h>

#include "lex/lex.h"
#include "parse/css21.h"
#include "parse/parse.h"

#include "utils/parserutilserror.h"
#include "utils/utils.h"

/**
 * Context for a CSS 2.1 parser
 */
struct css_css21 {
	css_stylesheet *sheet;		/**< The stylesheet to parse for */
	css_parser *parser;		/**< The underlying core parser */

#define STACK_CHUNK 32
	parserutils_stack *context;	/**< Context stack */

	enum {
		BEFORE_CHARSET,
		AFTER_CHARSET,
		HAD_RULE,
	} state;			/**< State flag, for at-rule handling */

	css_alloc alloc;		/**< Memory (de)allocation function */
	void *pw;			/**< Client's private data */
};

static css_error css21_handle_event(css_parser_event type, 
		const parserutils_vector *tokens, void *pw);
static inline css_error handleStartStylesheet(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleEndStylesheet(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleStartRuleset(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleEndRuleset(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleStartAtRule(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleEndAtRule(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleStartBlock(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleEndBlock(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleBlockContent(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleSelector(css_css21 *c, 
		const parserutils_vector *vector);
static inline css_error handleDeclaration(css_css21 *c, 
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

	css21->context = parserutils_stack_create(sizeof(css_parser_event), 
			STACK_CHUNK, (parserutils_alloc) alloc, pw);
	if (css21->context == NULL) {
		alloc(css21, 0, pw);
		return NULL;
	}

	params.event_handler.handler = css21_handle_event;
	params.event_handler.pw = css21;
	error = css_parser_setopt(parser, CSS_PARSER_EVENT_HANDLER, &params);
	if (error != CSS_OK) {
		parserutils_stack_destroy(css21->context);
		alloc(css21, 0, pw);
		return NULL;
	}

	css21->sheet = sheet;
	css21->parser = parser;
	css21->state = BEFORE_CHARSET;
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

	parserutils_stack_destroy(css21->context);

	css21->alloc(css21, 0, css21->pw);
}

/**
 * Handler for core parser events
 *
 * \param type    The event type
 * \param tokens  Vector of tokens read since last event, or NULL
 * \param pw      Pointer to handler context
 * \return CSS_OK on success, CSS_INVALID to indicate parse error, 
 *         appropriate error otherwise.
 */
css_error css21_handle_event(css_parser_event type, 
		const parserutils_vector *tokens, void *pw)
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

	return CSS_OK;
}

/******************************************************************************
 * Parser stages                                                              *
 ******************************************************************************/

css_error handleStartStylesheet(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event type = CSS_PARSER_START_STYLESHEET;

	UNUSED(vector);

	assert(c != NULL);

	perror = parserutils_stack_push(c->context, (void *) &type);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndStylesheet(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event *type;

	UNUSED(vector);

	assert(c != NULL);

	type = parserutils_stack_get_current(c->context);
	if (type == NULL || *type != CSS_PARSER_START_STYLESHEET)
		return CSS_INVALID;

	perror = parserutils_stack_pop(c->context, NULL);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleStartRuleset(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event type = CSS_PARSER_START_RULESET;

	UNUSED(vector);

	assert(c != NULL);

	perror = parserutils_stack_push(c->context, (void *) &type);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndRuleset(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event *type;

	UNUSED(vector);

	assert(c != NULL);

	type = parserutils_stack_get_current(c->context);
	if (type == NULL || *type != CSS_PARSER_START_RULESET)
		return CSS_INVALID;

	perror = parserutils_stack_pop(c->context, NULL);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleStartAtRule(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event type = CSS_PARSER_START_ATRULE;

	UNUSED(vector);

	assert(c != NULL);

	perror = parserutils_stack_push(c->context, (void *) &type);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	/* vector contains: ATKEYWORD ws any0 */
	const css_token *token = NULL;
	const css_token *atkeyword = NULL;
	int32_t any = 0;
	int32_t ctx = 0;

	do {
		any = ctx;

		token = parserutils_vector_iterate(vector, &ctx);
		if (token == NULL)
			break;

		if (atkeyword == NULL)
			atkeyword = token;
		else if (token->type != CSS_TOKEN_S)
			break;
	} while (token != NULL);

	/* We now have an ATKEYWORD and the context for the start of any0, if 
	 * there is one */
	assert(atkeyword != NULL && atkeyword->type == CSS_TOKEN_ATKEYWORD);

	/** \todo Erm. Strings are interned now. Stop looking at their data */
	if (atkeyword->data.len == SLEN("charset") && 
			strncasecmp((const char *) atkeyword->data.ptr, 
				"charset", SLEN("charset")) == 0) {
		if (c->state == BEFORE_CHARSET) {
			/* any0 = STRING */
			if (any == 0)
				return CSS_INVALID;

			token = parserutils_vector_iterate(vector, &any);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			token = parserutils_vector_iterate(vector, &any);
			if (token != NULL)
				return CSS_INVALID;

			c->state = AFTER_CHARSET;
		} else {
			return CSS_INVALID;
		}
	} else if (atkeyword->data.len == SLEN("import") &&
			strncasecmp((const char *) atkeyword->data.ptr,
				"import", SLEN("import")) == 0) {
		/** \todo any0 = (STRING | URI) ws 
		 *               (IDENT ws (',' ws IDENT ws)* )? */
	} else if (atkeyword->data.len == SLEN("media") &&
			strncasecmp((const char *) atkeyword->data.ptr,
				"media", SLEN("media")) == 0) {
		/** \todo any0 = IDENT ws (',' ws IDENT ws)* */
	} else if (atkeyword->data.len == SLEN("page") &&
			strncasecmp((const char *) atkeyword->data.ptr,
				"page", SLEN("page")) == 0) {
		/** \todo any0 = (':' IDENT)? ws */
	} else {
		return CSS_INVALID;
	}

	return CSS_OK;
}

css_error handleEndAtRule(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event *type;

	UNUSED(vector);

	assert(c != NULL);

	type = parserutils_stack_get_current(c->context);
	if (type == NULL || *type != CSS_PARSER_START_ATRULE)
		return CSS_INVALID;

	perror = parserutils_stack_pop(c->context, NULL);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleStartBlock(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event type = CSS_PARSER_START_BLOCK;

	UNUSED(vector);

	assert(c != NULL);

	perror = parserutils_stack_push(c->context, (void *) &type);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndBlock(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	css_parser_event *type;

	UNUSED(vector);

	assert(c != NULL);

	type = parserutils_stack_get_current(c->context);
	if (type == NULL || *type != CSS_PARSER_START_BLOCK)
		return CSS_INVALID;

	perror = parserutils_stack_pop(c->context, NULL);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleBlockContent(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return CSS_OK;
}

css_error handleSelector(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return CSS_OK;
}

css_error handleDeclaration(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	return CSS_OK;
}

