/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"

/**
 * Opcode names, indexed by opcode
 */
static const char *opcode_names[] = {
	"azimuth",
	"background-attachment",
	"background-color",
	"background-image",
	"background-position",
	"background-repeat",
	"border-collapse",
	"border-spacing",
	"border-trbl-color",
	"border-trbl-style",
	"border-trbl-width",
	"bottom",
	"caption-side",
	"clear",
	"clip",
	"color",
	"content",
	"counter-increment",
	"counter-reset",
	"cue-after",
	"cue-before",
	"cursor",
	"direction",
	"display",
	"elevation",
	"empty-cells",
	"float",
	"font-family",
	"font-size",
	"font-style",
	"font-variant",
	"font-weight",
	"height",
	"left",
	"letter-spacing",
	"line-height",
	"list-style-image",
	"list-style-position",
	"list-style-type",
	"margin-trbl",
	"max-height",
	"max-width",
	"min-height",
	"min-width",
	"orphans",
	"outline-color",
	"outline-style",
	"outline-width",
	"overflow",
	"padding-trbl",
	"page-break-after",
	"page-break-before",
	"page-break-inside",
	"pause-after",
	"pause-before",
	"pitch-range",
	"pitch",
	"play-during",
	"position",
	"quotes",
	"richness",
	"right",
	"speak-header",
	"speak-numeral",
	"speak-punctuation",
	"speak",
	"speech-rate",
	"stress",
	"table-layout",
	"text-align",
	"text-decoration",
	"text-indent",
	"text-transform",
	"top",
	"unicode-bidi",
	"vertical-align",
	"visibility",
	"voice-family",
	"volume",
	"white-space",
	"widows",
	"width",
	"word-spacing",
	"z-index",
};

/**
 * Dump a CSS bytecode stream to the given file handle
 * 
 * \param bytecode  The stream to dump
 * \param length    Length, in bytes, of bytecode
 * \param fp        File handle to output to
 */
void css_bytecode_dump(void *bytecode, uint32_t length, FILE *fp)
{
	uint32_t offset = 0;

#define ADVANCE(n) do {					\
	offset += (n);					\
	bytecode = ((uint8_t *) bytecode) + (n);	\
} while(0)

	while (offset < length) {
		opcode op;
		uint8_t flags;
		uint16_t value;
		uint32_t opv = *((uint32_t *) bytecode);

		ADVANCE(sizeof(opv));

		op = getOpcode(opv);

		fprintf(fp, "%s: ", opcode_names[op]);

		flags = getFlags(opv);

		if (flags & FLAG_INHERIT) {
			fprintf(fp, "inherit");
		} else { 
			value = getValue(opv);

			switch (op) {
			case OP_BACKGROUND_ATTACHMENT:
				switch (value) {
				case BACKGROUND_ATTACHMENT_FIXED:
					fprintf(fp, "fixed");
					break;
				case BACKGROUND_ATTACHMENT_SCROLL:
					fprintf(fp, "scroll");
					break;
				}
				break;
			case OP_BACKGROUND_COLOR:
				switch (value) {
				case BACKGROUND_COLOR_TRANSPARENT:
					fprintf(fp, "transparent");
					break;
				case BACKGROUND_COLOR_SET:
				{
					uint32_t colour = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(colour));
					fprintf(fp, "#%08x", colour);
				}	
					break;
				}
				break;
			case OP_BACKGROUND_IMAGE:
				switch (value) {
				case BACKGROUND_IMAGE_NONE:
					fprintf(fp, "none");
					break;
				case BACKGROUND_IMAGE_URI:
				{
					parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
					ADVANCE(sizeof(ptr));
					fprintf(fp, "url('%.*s')", 
							(int) ptr->len, 
							(char *) ptr->data);
				}
					break;
				}
				break;
			case OP_BACKGROUND_REPEAT:
				switch (value) {
				case BACKGROUND_REPEAT_NO_REPEAT:
					fprintf(fp, "no-repeat");
					break;
				case BACKGROUND_REPEAT_REPEAT_X:
					fprintf(fp, "repeat-x");
					break;
				case BACKGROUND_REPEAT_REPEAT_Y:
					fprintf(fp, "repeat-y");
					break;
				case BACKGROUND_REPEAT_REPEAT:
					fprintf(fp, "repeat");
					break;
				}
				break;
			case OP_CLEAR:
				switch (value) {
				case CLEAR_NONE:
					fprintf(fp, "none");
					break;
				case CLEAR_LEFT:
					fprintf(fp, "left");
					break;
				case CLEAR_RIGHT:
					fprintf(fp, "right");
					break;
				case CLEAR_BOTH:
					fprintf(fp, "both");
					break;
				}
				break;
			default:
				fprintf(fp, "Unknown opcode %x", op);
				break;
			}
		}

		if (flags & FLAG_IMPORTANT)
			fprintf(fp, " !important");

		fprintf(fp, "; ");
	}

#undef ADVANCE

}


