/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include <parserutils/utils/stack.h>

#include "stylesheet.h"
#include "lex/lex.h"
#include "parse/css21.h"
#include "parse/parse.h"

#include "utils/parserutilserror.h"
#include "utils/utils.h"

enum {
	CHARSET, IMPORT, MEDIA, PAGE,

	LAST_KNOWN
};

static struct {
	const char *ptr;
	size_t len;
} stringmap[] = {
	{ "charset", SLEN("charset") },
	{ "import", SLEN("import") },
	{ "media", SLEN("media") },
	{ "page", SLEN("page") },
};

typedef struct context_entry {
	css_parser_event type;		/**< Type of entry */
	const uint8_t *data;		/**< Pointer to interned string */
} context_entry;

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
		BEFORE_RULES,
		HAD_RULE,
	} state;			/**< State flag, for at-rule handling */

	const uint8_t *strings[LAST_KNOWN];	/**< Interned strings */

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
static inline css_error handleSelectorList(css_css21 *c, 
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

	css21->context = parserutils_stack_create(sizeof(context_entry), 
			STACK_CHUNK, (parserutils_alloc) alloc, pw);
	if (css21->context == NULL) {
		alloc(css21, 0, pw);
		return NULL;
	}

	/* Intern all known strings */
	for (int i = 0; i < LAST_KNOWN; i++) {
		css21->strings[i] = css_parser_dict_add(parser,
				(const uint8_t *) stringmap[i].ptr, 
				stringmap[i].len);
		if (css21->strings[i] == NULL) {
			parserutils_stack_destroy(css21->context);
			alloc(css21, 0, pw);
			return NULL;
		}
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
	context_entry entry = { CSS_PARSER_START_STYLESHEET, NULL };

	UNUSED(vector);

	assert(c != NULL);

	perror = parserutils_stack_push(c->context, (void *) &entry);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndStylesheet(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	context_entry *entry;

	UNUSED(vector);

	assert(c != NULL);

	entry = parserutils_stack_get_current(c->context);
	if (entry == NULL || entry->type != CSS_PARSER_START_STYLESHEET)
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
	css_error error;
	context_entry entry = { CSS_PARSER_START_RULESET, NULL };

	assert(c != NULL);

	error = handleSelectorList(c, vector);
	if (error != CSS_OK)
		return error;

	perror = parserutils_stack_push(c->context, (void *) &entry);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndRuleset(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	context_entry *entry;

	UNUSED(vector);

	assert(c != NULL);

	entry = parserutils_stack_get_current(c->context);
	if (entry == NULL || entry->type != CSS_PARSER_START_RULESET)
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
	context_entry entry = { CSS_PARSER_START_ATRULE, NULL };

	assert(c != NULL);

	/* vector contains: ATKEYWORD ws any0 */
	const css_token *token = NULL;
	const css_token *atkeyword = NULL;
	int32_t any = 0;
	int32_t ctx = 0;

	atkeyword = parserutils_vector_iterate(vector, &ctx);

	/* Skip whitespace */	
	do {
		any = ctx;
		token = parserutils_vector_iterate(vector, &ctx);
	} while (token != NULL && token->type == CSS_TOKEN_S);

	/* We now have an ATKEYWORD and the context for the start of any0, if 
	 * there is one */
	assert(atkeyword != NULL && atkeyword->type == CSS_TOKEN_ATKEYWORD);

	if (atkeyword->lower.ptr == c->strings[CHARSET]) {
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

			c->state = BEFORE_RULES;
		} else {
			return CSS_INVALID;
		}
	} else if (atkeyword->lower.ptr == c->strings[IMPORT]) {
		if (c->state != HAD_RULE) {
			/* any0 = (STRING | URI) ws 
			 *        (IDENT ws (',' ws IDENT ws)* )? */
			const css_token *uri = 
				parserutils_vector_iterate(vector, &any);
			if (uri == NULL || (token->type != CSS_TOKEN_STRING && 
					token->type != CSS_TOKEN_URI))
				return CSS_INVALID;

			/* Whitespace */
			do {
				token = parserutils_vector_iterate(vector, 
						&any);
				if (token == NULL || token->type != CSS_TOKEN_S)
					break;
			} while (token != NULL);

			/** \todo Media list */
			if (token != NULL) {
			}

			/** \todo trigger fetch of imported sheet */

			c->state = BEFORE_RULES;
		} else {
			return CSS_INVALID;
		}
#if 0
	/** \todo these depend on nested block support, so we'll disable them
	 * until we have such a thing. This means that we'll ignore the entire
	 * at-rule until then */
	} else if (atkeyword->lower.ptr == c->strings[MEDIA]) {
		/** \todo any0 = IDENT ws (',' ws IDENT ws)* */
	} else if (atkeyword->lower.ptr == c->strings[PAGE]) {
		/** \todo any0 = (':' IDENT)? ws */
#endif
	} else {
		return CSS_INVALID;
	}

	entry.data = atkeyword->lower.ptr;

	perror = parserutils_stack_push(c->context, (void *) &entry);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleEndAtRule(css_css21 *c, const parserutils_vector *vector)
{
	parserutils_error perror;
	context_entry *entry;

	UNUSED(vector);

	assert(c != NULL);

	entry = parserutils_stack_get_current(c->context);
	if (entry == NULL || entry->type != CSS_PARSER_START_ATRULE)
		return CSS_INVALID;

	perror = parserutils_stack_pop(c->context, NULL);
	if (perror != PARSERUTILS_OK) {
		return css_error_from_parserutils_error(perror);
	}

	return CSS_OK;
}

css_error handleStartBlock(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	/* We don't care about blocks. In CSS2.1 they're always attached to 
	 * rulesets or at-rules. */

	return CSS_OK;
}

css_error handleEndBlock(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	/* We don't care about blocks. In CSS 2.1 they're always attached to 
	 * rulesets or at-rules. */

	return CSS_OK;
}

css_error handleBlockContent(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	/* In CSS 2.1, block content comprises either declarations (if the 
	 * current block is associated with @page or a selector), or rulesets 
	 * (if the current block is associated with @media). */

	/** \todo implement nested blocks */

	return CSS_OK;
}

static bool tokenIsChar(const css_token *token, uint8_t c)
{
	return token != NULL && token->type == CSS_TOKEN_CHAR && 
			token->lower.len == 1 && token->lower.ptr[0] == c;
}

static css_error parseSimpleSelector(css_css21 *c, const parserutils_vector *vector,
		int *ctx, css_selector **result)
{

	/* simple_selector -> element_name [ HASH | class | attrib | pseudo ]*
	 *                 -> [ HASH | class | attrib | pseudo ]+
	 * element_name    -> IDENT | '*'
	 * class           -> '.' IDENT
	 * attrib          -> '[' ws IDENT ws [ 
	 *                           [ '=' | INCLUDES | DASHMATCH ] ws 
	 *                           [ IDENT | STRING ] ws ]? ']' 
	 * pseudo          -> ':' [ IDENT | FUNCTION ws IDENT? ws ')' ]
	 */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

static css_error parseCombinator(css_css21 *c, const parserutils_vector *vector,
		int *ctx, css_combinator *result)
{
	/* combinator      -> '+' ws | '>' ws | ws1 */

	UNUSED(c);
	UNUSED(vector);
	UNUSED(ctx);
	UNUSED(result);

	return CSS_OK;
}

static css_error parseSelector(css_css21 *c, const parserutils_vector *vector, 
		int *ctx, css_selector **result)
{
	css_error error;
	const css_token *token = NULL;
	css_selector *selector = NULL;

	/* selector -> simple_selector [ combinator simple_selector ]* */

	error = parseSimpleSelector(c, vector, ctx, &selector);
	if (error != CSS_OK)
		return error;
	*result = selector;

	do {
		css_combinator comb = CSS_COMBINATOR_NONE;
		css_selector *other = NULL;

		error = parseCombinator(c, vector, ctx, &comb);
		if (error != CSS_OK)
			return error;

		error = parseSimpleSelector(c, vector, ctx, &other);
		if (error != CSS_OK)
			return error;

		*result = other;

		error = css_stylesheet_selector_combine(c->sheet,
				comb, selector, other);
		if (error != CSS_OK)
			return error;

		selector = other;
		
		token = parserutils_vector_peek(vector, *ctx);
	} while (token != NULL && tokenIsChar(token, ',') == false);

	return CSS_OK;
}

css_error handleSelectorList(css_css21 *c, const parserutils_vector *vector)
{
	css_error error;
	const css_token *token = NULL;
	css_selector *selector = NULL;
	int ctx = 0;

	UNUSED(c);
	UNUSED(vector);

	/* CSS 2.1 selector syntax:
	 *
	 * selector_list   -> selector [ ',' ws selector ]*
	 */
	do {
		error = parseSelector(c, vector, &ctx, &selector);
		if (error != CSS_OK)
			return error;

		/** \todo Finish this */

		token = parserutils_vector_peek(vector, ctx);
	} while(token != NULL);

	return CSS_OK;
}

css_error handleDeclaration(css_css21 *c, const parserutils_vector *vector)
{
	UNUSED(c);
	UNUSED(vector);

	/* Locations where declarations are permitted:
	 *
	 * + In @page
	 * + In ruleset
	 */

	/* IDENT ws ':' ws value 
	 * 
	 * In CSS 2.1, value is any1, so '{' or ATKEYWORD => parse error
	 */

	return CSS_OK;
}

