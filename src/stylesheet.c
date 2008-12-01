/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <string.h>

#include "stylesheet.h"
#include "bytecode/bytecode.h"
#include "parse/language.h"
#include "utils/parserutilserror.h"
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
 * \param stylesheet       Pointer to location to receive stylesheet
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_create(css_language_level level,
		const char *charset, const char *url, const char *title,
		css_origin origin, uint32_t media,
		css_import_handler import_callback, void *import_pw,
		css_alloc alloc, void *alloc_pw, css_stylesheet **stylesheet)
{
	parserutils_error perror;
	css_error error;
	css_stylesheet *sheet;
	size_t len;

	if (url == NULL || alloc == NULL || stylesheet == NULL)
		return CSS_BADPARM;

	sheet = alloc(NULL, sizeof(css_stylesheet), alloc_pw);
	if (sheet == NULL)
		return CSS_NOMEM;

	memset(sheet, 0, sizeof(css_stylesheet));

	perror = parserutils_hash_create((parserutils_alloc) alloc, alloc_pw,
			&sheet->dictionary);
	if (perror != PARSERUTILS_OK) {
		alloc(sheet, 0, alloc_pw);
		return css_error_from_parserutils_error(perror);
	}

	error = css_parser_create(charset, 
			charset ? CSS_CHARSET_DICTATED : CSS_CHARSET_DEFAULT,
			sheet->dictionary, alloc, alloc_pw, &sheet->parser);
	if (error != CSS_OK) {
		parserutils_hash_destroy(sheet->dictionary);
		alloc(sheet, 0, alloc_pw);
		return error;
	}

	sheet->level = level;
	error = css_language_create(sheet, sheet->parser, alloc, alloc_pw,
			&sheet->parser_frontend);
	if (error != CSS_OK) {
		css_parser_destroy(sheet->parser);
		parserutils_hash_destroy(sheet->dictionary);
		alloc(sheet, 0, alloc_pw);
		return error;
	}

	/** \todo create selector hash */

	len = strlen(url) + 1;
	sheet->url = alloc(NULL, len, alloc_pw);
	if (sheet->url == NULL) {
		css_language_destroy(sheet->parser_frontend);
		css_parser_destroy(sheet->parser);
		parserutils_hash_destroy(sheet->dictionary);
		alloc(sheet, 0, alloc_pw);
		return CSS_NOMEM;
	}
	memcpy(sheet->url, url, len);

	if (title != NULL) {
		len = strlen(title) + 1;
		sheet->title = alloc(NULL, len, alloc_pw);
		if (sheet->title == NULL) {
			alloc(sheet->url, 0, alloc_pw);
			css_language_destroy(sheet->parser_frontend);
			css_parser_destroy(sheet->parser);
			parserutils_hash_destroy(sheet->dictionary);
			alloc(sheet, 0, alloc_pw);
			return CSS_NOMEM;
		}
		memcpy(sheet->title, title, len);
	}

	sheet->origin = origin;
	sheet->media = media;

	sheet->import = import_callback;
	sheet->import_pw = import_pw;

	sheet->alloc = alloc;
	sheet->pw = alloc_pw;

	*stylesheet = sheet;

	return CSS_OK;
}

/**
 * Destroy a stylesheet
 *
 * \param sheet  The stylesheet to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_destroy(css_stylesheet *sheet)
{
	if (sheet == NULL)
		return CSS_BADPARM;

	parserutils_hash_destroy(sheet->dictionary);

	if (sheet->title != NULL)
		sheet->alloc(sheet->title, 0, sheet->pw);

	sheet->alloc(sheet->url, 0, sheet->pw);

	/** \todo destroy selector hash + other data */

	css_language_destroy(sheet->parser_frontend);

	css_parser_destroy(sheet->parser);

	sheet->alloc(sheet, 0, sheet->pw);

	return CSS_OK;
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

	/** \todo We can destroy the parser here as it won't be needed
	 * Note, however, that, if we do so, then the dictionary of
	 * strings created by the parser *must* be preserved (and, ideally,
	 * created by us in the first place) because our stylesheet 
	 * datastructures contain pointers to strings stored in this
	 * dictionary. */
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
 * Create a style
 *
 * \param sheet  The stylesheet context
 * \param len    The required length of the style
 * \param style  Pointer to location to receive style
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_style_create(css_stylesheet *sheet, uint32_t len,
		css_style **style)
{
	css_style *s;

	if (sheet == NULL || len == 0 || style == NULL)
		return CSS_BADPARM;

	s = sheet->alloc(NULL, sizeof(css_style) + len, sheet->pw);
	if (s == NULL)
		return CSS_NOMEM;

	/* DIY variable-sized data member */
	s->bytecode = ((uint8_t *) s + sizeof(css_style));
	s->length = len;

	*style = s;

	return CSS_OK;
}

/**
 * Destroy a style
 *
 * \param sheet  The stylesheet context
 * \param style  The style to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_style_destroy(css_stylesheet *sheet, css_style *style)
{
	UNUSED(sheet);
	UNUSED(style);

	/** \todo destroy style */

	return CSS_OK;
}

/**
 * Create a selector
 *
 * \param sheet     The stylesheet context
 * \param type      The type of selector to create
 * \param name      Name of selector
 * \param value     Value of selector, or NULL
 * \param selector  Pointer to location to receive selector object
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_selector_create(css_stylesheet *sheet,
		css_selector_type type, const css_string *name, 
		const css_string *value, css_selector **selector)
{
	const parserutils_hash_entry *iname, *ivalue;
	css_selector *sel;
	parserutils_error perror;

	if (sheet == NULL || name == NULL || selector == NULL)
		return CSS_BADPARM;

	sel = sheet->alloc(NULL, sizeof(css_selector), sheet->pw);
	if (sel == NULL)
		return CSS_NOMEM;

	memset(sel, 0, sizeof(css_selector));

	sel->data.type = type;

	/** \todo Given that this information is already guaranteed to be in 
	 * the dictionary, it would be more efficient to pass a pointer to the 
	 * dictionary entry around rather than re-discovering it here. The same
	 * applies for value, and in css_stylesheet_selector_detail_create() */
	perror = parserutils_hash_insert(sheet->dictionary, name->data, 
			name->len, &iname);
	if (perror != PARSERUTILS_OK) {
		sheet->alloc(sel, 0, sheet->pw);
		return css_error_from_parserutils_error(perror);
	}
	sel->data.name = iname;

	if (value != NULL) {
		perror = parserutils_hash_insert(sheet->dictionary, 
				value->data, value->len, &ivalue);
		if (perror != PARSERUTILS_OK) {
			sheet->alloc(sel, 0, sheet->pw);
			return css_error_from_parserutils_error(perror);
		}
		sel->data.value = ivalue;
	}

	/** \todo specificity */
	sel->specificity = 0;
	sel->data.comb = CSS_COMBINATOR_NONE;

	*selector = sel;

	return CSS_OK;
}

/**
 * Destroy a selector object
 *
 * \param sheet     The stylesheet context
 * \param selector  The selector to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_selector_destroy(css_stylesheet *sheet,
		css_selector *selector)
{
	UNUSED(sheet);
	UNUSED(selector);

	/** \todo Need to ensure that selector is removed from whatever it's 
	 * attached to (be that the parent selector, parent rule, or the 
	 * hashtable of selectors (or any combination of these) */

	return CSS_OK;
}

/**
 * Create a selector detail
 *
 * \param sheet   The stylesheet context
 * \param type    The type of selector to create
 * \param name    Name of selector
 * \param value   Value of selector, or NULL
 * \param detail  Pointer to location to receive detail object
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_selector_detail_create(css_stylesheet *sheet,
		css_selector_type type, const css_string *name, 
		const css_string *value, css_selector_detail **detail)
{
	parserutils_error perror;
	const parserutils_hash_entry *iname, *ivalue;
	css_selector_detail *det;

	if (sheet == NULL || name == NULL || detail == NULL)
		return CSS_BADPARM;

	det = sheet->alloc(NULL, sizeof(css_selector_detail), sheet->pw);
	if (det == NULL)
		return CSS_NOMEM;

	memset(det, 0, sizeof(css_selector_detail));

	det->type = type;

	perror = parserutils_hash_insert(sheet->dictionary, name->data, 
			name->len, &iname);
	if (perror != PARSERUTILS_OK) {
		sheet->alloc(det, 0, sheet->pw);
		return css_error_from_parserutils_error(perror);
	}
	det->name = iname;

	if (value != NULL) {
		perror = parserutils_hash_insert(sheet->dictionary, 
				value->data, value->len, &ivalue);
		if (perror != PARSERUTILS_OK) {
			sheet->alloc(det, 0, sheet->pw);
			return css_error_from_parserutils_error(perror);
		}
		det->value = ivalue;
	}

	*detail = det;

	return CSS_OK;
}

/**
 * Destroy a selector detail
 *
 * \param sheet   The stylesheet context
 * \param detail  The detail to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_selector_detail_destroy(css_stylesheet *sheet,
		css_selector_detail *detail)
{
	if (sheet == NULL || detail == NULL)
		return CSS_BADPARM;

	sheet->alloc(detail, 0, sheet->pw);

	return CSS_OK;
}


/**
 * Append a selector to the specifics chain of another selector
 *
 * \param sheet     The stylesheet context
 * \param parent    Pointer to pointer to the parent selector (updated on exit)
 * \param specific  The selector to append (destroyed)
 * \return CSS_OK on success, appropriate error otherwise.
 */
css_error css_stylesheet_selector_append_specific(css_stylesheet *sheet,
		css_selector **parent, css_selector_detail *detail)
{
	css_selector *temp;
	css_selector_detail *d;
	size_t num_details = 0;

	if (sheet == NULL || parent == NULL || 
			*parent == NULL || detail == NULL)
		return CSS_BADPARM;

	/** \todo this may want optimising -- counting blocks is O(n) 
	 * In practice, however, n isn't likely to be large, so may be fine
	 */

	/* Count current number of detail blocks */
	for (d = &(*parent)->data; d->next != 0; d++)
		num_details++;

	/* Grow selector by one detail block */
	temp = sheet->alloc((*parent), sizeof(css_selector) + 
			(num_details + 1) * sizeof(css_selector_detail), 
			sheet->pw);
	if (temp == NULL)
		return CSS_NOMEM;

	/* Copy detail into empty block */
	(&temp->data)[num_details + 1] = *detail;
	/* Flag that there's another block */
	(&temp->data)[num_details].next = 1;

	css_stylesheet_selector_detail_destroy(sheet, detail);

	(*parent) = temp;

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
	b->data.comb = type;

	return CSS_OK;
}

/**
 * Create a CSS rule
 *
 * \param sheet  The stylesheet context
 * \param type   The rule type
 * \param rule   Pointer to location to receive rule object
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_rule_create(css_stylesheet *sheet, css_rule_type type,
		css_rule **rule)
{
	css_rule *r;
	size_t required = 0;

	if (sheet == NULL || rule == NULL)
		return CSS_BADPARM;

	switch (type) {
	case CSS_RULE_UNKNOWN:
		required = sizeof(css_rule);
		break;
	case CSS_RULE_SELECTOR:
		required = sizeof(css_rule_selector);
		break;
	case CSS_RULE_CHARSET:
		required = sizeof(css_rule_charset);
		break;
	case CSS_RULE_IMPORT:
		required = sizeof(css_rule_import);
		break;
	case CSS_RULE_MEDIA:
		required = sizeof(css_rule_media);
		break;
	case CSS_RULE_FONT_FACE:
		required = sizeof(css_rule_font_face);
		break;
	case CSS_RULE_PAGE:
		required = sizeof(css_rule_page);
		break;
	}

	r = sheet->alloc(NULL, required, sheet->pw);
	if (r == NULL)
		return CSS_NOMEM;

	memset(r, 0, required);

	r->type = type;

	*rule = r;

	return CSS_OK;
}

/**
 * Destroy a CSS rule
 *
 * \param sheet  The stylesheet context
 * \param rule   The rule to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_rule_destroy(css_stylesheet *sheet, css_rule *rule)
{
	UNUSED(sheet);
	UNUSED(rule);

	/** \todo should this be recursive? */
	/** \todo what happens to non-rule objects owned by this rule? */

	return CSS_OK;
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
	css_rule_selector *r = (css_rule_selector *) rule;
	css_selector **sels;

	if (sheet == NULL || rule == NULL || selector == NULL)
		return CSS_BADPARM;

	/* Ensure rule is a CSS_RULE_SELECTOR */
	if (rule->type != CSS_RULE_SELECTOR)
		return CSS_INVALID;

	sels = sheet->alloc(r->selectors, 
			(r->base.items + 1) * sizeof(css_selector *), 
			sheet->pw);
	if (sels == NULL)
		return CSS_NOMEM;

	/* Insert into rule's selector list */
	sels[r->base.items] = selector;
	r->base.items++;
	r->selectors = sels;

	/* Set selector's rule field */
	selector->rule = rule;
	
	return CSS_OK;
}

/**
 * Append a style to a CSS rule
 *
 * \param sheet  The stylesheet context
 * \param rule   The rule to add to (must be CSS_RULE_SELECTOR or CSS_RULE_PAGE)
 * \param style  The style to add
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_rule_append_style(css_stylesheet *sheet,
		css_rule *rule, css_style *style)
{
	css_style *cur;

	if (sheet == NULL || rule == NULL || style == NULL)
		return CSS_BADPARM;

	if (rule->type != CSS_RULE_SELECTOR && rule->type != CSS_RULE_PAGE)
		return CSS_INVALID;

	if (rule->type == CSS_RULE_SELECTOR)
		cur = ((css_rule_selector *) rule)->style;
	else
		cur = ((css_rule_page *) rule)->style;

	if (cur != NULL) {
		/* Already have a style, so append to the end of the bytecode */
		css_style *temp = sheet->alloc(cur, 
				sizeof(css_style) + cur->length + style->length,
				sheet->pw);
		if (temp == NULL)
			return CSS_NOMEM;

		/* Ensure bytecode pointer is correct */
		temp->bytecode = ((uint8_t *) temp + sizeof(css_style));

		/** \todo Can we optimise the bytecode here? */
		memcpy((uint8_t *) temp->bytecode + temp->length, 
				style->bytecode, style->length);

		cur = temp;
		cur->length += style->length;

		/* Done with style */
		css_stylesheet_style_destroy(sheet, style);
	} else {
		/* No current style, so use this one */
		cur = style;
	}

	if (rule->type == CSS_RULE_SELECTOR)
		((css_rule_selector *) rule)->style = cur;
	else
		((css_rule_page *) rule)->style = cur;

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
	if (sheet == NULL || rule == NULL)
		return CSS_BADPARM;

	/* Fill in rule's index and parent fields */
	rule->index = sheet->rule_count;
	rule->ptype = CSS_RULE_PARENT_STYLESHEET;
	rule->parent = sheet;

	/* Add rule to sheet */
	sheet->rule_count++;

	if (sheet->last_rule == NULL) {
		rule->prev = rule->next = NULL;
		sheet->rule_list = sheet->last_rule = rule;
	} else {
		sheet->last_rule->next = rule;
		rule->prev = sheet->last_rule;
		rule->next = NULL;
		sheet->last_rule = rule;
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

static void css_stylesheet_dump_rule(css_rule *rule, FILE *target, 
		size_t *size);
static void css_stylesheet_dump_selector_list(css_selector *list, FILE *target,
		size_t *size);
static void css_stylesheet_dump_selector(css_selector *selector, FILE *target,
		size_t *size);
static void css_stylesheet_dump_selector_detail(css_selector_detail *detail,
		FILE *target, size_t *size);
static void css_stylesheet_dump_string(const parserutils_hash_entry *string, 
		FILE *target);

/**
 * Dump a stylesheet
 *
 * \param sheet   The stylesheet to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump(css_stylesheet *sheet, FILE *target)
{
	css_rule *r;
	size_t size = sizeof(css_stylesheet);

	if (sheet == NULL || target == NULL)
		return;

	fprintf(target, "%s (%s) %sabled origin: %d media: %d\n",
			sheet->url, sheet->title ? sheet->title : "",
			sheet->disabled ? "dis" : "en", sheet->origin,
			sheet->media);

	fprintf(target, "Rules:\n");

	for (r = sheet->rule_list; r != NULL; r = r->next)
		css_stylesheet_dump_rule(r, target, &size);

	fprintf(target, "Total size: %zu bytes\n", size);
}

/**
 * Dump a CSS rule
 *
 * \param rule    The rule to dump
 * \param target  The file handle to output to
 * \param size    Pointer to current size of sheet, updated on exit
 */
void css_stylesheet_dump_rule(css_rule *rule, FILE *target, size_t *size)
{
	fprintf(target, "  Rule %d (type %d):\n",
			rule->index, rule->type);

	fprintf(target, "    ");

	switch (rule->type) {
	case CSS_RULE_UNKNOWN:
		*size += sizeof(css_rule);
		break;
	case CSS_RULE_SELECTOR:
		*size += sizeof(css_rule_selector);
		for (uint32_t i = 0; i < rule->items; i++) {
			css_stylesheet_dump_selector_list(
				((css_rule_selector *) rule)->selectors[i], 
				target, size);
			if (i != (uint32_t) (rule->items - 1))
				fprintf(target, ", ");
		}
		fprintf(target, " { ");
		if (((css_rule_selector *) rule)->style != NULL) {
			*size += ((css_rule_selector *) rule)->style->length;

			css_bytecode_dump(
				((css_rule_selector *) rule)->style->bytecode,
				((css_rule_selector *) rule)->style->length, 
				target);
		}
		fprintf(target, "}");
		break;
	case CSS_RULE_CHARSET:
		*size += sizeof(css_rule_charset);
		break;
	case CSS_RULE_IMPORT:
		*size += sizeof(css_rule_import);
		break;
	case CSS_RULE_MEDIA:
		*size += sizeof(css_rule_media);
		break;
	case CSS_RULE_FONT_FACE:
		*size += sizeof(css_rule_font_face);
		break;
	case CSS_RULE_PAGE:
		*size += sizeof(css_rule_page);
		break;
	}

	fprintf(target, "\n");
}

/**
 * Dump a CSS selector list
 *
 * \param list    The selector list to dump
 * \param target  The file handle to output to
 * \param size    Pointer to current size of sheet, updated on exit
 */
void css_stylesheet_dump_selector_list(css_selector *list, FILE *target, 
		size_t *size)
{
	if (list->combinator != NULL) {
		css_stylesheet_dump_selector_list(list->combinator, target, 
				size);
	}

	switch (list->data.comb) {
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

	css_stylesheet_dump_selector(list, target, size);
}

/**
 * Dump a CSS selector
 *
 * \param selector  The selector to dump
 * \param target    The file handle to output to
 * \param size      Pointer to current size of sheet, updated on exit
 */
void css_stylesheet_dump_selector(css_selector *selector, FILE *target,
		size_t *size)
{
	css_selector_detail *d = &selector->data;

	*size += sizeof(css_selector) - sizeof(css_selector_detail);

	while (true) {
		css_stylesheet_dump_selector_detail(d, target, size);

		if (d->next == 0)
			break;

		d++;
	}
}

/**
 * Dump a CSS selector detail
 *
 * \param detail  The detail to dump
 * \param target  The file handle to output to
 * \param size    Pointer to current size of sheet, updated on exit
 */
void css_stylesheet_dump_selector_detail(css_selector_detail *detail,
		FILE *target, size_t *size)
{
	*size += sizeof(css_selector_detail);

	switch (detail->type) {
	case CSS_SELECTOR_ELEMENT:
		if (detail->name->len == 1 && detail->name->data[0] == '*' &&
				detail->next == 0) {
			css_stylesheet_dump_string(detail->name, target);
		} else if (detail->name->len != 1 ||
				detail->name->data[0] != '*') {
			css_stylesheet_dump_string(detail->name, target);
		}
		break;
	case CSS_SELECTOR_CLASS:
		fprintf(target, ".");
		css_stylesheet_dump_string(detail->name, target);
		break;
	case CSS_SELECTOR_ID:
		fprintf(target, "#");
		css_stylesheet_dump_string(detail->name, target);
		break;
	case CSS_SELECTOR_PSEUDO:
		fprintf(target, ":");
		css_stylesheet_dump_string(detail->name, target);
		if (detail->value != NULL) {
			fprintf(target, "(");
			css_stylesheet_dump_string(detail->value, target);
			fprintf(target, ")");
		}
		break;
	case CSS_SELECTOR_ATTRIBUTE:
		fprintf(target, "[");
		css_stylesheet_dump_string(detail->name, target);
		fprintf(target, "]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_EQUAL:
		fprintf(target, "[");
		css_stylesheet_dump_string(detail->name, target);
		fprintf(target, "=\"");
		css_stylesheet_dump_string(detail->value, target);
		fprintf(target, "\"]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_DASHMATCH:
		fprintf(target, "[");
		css_stylesheet_dump_string(detail->name, target);
		fprintf(target, "|=\"");
		css_stylesheet_dump_string(detail->value, target);
		fprintf(target, "\"]");
		break;
	case CSS_SELECTOR_ATTRIBUTE_INCLUDES:
		fprintf(target, "[");
		css_stylesheet_dump_string(detail->name, target);
		fprintf(target, "~=\"");
		css_stylesheet_dump_string(detail->value, target);
		fprintf(target, "\"]");
		break;
	}
}

/**
 * Dump a CSS string
 *
 * \param string  The string to dump
 * \param target  The file handle to output to
 */
void css_stylesheet_dump_string(const parserutils_hash_entry *string, 
		FILE *target)
{
	fprintf(target, "%.*s", (int) string->len, string->data);
}

