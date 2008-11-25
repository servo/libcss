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
	/* At-rules */
	CHARSET, IMPORT, MEDIA, PAGE,

	/* Properties */
	FIRST_PROP,

	AZIMUTH = FIRST_PROP, BACKGROUND_ATTACHMENT, BACKGROUND_COLOR, 
	BACKGROUND_IMAGE, BACKGROUND_POSITION, BACKGROUND_REPEAT, 
	BORDER_BOTTOM_COLOR, BORDER_BOTTOM_STYLE, BORDER_BOTTOM_WIDTH, 
	BORDER_COLLAPSE, BORDER_LEFT_COLOR, BORDER_LEFT_STYLE, 
	BORDER_LEFT_WIDTH, BORDER_RIGHT_COLOR, BORDER_RIGHT_STYLE, 
	BORDER_RIGHT_WIDTH, BORDER_SPACING, BORDER_TOP_COLOR, BORDER_TOP_STYLE, 
	BORDER_TOP_WIDTH, BOTTOM, CAPTION_SIDE, CLEAR, CLIP, COLOR, CONTENT, 
	COUNTER_INCREMENT, COUNTER_RESET, CUE_AFTER, CUE_BEFORE, CURSOR, 
	DIRECTION, DISPLAY, ELEVATION, EMPTY_CELLS, FLOAT, FONT_FAMILY, 
	FONT_SIZE, FONT_STYLE, FONT_VARIANT, FONT_WEIGHT, HEIGHT, LEFT, 
	LETTER_SPACING, LINE_HEIGHT, LIST_STYLE_IMAGE, LIST_STYLE_POSITION, 
	LIST_STYLE_TYPE, MARGIN_BOTTOM, MARGIN_LEFT, MARGIN_RIGHT, MARGIN_TOP, 
	MAX_HEIGHT, MAX_WIDTH, MIN_HEIGHT, MIN_WIDTH, ORPHANS, OUTLINE_COLOR, 
	OUTLINE_STYLE, OUTLINE_WIDTH, OVERFLOW, PADDING_BOTTOM, PADDING_LEFT, 
	PADDING_RIGHT, PADDING_TOP, PAGE_BREAK_AFTER, PAGE_BREAK_BEFORE, 
	PAGE_BREAK_INSIDE, PAUSE_AFTER, PAUSE_BEFORE, PITCH_RANGE, PITCH, 
	PLAY_DURING, POSITION, QUOTES, RICHNESS, RIGHT, SPEAK_HEADER, 
	SPEAK_NUMERAL, SPEAK_PUNCTUATION, SPEAK, SPEECH_RATE, STRESS, 
	TABLE_LAYOUT, TEXT_ALIGN, TEXT_DECORATION, TEXT_INDENT, TEXT_TRANSFORM, 
	TOP, UNICODE_BIDI, VERTICAL_ALIGN, VISIBILITY, VOICE_FAMILY, VOLUME, 
	WHITE_SPACE, WIDOWS, WIDTH, WORD_SPACING, Z_INDEX,

	LAST_PROP = Z_INDEX,

	/* Other keywords */
	INHERIT, IMPORTANT, NONE, BOTH, FIXED, SCROLL, TRANSPARENT,
	NO_REPEAT, REPEAT_X, REPEAT_Y, REPEAT, HIDDEN, DOTTED, DASHED,
	SOLID, DOUBLE, GROOVE, RIDGE, INSET, OUTSET, THIN, MEDIUM, THICK,
	COLLAPSE, SEPARATE, AUTO, LTR, RTL, INLINE, BLOCK, LIST_ITEM, RUN_IN,
	INLINE_BLOCK, TABLE, INLINE_TABLE, TABLE_ROW_GROUP, TABLE_HEADER_GROUP,
	TABLE_FOOTER_GROUP, TABLE_ROW, TABLE_COLUMN_GROUP, TABLE_COLUMN,
	TABLE_CELL, TABLE_CAPTION, BELOW, LEVEL, ABOVE, HIGHER, LOWER,
	SHOW, HIDE, XX_SMALL, X_SMALL, SMALL, LARGE, X_LARGE, XX_LARGE,
	LARGER, SMALLER, NORMAL, ITALIC, OBLIQUE, SMALL_CAPS, BOLD, BOLDER,
	LIGHTER, INSIDE, OUTSIDE,

	LAST_KNOWN
};

/* Must be synchronised with above enum */
static struct {
	const char *ptr;
	size_t len;
} stringmap[LAST_KNOWN] = {
	{ "charset", SLEN("charset") },
	{ "import", SLEN("import") },
	{ "media", SLEN("media") },
	{ "page", SLEN("page") },

	{ "azimuth", SLEN("azimuth") },
	{ "background-attachment", SLEN("background-attachment") },
	{ "background-color", SLEN("background-color") },
	{ "background-image", SLEN("background-image") },
	{ "background-position", SLEN("background-position") },
	{ "background-repeat", SLEN("background-repeat") },
	{ "border-bottom-color", SLEN("border-bottom-color") },
	{ "border-bottom-style", SLEN("border-bottom-style") },
	{ "border-bottom-width", SLEN("border-bottom-width") },
	{ "border-collapse", SLEN("border-collapse") },
	{ "border-left-color", SLEN("border-left-color") },
	{ "border-left-style", SLEN("border-left-style") },
	{ "border-left-width", SLEN("border-left-width") },
	{ "border-right-color", SLEN("border-right-color") },
	{ "border-right-style", SLEN("border-right-style") },
	{ "border-right-width", SLEN("border-right-width") },
	{ "border-spacing", SLEN("border-spacing") },
	{ "border-top-color", SLEN("border-top-color") },
	{ "border-top-style", SLEN("border-top-style") },
	{ "border-top-width", SLEN("border-top-width") },
	{ "bottom", SLEN("bottom") },
	{ "caption-side", SLEN("caption-side") },
	{ "clear", SLEN("clear") },
	{ "clip", SLEN("clip") },
	{ "color", SLEN("color") },
	{ "content", SLEN("content") },
	{ "counter-increment", SLEN("counter-increment") },
	{ "counter-reset", SLEN("counter-reset") },
	{ "cue-after", SLEN("cue-after") },
	{ "cue-before", SLEN("cue-before") },
	{ "cursor", SLEN("cursor") },
	{ "direction", SLEN("direction") },
	{ "display", SLEN("display") },
	{ "elevation", SLEN("elevation") },
	{ "empty-cells", SLEN("empty-cells") },
	{ "float", SLEN("float") },
	{ "font-family", SLEN("font-family") },
	{ "font-size", SLEN("font-size") },
	{ "font-style", SLEN("font-style") },
	{ "font-variant", SLEN("font-variant") },
	{ "font-weight", SLEN("font-weight") },
	{ "height", SLEN("height") },
	{ "left", SLEN("left") },
	{ "letter-spacing", SLEN("letter-spacing") },
	{ "line-height", SLEN("line-height") },
	{ "list-style-image", SLEN("list-style-image") },
	{ "list-style-position", SLEN("list-style-position") },
	{ "list-style-type", SLEN("list-style-type") },
	{ "margin-bottom", SLEN("margin-bottom") },
	{ "margin-left", SLEN("margin-left") },
	{ "margin-right", SLEN("margin-right") },
	{ "margin-top", SLEN("margin-top") },
	{ "max-height", SLEN("max-height") },
	{ "max-width", SLEN("max-width") },
	{ "min-height", SLEN("min-height") },
	{ "min-width", SLEN("min-width") },
	{ "orphans", SLEN("orphans") },
	{ "outline-color", SLEN("outline-color") },
	{ "outline-style", SLEN("outline-style") },
	{ "outline-width", SLEN("outline-width") },
	{ "overflow", SLEN("overflow") },
	{ "padding-bottom", SLEN("padding-bottom") },
	{ "padding-left", SLEN("padding-left") },
	{ "padding-right", SLEN("padding-right") },
	{ "padding-top", SLEN("padding-top") },
	{ "page-break-after", SLEN("page-break-after") },
	{ "page-break-before", SLEN("page-break-before") },
	{ "page-break-inside", SLEN("page-break-inside") },
	{ "pause-after", SLEN("pause-after") },
	{ "pause-before", SLEN("pause-before") },
	{ "pitch-range", SLEN("pitch-range") },
	{ "pitch", SLEN("pitch") },
	{ "play-during", SLEN("play-during") },
	{ "position", SLEN("position") },
	{ "quotes", SLEN("quotes") },
	{ "richness", SLEN("richness") },
	{ "right", SLEN("right") },
	{ "speak-header", SLEN("speak-header") },
	{ "speak-numeral", SLEN("speak-numeral") },
	{ "speak-punctuation", SLEN("speak-punctuation") },
	{ "speak", SLEN("speak") },
	{ "speech-rate", SLEN("speech-rate") },
	{ "stress", SLEN("stress") },
	{ "table-layout", SLEN("table-layout") },
	{ "text-align", SLEN("text-align") },
	{ "text-decoration", SLEN("text-decoration") },
	{ "text-indent", SLEN("text-indent") },
	{ "text-transform", SLEN("text-transform") },
	{ "top", SLEN("top") },
	{ "unicode-bidi", SLEN("unicode-bidi") },
	{ "vertical-align", SLEN("vertical-align") },
	{ "visibility", SLEN("visibility") },
	{ "voice-family", SLEN("voice-family") },
	{ "volume", SLEN("volume") },
	{ "white-space", SLEN("white-space") },
	{ "widows", SLEN("widows") },
	{ "width", SLEN("width") },
	{ "word-spacing", SLEN("word-spacing") },
	{ "z-index", SLEN("z-index") },

	{ "inherit", SLEN("inherit") },
	{ "important", SLEN("important") },
	{ "none", SLEN("none") },
	{ "both", SLEN("both") },
	{ "fixed", SLEN("fixed") },
	{ "scroll", SLEN("scroll") },
	{ "transparent", SLEN("transparent") },
	{ "no-repeat", SLEN("no-repeat") },
	{ "repeat-x", SLEN("repeat-x") },
	{ "repeat-y", SLEN("repeat-y") },
	{ "repeat", SLEN("repeat") },
	{ "hidden", SLEN("hidden") },
	{ "dotted", SLEN("dotted") },
	{ "dashed", SLEN("dashed") },
	{ "solid", SLEN("solid") },
	{ "double", SLEN("double") },
	{ "groove", SLEN("groove") },
	{ "ridge", SLEN("ridge") },
	{ "inset", SLEN("inset") },
	{ "outset", SLEN("outset") },
	{ "thin", SLEN("thin") },
	{ "medium", SLEN("medium") },
	{ "thick", SLEN("thick") },
	{ "collapse", SLEN("collapse") },
	{ "separate", SLEN("separate") },
	{ "auto", SLEN("auto") },
	{ "ltr", SLEN("ltr") },
	{ "rtl", SLEN("rtl") },
	{ "inline", SLEN("inline") },
	{ "block", SLEN("block") },
	{ "list-item", SLEN("list-item") },
	{ "run-in", SLEN("run-in") },
	{ "inline-block", SLEN("inline-block") },
	{ "table", SLEN("table") },
	{ "inline-table", SLEN("inline-table") },
	{ "table-row-group", SLEN("table-row-group") },
	{ "table-header-group", SLEN("table-header-group") },
	{ "table-footer-group", SLEN("table-footer-group") },
	{ "table-row", SLEN("table-row") },
	{ "table-column-group", SLEN("table-column-group") },
	{ "table-column", SLEN("table-column") },
	{ "table-cell", SLEN("table-cell") },
	{ "table-caption", SLEN("table-caption") },
	{ "below", SLEN("below") },
	{ "level", SLEN("level") },
	{ "above", SLEN("above") },
	{ "higher", SLEN("higher") },
	{ "lower", SLEN("lower") },
	{ "show", SLEN("show") },
	{ "hide", SLEN("hide") },
	{ "xx-small", SLEN("xx-small") },
	{ "x-small", SLEN("x-small") },
	{ "small", SLEN("small") },
	{ "large", SLEN("large") },
	{ "x-large", SLEN("x-large") },
	{ "xx-large", SLEN("xx-large") },
	{ "larger", SLEN("larger") },
	{ "smaller", SLEN("smaller") },
	{ "normal", SLEN("normal") },
	{ "italic", SLEN("italic") },
	{ "oblique", SLEN("oblique") },
	{ "small-caps", SLEN("small-caps") },
	{ "bold", SLEN("bold") },
	{ "bolder", SLEN("bolder") },
	{ "lighter", SLEN("lighter") },
	{ "inside", SLEN("inside") },
	{ "outside", SLEN("outside") },
};

typedef struct context_entry {
	css_parser_event type;		/**< Type of entry */
	void *data;			/**< Data for context */
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

/* Event handlers */
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
static inline css_error handleDeclaration(css_css21 *c, 
		const parserutils_vector *vector);

/* Selector list parsing */
static inline css_error parseClass(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		css_selector **specific);
static inline css_error parseAttrib(css_css21 *c, 
		const parserutils_vector *vector, int *ctx,
		css_selector **specific);
static inline css_error parsePseudo(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		css_selector **specific);
static inline css_error parseSpecific(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		css_selector *parent);
static inline css_error parseSelectorSpecifics(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		css_selector *parent);
static inline css_error parseSimpleSelector(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_selector **result);
static inline css_error parseCombinator(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_combinator *result);
static inline css_error parseSelector(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_selector **result);
static inline css_error parseSelectorList(css_css21 *c, 
		const parserutils_vector *vector, css_rule *rule);

/* Declaration parsing */
static inline css_error parseProperty(css_css21 *c,
		const css_token *property, const parserutils_vector *vector,
		int *ctx, css_rule *rule);

/* Helpers */
static inline void consumeWhitespace(const parserutils_vector *vector, 
		int *ctx);
static inline bool tokenIsChar(const css_token *token, uint8_t c);

/**
 * Create a CSS 2.1 parser
 *
 * \param sheet   The stylesheet object to parse for
 * \param parser  The core parser object to use
 * \param alloc   Memory (de)allocation function
 * \param pw      Pointer to client-specific private data
 * \param css21   Pointer to location to receive parser object
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_css21_create(css_stylesheet *sheet, css_parser *parser,
		css_alloc alloc, void *pw, void **css21)
{
	css_css21 *c;
	css_parser_optparams params;
	parserutils_error perror;
	css_error error;

	if (sheet == NULL || parser == NULL || alloc == NULL || css21 == NULL)
		return CSS_BADPARM;

	c = alloc(NULL, sizeof(css_css21), pw);
	if (c == NULL)
		return CSS_NOMEM;

	perror = parserutils_stack_create(sizeof(context_entry), 
			STACK_CHUNK, (parserutils_alloc) alloc, pw, 
			&c->context);
	if (perror != PARSERUTILS_OK) {
		alloc(c, 0, pw);
		return css_error_from_parserutils_error(perror);
	}

	/* Intern all known strings */
	for (int i = 0; i < LAST_KNOWN; i++) {
		c->strings[i] = css_parser_dict_add(parser,
				(const uint8_t *) stringmap[i].ptr, 
				stringmap[i].len);
		if (c->strings[i] == NULL) {
			parserutils_stack_destroy(c->context);
			alloc(c, 0, pw);
			return CSS_NOMEM;
		}
	}

	params.event_handler.handler = css21_handle_event;
	params.event_handler.pw = c;
	error = css_parser_setopt(parser, CSS_PARSER_EVENT_HANDLER, &params);
	if (error != CSS_OK) {
		parserutils_stack_destroy(c->context);
		alloc(c, 0, pw);
		return error;
	}

	c->sheet = sheet;
	c->parser = parser;
	c->state = BEFORE_CHARSET;
	c->alloc = alloc;
	c->pw = pw;

	*css21 = c;

	return CSS_OK;
}

/**
 * Destroy a CSS 2.1 parser
 *
 * \param css21  The parser to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_css21_destroy(css_css21 *css21)
{
	if (css21 == NULL)
		return CSS_BADPARM;

	parserutils_stack_destroy(css21->context);

	css21->alloc(css21, 0, css21->pw);

	return CSS_OK;
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
	css_rule *rule = NULL;

	assert(c != NULL);

	error = css_stylesheet_rule_create(c->sheet, CSS_RULE_SELECTOR, &rule);
	if (error != CSS_OK)
		return error;

	error = parseSelectorList(c, vector, rule);
	if (error != CSS_OK) {
		css_stylesheet_rule_destroy(c->sheet, rule);
		return error;
	}

	entry.data = rule;

	perror = parserutils_stack_push(c->context, (void *) &entry);
	if (perror != PARSERUTILS_OK) {
		css_stylesheet_rule_destroy(c->sheet, rule);
		return css_error_from_parserutils_error(perror);
	}

	error = css_stylesheet_add_rule(c->sheet, rule);
	if (error != CSS_OK) {
		parserutils_stack_pop(c->context, NULL);
		css_stylesheet_rule_destroy(c->sheet, rule);
		return error;
	}

	/* Rule is now owned by the sheet, so no need to destroy it */

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
	int32_t ctx = 0;

	atkeyword = parserutils_vector_iterate(vector, &ctx);

	consumeWhitespace(vector, &ctx);

	/* We now have an ATKEYWORD and the context for the start of any0, if 
	 * there is one */
	assert(atkeyword != NULL && atkeyword->type == CSS_TOKEN_ATKEYWORD);

	if (atkeyword->lower.ptr == c->strings[CHARSET]) {
		if (c->state == BEFORE_CHARSET) {
			/* any0 = STRING */
			if (ctx == 0)
				return CSS_INVALID;

			token = parserutils_vector_iterate(vector, &ctx);
			if (token == NULL || token->type != CSS_TOKEN_STRING)
				return CSS_INVALID;

			token = parserutils_vector_iterate(vector, &ctx);
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
				parserutils_vector_iterate(vector, &ctx);
			if (uri == NULL || (uri->type != CSS_TOKEN_STRING && 
					uri->type != CSS_TOKEN_URI))
				return CSS_INVALID;

			consumeWhitespace(vector, &ctx);

			/** \todo Media list */
			if (parserutils_vector_peek(vector, ctx) != NULL) {
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

css_error handleDeclaration(css_css21 *c, const parserutils_vector *vector)
{
	css_error error;
	const css_token *token, *ident;
	int ctx = 0;
	context_entry *entry;
	css_rule *rule;

	/* Locations where declarations are permitted:
	 *
	 * + In @page
	 * + In ruleset
	 */
	entry = parserutils_stack_get_current(c->context);
	if (entry == NULL || (entry->type != CSS_PARSER_START_RULESET &&
			entry->type != CSS_PARSER_START_ATRULE))
		return CSS_INVALID;

	rule = entry->data;
	if (rule == NULL || (rule->type != CSS_RULE_SELECTOR && 
			rule->type != CSS_RULE_PAGE))
		return CSS_INVALID;

	/* IDENT ws ':' ws value 
	 * 
	 * In CSS 2.1, value is any1, so '{' or ATKEYWORD => parse error
	 */
	ident = parserutils_vector_iterate(vector, &ctx);
	if (ident == NULL || ident->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	consumeWhitespace(vector, &ctx);

	token = parserutils_vector_iterate(vector, &ctx);
	if (token == NULL || tokenIsChar(token, ':') == false)
		return CSS_INVALID;

	consumeWhitespace(vector, &ctx);

	error = parseProperty(c, ident, vector, &ctx, rule);
	if (error != CSS_OK)
		return error;

	return CSS_OK;
}

/******************************************************************************
 * Selector list parsing functions                                            *
 ******************************************************************************/

css_error parseClass(css_css21 *c, const parserutils_vector *vector, int *ctx,
		css_selector **specific)
{
	const css_token *token;

	/* class     -> '.' IDENT */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || tokenIsChar(token, '.') == false)
		return CSS_INVALID;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	return css_stylesheet_selector_create(c->sheet, 
			CSS_SELECTOR_CLASS, &token->data, NULL, specific);
}

css_error parseAttrib(css_css21 *c, const parserutils_vector *vector, int *ctx,
		css_selector **specific)
{
	const css_token *token, *name, *value = NULL;
	css_selector_type type = CSS_SELECTOR_ATTRIBUTE;

	/* attrib    -> '[' ws IDENT ws [
	 *                     [ '=' | INCLUDES | DASHMATCH ] ws
	 *                     [ IDENT | STRING ] ws ]? ']'
	 */
	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || tokenIsChar(token, '[') == false)
		return CSS_INVALID;

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || token->type != CSS_TOKEN_IDENT)
		return CSS_INVALID;

	name = token;

	consumeWhitespace(vector, ctx);

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (tokenIsChar(token, ']') == false) {
		if (tokenIsChar(token, '='))
			type = CSS_SELECTOR_ATTRIBUTE_EQUAL;
		else if (token->type == CSS_TOKEN_INCLUDES)
			type = CSS_SELECTOR_ATTRIBUTE_INCLUDES;
		else if (token->type == CSS_TOKEN_DASHMATCH)
			type = CSS_SELECTOR_ATTRIBUTE_DASHMATCH;
		else
			return CSS_INVALID;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_iterate(vector, ctx);
		if (token == NULL || (token->type != CSS_TOKEN_IDENT &&
				token->type != CSS_TOKEN_STRING))
			return CSS_INVALID;

		value = token;

		consumeWhitespace(vector, ctx);

		token = parserutils_vector_iterate(vector, ctx);
		if (token == NULL || tokenIsChar(token, ']') == false)
			return CSS_INVALID;
	}

	return css_stylesheet_selector_create(c->sheet, type, 
			&name->data, value != NULL ? &value->data : NULL,
			specific);
}

css_error parsePseudo(css_css21 *c, const parserutils_vector *vector, int *ctx,
		css_selector **specific)
{
	const css_token *token, *name, *value = NULL;

	/* pseudo    -> ':' [ IDENT | FUNCTION ws IDENT? ws ')' ] */

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || tokenIsChar(token, ':') == false)
		return CSS_INVALID;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL || (token->type != CSS_TOKEN_IDENT && 
			token->type != CSS_TOKEN_FUNCTION))
		return CSS_INVALID;

	name = token;

	if (token->type == CSS_TOKEN_FUNCTION) {
		consumeWhitespace(vector, ctx);

		token = parserutils_vector_iterate(vector, ctx);

		if (token != NULL && token->type == CSS_TOKEN_IDENT) {
			value = token;

			consumeWhitespace(vector, ctx);

			token = parserutils_vector_iterate(vector, ctx);
		}

		if (token == NULL || tokenIsChar(token, ')') == false)
			return CSS_INVALID;
	}

	return css_stylesheet_selector_create(c->sheet, 
			CSS_SELECTOR_PSEUDO, &name->data, 
			value != NULL ? &value->data : NULL, specific);
}

css_error parseSpecific(css_css21 *c, 
		const parserutils_vector *vector, int *ctx,
		css_selector *parent)
{
	css_error error;
	const css_token *token;
	css_selector *specific = NULL;

	/* specific  -> [ HASH | class | attrib | pseudo ] */

	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_HASH) {
		error = css_stylesheet_selector_create(c->sheet,
				CSS_SELECTOR_ID, &token->data, NULL, &specific);
		if (error != CSS_OK)
			return error;

		parserutils_vector_iterate(vector, ctx);
	} else if (tokenIsChar(token, '.')) {
		error = parseClass(c, vector, ctx, &specific);
		if (error != CSS_OK)
			return error;
	} else if (tokenIsChar(token, '[')) {
		error = parseAttrib(c, vector, ctx, &specific);
		if (error != CSS_OK)
			return error;
	} else if (tokenIsChar(token, ':')) {
		error = parsePseudo(c, vector, ctx, &specific);
		if (error != CSS_OK)
			return error;
	} else {
		return CSS_INVALID;
	}

	error = css_stylesheet_selector_append_specific(c->sheet, parent, 
			specific);
	if (error != CSS_OK) {
		css_stylesheet_selector_destroy(c->sheet, specific);
		return error;
	}

	return CSS_OK;
}

css_error parseSelectorSpecifics(css_css21 *c,
		const parserutils_vector *vector, int *ctx,
		css_selector *parent)
{
	css_error error;
	const css_token *token;

	/* specifics -> specific* */
	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL &&
			token->type != CSS_TOKEN_S && 
			tokenIsChar(token, '+') == false &&
			tokenIsChar(token, '>') == false &&
			tokenIsChar(token, ',') == false) {
		error = parseSpecific(c, vector, ctx, parent);
		if (error != CSS_OK)
			return error;
	}

	return CSS_OK;
}

css_error parseSimpleSelector(css_css21 *c, 
		const parserutils_vector *vector, int *ctx, 
		css_selector **result)
{
	css_error error;
	const css_token *token;
	css_selector *selector;

	/* simple_selector -> element_name specifics
	 *                 -> specific specifics
	 * element_name    -> IDENT | '*'
	 */

	token = parserutils_vector_peek(vector, *ctx);
	if (token == NULL)
		return CSS_INVALID;

	if (token->type == CSS_TOKEN_IDENT || tokenIsChar(token, '*')) {
		/* Have element name */
		error = css_stylesheet_selector_create(c->sheet,
				CSS_SELECTOR_ELEMENT, &token->data, NULL,
				&selector);
		if (error != CSS_OK)
			return error;

		parserutils_vector_iterate(vector, ctx);
	} else {
		/* Universal selector */
		css_string name = { (uint8_t *) "*", 1 };

		error = css_stylesheet_selector_create(c->sheet,
				CSS_SELECTOR_ELEMENT, &name, NULL, &selector);
		if (error != CSS_OK)
			return error;

		/* Ensure we have at least one specific selector */
		error = parseSpecific(c, vector, ctx, selector);
		if (error != CSS_OK) {
			css_stylesheet_selector_destroy(c->sheet, selector);
			return error;
		}
	}

	*result = selector;

	return parseSelectorSpecifics(c, vector, ctx, selector);
}

css_error parseCombinator(css_css21 *c, const parserutils_vector *vector,
		int *ctx, css_combinator *result)
{
	const css_token *token;
	css_combinator comb = CSS_COMBINATOR_NONE;

	/* combinator      -> ws '+' ws | ws '>' ws | ws1 */

	UNUSED(c);

	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL) {
		if (tokenIsChar(token, '+'))
			comb = CSS_COMBINATOR_SIBLING;
		else if (tokenIsChar(token, '>'))
			comb = CSS_COMBINATOR_PARENT;
		else if (token->type == CSS_TOKEN_S)
			comb = CSS_COMBINATOR_ANCESTOR;
		else
			break;

		parserutils_vector_iterate(vector, ctx);

		/* If we've seen a '+' or '>', we're done. */
		if (comb != CSS_COMBINATOR_ANCESTOR)
			break;
	}

	/* No valid combinator found */
	if (comb == CSS_COMBINATOR_NONE)
		return CSS_INVALID;

	/* Consume any trailing whitespace */
	consumeWhitespace(vector, ctx);

	*result = comb;

	return CSS_OK;
}

css_error parseSelector(css_css21 *c, const parserutils_vector *vector, 
		int *ctx, css_selector **result)
{
	css_error error;
	const css_token *token = NULL;
	css_selector *selector = NULL;

	/* selector -> simple_selector [ combinator simple_selector ]* ws
	 * 
	 * Note, however, that, as combinator can be wholly whitespace,
	 * there's an ambiguity as to whether "ws" has been reached. We 
	 * resolve this by attempting to extract a combinator, then 
	 * recovering when we detect that we've reached the end of the
	 * selector.
	 */

	error = parseSimpleSelector(c, vector, ctx, &selector);
	if (error != CSS_OK)
		return error;
	*result = selector;

	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL &&
			tokenIsChar(token, ',') == false) {
		css_combinator comb = CSS_COMBINATOR_NONE;
		css_selector *other = NULL;

		error = parseCombinator(c, vector, ctx, &comb);
		if (error != CSS_OK)
			return error;

		/* In the case of "html , body { ... }", the whitespace after
		 * "html" and "body" will be considered an ancestor combinator.
		 * This clearly is not the case, however. Therefore, as a 
		 * special case, if we've got an ancestor combinator and there 
		 * are no further tokens, or if the next token is a comma,
		 * we ignore the supposed combinator and continue. */
		if (comb == CSS_COMBINATOR_ANCESTOR && 
				((token = parserutils_vector_peek(vector, 
					*ctx)) == NULL || 
				tokenIsChar(token, ',')))
			continue;

		error = parseSimpleSelector(c, vector, ctx, &other);
		if (error != CSS_OK)
			return error;

		*result = other;

		error = css_stylesheet_selector_combine(c->sheet,
				comb, selector, other);
		if (error != CSS_OK)
			return error;

		selector = other;
	}

	return CSS_OK;
}

css_error parseSelectorList(css_css21 *c, const parserutils_vector *vector,
		css_rule *rule)
{
	css_error error;
	const css_token *token = NULL;
	css_selector *selector = NULL;
	int ctx = 0;

	/* selector_list   -> selector [ ',' ws selector ]* */

	error = parseSelector(c, vector, &ctx, &selector);
	if (error != CSS_OK) {
		if (selector != NULL)
			css_stylesheet_selector_destroy(c->sheet, selector);
		return error;
	}

	assert(selector != NULL);

	error = css_stylesheet_rule_add_selector(c->sheet, rule, selector);
	if (error != CSS_OK) {
		css_stylesheet_selector_destroy(c->sheet, selector);
		return error;
	}

	while ((token = parserutils_vector_peek(vector, ctx)) != NULL) {
		token = parserutils_vector_iterate(vector, &ctx);
		if (tokenIsChar(token, ',') == false)
			return CSS_INVALID;

		consumeWhitespace(vector, &ctx);

		selector = NULL;

		error = parseSelector(c, vector, &ctx, &selector);
		if (error != CSS_OK) {
			if (selector != NULL) {
				css_stylesheet_selector_destroy(c->sheet, 
						selector);
			}
			return error;
		}

		assert(selector != NULL);

		error = css_stylesheet_rule_add_selector(c->sheet, rule, 
				selector);
		if (error != CSS_OK) {
			css_stylesheet_selector_destroy(c->sheet, selector);
			return error;
		}
	}

	return CSS_OK;
}

/******************************************************************************
 * Property parsing functions                                                 *
 ******************************************************************************/

#include "css21props.c"

css_error parseProperty(css_css21 *c, const css_token *property, 
		const parserutils_vector *vector, int *ctx, css_rule *rule)
{
	css_error error;
	css_prop_handler handler = NULL;
	int i = 0;
	css_style *style = NULL;

	/* Find property index */
	/** \todo improve on this linear search */
	for (i = FIRST_PROP; i <= LAST_PROP; i++) {
		if (property->lower.ptr == c->strings[i])
			break;
	}
	if (i == LAST_PROP + 1)
		return CSS_INVALID;

	/* Get handler */
	handler = property_handlers[i - FIRST_PROP];
	assert(handler != NULL);

	/* Call it */
	error = handler(c, vector, ctx, &style);
	if (error != CSS_OK)
		return error;

	/** \todo we should probably assert this, but until we've implemented 
	 * all the property parsers, this will have to suffice. */
	if (style != NULL) {
		/* Append style to rule */
		error = css_stylesheet_rule_append_style(c->sheet, rule, style);
		if (error != CSS_OK) {
			css_stylesheet_style_destroy(c->sheet, style);
			return error;
		}
	}

	/* Style owned or destroyed by stylesheet, so forget about it */

	return CSS_OK;
}

/******************************************************************************
 * Helper functions                                                           *
 ******************************************************************************/

/**
 * Consume all leading whitespace tokens
 *
 * \param vector  The vector to consume from
 * \param ctx     The vector's context
 */
void consumeWhitespace(const parserutils_vector *vector, int *ctx)
{
	const css_token *token = NULL;

	while ((token = parserutils_vector_peek(vector, *ctx)) != NULL &&
			token->type == CSS_TOKEN_S)
		token = parserutils_vector_iterate(vector, ctx);
}

/**
 * Determine if a token is a character
 *
 * \param token  The token to consider
 * \param c      The character to match (lowercase ASCII only)
 * \return True if the token matches, false otherwise
 */
bool tokenIsChar(const css_token *token, uint8_t c)
{
	return token != NULL && token->type == CSS_TOKEN_CHAR && 
			token->lower.len == 1 && token->lower.ptr[0] == c;
}


