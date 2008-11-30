#include <inttypes.h>
#include <stdio.h>

#include <libcss/libcss.h>
#include "stylesheet.h"

#include "testutils.h"

#define ITERATIONS (10)
#define DUMP_HASH (0)
#define DUMP_CSS (0)

extern void parserutils_hash_dump(parserutils_hash *hash);

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

int main(int argc, char **argv)
{
	css_stylesheet *sheet;
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

	for (int count = 0; count < ITERATIONS; count++) {

		assert(css_stylesheet_create(CSS_LEVEL_21, "UTF-8", argv[2], 
				NULL, CSS_ORIGIN_AUTHOR, CSS_MEDIA_ALL, NULL, 
				NULL, myrealloc, NULL, &sheet) == CSS_OK);

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

			error = css_stylesheet_append_data(sheet, buf, 
					CHUNK_SIZE);
			assert(error == CSS_OK || error == CSS_NEEDDATA);

			len -= CHUNK_SIZE;
		}

		if (len > 0) {
			fread(buf, 1, len, fp);

			error = css_stylesheet_append_data(sheet, buf, len);
			assert(error == CSS_OK || error == CSS_NEEDDATA);

			len = 0;
		}

		fclose(fp);

		assert(css_stylesheet_data_done(sheet) == CSS_OK);

#if DUMP_HASH
		parserutils_hash_dump(sheet->dictionary);
#endif

#if DUMP_CSS
		css_stylesheet_dump(sheet, stdout);
#endif

		css_stylesheet_destroy(sheet);
	}

	assert(css_finalise(myrealloc, NULL) == CSS_OK);

	printf("PASS\n");

	return 0;
}

