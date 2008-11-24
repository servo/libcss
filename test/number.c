#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utils.h"

#include "testutils.h"

typedef struct line_ctx {
	size_t buflen;
	size_t bufused;
	uint8_t *buf;

	size_t explen;
	char exp[256];

	bool indata;
	bool inexp;
} line_ctx;

static bool handle_line(const char *data, size_t datalen, void *pw);
static void run_test(const uint8_t *data, size_t len, 
		const char *exp, size_t explen);

int main(int argc, char **argv)
{
	line_ctx ctx;

	if (argc != 3) {
		printf("Usage: %s <aliases_file> <filename>\n", argv[0]);
		return 1;
	}

	ctx.buflen = parse_filesize(argv[2]);
	if (ctx.buflen == 0)
		return 1;

	ctx.buf = malloc(ctx.buflen);
	if (ctx.buf == NULL) {
		printf("Failed allocating %u bytes\n",
				(unsigned int) ctx.buflen);
		return 1;
	}

	ctx.buf[0] = '\0';
	ctx.bufused = 0;
	ctx.explen = 0;
	ctx.indata = false;
	ctx.inexp = false;

	assert(parse_testfile(argv[2], handle_line, &ctx) == true);

	/* and run final test */
	if (ctx.bufused > 0)
		run_test(ctx.buf, ctx.bufused - 1, ctx.exp, ctx.explen);

	free(ctx.buf);

	printf("PASS\n");

	return 0;
}

bool handle_line(const char *data, size_t datalen, void *pw)
{
	line_ctx *ctx = (line_ctx *) pw;

	if (data[0] == '#') {
		if (ctx->inexp) {
			/* This marks end of testcase, so run it */

			run_test(ctx->buf, ctx->bufused - 1, 
					ctx->exp, ctx->explen);

			ctx->buf[0] = '\0';
			ctx->bufused = 0;

			ctx->explen = 0;
		}

		if (ctx->indata && strncasecmp(data+1, "expected", 8) == 0) {
			ctx->indata = false;
			ctx->inexp = true;
		} else if (!ctx->indata) {
			ctx->indata = (strncasecmp(data+1, "data", 4) == 0);
			ctx->inexp  = (strncasecmp(data+1, "expected", 8) == 0);
		} else {
			memcpy(ctx->buf + ctx->bufused, data, datalen);
			ctx->bufused += datalen;
		}
	} else {
		if (ctx->indata) {
			memcpy(ctx->buf + ctx->bufused, data, datalen);
			ctx->bufused += datalen;
		}
		if (ctx->inexp) {
			if (data[datalen - 1] == '\n')
				datalen -= 1;

			memcpy(ctx->exp, data, datalen);
			ctx->explen = datalen;
		}
	}

	return true;
}

void run_test(const uint8_t *data, size_t len, const char *exp, size_t explen)
{
	css_string in = { (uint8_t *) data, len };
	size_t consumed;
	fixed result;
	char buf[256];

	UNUSED(exp);
	UNUSED(explen);

	result = number_from_css_string(&in, &consumed);

	snprintf(buf, sizeof buf, "%.3f", FIXTOFLT(result));

	printf("got: %s expected: %.*s\n", buf, (int) explen, exp);

	assert(strncmp(buf, exp, explen) == 0);
}

