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

	error = css_selector_hash_create(alloc, alloc_pw, &sheet->selectors);
	if (error != CSS_OK) {
		css_language_destroy(sheet->parser_frontend);
		css_parser_destroy(sheet->parser);
		parserutils_hash_destroy(sheet->dictionary);
		alloc(sheet, 0, alloc_pw);
		return error;
	}

	len = strlen(url) + 1;
	sheet->url = alloc(NULL, len, alloc_pw);
	if (sheet->url == NULL) {
		css_selector_hash_destroy(sheet->selectors);
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
			css_selector_hash_destroy(sheet->selectors);
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
	css_rule *r, *s;

	if (sheet == NULL)
		return CSS_BADPARM;

	if (sheet->title != NULL)
		sheet->alloc(sheet->title, 0, sheet->pw);

	sheet->alloc(sheet->url, 0, sheet->pw);

	for (r = sheet->rule_list; r != NULL; r = s) {
		s = r->next;

		/* Detach from list */
		r->parent = NULL;
		r->prev = NULL;
		r->next = NULL;

		css_stylesheet_rule_destroy(sheet, r);
	}

	css_selector_hash_destroy(sheet->selectors);

	/* These two may have been destroyed when parsing completed */
	if (sheet->parser_frontend != NULL)
		css_language_destroy(sheet->parser_frontend);

	if (sheet->parser != NULL)
		css_parser_destroy(sheet->parser);

	parserutils_hash_destroy(sheet->dictionary);

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

	if (sheet->parser == NULL)
		return CSS_INVALID;

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
	css_error error;

	if (sheet == NULL)
		return CSS_BADPARM;

	if (sheet->parser == NULL)
		return CSS_INVALID;

	error = css_parser_completed(sheet->parser);
	if (error != CSS_OK)
		return error;

	/* Destroy the parser, as it's no longer needed */
	css_language_destroy(sheet->parser_frontend);
	css_parser_destroy(sheet->parser);

	sheet->parser_frontend = NULL;
	sheet->parser = NULL;

	return CSS_OK;
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
	if (sheet == NULL || style == NULL)
		return CSS_BADPARM;

	sheet->alloc(style, 0, sheet->pw);

	return CSS_OK;
}

/**
 * Create an element selector
 *
 * \param sheet     The stylesheet context
 * \param name      Name of selector
 * \param selector  Pointer to location to receive selector object
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters,
 *         CSS_NOMEM on memory exhaustion
 */
css_error css_stylesheet_selector_create(css_stylesheet *sheet,
		const parserutils_hash_entry *name, css_selector **selector)
{
	css_selector *sel;

	if (sheet == NULL || name == NULL || selector == NULL)
		return CSS_BADPARM;

	sel = sheet->alloc(NULL, sizeof(css_selector), sheet->pw);
	if (sel == NULL)
		return CSS_NOMEM;

	memset(sel, 0, sizeof(css_selector));

	sel->data.type = CSS_SELECTOR_ELEMENT;
	sel->data.name = name;
	sel->data.value = NULL;

	/* Initial specificity -- 1 for an element, 0 for universal */
	if (name->len != 1 || name->data[0] != '*')
		sel->specificity = CSS_SPECIFICITY_D;
	else
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
	css_selector *c, *d;

	if (sheet == NULL || selector == NULL)
		return CSS_BADPARM;

	/* Must not be attached to a rule */
	if (selector->rule != NULL)
		return CSS_INVALID;

	/* Destroy combinator chain */
	for (c = selector->combinator; c != NULL; c = d) {
		d = c->combinator;

		sheet->alloc(c, 0, sheet->pw);
	}

	/* Destroy this selector */
	sheet->alloc(selector, 0, sheet->pw);

	return CSS_OK;
}

/**
 * Initialise a selector detail
 *
 * \param sheet   The stylesheet context
 * \param type    The type of selector to create
 * \param name    Name of selector
 * \param value   Value of selector, or NULL
 * \param detail  Pointer to detail object to initialise
 * \return CSS_OK on success,
 *         CSS_BADPARM on bad parameters
 */
css_error css_stylesheet_selector_detail_init(css_stylesheet *sheet,
		css_selector_type type, const parserutils_hash_entry *name, 
		const parserutils_hash_entry *value, 
		css_selector_detail *detail)
{
	if (sheet == NULL || name == NULL || detail == NULL)
		return CSS_BADPARM;

	memset(detail, 0, sizeof(css_selector_detail));

	detail->type = type;
	detail->name = name;
	detail->value = value;

	return CSS_OK;
}

/**
 * Append a selector to the specifics chain of another selector
 *
 * \param sheet     The stylesheet context
 * \param parent    Pointer to pointer to the parent selector (updated on exit)
 * \param specific  The selector to append (copied)
 * \return CSS_OK on success, appropriate error otherwise.
 */
css_error css_stylesheet_selector_append_specific(css_stylesheet *sheet,
		css_selector **parent, const css_selector_detail *detail)
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

	(*parent) = temp;

	/* Update parent's specificity */
	switch (detail->type) {
	case CSS_SELECTOR_CLASS:
	case CSS_SELECTOR_ATTRIBUTE:
	case CSS_SELECTOR_ATTRIBUTE_EQUAL:
	case CSS_SELECTOR_ATTRIBUTE_DASHMATCH:
	case CSS_SELECTOR_ATTRIBUTE_INCLUDES:
		(*parent)->specificity += CSS_SPECIFICITY_C;
		break;
	case CSS_SELECTOR_ID:
		(*parent)->specificity += CSS_SPECIFICITY_B;
		break;
	case CSS_SELECTOR_PSEUDO:
		/** \todo distinguish between pseudo classes and elements */
		/* Assume pseudo class for now */
		(*parent)->specificity += CSS_SPECIFICITY_C;
		break;
	case CSS_SELECTOR_ELEMENT:
		(*parent)->specificity += CSS_SPECIFICITY_D;
		break;
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
	if (sheet == NULL || rule == NULL)
		return CSS_BADPARM;

	/* Must be detached from parent/siblings */
	if (rule->parent != NULL || rule->next != NULL || rule->prev != NULL)
		return CSS_INVALID;

	/* Destroy type-specific contents */
	switch (rule->type) {
	case CSS_RULE_UNKNOWN:
		break;
	case CSS_RULE_SELECTOR:
	{
		css_rule_selector *s = (css_rule_selector *) rule;
		uint32_t i;

		for (i = 0; i < rule->items; i++) {
			css_selector *sel = s->selectors[i];

			/* Detach from rule */
			sel->rule = NULL;

			css_stylesheet_selector_destroy(sheet, sel);
		}

		css_stylesheet_style_destroy(sheet, s->style);
	}
		break;
	case CSS_RULE_CHARSET:
		break;
	case CSS_RULE_IMPORT:
	{
		css_rule_import *import = (css_rule_import *) rule;

		css_stylesheet_destroy(import->sheet);
	}
		break;
	case CSS_RULE_MEDIA:
	{
		css_rule_media *media = (css_rule_media *) rule;
		css_rule *c, *d;

		for (c = media->first_child; c != NULL; c = d) {
			d = c->next;

			/* Detach from list */
			c->parent = NULL;
			c->prev = NULL;
			c->next = NULL;

			css_stylesheet_rule_destroy(sheet, c);
		}
	}
		break;
	case CSS_RULE_FONT_FACE:
	{
		css_rule_font_face *font_face = (css_rule_font_face *) rule;

		css_stylesheet_style_destroy(sheet, font_face->style);
	}
		break;
	case CSS_RULE_PAGE:
	{
		css_rule_page *page = (css_rule_page *) rule;
		uint32_t i;

		for (i = 0; i < rule->items; i++) {
			css_selector *sel = page->selectors[i];

			/* Detach from rule */
			sel->rule = NULL;

			css_stylesheet_selector_destroy(sheet, sel);
		}

		css_stylesheet_style_destroy(sheet, page->style);
	}
		break;
	}

	/* Destroy rule */
	sheet->alloc(rule, 0, sheet->pw);

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
 * Set the charset of a CSS rule
 *
 * \param sheet    The stylesheet context
 * \param rule     The rule to add to (must be of type CSS_RULE_CHARSET)
 * \param charset  The charset
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_rule_set_charset(css_stylesheet *sheet,
		css_rule *rule, const parserutils_hash_entry *charset)
{
	css_rule_charset *r = (css_rule_charset *) rule;

	if (sheet == NULL || rule == NULL || charset == NULL)
		return CSS_BADPARM;

	/* Ensure rule is a CSS_RULE_CHARSET */
	if (rule->type != CSS_RULE_CHARSET)
		return CSS_INVALID;

	/* Set rule's encoding field */
	r->encoding = charset;
	
	return CSS_OK;
}

/**
 * Set the imported stylesheet associated with a rule
 *
 * \param sheet   The stylesheet context
 * \param rule    The rule to add to (must be of type CSS_RULE_IMPORT)
 * \param import  The imported sheet
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_stylesheet_rule_set_import(css_stylesheet *sheet,
		css_rule *rule, css_stylesheet *import)
{
	css_rule_import *r = (css_rule_import *) rule;

	if (sheet == NULL || rule == NULL || import == NULL)
		return CSS_BADPARM;

	/* Ensure rule is a CSS_RULE_IMPORT */
	if (rule->type != CSS_RULE_IMPORT)
		return CSS_INVALID;

	/* Set the rule's sheet field */
	r->sheet = import;

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

