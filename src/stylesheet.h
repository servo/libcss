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

#include <parserutils/utils/hash.h>

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/stylesheet.h>
#include <libcss/types.h>

#include "parse/parse.h"
#include "select/hash.h"

typedef struct css_rule css_rule;
typedef struct css_selector css_selector;

/** \todo would a parserutils_buffer be better here? */
typedef struct css_style {
	uint32_t length;		/**< Length, in bytes, of bytecode */
	void *bytecode;			/**< Pointer to bytecode */
} css_style;

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

typedef struct css_selector_detail {
	const parserutils_hash_entry *name;	/**< Interned name */
	const parserutils_hash_entry *value;	/**< Interned value, or NULL */

	uint32_t type : 4,			/**< Type of selector */
	         comb : 2,			/**< Type of combinator */
	         next : 1;			/**< Another selector detail 
						 * follows */
} css_selector_detail;

struct css_selector {
	css_selector *combinator;		/**< Combining selector */

	css_rule *rule;				/**< Owning rule */

#define CSS_SPECIFICITY_A 0x01000000
#define CSS_SPECIFICITY_B 0x00010000
#define CSS_SPECIFICITY_C 0x00000100
#define CSS_SPECIFICITY_D 0x00000001
	uint32_t specificity;			/**< Specificity of selector */

	css_selector_detail data;		/**< Selector data */
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

typedef enum css_rule_parent_type {
	CSS_RULE_PARENT_STYLESHEET,
	CSS_RULE_PARENT_RULE
} css_rule_parent_type;

struct css_rule {
	void *parent;				/**< containing rule or owning 
						 * stylesheet (defined by ptype)
						 */
	css_rule *next;				/**< next in list */
	css_rule *prev;				/**< previous in list */

	uint32_t type  :  4,			/**< css_rule_type */
	         index : 16,			/**< index in sheet */
	         items :  8,			/**< # items in rule */
	         ptype :  1;			/**< css_rule_parent_type */
};

typedef struct css_rule_selector {
	css_rule base;

	css_selector **selectors;
	css_style *style;
} css_rule_selector;

typedef struct css_rule_media {
	css_rule base;

	uint32_t media;

	css_rule *first_child;
	css_rule *last_child;
} css_rule_media;

typedef struct css_rule_font_face {
	css_rule base;

	css_style *style;
} css_rule_font_face;

typedef struct css_rule_page {
	css_rule base;

	css_selector **selectors;
	css_style *style;
} css_rule_page;

typedef struct css_rule_import {
	css_rule base;

	css_stylesheet *sheet;
} css_rule_import;

typedef struct css_rule_charset {
	css_rule base;

	const parserutils_hash_entry *encoding;	/** \todo use MIB enum? */
} css_rule_charset;

struct css_stylesheet {
	css_selector_hash *selectors;		/**< Hashtable of selectors */

	uint32_t rule_count;			/**< Number of rules in sheet */
	css_rule *rule_list;			/**< List of rules in sheet */
	css_rule *last_rule;			/**< Last rule in list */

	bool disabled;				/**< Whether this sheet is 
						 * disabled */

	char *url;				/**< URL of this sheet */
	char *title;				/**< Title of this sheet */

	css_origin origin;			/**< Origin of stylesheet */
	uint32_t media;				/**< Bitfield of media types */

	void *ownerNode;			/**< Owning node in document */
	css_rule *ownerRule;			/**< Owning rule in parent */

	css_import_handler import;		/**< Import callback */
	void *import_pw;			/**< Import handler data */

	css_language_level level;		/**< Language level of sheet */
	css_parser *parser;			/**< Core parser for sheet */
	void *parser_frontend;			/**< Frontend parser */

	parserutils_hash *dictionary;		/**< String dictionary */

	css_alloc alloc;			/**< Allocation function */
	void *pw;				/**< Private word */
};

css_error css_stylesheet_style_create(css_stylesheet *sheet, uint32_t len,
		css_style **style);
css_error css_stylesheet_style_destroy(css_stylesheet *sheet, css_style *style);

css_error css_stylesheet_selector_create(css_stylesheet *sheet,
		const parserutils_hash_entry *name, css_selector **selector);
css_error css_stylesheet_selector_destroy(css_stylesheet *sheet,
		css_selector *selector);

css_error css_stylesheet_selector_detail_init(css_stylesheet *sheet,
		css_selector_type type, const parserutils_hash_entry *name,
		const parserutils_hash_entry *value, 
		css_selector_detail *detail);

css_error css_stylesheet_selector_append_specific(css_stylesheet *sheet,
		css_selector **parent, const css_selector_detail *specific);

css_error css_stylesheet_selector_combine(css_stylesheet *sheet,
		css_combinator type, css_selector *a, css_selector *b);

css_error css_stylesheet_rule_create(css_stylesheet *sheet, css_rule_type type,
		css_rule **rule);
css_error css_stylesheet_rule_destroy(css_stylesheet *sheet, css_rule *rule);

css_error css_stylesheet_rule_add_selector(css_stylesheet *sheet, 
		css_rule *rule, css_selector *selector);

css_error css_stylesheet_rule_append_style(css_stylesheet *sheet,
		css_rule *rule, css_style *style);

css_error css_stylesheet_rule_set_charset(css_stylesheet *sheet,
		css_rule *rule, const parserutils_hash_entry *charset);

css_error css_stylesheet_rule_set_import(css_stylesheet *sheet,
		css_rule *rule, css_stylesheet *import);

/** \todo registering other rule-type data with css_rules */

css_error css_stylesheet_add_rule(css_stylesheet *sheet, css_rule *rule);
css_error css_stylesheet_remove_rule(css_stylesheet *sheet, css_rule *rule);

#endif

