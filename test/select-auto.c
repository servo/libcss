#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcss/libcss.h>
#include <libcss/computed.h>
#include <libcss/select.h>
#include <libcss/stylesheet.h>

#include "utils/utils.h"

#include "dump_computed.h"
#include "testutils.h"

typedef struct attribute {
	char name[32];
	char value[32];
} attribute;

typedef struct node {
	char name[32];

	uint32_t n_attrs;
	attribute *attrs;

	struct node *parent;
	struct node *next;
	struct node *prev;
	struct node *children;
	struct node *last_child;
} node;

typedef struct line_ctx {
	size_t explen;
	size_t expused;
	char *exp;

	bool intree;
	bool insheet;
	bool inerrors;
	bool inexp;

	node *tree;
	node *current;
	uint32_t depth;

	uint32_t n_sheets;
	css_stylesheet **sheets;

	uint64_t media;
	uint64_t pseudo_elements;
	uint64_t pseudo_classes;
	node *target;
} line_ctx;

static bool handle_line(const char *data, size_t datalen, void *pw);
static void parse_tree(line_ctx *ctx, const char *data, size_t len);
static void parse_tree_data(line_ctx *ctx, const char *data, size_t len);
static void parse_sheet(line_ctx *ctx, const char *data, size_t len);
static void parse_media_list(const char **data, size_t *len, uint64_t *media);
static void parse_pseudo_list(const char **data, size_t *len,
		uint64_t *elements, uint64_t *classes);
static void parse_expected(line_ctx *ctx, const char *data, size_t len);
static void run_test(line_ctx *ctx, const char *exp, size_t explen);
static void destroy_tree(node *root);

static css_error node_name(void *pw, void *node, const uint8_t **name, 
		size_t *len);
static css_error named_ancestor_node(void *pw, void *node,
		const uint8_t *name, size_t len,
		void **ancestor);
static css_error named_parent_node(void *pw, void *node,
		const uint8_t *name, size_t len,
		void **parent);
static css_error named_sibling_node(void *pw, void *node,
		const uint8_t *name, size_t len,
		void **sibling);
static css_error parent_node(void *pw, void *node, void **parent);
static css_error sibling_node(void *pw, void *node, void **sibling);
static css_error node_has_class(void *pw, void *node,
		const uint8_t *name, size_t len,
		bool *match);
static css_error node_has_id(void *pw, void *node,
		const uint8_t *name, size_t len,
		bool *match);
static css_error node_has_attribute(void *pw, void *node,
		const uint8_t *name, size_t len,
		bool *match);
static css_error node_has_attribute_equal(void *pw, void *node,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match);
static css_error node_has_attribute_dashmatch(void *pw, void *node,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match);
static css_error node_has_attribute_includes(void *pw, void *node,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match);
static css_error node_is_first_child(void *pw, void *node, bool *match);

static css_select_handler select_handler = {
	node_name,
	named_ancestor_node,
	named_parent_node,
	named_sibling_node,
	parent_node,
	sibling_node,
	node_has_class,
	node_has_id,
	node_has_attribute,
	node_has_attribute_equal,
	node_has_attribute_dashmatch,
	node_has_attribute_includes,
	node_is_first_child
};

static void *myrealloc(void *data, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(data, len);
}

int main(int argc, char **argv)
{
	line_ctx ctx;

	if (argc != 3) {
		printf("Usage: %s <aliases_file> <filename>\n", argv[0]);
		return 1;
	}

	assert(css_initialise(argv[1], myrealloc, NULL) == CSS_OK);

	memset(&ctx, 0, sizeof(ctx));

	assert(parse_testfile(argv[2], handle_line, &ctx) == true);

	/* and run final test */
	if (ctx.tree != NULL)
		run_test(&ctx, ctx.exp, ctx.expused);

	free(ctx.exp);

	assert(css_finalise(myrealloc, NULL) == CSS_OK);

	printf("PASS\n");

	return 0;
}

bool handle_line(const char *data, size_t datalen, void *pw)
{
	line_ctx *ctx = (line_ctx *) pw;
	css_error error;

	if (data[0] == '#') {
		if (ctx->intree) {
			if (strncasecmp(data+1, "errors", 6) == 0) {
				ctx->intree = false;
				ctx->insheet = false;
				ctx->inerrors = true;
				ctx->inexp = false;
			} else {
				/* Assume start of stylesheet */
				parse_sheet(ctx, data + 1, datalen - 1);

				ctx->intree = false;
				ctx->insheet = true;
				ctx->inerrors = false;
				ctx->inexp = false;
			}
		} else if (ctx->insheet) {
			if (strncasecmp(data+1, "errors", 6) == 0) {
				assert(css_stylesheet_data_done(
						ctx->sheets[ctx->n_sheets - 1]) 
						== CSS_OK);

				ctx->intree = false;
				ctx->insheet = false;
				ctx->inerrors = true;
				ctx->inexp = false;
			} else if (strncasecmp(data+1, "ua", 2) == 0 ||
					strncasecmp(data+1, "user", 4) == 0 ||
					strncasecmp(data+1, "author", 6) == 0) {
				assert(css_stylesheet_data_done(
						ctx->sheets[ctx->n_sheets - 1])
						== CSS_OK);

				parse_sheet(ctx, data + 1, datalen - 1);
			} else {
				error = css_stylesheet_append_data(
						ctx->sheets[ctx->n_sheets - 1], 
						(const uint8_t *) data, 
						datalen);
				assert(error == CSS_OK || 
						error == CSS_NEEDDATA);
			}
		} else if (ctx->inerrors) {
			ctx->intree = false;
			ctx->insheet = false;
			ctx->inerrors = false;
			ctx->inexp = true;
		} else if (ctx->inexp) {
			/* This marks end of testcase, so run it */
			run_test(ctx, ctx->exp, ctx->expused);

			ctx->expused = 0;


			ctx->intree = false;
			ctx->insheet = false;
			ctx->inerrors = false;
			ctx->inexp = false;
		} else {
			/* Start state */
			if (strncasecmp(data+1, "tree", 4) == 0) {
				parse_tree(ctx, data + 5, datalen - 5);

				ctx->intree = true;
				ctx->insheet = false;
				ctx->inerrors = false;
				ctx->inexp = false;
			}
		}
	} else {
		if (ctx->intree) {
			/* Not interested in the '|' */
			parse_tree_data(ctx, data + 1, datalen - 1);
		} else if (ctx->insheet) {
			error = css_stylesheet_append_data(
					ctx->sheets[ctx->n_sheets - 1], 
					(const uint8_t *) data, datalen);
			assert(error == CSS_OK || error == CSS_NEEDDATA);
		} else if (ctx->inexp) {
			parse_expected(ctx, data, datalen);
		}
	}

	return true;
}

void parse_tree(line_ctx *ctx, const char *data, size_t len)
{
	const char *p = data;
	const char *end = data + len;
	size_t left;

	/* [ <media_list> <pseudo>? ] ? */

	ctx->media = CSS_MEDIA_ALL;
	ctx->pseudo_elements = 0;
	ctx->pseudo_classes = 0;

	/* Consume any leading whitespace */
	while (p < end && isspace(*p))
		p++;

	if (p < end) {
		left = end - p;

		parse_media_list(&p, &left, &ctx->media);

		end = p + left;
	}

	if (p < end) {
		left = end - p;

		parse_pseudo_list(&p, &left, &ctx->pseudo_elements, 
				&ctx->pseudo_classes);
	}
}

void parse_tree_data(line_ctx *ctx, const char *data, size_t len)
{
	const char *p = data;
	const char *end = data + len;
	const char *name = NULL;
	const char *value = NULL;
	size_t namelen = 0;
	size_t valuelen = 0;
	uint32_t depth = 0;
	bool target = false;

	/* ' '{depth+1} [ <element> '*'? | <attr> ]
	 * 
	 * <element> ::= [^=*[:space:]]+
	 * <attr>    ::= [^=*[:space:]]+ '=' [^[:space:]]*
	 */

	while (p < end && isspace(*p)) {
		depth++;
		p++;
	}
	depth--;

	/* Get element/attribute name */
	name = p;
	while (p < end && *p != '=' && *p != '*' && isspace(*p) == false) {
		namelen++;
		p++;
	}

	/* Skip whitespace */
	while (p < end && isspace(*p))
		p++;

	if (p < end && *p == '=') {
		/* Attribute value */
		p++;

		value = p;

		while (p < end && isspace(*p) == false) {
			valuelen++;
			p++;
		}
	} else if (p < end && *p == '*') {
		/* Element is target node */
		target = true;
	}

	if (value == NULL) {
		/* We have an element, so create it */
		node *n = malloc(sizeof(node));
		assert(n != NULL);

		memset(n, 0, sizeof(node));

		memcpy(n->name, name, min(namelen, sizeof(n->name)));
		n->name[min(namelen, sizeof(n->name))] = '\0';

		/* Insert it into tree */
		if (ctx->tree == NULL) {
			ctx->tree = n;
		} else {
			assert(depth > 0);
			assert(depth <= ctx->depth + 1);

			/* Find node to insert into */
			while (depth <= ctx->depth) {
				ctx->depth--;
				ctx->current = ctx->current->parent;
			}

			/* Insert into current node */
			if (ctx->current->children == NULL) {
				ctx->current->children = n;
				ctx->current->last_child = n;
			} else {
				ctx->current->last_child->next = n;
				n->prev = ctx->current->last_child;

				ctx->current->last_child = n;
			}
			n->parent = ctx->current;
		}

		ctx->current = n;
		ctx->depth = depth;

		/* Mark the target, if it's us */
		if (target)
			ctx->target = n;
	} else {
		/* New attribute */
		attribute *attr;

		attribute *temp = realloc(ctx->current->attrs,
			(ctx->current->n_attrs + 1) * sizeof(attribute));
		assert(temp != NULL);

		ctx->current->attrs = temp;

		attr = &ctx->current->attrs[ctx->current->n_attrs];

		memcpy(attr->name, name, min(namelen, sizeof(attr->name)));
		attr->name[min(namelen, sizeof(attr->name))] = '\0';

		memcpy(attr->value, value, min(valuelen, sizeof(attr->value)));
		attr->value[min(valuelen, sizeof(attr->value))] = '\0';

		ctx->current->n_attrs++;
	}
}

void parse_sheet(line_ctx *ctx, const char *data, size_t len)
{
	const char *p;
	const char *end = data + len;
	css_origin origin;
	uint64_t media = CSS_MEDIA_ALL;
	css_stylesheet *sheet;
	css_stylesheet **temp;

	/* <origin> <media_list>? */

	/* Find end of origin */
	for (p = data; p < end; p++) {
		if (isspace(*p))
			break;
	}

	if (p - data == 6 && strncasecmp(data, "author", 6) == 0)
		origin = CSS_ORIGIN_AUTHOR;
	else if (p - data == 4 && strncasecmp(data, "user", 4) == 0)
		origin = CSS_ORIGIN_USER;
	else if (p - data == 2 && strncasecmp(data, "ua", 2) == 0)
		origin = CSS_ORIGIN_UA;
	else
		assert(0 && "Unknown stylesheet origin");

	/* Skip any whitespace */
	while (p < end && isspace(*p))
		p++;

	if (p < end) {
		size_t ignored = end - p;

		parse_media_list(&p, &ignored, &media);
	}

	/** \todo How are we going to handle @import? */
	assert(css_stylesheet_create(CSS_LEVEL_21, "UTF-8", "foo", "foo", 
			origin, media, NULL, NULL, myrealloc, NULL, &sheet) == 
			CSS_OK);

	/* Extend array of sheets and append new sheet to it */
	temp = realloc(ctx->sheets, 
			(ctx->n_sheets + 1) * sizeof(css_stylesheet *));
	assert(temp != NULL);

	ctx->sheets = temp;

	ctx->sheets[ctx->n_sheets] = sheet;

	ctx->n_sheets++;
}

void parse_media_list(const char **data, size_t *len, uint64_t *media)
{
	const char *p = *data;
	const char *end = p + *len;
	uint64_t result = 0;

	/* <medium> [ ',' <medium> ]* */

	while (p < end) {
		const char *start = p;

		/* consume a medium */
		while (isspace(*p) == false && *p != ',')
			p++;

		if (p - start == 10 && 
				strncasecmp(start, "projection", 10) == 0)
			result |= CSS_MEDIA_PROJECTION;
		else if (p - start == 8 &&
				strncasecmp(start, "handheld", 8) == 0)
			result |= CSS_MEDIA_HANDHELD;
		else if (p - start == 8 &&
				strncasecmp(start, "embossed", 8) == 0)
			result |= CSS_MEDIA_EMBOSSED;
		else if (p - start == 7 &&
				strncasecmp(start, "braille", 7) == 0)
			result |= CSS_MEDIA_BRAILLE;
		else if (p - start == 6 &&
				strncasecmp(start, "speech", 6) == 0)
			result |= CSS_MEDIA_SPEECH;
		else if (p - start == 6 &&
				strncasecmp(start, "screen", 6) == 0)
			result |= CSS_MEDIA_SCREEN;
		else if (p - start == 5 &&
				strncasecmp(start, "print", 5) == 0)
			result |= CSS_MEDIA_PRINT;
		else if (p - start == 5 &&
				strncasecmp(start, "aural", 5) == 0)
			result |= CSS_MEDIA_AURAL;
		else if (p - start == 3 &&
				strncasecmp(start, "tty", 3) == 0)
			result |= CSS_MEDIA_TTY;
		else if (p - start == 3 &&
				strncasecmp(start, "all", 3) == 0)
			result |= CSS_MEDIA_ALL;
		else if (p - start == 2 &&
				strncasecmp(start, "tv", 2) == 0)
			result |= CSS_MEDIA_TV;
		else
			assert(0 && "Unknown media type");

		/* Consume whitespace */
		while (p < end && isspace(*p))
			p++;

		/* Stop if we've reached the end */
		if (p == end || *p != ',')
			break;

		/* Consume comma */
		p++;

		/* Consume whitespace */
		while (p < end && isspace(*p))
			p++;
	}

	*media = result;

	*data = p;
	*len = end - p;
}

void parse_pseudo_list(const char **data, size_t *len, uint64_t *elements, 
		uint64_t *classes)
{
	const char *p = *data;
	const char *end = p + *len;

	/* <pseudo> [ ',' <pseudo> ]* */

	*elements = 0;
	*classes = 0;

	while (p < end) {
		const char *start = p;

		/* consume a pseudo */
		while (isspace(*p) == false && *p != ',')
			p++;

		/* Pseudo classes */
		if (p - start == 7 &&
				strncasecmp(start, "visited", 7) == 0)
			*classes |= CSS_PSEUDO_CLASS_VISITED;
		else if (p - start == 6 &&
				strncasecmp(start, "active", 6) == 0)
			*classes |= CSS_PSEUDO_CLASS_ACTIVE;
		else if (p - start == 5 &&
				strncasecmp(start, "first", 5) == 0)
			*classes |= CSS_PSEUDO_CLASS_FIRST;
		else if (p - start == 5 &&
				strncasecmp(start, "right", 5) == 0)
			*classes |= CSS_PSEUDO_CLASS_RIGHT;
		else if (p - start == 5 &&
				strncasecmp(start, "focus", 5) == 0)
			*classes |= CSS_PSEUDO_CLASS_FOCUS;
		else if (p - start == 5 &&
				strncasecmp(start, "hover", 5) == 0)
			*classes |= CSS_PSEUDO_CLASS_HOVER;
		else if (p - start == 4 &&
				strncasecmp(start, "left", 4) == 0)
			*classes |= CSS_PSEUDO_CLASS_LEFT;
		else if (p - start == 4 &&
				strncasecmp(start, "link", 4) == 0)
			*classes |= CSS_PSEUDO_CLASS_LINK;
		/* Pseudo elements */
		else if (p - start == 12 &&
				strncasecmp(start, "first-letter", 12) == 0)
			*elements |= CSS_PSEUDO_ELEMENT_FIRST_LETTER;
		else if (p - start == 10 &&
				strncasecmp(start, "first-line", 10) == 0)
			*elements |= CSS_PSEUDO_ELEMENT_FIRST_LINE;
		else if (p - start == 6 &&
				strncasecmp(start, "before", 6) == 0)
			*elements |= CSS_PSEUDO_ELEMENT_BEFORE;
		else if (p - start == 5 &&
				strncasecmp(start, "after", 5) == 0)
			*elements |= CSS_PSEUDO_ELEMENT_AFTER;
		else
			assert(0 && "Unknown pseudo");

		/* Consume whitespace */
		while (p < end && isspace(*p))
			p++;

		/* Stop if we've reached the end */
		if (p == end || *p != ',')
			break;

		/* Consume comma */
		p++;

		/* Consume whitespace */
		while (p < end && isspace(*p))
			p++;
	}

	*data = p;
	*len = end - p;
}

void parse_expected(line_ctx *ctx, const char *data, size_t len)
{
	while (ctx->expused + len >= ctx->explen) {
		size_t required = ctx->explen == 0 ? 64 : ctx->explen * 2;
		char *temp = realloc(ctx->exp, required);
		if (temp == NULL) {
			assert(0 && "No memory for expected output");
		}

		ctx->exp = temp;
		ctx->explen = required;
	}

	memcpy(ctx->exp + ctx->expused, data, len);

	ctx->expused += len;
}

void run_test(line_ctx *ctx, const char *exp, size_t explen)
{
	css_select_ctx *select;
	css_computed_style *computed;
	uint32_t i;
	char *buf;
	size_t buflen;
	static int testnum;

	UNUSED(exp);

	buf = malloc(8192);
	if (buf == NULL) {
		assert(0 && "No memory for result data");
	}
	buflen = 8192;

	assert(css_select_ctx_create(myrealloc, NULL, &select) == CSS_OK);

	for (i = 0; i < ctx->n_sheets; i++) {
		assert(css_select_ctx_append_sheet(select, ctx->sheets[i]) == 
				CSS_OK);
	}

	assert(css_computed_style_create(myrealloc, NULL, &computed) == CSS_OK);

	testnum++;

	assert(css_select_style(select, ctx->target, ctx->pseudo_elements,
			ctx->pseudo_classes, ctx->media, computed,
			&select_handler, NULL) == CSS_OK);

	dump_computed_style(computed, buf, &buflen);

	if (8192 - buflen != explen || memcmp(buf, exp, explen) != 0) {
		printf("Expected (%zu):\n%.*s\n", explen, (int) explen, exp);
		printf("Result (%zu):\n%.*s\n", 8192 - buflen,
			(int) (8192 - buflen), buf);
		assert(0 && "Result doesn't match expected");
	}

	/* Clean up */
	css_computed_style_destroy(computed);
	css_select_ctx_destroy(select);

	destroy_tree(ctx->tree);

	for (i = 0; i < ctx->n_sheets; i++) {
		css_stylesheet_destroy(ctx->sheets[i]);
	}

	ctx->tree = NULL;
	ctx->current = NULL;
	ctx->depth = 0;
	ctx->n_sheets = 0;
	free(ctx->sheets);
	ctx->sheets = NULL;
	ctx->target = NULL;

	free(buf);

	printf("Test %d: PASS\n", testnum);
}

void destroy_tree(node *root)
{
	node *n, *p;

	for (n = root->children; n != NULL; n = p) {
		p = n->next;

		destroy_tree(n);
	}

	free(root->attrs);
	free(root);
}


css_error node_name(void *pw, void *n, const uint8_t **name, size_t *len)
{
	node *node = n;

	UNUSED(pw);

	*name = (const uint8_t *) node->name;
	*len = strlen(node->name);

	return CSS_OK;
}

css_error named_ancestor_node(void *pw, void *n,
		const uint8_t *name, size_t len,
		void **ancestor)
{
	node *node = n;

	UNUSED(pw);

	for (node = node->parent; node != NULL; node = node->parent) {
		if (strncasecmp(node->name, (const char *) name, len) == 0)
			break;
	}

	*ancestor = (void *) node;

	return CSS_OK;
}

css_error named_parent_node(void *pw, void *n,
		const uint8_t *name, size_t len,
		void **parent)
{
	node *node = n;

	UNUSED(pw);

	if (node->parent != NULL && strncasecmp(node->parent->name, 
			(const char *) name, len) == 0)
		*parent = (void *) node->parent;
	else
		*parent = NULL;

	return CSS_OK;
}

css_error named_sibling_node(void *pw, void *n,
		const uint8_t *name, size_t len,
		void **sibling)
{
	node *node = n;

	UNUSED(pw);

	for (node = node->prev; node != NULL; node = node->prev) {
		if (strncasecmp(node->name, (const char *) name, len) == 0)
			break;
	}

	*sibling = (void *) node;

	return CSS_OK;
}

css_error parent_node(void *pw, void *n, void **parent)
{
	node *node = n;

	UNUSED(pw);

	*parent = (void *) node->parent;

	return CSS_OK;
}

css_error sibling_node(void *pw, void *n, void **sibling)
{
	node *node = n;

	UNUSED(pw);

	*sibling = (void *) node->prev;

	return CSS_OK;
}

css_error node_has_class(void *pw, void *n,
		const uint8_t *name, size_t len,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, "class", 5) == 0)
			break;
	}

	/* Classes are case-sensitive in HTML */
	if (i != node->n_attrs && strncmp(node->attrs[i].value,
			(const char *) name, len) == 0)
		*match = true;
	else
		*match = false;

	return CSS_OK;
}

css_error node_has_id(void *pw, void *n,
		const uint8_t *name, size_t len,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, "id", 2) == 0)
			break;
	}

	/* IDs are case-sensitive in HTML */
	if (i != node->n_attrs && strncmp(node->attrs[i].value,
			(const char *) name, len) == 0)
		*match = true;
	else
		*match = false;

	return CSS_OK;
}

css_error node_has_attribute(void *pw, void *n,
		const uint8_t *name, size_t len,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, 
				(const char *) name, len) == 0)
			break;
	}

	if (i != node->n_attrs)
		*match = true;
	else
		*match = false;

	return CSS_OK;
}

css_error node_has_attribute_equal(void *pw, void *n,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, 
				(const char *) name, nlen) == 0)
			break;
	}

	/* Attribute values are (mostly) case insensitive */
	if (i != node->n_attrs && strncasecmp(node->attrs[i].value,
			(const char *) value, vlen) == 0)
		*match = true;
	else
		*match = false;

	return CSS_OK;
}

css_error node_has_attribute_dashmatch(void *pw, void *n,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, 
				(const char *) name, nlen) == 0)
			break;
	}

	*match = false;

	if (i != node->n_attrs) {
		const char *p;
		const char *start = node->attrs[i].value;
		const char *end = start + strlen(start) + 1;

		for (p = node->attrs[i].value; p < end; p++) {
			if (*p == '-') {
				if ((size_t) (p - start) == vlen && 
						strncasecmp(start,
						(const char *) value, 
						vlen) == 0) {
					*match = true;
					break;
				}

				start = p + 1;
			}
		}
	}

	return CSS_OK;
}

css_error node_has_attribute_includes(void *pw, void *n,
		const uint8_t *name, size_t nlen,
		const uint8_t *value, size_t vlen,
		bool *match)
{
	node *node = n;
	uint32_t i;

	UNUSED(pw);

	for (i = 0; i < node->n_attrs; i++) {
		if (strncasecmp(node->attrs[i].name, 
				(const char *) name, nlen) == 0)
			break;
	}

	*match = false;

	if (i != node->n_attrs) {
		const char *p;
		const char *start = node->attrs[i].value;
		const char *end = start + strlen(start) + 1;

		for (p = node->attrs[i].value; p < end; p++) {
			if (*p == ' ') {
				if ((size_t) (p - start) == vlen && 
						strncasecmp(start,
						(const char *) value, 
						vlen) == 0) {
					*match = true;
					break;
				}

				start = p + 1;
			}
		}
	}

	return CSS_OK;
}

css_error node_is_first_child(void *pw, void *n, bool *match)
{
	node *node = n;

	UNUSED(pw);

	*match = (node->parent != NULL && node->parent->children == node);

	return CSS_OK;
}
