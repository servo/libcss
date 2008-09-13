/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "stylesheet.h"
#include "utils/utils.h"

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
		css_selector_type type, css_string *name, css_string *value)
{
	UNUSED(sheet);
	UNUSED(type);
	UNUSED(name);
	UNUSED(value);

	return NULL;
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
	UNUSED(sheet);
	UNUSED(parent);
	UNUSED(specific);

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
	UNUSED(sheet);
	UNUSED(type);
	UNUSED(a);
	UNUSED(b);

	/** \todo Need to ensure that there is no existing combinator on B */

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
	UNUSED(sheet);
	UNUSED(type);

	return NULL;
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
	UNUSED(sheet);
	UNUSED(rule);
	UNUSED(selector);

	/** \todo Ensure rule is a CSS_RULE_SELECTOR */
	/** \todo Set selector's rule field */
	
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
	UNUSED(sheet);
	UNUSED(rule);

	/** \todo Fill in rule's index and owner fields */
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


