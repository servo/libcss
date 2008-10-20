/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include "stylesheet.h"
#include "parse/css21.h"
#include "utils/utils.h"

/**
 * Create a stylesheet
 *
 * \param level            The language level of the stylesheet
 * \param charset          The charset of the stylesheet data, or NULL to detect
 * \param url              URL of stylesheet
 * \param title            Title of stylesheet
 * \param origin           Origin of stylesheet
 * \param media            Media stylesheet applies to
 * \param import_callback  Handler for imported stylesheets, or NULL
 * \param import_pw        Client private data for import_callback
 * \param alloc            Memory (de)allocation function
 * \param alloc_pw         Client private data for alloc
 * \return Pointer to stylesheet on success, NULL otherwise
 */
css_stylesheet *css_stylesheet_create(css_language_level level,
		const char *charset, const char *url, const char *title,
		css_origin origin, uint32_t media,
		css_import_handler import_callback, void *import_pw,
		css_alloc alloc, void *alloc_pw)
{
	css_stylesheet *sheet;
	size_t len;

	if (url == NULL || alloc == NULL)
		return NULL;

	sheet = alloc(NULL, sizeof(css_stylesheet), alloc_pw);
	if (sheet == NULL)
		return NULL;

	memset(sheet, 0, sizeof(css_stylesheet));

	sheet->parser = css_parser_create(charset, 
			charset ? CSS_CHARSET_DICTATED : CSS_CHARSET_DEFAULT,
			alloc, alloc_pw);
	if (sheet->parser == NULL) {
		alloc(sheet, 0, alloc_pw);
		return NULL;
	}

	/* We only support CSS 2.1 */
	if (level != CSS_LEVEL_21) {
		css_parser_destroy(sheet->parser);
		alloc(sheet, 0, alloc_pw);
		return NULL;
	}

	sheet->level = level;
	sheet->parser_frontend = 
			css_css21_create(sheet, sheet->parser, alloc, alloc_pw);
	if (sheet->parser_frontend == NULL) {
		css_parser_destroy(sheet->parser);
		alloc(sheet, 0, alloc_pw);
		return NULL;
	}

	/** \todo create selector hash */

	len = strlen(url) + 1;
	sheet->url = alloc(NULL, len, alloc_pw);
	if (sheet->url == NULL) {
		css_css21_destroy(sheet->parser_frontend);
		css_parser_destroy(sheet->parser);
		alloc(sheet, 0, alloc_pw);
		return NULL;
	}
	memcpy(sheet->url, url, len);

	if (title != NULL) {
		len = strlen(title) + 1;
		sheet->title = alloc(NULL, len, alloc_pw);
		if (sheet->title == NULL) {
			css_css21_destroy(sheet->parser_frontend);
			css_parser_destroy(sheet->parser);
			alloc(sheet->url, 0, alloc_pw);
			alloc(sheet, 0, alloc_pw);
			return NULL;
		}
		memcpy(sheet->title, title, len);
	}

	sheet->origin = origin;
	sheet->media = media;

	sheet->import = import_callback;
	sheet->import_pw = import_pw;

	sheet->alloc = alloc;
	sheet->pw = alloc_pw;

	return sheet;
}

/**
 * Destroy a stylesheet
 *
 * \param sheet  The stylesheet to destroy
 */
void css_stylesheet_destroy(css_stylesheet *sheet)
{
	if (sheet == NULL)
		return;

	if (sheet->title != NULL)
		sheet->alloc(sheet->title, 0, sheet->pw);

	sheet->alloc(sheet->url, 0, sheet->pw);

	/** \todo destroy selector hash + other data */

	css_css21_destroy(sheet->parser_frontend);

	css_parser_destroy(sheet->parser);

	sheet->alloc(sheet, 0, sheet->pw);
}

/**
 * Append source data to a stylesheet
 *
 * \param sheet  The stylesheet to append data to
 * \param data   Pointer to data to append
 * \param len    Length, in bytes, of data to append
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_append_data(css_stylesheet *sheet,
		const uint8_t *data, size_t len)
{
	if (sheet == NULL || data == NULL)
		return CSS_BADPARM;

	return css_parser_parse_chunk(sheet->parser, data, len);
}

/**
 * Flag that the last of a stylesheet's data has been seen
 *
 * \param sheet  The stylesheet in question
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_data_done(css_stylesheet *sheet)
{
	if (sheet == NULL)
		return CSS_BADPARM;

	return css_parser_completed(sheet->parser);
}

/**
 * Retrieve the URL associated with a stylesheet
 *
 * \param sheet  The stylesheet to retrieve the URL from
 * \param url    Pointer to location to receive pointer to URL
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_get_url(css_stylesheet *sheet, const char **url)
{
	if (sheet == NULL || url == NULL)
		return CSS_BADPARM;

	*url = sheet->url;

	return CSS_OK;
}

/**
 * Retrieve the title associated with a stylesheet
 *
 * \param sheet  The stylesheet to retrieve the title from
 * \param title  Pointer to location to receive pointer to title
 * \return CSS_Ok on success, appropriate error otherwise
 */
css_error css_stylesheet_get_title(css_stylesheet *sheet, const char **title)
{
	if (sheet == NULL || title == NULL)
		return CSS_BADPARM;

	*title = sheet->title;

	return CSS_OK;
}

/**
 * Retrieve the origin of a stylesheet
 *
 * \param sheet   The stylesheet to retrieve the origin of
 * \param origin  Pointer to location to receive origin
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_get_origin(css_stylesheet *sheet, css_origin *origin)
{
	if (sheet == NULL || origin == NULL)
		return CSS_BADPARM;

	*origin = sheet->origin;

	return CSS_OK;
}

/**
 * Retrieve the media types associated with a stylesheet
 *
 * \param sheet  The stylesheet to retrieve the media types for
 * \param media  Pointer to location to receive media types
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_get_media(css_stylesheet *sheet, uint32_t *media)
{
	if (sheet == NULL || media == NULL)
		return CSS_BADPARM;

	*media = sheet->media;

	return CSS_OK;
}

/**
 * Get disabled status of a stylesheet
 *
 * \param sheet     The stylesheet to consider
 * \param disabled  Pointer to location to receive disabled state
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_get_disabled(css_stylesheet *sheet, bool *disabled)
{
	if (sheet == NULL || disabled == NULL)
		return CSS_BADPARM;

	*disabled = sheet->disabled;

	return CSS_OK;
}

/**
 * Set a stylesheet's disabled state
 *
 * \param sheet     The stylesheet to modify
 * \param disabled  The new disabled state
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_set_disabled(css_stylesheet *sheet, bool disabled)
{
	if (sheet == NULL)
		return CSS_BADPARM;

	sheet->disabled = disabled;

	/** \todo needs to trigger some event announcing styles have changed */

	return CSS_OK;
}

/******************************************************************************
 * Private API below here                                                     *
 ******************************************************************************/

/**
 * Create a selector
 *
 * \param sheet  The stylesheet context
 * \param type   The type of selector to create
 * \param name   Name of selector
 * \param value  Value of selector, or NULL
 * \return Pointer to selector object, or NULL on failure
 */
css_selector *css_stylesheet_selector_create(css_stylesheet *sheet,
		css_selector_type type, const css_string *name, 
		const css_string *value)
{
	css_selector *sel;

	if (sheet == NULL || name == NULL)
		return NULL;

	sel = sheet->alloc(NULL, sizeof(css_selector), sheet->pw);
	if (sel == NULL)
		return NULL;

	memset(sel, 0, sizeof(css_selector));

	sel->type = type;
	sel->data.name = *name;
	if (value != NULL) {
		sel->data.value = *value;
	}
	/** \todo specificity */
	sel->specificity = 0;
	sel->combinator_type = CSS_COMBINATOR_NONE;

	return sel;
}

/**
 * Destroy a selector object
 *
 * \param sheet     The stylesheet context
 * \param selector  The selector to destroy
 */
void css_stylesheet_selector_destroy(css_stylesheet *sheet,
		css_selector *selector)
{
	UNUSED(sheet);
	UNUSED(selector);

	/** \todo Need to ensure that selector is removed from whatever it's 
	 * attached to (be that the parent selector, parent rule, or the 
	 * hashtable of selectors (or any combination of these) */
}

/**
 * Append a selector to the specifics chain of another selector
 *
 * \param sheet     The stylesheet context
 * \param parent    The parent selector
 * \param specific  The selector to append
 * \return CSS_OK on success, appropriate error otherwise.
 */
css_error css_stylesheet_selector_append_specific(css_stylesheet *sheet,
		css_selector *parent, css_selector *specific)
{
	css_selector *s;

	if (sheet == NULL || parent == NULL || specific == NULL)
		return CSS_BADPARM;

	/** \todo this may want optimising */
	for (s = parent->specifics; s != NULL && s->next != NULL; s = s->next)
		/* do nothing */;
	if (s == NULL) {
		specific->prev = specific->next = NULL;
		parent->specifics = specific;
	} else {
		s->next = specific;
		specific->prev = s;
		specific->next = NULL;
	}

	return CSS_OK;
}

/**
 * Combine a pair of selectors
 *
 * \param sheet  The stylesheet context
 * \param type   The combinator type
 * \param a      The first operand
 * \param b      The second operand
 * \return CSS_OK on success, appropriate error otherwise.
 *
 * For example, given A + B, the combinator field of B would point at A, 
 * with a combinator type of CSS_COMBINATOR_SIBLING. Thus, given B, we can
 * find its combinator. It is not possible to find B given A.
 *
 * \todo Check that this (backwards) representation plays well with CSSOM.
 */
css_error css_stylesheet_selector_combine(css_stylesheet *sheet,
		css_combinator type, css_selector *a, css_selector *b)
{
	if (sheet == NULL || a == NULL || b == NULL)
		return CSS_BADPARM;

	/* Ensure that there is no existing combinator on B */
	if (b->combinator != NULL)
		return CSS_INVALID;

	b->combinator = a;
	b->combinator_type = type;

	return CSS_OK;
}

/**
 * Create a CSS rule
 *
 * \param sheet  The stylesheet context
 * \param type   The rule type
 * \return Pointer to rule object, or NULL on failure.
 */
css_rule *css_stylesheet_rule_create(css_stylesheet *sheet, css_rule_type type)
{
	css_rule *rule;

	if (sheet == NULL)
		return NULL;

	rule = sheet->alloc(NULL, sizeof(css_rule), sheet->pw);
	if (rule == NULL)
		return NULL;

	memset(rule, 0, sizeof(css_rule));

	rule->type = type;

	return rule;
}

/**
 * Destroy a CSS rule
 *
 * \param sheet  The stylesheet context
 * \param rule   The rule to destroy
 */
void css_stylesheet_rule_destroy(css_stylesheet *sheet, css_rule *rule)
{
	UNUSED(sheet);
	UNUSED(rule);

	/** \todo should this be recursive? */
	/** \todo what happens to non-rule objects owned by this rule? */
}

/**
 * Add a selector to a CSS rule
 *
 * \param sheet     The stylesheet context
 * \param rule      The rule to add to (must be of type CSS_RULE_SELECTOR)
 * \param selector  The selector to add
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_rule_add_selector(css_stylesheet *sheet, 
		css_rule *rule, css_selector *selector)
{
	css_selector **sels;

	if (sheet == NULL || rule == NULL || selector == NULL)
		return CSS_BADPARM;

	/* Ensure rule is a CSS_RULE_SELECTOR */
	if (rule->type != CSS_RULE_SELECTOR)
		return CSS_INVALID;

	sels = sheet->alloc(rule->data.selector.selectors, 
			(rule->data.selector.selector_count + 1) * 
				sizeof(css_selector *), 
			sheet->pw);
	if (sels == NULL)
		return CSS_NOMEM;

	/* Insert into rule's selector list */
	sels[rule->data.selector.selector_count] = selector;
	rule->data.selector.selector_count++;
	rule->data.selector.selectors = sels;

	/* Set selector's rule field */
	selector->rule = rule;
	
	return CSS_OK;
}

/**
 * Add a rule to a stylesheet
 *
 * \param sheet  The stylesheet to add to
 * \param rule   The rule to add
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_add_rule(css_stylesheet *sheet, css_rule *rule)
{
	css_rule *r;

	if (sheet == NULL || rule == NULL)
		return CSS_BADPARM;

	/* Fill in rule's index and owner fields */
	rule->index = sheet->rule_count;
	rule->owner = sheet;

	/* Add rule to sheet */
	sheet->rule_count++;
	/** \todo this may need optimising */
	for (r = sheet->rule_list; r != NULL && r->next != NULL; r = r->next)
		/* do nothing */;
	if (r == NULL) {
		rule->prev = rule->next = NULL;
		sheet->rule_list = rule;
	} else {
		r->next = rule;
		rule->prev = r;
		rule->next = NULL;
	}

	/** \todo If there are selectors in the rule, add them to the hash 
	 * (this needs to recurse over child rules, too) */

	return CSS_OK;
}

/**
 * Remove a rule from a stylesheet
 *
 * \param sheet  The sheet to remove from
 * \param rule   The rule to remove
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_remove_rule(css_stylesheet *sheet, css_rule *rule)
{
	UNUSED(sheet);
	UNUSED(rule);

	/** \todo If there are selectors (recurse over child rules, too),
	 * then they must be removed from the hash */
	/**\ todo renumber subsequent rules? may not be necessary, as there's 
	 * only an expectation that rules which occur later in the stylesheet 
	 * have a higher index than those that appear earlier. There's no 
	 * guarantee that the number space is continuous. */

	return CSS_OK;
}

static void css_stylesheet_dump_rule(css_rule *rule, FILE *target);
static void css_stylesheet_dump_selector_list(css_selector *list, FILE *target);
static void css_stylesheet_dump_selector(css_selector *selector, FILE *target);
static void css_stylesheet_dump_string(css_string *string, FILE *target);

/**
 * Dump a stylesheet
 *
 * \param sheet   The stylesheet to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump(css_stylesheet *sheet, FILE *target)
{
	css_rule *r;

	if (sheet == NULL || target == NULL)
		return;

	fprintf(target, "%s (%s) %sabled origin: %d media: %d\n",
			sheet->url, sheet->title ? sheet->title : "",
			sheet->disabled ? "dis" : "en", sheet->origin,
			sheet->media);

	fprintf(target, "Rules:\n");

	for (r = sheet->rule_list; r != NULL; r = r->next)
		css_stylesheet_dump_rule(r, target);
}

/**
 * Dump a CSS rule
 *
 * \param rule    The rule to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump_rule(css_rule *rule, FILE *target)
{
	fprintf(target, "  Rule %d (type %d):\n",
			rule->index, rule->type);

	fprintf(target, "    ");

	switch (rule->type) {
	case CSS_RULE_UNKNOWN:
		break;
	case CSS_RULE_SELECTOR:
		for (uint32_t i = 0; i < rule->data.selector.selector_count;
				i++) {
			css_stylesheet_dump_selector_list(
				rule->data.selector.selectors[i], target);
			if (i != rule->data.selector.selector_count - 1)
				fprintf(target, ", ");
		}
		break;
	case CSS_RULE_CHARSET:
	case CSS_RULE_IMPORT:
	case CSS_RULE_MEDIA:
	case CSS_RULE_FONT_FACE:
	case CSS_RULE_PAGE:
		break;
	}

	fprintf(target, "\n");
}

/**
 * Dump a CSS selector list
 *
 * \param list    The selector list to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump_selector_list(css_selector *list, FILE *target)
{
	if (list->combinator != NULL)
		css_stylesheet_dump_selector_list(list->combinator, target);

	switch (list->combinator_type) {
	case CSS_COMBINATOR_NONE:
		break;
	case CSS_COMBINATOR_ANCESTOR:
		fprintf(target, " ");
		break;
	case CSS_COMBINATOR_PARENT:
		fprintf(target, " > ");
		break;
	case CSS_COMBINATOR_SIBLING:
		fprintf(target, " + ");
		break;
	}

	css_stylesheet_dump_selector(list, target);
}

/**
 * Dump a CSS selector
 *
 * \param selector  The selector to dump
 * \param target    The file handle to output to
 */
void css_stylesheet_dump_selector(css_selector *selector, FILE *target)
{
	css_selector *s;

	switch (selector->type) {
	case CSS_SELECTOR_ELEMENT:
		if (selector->data.name.len == 1 && 
				selector->data.name.ptr[0] == '*' &&
				selector->specifics == NULL) {
			css_stylesheet_dump_string(&selector->data.name,
					target);
		} else if (selector->data.name.len != 1 ||
				selector->data.name.ptr[0] != '*') {
			css_stylesheet_dump_string(&selector->data.name,
					target);
		}
		break;
	case CSS_SELECTOR_CLASS:
		fprintf(target, ".");
		css_stylesheet_dump_string(&selector->data.name, target);
		break;
	case CSS_SELECTOR_ID:
		fprintf(target, "#");
		css_stylesheet_dump_string(&selector->data.name, target);
		break;
	case CSS_SELECTOR_PSEUDO:
		fprintf(target, ":");
		css_stylesheet_dump_string(&selector->data.name, target);
		if (selector->data.value.len > 0) {
			fprintf(target, "(");
			css_stylesheet_dump_string(&selector->data.value, 
					target);
			fprintf(target, ")");
		}
		break;
	case CSS_SELECTOR_ATTRIBUTE:
		fprintf(target, "[");
		css_stylesheet_dump_string(&selector->data.name, target);
		fprintf(target, "]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_EQUAL:
		fprintf(target, "[");
		css_stylesheet_dump_string(&selector->data.name, target);
		fprintf(target, "=\"");
		css_stylesheet_dump_string(&selector->data.value, target);
		fprintf(target, "\"]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_DASHMATCH:
		fprintf(target, "[");
		css_stylesheet_dump_string(&selector->data.name, target);
		fprintf(target, "|=\"");
		css_stylesheet_dump_string(&selector->data.value, target);
		fprintf(target, "\"]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_INCLUDES:
		fprintf(target, "[");
		css_stylesheet_dump_string(&selector->data.name, target);
		fprintf(target, "~=\"");
		css_stylesheet_dump_string(&selector->data.value, target);
		fprintf(target, "\"]");
		break;
	}

	for (s = selector->specifics; s != NULL; s = s->next)
		css_stylesheet_dump_selector(s, target);
}

/**
 * Dump a CSS string
 *
 * \param string  The string to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump_string(css_string *string, FILE *target)
{
	fprintf(target, "%.*s", (int) string->len, string->ptr);
}

