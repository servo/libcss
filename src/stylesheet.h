/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_stylesheet_h_
#define css_stylesheet_h_

#include <inttypes.h>
#include <stdio.h>

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/stylesheet.h>
#include <libcss/types.h>

#include "parse/parse.h"

typedef struct css_rule css_rule;
typedef struct css_selector css_selector;
typedef struct css_style css_style;

typedef enum css_selector_type {
	CSS_SELECTOR_ELEMENT,
	CSS_SELECTOR_CLASS,
	CSS_SELECTOR_ID,
	CSS_SELECTOR_PSEUDO,
	CSS_SELECTOR_ATTRIBUTE,
	CSS_SELECTOR_ATTRIBUTE_EQUAL,
	CSS_SELECTOR_ATTRIBUTE_DASHMATCH,
	CSS_SELECTOR_ATTRIBUTE_INCLUDES
} css_selector_type;

typedef enum css_combinator {
	CSS_COMBINATOR_NONE,
	CSS_COMBINATOR_ANCESTOR,
	CSS_COMBINATOR_PARENT,
	CSS_COMBINATOR_SIBLING
} css_combinator;

struct css_selector {
	css_selector_type type;			/**< Type of selector */

	struct {
		css_string name;
		css_string value;
	} data;					/**< Selector data */

	uint32_t specificity;			/**< Specificity of selector */
	css_selector *specifics;		/**< Selector specifics */

	css_combinator combinator_type;		/**< Type of combinator */
	css_selector *combinator;		/**< Combining selector */

	css_rule *rule;				/**< Owning rule */

	css_style *style;			/**< Applicable style */

	css_selector *next;			/**< Next selector in list */
	css_selector *prev;			/**< Previous selector */
};

typedef enum css_rule_type {
	CSS_RULE_UNKNOWN,
	CSS_RULE_SELECTOR,
	CSS_RULE_CHARSET,
	CSS_RULE_IMPORT,
	CSS_RULE_MEDIA,
	CSS_RULE_FONT_FACE,
	CSS_RULE_PAGE
} css_rule_type;

struct css_rule {
	css_rule_type type;			/**< Type of rule */

	union {
		struct {
			uint32_t selector_count;
			css_selector **selectors;
		} selector;
		struct {
			uint32_t media;
			uint32_t rule_count;
			css_rule **rules;	/** \todo why this? isn't the 
						 * child list sufficient? */
		} media;
		struct {
			css_style *style;
		} font_face;
		struct {
			uint32_t selector_count;
			css_selector **selectors;
			css_style *style;
		} page;
		struct {
			css_stylesheet *sheet;
		} import;
		struct {
			char *encoding;		/** \todo use MIB enum? */
		} charset;
	} data;					/**< Rule data */

	uint32_t index;				/**< Index of rule in sheet */

	css_stylesheet *owner;			/**< Owning sheet */

	css_rule *parent;			/**< Parent rule */
	css_rule *first_child;			/**< First in child list */
	css_rule *last_child;			/**< Last in child list */
	css_rule *next;				/**< Next rule */
	css_rule *prev;				/**< Previous rule */
};

struct css_stylesheet {
#define HASH_SIZE (37)
	css_selector *selectors[HASH_SIZE];	/**< Hashtable of selectors */

	uint32_t rule_count;			/**< Number of rules in sheet */
	css_rule *rule_list;			/**< List of rules in sheet */

	bool disabled;				/**< Whether this sheet is 
						 * disabled */

	char *url;				/**< URL of this sheet */
	char *title;				/**< Title of this sheet */

	css_origin origin;			/**< Origin of stylesheet */
	uint32_t media;				/**< Bitfield of media types */

	void *ownerNode;			/**< Owning node in document */
	css_rule *ownerRule;			/**< Owning rule in parent */

	css_stylesheet *parent;			/**< Parent sheet */
	css_stylesheet *first_child;		/**< First in child list */
	css_stylesheet *last_child;		/**< Last in child list */
	css_stylesheet *next;			/**< Next in sibling list */
	css_stylesheet *prev;			/**< Previous in sibling list */

	css_import_handler import;		/**< Import callback */
	void *import_pw;			/**< Import handler data */

	css_language_level level;		/**< Language level of sheet */
	css_parser *parser;			/**< Core parser for sheet */
	void *parser_frontend;			/**< Frontend parser */

	css_alloc alloc;			/**< Allocation function */
	void *pw;				/**< Private word */
};

css_selector *css_stylesheet_selector_create(css_stylesheet *sheet,
		css_selector_type type, const css_string *name, 
		const css_string *value);
void css_stylesheet_selector_destroy(css_stylesheet *sheet,
		css_selector *selector);

css_error css_stylesheet_selector_append_specific(css_stylesheet *sheet,
		css_selector *parent, css_selector *specific);

css_error css_stylesheet_selector_combine(css_stylesheet *sheet,
		css_combinator type, css_selector *a, css_selector *b);

/** \todo something about adding style declarations to a selector */

css_rule *css_stylesheet_rule_create(css_stylesheet *sheet, css_rule_type type);
void css_stylesheet_rule_destroy(css_stylesheet *sheet, css_rule *rule);

css_error css_stylesheet_rule_add_selector(css_stylesheet *sheet, 
		css_rule *rule, css_selector *selector);

/** \todo registering other rule-type data with css_rules */

css_error css_stylesheet_add_rule(css_stylesheet *sheet, css_rule *rule);
css_error css_stylesheet_remove_rule(css_stylesheet *sheet, css_rule *rule);

void css_stylesheet_dump(css_stylesheet *sheet, FILE *target);

#endif

