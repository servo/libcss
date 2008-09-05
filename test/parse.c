#include <inttypes.h>
#include <stdio.h>

#include <libcss/libcss.h>

#include "charset/detect.h"
#include "utils/utils.h"

#include "lex/lex.h"
#include "parse/parse.h"

#include "testutils.h"

static const char *event_names[] = {
	"START_STYLESHEET",
	"END_STYLESHEET",
	"START_RULESET",
	"END_RULESET",
	"START_ATRULE",
	"END_ATRULE",
	"START_BLOCK",
	"END_BLOCK",
	"BLOCK_CONTENT",
	"DECLARATION"
};

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

static css_error event_handler(css_parser_event type, 
		const parserutils_vector *tokens, void *pw)
{
#if 0
	UNUSED(type);
	UNUSED(tokens);
	UNUSED(pw);
#else
	int32_t ctx = 0;
	const css_token *token;

	UNUSED(pw);

	printf("%s%s", tokens != NULL ? "  " : "", event_names[type]);

	if (tokens == NULL) {
		printf("\n");
		return CSS_OK;
	}

	do {
		token = parserutils_vector_iterate(tokens, &ctx);
		if (token == NULL)
			break;

		printf("\n    %d", token->type);

		if (token->data.ptr != NULL)
			printf(" %.*s", token->data.len, token->data.ptr);
	} while (token != NULL);

	printf("\n");
#endif

	return CSS_OK;
}

int main(int argc, char **argv)
{
	css_parser_optparams params;
	css_parser *parser;
	FILE *fp;
	size_t len, origlen;
#define CHUNK_SIZE (4096)
	uint8_t buf[CHUNK_SIZE];
	css_error error;

	if (argc != 3) {
		printf("Usage: %s <aliases_file> <filename>\n", argv[0]);
		return 1;
	}

	/* Initialise library */
	assert(css_initialise(argv[1], myrealloc, NULL) == CSS_OK);

	parser = css_parser_create("UTF-8", CSS_CHARSET_DICTATED, 
			myrealloc, NULL);
	assert(parser != NULL);

	params.event_handler.handler = event_handler;
	params.event_handler.pw = NULL;
	assert(css_parser_setopt(parser, CSS_PARSER_EVENT_HANDLER, 
			&params) == CSS_OK);

	fp = fopen(argv[2], "rb");
	if (fp == NULL) {
		printf("Failed opening %s\n", argv[2]);
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	origlen = len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	while (len >= CHUNK_SIZE) {
		fread(buf, 1, CHUNK_SIZE, fp);

		error = css_parser_parse_chunk(parser, buf, CHUNK_SIZE);
		assert(error == CSS_OK || error == CSS_NEEDDATA);

		len -= CHUNK_SIZE;
	}

	if (len > 0) {
		fread(buf, 1, len, fp);

		error = css_parser_parse_chunk(parser, buf, len);
		assert(error == CSS_OK || error == CSS_NEEDDATA);

		len = 0;
	}

	fclose(fp);

	assert(css_parser_completed(parser) == CSS_OK);

	css_parser_destroy(parser);

	assert(css_finalise(myrealloc, NULL) == CSS_OK);

	printf("PASS\n");

	return 0;
}

