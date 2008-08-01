#include <inttypes.h>
#include <stdio.h>

#include <libcss/libcss.h>

#include "charset/detect.h"
#include "utils/utils.h"

#include "lex/lex.h"
#include "parse/parse.h"
#include "parse/css21.h"

#include "testutils.h"

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

int main(int argc, char **argv)
{
	css_parser *parser;
	css_css21 *css21;
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

	css21 = css_css21_create((css_stylesheet *) 10, parser, 
			myrealloc, NULL);
	assert(css21 != NULL);

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

	css_css21_destroy(css21);

	css_parser_destroy(parser);

	assert(css_finalise(myrealloc, NULL) == CSS_OK);

	printf("PASS\n");

	return 0;
}

