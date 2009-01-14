/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <assert.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "utils/fpmath.h"

#ifndef NDEBUG
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
	"border-%s-color",
	"border-%s-style",
	"border-%s-width",
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
	"margin-%s",
	"max-height",
	"max-width",
	"min-height",
	"min-width",
	"orphans",
	"outline-color",
	"outline-style",
	"outline-width",
	"overflow",
	"padding-%s",
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
 * Sides, indexed by SIDE_*
 */
static const char *sides[] = { "top", "right", "bottom", "left" };

static void dump_number(FILE *fp, fixed val)
{
	if (INTTOFIX(FIXTOINT(val)) == val)
		fprintf(fp, "%d", FIXTOINT(val));
	else
		fprintf(fp, "%f", FIXTOFLT(val));
}

static void dump_unit(FILE *fp, fixed val, uint32_t unit)
{
	dump_number(fp, val);

	switch (unit) {
	case UNIT_PX:
		fprintf(fp, "px");
		break;
	case UNIT_EX:
		fprintf(fp, "ex");
		break;
	case UNIT_EM:
		fprintf(fp, "em");
		break;
	case UNIT_IN:
		fprintf(fp, "in");
		break;
	case UNIT_CM:
		fprintf(fp, "cm");
		break;
	case UNIT_MM:
		fprintf(fp, "mm");
		break;
	case UNIT_PT:
		fprintf(fp, "pt");
		break;
	case UNIT_PC:
		fprintf(fp, "pc");
		break;
	case UNIT_PCT:
		fprintf(fp, "%%");
		break;
	case UNIT_DEG:
		fprintf(fp, "deg");
		break;
	case UNIT_GRAD:
		fprintf(fp, "grad");
		break;
	case UNIT_RAD:
		fprintf(fp, "rad");
		break;
	case UNIT_MS:
		fprintf(fp, "ms");
		break;
	case UNIT_S:
		fprintf(fp, "s");
		break;
	case UNIT_HZ:
		fprintf(fp, "Hz");
		break;
	case UNIT_KHZ:
		fprintf(fp, "kHz");
		break;
	}
}

static void dump_counter(FILE *fp, const parserutils_hash_entry *name, 
		uint32_t value)
{
	fprintf(fp, "counter(%.*s", (int) name->len, (char *) name->data);

	value >>= CONTENT_COUNTER_STYLE_SHIFT;

	switch (value) {
	case LIST_STYLE_TYPE_DISC:
		fprintf(fp, ", disc");
		break;
	case LIST_STYLE_TYPE_CIRCLE:
		fprintf(fp, ", circle");
		break;
	case LIST_STYLE_TYPE_SQUARE:
		fprintf(fp, ", square");
		break;
	case LIST_STYLE_TYPE_DECIMAL:
		break;
	case LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO:
		fprintf(fp, ", decimal-leading-zero");
		break;
	case LIST_STYLE_TYPE_LOWER_ROMAN:
		fprintf(fp, ", lower-roman");
		break;
	case LIST_STYLE_TYPE_UPPER_ROMAN:
		fprintf(fp, ", upper-roman");
		break;
	case LIST_STYLE_TYPE_LOWER_GREEK:
		fprintf(fp, ", lower-greek");
		break;
	case LIST_STYLE_TYPE_LOWER_LATIN:
		fprintf(fp, ", lower-latin");
		break;
	case LIST_STYLE_TYPE_UPPER_LATIN:
		fprintf(fp, ", upper-latin");
		break;
	case LIST_STYLE_TYPE_ARMENIAN:
		fprintf(fp, ", armenian");
		break;
	case LIST_STYLE_TYPE_GEORGIAN:
		fprintf(fp, ", georgian");
		break;
	case LIST_STYLE_TYPE_LOWER_ALPHA:
		fprintf(fp, ", lower-alpha");
		break;
	case LIST_STYLE_TYPE_UPPER_ALPHA:
		fprintf(fp, ", upper-alpha");
		break;
	case LIST_STYLE_TYPE_NONE:
		fprintf(fp, ", none");
		break;
	}

	fprintf(fp, ")");
}

static void dump_counters(FILE *fp, const parserutils_hash_entry *name, 
		const parserutils_hash_entry *separator,
		uint32_t value)
{
	fprintf(fp, "counter(%.*s, %.*s", 
			(int) name->len, (char *) name->data,
			(int) separator->len, (char *) separator->data);

	value >>= CONTENT_COUNTER_STYLE_SHIFT;

	switch (value) {
	case LIST_STYLE_TYPE_DISC:
		fprintf(fp, ", disc");
		break;
	case LIST_STYLE_TYPE_CIRCLE:
		fprintf(fp, ", circle");
		break;
	case LIST_STYLE_TYPE_SQUARE:
		fprintf(fp, ", square");
		break;
	case LIST_STYLE_TYPE_DECIMAL:
		break;
	case LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO:
		fprintf(fp, ", decimal-leading-zero");
		break;
	case LIST_STYLE_TYPE_LOWER_ROMAN:
		fprintf(fp, ", lower-roman");
		break;
	case LIST_STYLE_TYPE_UPPER_ROMAN:
		fprintf(fp, ", upper-roman");
		break;
	case LIST_STYLE_TYPE_LOWER_GREEK:
		fprintf(fp, ", lower-greek");
		break;
	case LIST_STYLE_TYPE_LOWER_LATIN:
		fprintf(fp, ", lower-latin");
		break;
	case LIST_STYLE_TYPE_UPPER_LATIN:
		fprintf(fp, ", upper-latin");
		break;
	case LIST_STYLE_TYPE_ARMENIAN:
		fprintf(fp, ", armenian");
		break;
	case LIST_STYLE_TYPE_GEORGIAN:
		fprintf(fp, ", georgian");
		break;
	case LIST_STYLE_TYPE_LOWER_ALPHA:
		fprintf(fp, ", lower-alpha");
		break;
	case LIST_STYLE_TYPE_UPPER_ALPHA:
		fprintf(fp, ", upper-alpha");
		break;
	case LIST_STYLE_TYPE_NONE:
		fprintf(fp, ", none");
		break;
	}

	fprintf(fp, ")");
}


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
		uint32_t value;
		uint32_t opv = *((uint32_t *) bytecode);

		ADVANCE(sizeof(opv));

		op = getOpcode(opv);

		if (op == OP_BORDER_TRBL_COLOR || op == OP_BORDER_TRBL_STYLE ||
				op == OP_BORDER_TRBL_WIDTH ||
				op == OP_MARGIN_TRBL || op == OP_PADDING_TRBL) {
			const uint32_t side = (getValue(opv) & SIDE_LEFT) >> 8;
			fprintf(fp, opcode_names[op], sides[side]);
			fprintf(fp, ": ");
		} else
			fprintf(fp, "%s: ", opcode_names[op]);

		flags = getFlags(opv);

		if (flags & FLAG_INHERIT) {
			fprintf(fp, "inherit");
		} else { 
			value = getValue(opv);

			switch (op) {
			case OP_AZIMUTH:
				switch (value) {
				case AZIMUTH_ANGLE:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case AZIMUTH_LEFTWARDS:
					fprintf(fp, "leftwards");
					break;
				case AZIMUTH_RIGHTWARDS:
					fprintf(fp, "rightwards");
					break;
				case AZIMUTH_LEFT_SIDE:
					fprintf(fp, "left-side");
					break;
				case AZIMUTH_FAR_LEFT:
					fprintf(fp, "far-left");
					break;
				case AZIMUTH_LEFT:
					fprintf(fp, "left");
					break;
				case AZIMUTH_CENTER_LEFT:
					fprintf(fp, "center-left");
					break;
				case AZIMUTH_CENTER:
					fprintf(fp, "center");
					break;
				case AZIMUTH_CENTER_RIGHT:
					fprintf(fp, "center-right");
					break;
				case AZIMUTH_RIGHT:
					fprintf(fp, "right");
					break;
				case AZIMUTH_FAR_RIGHT:
					fprintf(fp, "far-right");
					break;
				case AZIMUTH_RIGHT_SIDE:
					fprintf(fp, "right-side");
					break;
				}
				if (value & AZIMUTH_BEHIND)
					fprintf(fp, " behind");
				break;
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
			case OP_BORDER_TRBL_COLOR:
				assert(BACKGROUND_COLOR_TRANSPARENT == 
						BORDER_COLOR_TRANSPARENT);
				assert(BACKGROUND_COLOR_SET ==
						BORDER_COLOR_SET);

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
			case OP_CUE_AFTER:
			case OP_CUE_BEFORE:
			case OP_LIST_STYLE_IMAGE:
				assert(BACKGROUND_IMAGE_NONE == CUE_AFTER_NONE);
				assert(BACKGROUND_IMAGE_URI == CUE_AFTER_URI);
				assert(BACKGROUND_IMAGE_NONE == CUE_BEFORE_NONE);
				assert(BACKGROUND_IMAGE_URI == CUE_BEFORE_URI);
				assert(BACKGROUND_IMAGE_NONE ==
						LIST_STYLE_IMAGE_NONE);
				assert(BACKGROUND_IMAGE_URI ==
						LIST_STYLE_IMAGE_URI);

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
			case OP_BACKGROUND_POSITION:
				switch (value & 0xf0) {
				case BACKGROUND_POSITION_HORZ_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case BACKGROUND_POSITION_HORZ_CENTER:
					fprintf(fp, "center");
					break;
				case BACKGROUND_POSITION_HORZ_RIGHT:
					fprintf(fp, "right");
					break;
				case BACKGROUND_POSITION_HORZ_LEFT:
					fprintf(fp, "left");
					break;
				}
				fprintf(fp, " ");
				switch (value & 0x0f) {
				case BACKGROUND_POSITION_VERT_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case BACKGROUND_POSITION_VERT_CENTER:
					fprintf(fp, "center");
					break;
				case BACKGROUND_POSITION_VERT_BOTTOM:
					fprintf(fp, "bottom");
					break;
				case BACKGROUND_POSITION_VERT_TOP:
					fprintf(fp, "top");
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
			case OP_BORDER_COLLAPSE:
				switch (value) {
				case BORDER_COLLAPSE_SEPARATE:
					fprintf(fp, "separate");
					break;
				case BORDER_COLLAPSE_COLLAPSE:
					fprintf(fp, "collapse");
					break;
				}
				break;
			case OP_BORDER_SPACING:
				switch (value) {
				case BORDER_SPACING_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);

					val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					unit = *((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				}
				break;
			case OP_BORDER_TRBL_STYLE:
			case OP_OUTLINE_STYLE:
				assert(BORDER_STYLE_NONE == OUTLINE_STYLE_NONE);
				assert(BORDER_STYLE_HIDDEN == 
						OUTLINE_STYLE_HIDDEN);
				assert(BORDER_STYLE_DOTTED ==
						OUTLINE_STYLE_DOTTED);
				assert(BORDER_STYLE_DASHED ==
						OUTLINE_STYLE_DASHED);
				assert(BORDER_STYLE_SOLID ==
						OUTLINE_STYLE_SOLID);
				assert(BORDER_STYLE_DOUBLE ==
						OUTLINE_STYLE_DOUBLE);
				assert(BORDER_STYLE_GROOVE ==
						OUTLINE_STYLE_GROOVE);
				assert(BORDER_STYLE_RIDGE ==
						OUTLINE_STYLE_RIDGE);
				assert(BORDER_STYLE_INSET ==
						OUTLINE_STYLE_INSET);
				assert(BORDER_STYLE_OUTSET ==
						OUTLINE_STYLE_OUTSET);

				switch (value) {
				case BORDER_STYLE_NONE:
					fprintf(fp, "none");
					break;
				case BORDER_STYLE_HIDDEN:
					fprintf(fp, "hidden");
					break;
				case BORDER_STYLE_DOTTED:
					fprintf(fp, "dotted");
					break;
				case BORDER_STYLE_DASHED:
					fprintf(fp, "dashed");
					break;
				case BORDER_STYLE_SOLID:
					fprintf(fp, "solid");
					break;
				case BORDER_STYLE_DOUBLE:
					fprintf(fp, "double");
					break;
				case BORDER_STYLE_GROOVE:
					fprintf(fp, "groove");
					break;
				case BORDER_STYLE_RIDGE:
					fprintf(fp, "ridge");
					break;
				case BORDER_STYLE_INSET:
					fprintf(fp, "inset");
					break;
				case BORDER_STYLE_OUTSET:
					fprintf(fp, "outset");
					break;
				}
				break;
			case OP_BORDER_TRBL_WIDTH:
			case OP_OUTLINE_WIDTH:
				assert(BORDER_WIDTH_SET == OUTLINE_WIDTH_SET);
				assert(BORDER_WIDTH_THIN == OUTLINE_WIDTH_THIN);
				assert(BORDER_WIDTH_MEDIUM ==
						OUTLINE_WIDTH_MEDIUM);
				assert(BORDER_WIDTH_THICK ==
						OUTLINE_WIDTH_THICK);

				switch (value) {
				case BORDER_WIDTH_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case BORDER_WIDTH_THIN:
					fprintf(fp, "thin");
					break;
				case BORDER_WIDTH_MEDIUM:
					fprintf(fp, "medium");
					break;
				case BORDER_WIDTH_THICK:
					fprintf(fp, "thick");
					break;
				}
				break;
			case OP_MARGIN_TRBL:
				/* Clear side bits */
				value &= ~SIDE_LEFT;
				/* Fall through */
			case OP_BOTTOM:
			case OP_LEFT:
			case OP_RIGHT:
			case OP_TOP:
			case OP_HEIGHT:
			case OP_WIDTH:
			case OP_Z_INDEX:
				assert(BOTTOM_SET == LEFT_SET);
				assert(BOTTOM_AUTO == LEFT_AUTO);
				assert(BOTTOM_SET == RIGHT_SET);
				assert(BOTTOM_AUTO == RIGHT_AUTO);
				assert(BOTTOM_SET == TOP_SET);
				assert(BOTTOM_AUTO == TOP_AUTO);
				assert(BOTTOM_SET == HEIGHT_SET);
				assert(BOTTOM_AUTO == HEIGHT_AUTO);
				assert(BOTTOM_SET == MARGIN_SET);
				assert(BOTTOM_AUTO == MARGIN_AUTO);
				assert(BOTTOM_SET == WIDTH_SET);
				assert(BOTTOM_AUTO == WIDTH_AUTO);
				assert(BOTTOM_SET == Z_INDEX_SET);
				assert(BOTTOM_AUTO == Z_INDEX_AUTO);

				switch (value) {
				case BOTTOM_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case BOTTOM_AUTO:
					fprintf(fp, "auto");
					break;
				}
				break;
			case OP_CAPTION_SIDE:
				switch (value) {
				case CAPTION_SIDE_TOP:
					fprintf(fp, "top");
					break;
				case CAPTION_SIDE_BOTTOM:
					fprintf(fp, "bottom");
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
			case OP_CLIP:
				if ((value & CLIP_SHAPE_MASK) == 
						CLIP_SHAPE_RECT) {
					fprintf(fp, "rect(");
					if (value & CLIP_RECT_TOP_AUTO) {
						fprintf(fp, "auto");
					} else {
						fixed val = 
							*((fixed *) bytecode);
						ADVANCE(sizeof(val));
						uint32_t unit = 
							*((uint32_t *) bytecode);
						ADVANCE(sizeof(unit));
						dump_unit(fp, val, unit);
					}
					fprintf(fp, ", ");
					if (value & CLIP_RECT_RIGHT_AUTO) {
						fprintf(fp, "auto");
					} else {
						fixed val = 
							*((fixed *) bytecode);
						ADVANCE(sizeof(val));
						uint32_t unit = 
							*((uint32_t *) bytecode);
						ADVANCE(sizeof(unit));
						dump_unit(fp, val, unit);
					}
					fprintf(fp, ", ");
					if (value & CLIP_RECT_BOTTOM_AUTO) {
						fprintf(fp, "auto");
					} else {
						fixed val = 
							*((fixed *) bytecode);
						ADVANCE(sizeof(val));
						uint32_t unit = 
							*((uint32_t *) bytecode);
						ADVANCE(sizeof(unit));
						dump_unit(fp, val, unit);
					}
					fprintf(fp, ", ");
					if (value & CLIP_RECT_LEFT_AUTO) {
						fprintf(fp, "auto");
					} else {
						fixed val = 
							*((fixed *) bytecode);
						ADVANCE(sizeof(val));
						uint32_t unit = 
							*((uint32_t *) bytecode);
						ADVANCE(sizeof(unit));
						dump_unit(fp, val, unit);
					}
					fprintf(fp, ")");
				} else
					fprintf(fp, "auto");
				break;
			case OP_COLOR:
				switch (value) {
				case COLOR_SET:
				{
					uint32_t colour = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(colour));
					fprintf(fp, "#%08x", colour);
				}	
					break;
				}
				break;
			case OP_CONTENT:
				if (value == CONTENT_NORMAL) {
					fprintf(fp, "normal");
					break;
				} else if (value == CONTENT_NONE) {
					fprintf(fp, "none");
					break;
				}

				while (value != CONTENT_NORMAL) {
					parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
					const char *end = "";

					switch (value & 0xff) {
					case CONTENT_COUNTER:
						ADVANCE(sizeof(ptr));
						dump_counter(fp, ptr, value);
						break;
					case CONTENT_COUNTERS:
					{
						parserutils_hash_entry *sep;

						ADVANCE(sizeof(ptr));

						sep = *((parserutils_hash_entry **) bytecode);
						ADVANCE(sizeof(sep));

						dump_counters(fp, ptr, sep,
								value);
					}
						break;
					case CONTENT_URI:
					case CONTENT_ATTR:	
					case CONTENT_STRING:
						if (value == CONTENT_URI) 
							fprintf(fp, "url(");
						if (value == CONTENT_ATTR)
							fprintf(fp, "attr(");
						if (value != CONTENT_STRING)
							end = ")";

						ADVANCE(sizeof(ptr));

						fprintf(fp, "'%.*s'%s",
							(int) ptr->len,
							(char *) ptr->data,
							end);
						break;
					case CONTENT_OPEN_QUOTE:
						fprintf(fp, "open-quote");
						break;
					case CONTENT_CLOSE_QUOTE:
						fprintf(fp, "close-quote");
						break;
					case CONTENT_NO_OPEN_QUOTE:
						fprintf(fp, "no-open-quote");
						break;
					case CONTENT_NO_CLOSE_QUOTE:
						fprintf(fp, "no-close-quote");
						break;
					}

					value = *((uint32_t *) bytecode);
					ADVANCE(sizeof(value));

					if (value != CONTENT_NORMAL)
						fprintf(fp, " ");
				}
				break;
			case OP_COUNTER_INCREMENT:
			case OP_COUNTER_RESET:
				assert(COUNTER_INCREMENT_NONE == 
						COUNTER_RESET_NONE);
				assert(COUNTER_INCREMENT_NAMED ==
						COUNTER_RESET_NAMED);

				switch (value) {
				case COUNTER_INCREMENT_NAMED:
					while (value != COUNTER_INCREMENT_NONE) {
						parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
						ADVANCE(sizeof(ptr));
						fprintf(fp, " %.*s ", 
							(int) ptr->len, 
							(char *) ptr->data);
						fixed val =
							*((fixed *) bytecode);
						ADVANCE(sizeof(val));
						dump_number(fp, val);

						value = *((uint32_t *) bytecode);
						ADVANCE(sizeof(value));
					}
					break;
				case COUNTER_INCREMENT_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_CURSOR:
				switch (value) {
				case CURSOR_URI:
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
				case CURSOR_AUTO:
					fprintf(fp, "auto");
					break;
				case CURSOR_CROSSHAIR:
					fprintf(fp, "crosshair");
					break;
				case CURSOR_DEFAULT:
					fprintf(fp, "default");
					break;
				case CURSOR_POINTER:
					fprintf(fp, "pointer");
					break;
				case CURSOR_MOVE:
					fprintf(fp, "move");
					break;
				case CURSOR_E_RESIZE:
					fprintf(fp, "e-resize");
					break;
				case CURSOR_NE_RESIZE:
					fprintf(fp, "ne-resize");
					break;
				case CURSOR_NW_RESIZE:
					fprintf(fp, "nw-resize");
					break;
				case CURSOR_N_RESIZE:
					fprintf(fp, "n-resize");
					break;
				case CURSOR_SE_RESIZE:
					fprintf(fp, "se-resize");
					break;
				case CURSOR_SW_RESIZE:
					fprintf(fp, "sw-resize");
					break;
				case CURSOR_S_RESIZE:
					fprintf(fp, "s-resize");
					break;
				case CURSOR_W_RESIZE:
					fprintf(fp, "w-resize");
					break;
				case CURSOR_TEXT:
					fprintf(fp, "text");
					break;
				case CURSOR_WAIT:
					fprintf(fp, "wait");
					break;
				case CURSOR_HELP:
					fprintf(fp, "help");
					break;
				case CURSOR_PROGRESS:
					fprintf(fp, "progress");
					break;
				}
				break;
			case OP_DIRECTION:
				switch (value) {
				case DIRECTION_LTR:
					fprintf(fp, "ltr");
					break;
				case DIRECTION_RTL:
					fprintf(fp, "rtl");
					break;
				}
				break;
			case OP_DISPLAY:
				switch (value) {
				case DISPLAY_INLINE:
					fprintf(fp, "inline");
					break;
				case DISPLAY_BLOCK:
					fprintf(fp, "block");
					break;
				case DISPLAY_LIST_ITEM:
					fprintf(fp, "list-item");
					break;
				case DISPLAY_RUN_IN:
					fprintf(fp, "run-in");
					break;
				case DISPLAY_INLINE_BLOCK:
					fprintf(fp, "inline-block");
					break;
				case DISPLAY_TABLE:
					fprintf(fp, "table");
					break;
				case DISPLAY_INLINE_TABLE:
					fprintf(fp, "inline-table");
					break;
				case DISPLAY_TABLE_ROW_GROUP:
					fprintf(fp, "table-row-group");
					break;
				case DISPLAY_TABLE_HEADER_GROUP:
					fprintf(fp, "table-header-group");
					break;
				case DISPLAY_TABLE_FOOTER_GROUP:
					fprintf(fp, "table-footer-group");
					break;
				case DISPLAY_TABLE_ROW:
					fprintf(fp, "table-row");
					break;
				case DISPLAY_TABLE_COLUMN_GROUP:
					fprintf(fp, "table-column-group");
					break;
				case DISPLAY_TABLE_COLUMN:
					fprintf(fp, "table-column");
					break;
				case DISPLAY_TABLE_CELL:
					fprintf(fp, "table-cell");
					break;
				case DISPLAY_TABLE_CAPTION:
					fprintf(fp, "table-caption");
					break;
				case DISPLAY_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_ELEVATION:
				switch (value) {
				case ELEVATION_ANGLE:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case ELEVATION_BELOW:
					fprintf(fp, "below");
					break;
				case ELEVATION_LEVEL:
					fprintf(fp, "level");
					break;
				case ELEVATION_ABOVE:
					fprintf(fp, "above");
					break;
				case ELEVATION_HIGHER:
					fprintf(fp, "higher");
					break;
				case ELEVATION_LOWER:
					fprintf(fp, "lower");
					break;
				}
				break;
			case OP_EMPTY_CELLS:
				switch (value) {
				case EMPTY_CELLS_SHOW:
					fprintf(fp, "show");
					break;
				case EMPTY_CELLS_HIDE:
					fprintf(fp, "hide");
					break;
				}
				break;
			case OP_FLOAT:
				switch (value) {
				case FLOAT_LEFT:
					fprintf(fp, "left");
					break;
				case FLOAT_RIGHT:
					fprintf(fp, "right");
					break;
				case FLOAT_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_FONT_FAMILY:
				while (value != FONT_FAMILY_END) {
					switch (value) {
					case FONT_FAMILY_STRING:
					case FONT_FAMILY_IDENT_LIST:
					{
						parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
						ADVANCE(sizeof(ptr));
						fprintf(fp, "'%.*s'", 
							(int) ptr->len, 
							(char *) ptr->data);
					}
						break;
					case FONT_FAMILY_SERIF:
						fprintf(fp, "serif");
						break;
					case FONT_FAMILY_SANS_SERIF:
						fprintf(fp, "sans-serif");
						break;
					case FONT_FAMILY_CURSIVE:
						fprintf(fp, "cursive");
						break;
					case FONT_FAMILY_FANTASY:
						fprintf(fp, "fantasy");
						break;
					case FONT_FAMILY_MONOSPACE:
						fprintf(fp, "monospace");
						break;
					}

					value = *((uint32_t *) bytecode);
					ADVANCE(sizeof(value));

					if (value != FONT_FAMILY_END)
						fprintf(fp, ", ");
				}
				break;
			case OP_FONT_SIZE:
				switch (value) {
				case FONT_SIZE_DIMENSION:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case FONT_SIZE_XX_SMALL:
					fprintf(fp, "xx-small");
					break;
				case FONT_SIZE_X_SMALL:
					fprintf(fp, "x-small");
					break;
				case FONT_SIZE_SMALL:
					fprintf(fp, "small");
					break;
				case FONT_SIZE_MEDIUM:
					fprintf(fp, "medium");
					break;
				case FONT_SIZE_LARGE:
					fprintf(fp, "large");
					break;
				case FONT_SIZE_X_LARGE:
					fprintf(fp, "x-large");
					break;
				case FONT_SIZE_XX_LARGE:
					fprintf(fp, "xx-large");
					break;
				case FONT_SIZE_LARGER:
					fprintf(fp, "larger");
					break;
				case FONT_SIZE_SMALLER:
					fprintf(fp, "smaller");
					break;
				}
				break;
			case OP_FONT_STYLE:
				switch (value) {
				case FONT_STYLE_NORMAL:
					fprintf(fp, "normal");
					break;
				case FONT_STYLE_ITALIC:
					fprintf(fp, "italic");
					break;
				case FONT_STYLE_OBLIQUE:
					fprintf(fp, "oblique");
					break;
				}
				break;
			case OP_FONT_VARIANT:
				switch (value) {
				case FONT_VARIANT_NORMAL:
					fprintf(fp, "normal");
					break;
				case FONT_VARIANT_SMALL_CAPS:
					fprintf(fp, "small-caps");
					break;
				}
				break;
			case OP_FONT_WEIGHT:
				switch (value) {
				case FONT_WEIGHT_NORMAL:
					fprintf(fp, "normal");
					break;
				case FONT_WEIGHT_BOLD:
					fprintf(fp, "bold");
					break;
				case FONT_WEIGHT_BOLDER:
					fprintf(fp, "bolder");
					break;
				case FONT_WEIGHT_LIGHTER:
					fprintf(fp, "lighter");
					break;
				case FONT_WEIGHT_100:
					fprintf(fp, "100");
					break;
				case FONT_WEIGHT_200:
					fprintf(fp, "200");
					break;
				case FONT_WEIGHT_300:
					fprintf(fp, "300");
					break;
				case FONT_WEIGHT_400:
					fprintf(fp, "400");
					break;
				case FONT_WEIGHT_500:
					fprintf(fp, "500");
					break;
				case FONT_WEIGHT_600:
					fprintf(fp, "600");
					break;
				case FONT_WEIGHT_700:
					fprintf(fp, "700");
					break;
				case FONT_WEIGHT_800:
					fprintf(fp, "800");
					break;
				case FONT_WEIGHT_900:
					fprintf(fp, "900");
					break;
				}
				break;
			case OP_LETTER_SPACING:
			case OP_WORD_SPACING:
				assert(LETTER_SPACING_SET == WORD_SPACING_SET);
				assert(LETTER_SPACING_NORMAL ==
						WORD_SPACING_NORMAL);

				switch (value) {
				case LETTER_SPACING_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case LETTER_SPACING_NORMAL:
					fprintf(fp, "normal");
					break;
				}
				break;
			case OP_LINE_HEIGHT:
				switch (value) {
				case LINE_HEIGHT_NUMBER:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					dump_number(fp, val);
				}
					break;
				case LINE_HEIGHT_DIMENSION:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case LINE_HEIGHT_NORMAL:
					fprintf(fp, "normal");
					break;
				}
				break;
			case OP_LIST_STYLE_POSITION:
				switch (value) {
				case LIST_STYLE_POSITION_INSIDE:
					fprintf(fp, "inside");
					break;
				case LIST_STYLE_POSITION_OUTSIDE:
					fprintf(fp, "outside");
					break;
				}
				break;
			case OP_LIST_STYLE_TYPE:
				switch (value) {
				case LIST_STYLE_TYPE_DISC:
					fprintf(fp, "disc");
					break;
				case LIST_STYLE_TYPE_CIRCLE:
					fprintf(fp, "circle");
					break;
				case LIST_STYLE_TYPE_SQUARE:
					fprintf(fp, "square");
					break;
				case LIST_STYLE_TYPE_DECIMAL:
					fprintf(fp, "decimal");
					break;
				case LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO:
					fprintf(fp, "decimal-leading-zero");
					break;
				case LIST_STYLE_TYPE_LOWER_ROMAN:
					fprintf(fp, "lower-roman");
					break;
				case LIST_STYLE_TYPE_UPPER_ROMAN:
					fprintf(fp, "upper-roman");
					break;
				case LIST_STYLE_TYPE_LOWER_GREEK:
					fprintf(fp, "lower-greek");
					break;
				case LIST_STYLE_TYPE_LOWER_LATIN:
					fprintf(fp, "lower-latin");
					break;
				case LIST_STYLE_TYPE_UPPER_LATIN:
					fprintf(fp, "upper-latin");
					break;
				case LIST_STYLE_TYPE_ARMENIAN:
					fprintf(fp, "armenian");
					break;
				case LIST_STYLE_TYPE_GEORGIAN:
					fprintf(fp, "georgian");
					break;
				case LIST_STYLE_TYPE_LOWER_ALPHA:
					fprintf(fp, "lower-alpha");
					break;
				case LIST_STYLE_TYPE_UPPER_ALPHA:
					fprintf(fp, "upper-alpha");
					break;
				case LIST_STYLE_TYPE_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_MAX_HEIGHT:
			case OP_MAX_WIDTH:
				assert(MAX_HEIGHT_SET == MAX_WIDTH_SET);
				assert(MAX_HEIGHT_NONE == MAX_WIDTH_NONE);

				switch (value) {
				case MAX_HEIGHT_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case MAX_HEIGHT_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_PADDING_TRBL:
				/* Clear side bits */
				value &= ~SIDE_LEFT;
				/* Fall through */
			case OP_MIN_HEIGHT:
			case OP_MIN_WIDTH:
			case OP_PAUSE_AFTER:
			case OP_PAUSE_BEFORE:
			case OP_TEXT_INDENT:
				assert(MIN_HEIGHT_SET == MIN_WIDTH_SET);
				assert(MIN_HEIGHT_SET == PADDING_SET);
				assert(MIN_HEIGHT_SET == PAUSE_AFTER_SET);
				assert(MIN_HEIGHT_SET == PAUSE_BEFORE_SET);
				assert(MIN_HEIGHT_SET == TEXT_INDENT_SET);

				switch (value) {
				case MIN_HEIGHT_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				}
				break;
			case OP_ORPHANS:
			case OP_PITCH_RANGE:
			case OP_RICHNESS:
			case OP_STRESS:
			case OP_WIDOWS:
				assert(ORPHANS_SET == PITCH_RANGE_SET);
				assert(ORPHANS_SET == RICHNESS_SET);
				assert(ORPHANS_SET == STRESS_SET);
				assert(ORPHANS_SET == WIDOWS_SET);

				switch (value) {
				case ORPHANS_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					dump_number(fp, val);
				}
					break;
				}
				break;
			case OP_OUTLINE_COLOR:
				switch (value) {
				case OUTLINE_COLOR_SET:
				{
					uint32_t colour = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(colour));
					fprintf(fp, "#%08x", colour);
				}
					break;
				case OUTLINE_COLOR_INVERT:
					fprintf(fp, "invert");
					break;
				}
				break;
			case OP_OVERFLOW:
				switch (value) {
				case OVERFLOW_VISIBLE:
					fprintf(fp, "visible");
					break;
				case OVERFLOW_HIDDEN:
					fprintf(fp, "hidden");
					break;
				case OVERFLOW_SCROLL:
					fprintf(fp, "scroll");
					break;
				case OVERFLOW_AUTO:
					fprintf(fp, "auto");
					break;
				}
				break;
			case OP_PAGE_BREAK_AFTER:
			case OP_PAGE_BREAK_BEFORE:
				assert(PAGE_BREAK_AFTER_AUTO == 
						PAGE_BREAK_BEFORE_AUTO);
				assert(PAGE_BREAK_AFTER_ALWAYS ==
						PAGE_BREAK_BEFORE_ALWAYS);
				assert(PAGE_BREAK_AFTER_AVOID ==
						PAGE_BREAK_BEFORE_AVOID);
				assert(PAGE_BREAK_AFTER_LEFT ==
						PAGE_BREAK_BEFORE_LEFT);
				assert(PAGE_BREAK_AFTER_RIGHT ==
						PAGE_BREAK_BEFORE_RIGHT);

				switch (value) {
				case PAGE_BREAK_AFTER_AUTO:
					fprintf(fp, "auto");
					break;
				case PAGE_BREAK_AFTER_ALWAYS:
					fprintf(fp, "always");
					break;
				case PAGE_BREAK_AFTER_AVOID:
					fprintf(fp, "avoid");
					break;
				case PAGE_BREAK_AFTER_LEFT:
					fprintf(fp, "left");
					break;
				case PAGE_BREAK_AFTER_RIGHT:
					fprintf(fp, "right");
					break;
				}
				break;
			case OP_PAGE_BREAK_INSIDE:
				switch (value) {
				case PAGE_BREAK_INSIDE_AUTO:
					fprintf(fp, "auto");
					break;
				case PAGE_BREAK_INSIDE_AVOID:
					fprintf(fp, "avoid");
					break;
				}
				break;
			case OP_PITCH:
				switch (value) {
				case PITCH_FREQUENCY:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case PITCH_X_LOW:
					fprintf(fp, "x-low");
					break;
				case PITCH_LOW:
					fprintf(fp, "low");
					break;
				case PITCH_MEDIUM:
					fprintf(fp, "medium");
					break;
				case PITCH_HIGH:
					fprintf(fp, "high");
					break;
				case PITCH_X_HIGH:
					fprintf(fp, "x-high");
					break;
				}
				break;
			case OP_PLAY_DURING:
				switch (value) {
				case PLAY_DURING_URI:
				{
					parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
					ADVANCE(sizeof(ptr));
					fprintf(fp, "'%.*s'", 
						(int) ptr->len, 
						(char *) ptr->data);
				}
					break;
				case PLAY_DURING_AUTO:
					fprintf(fp, "auto");
					break;
				case PLAY_DURING_NONE:
					fprintf(fp, "none");
					break;
				}

				if (value & PLAY_DURING_MIX)
					fprintf(fp, " mix");
				if (value & PLAY_DURING_REPEAT)
					fprintf(fp, " repeat");
				break;
			case OP_POSITION:
				switch (value) {
				case POSITION_STATIC:
					fprintf(fp, "static");
					break;
				case POSITION_RELATIVE:
					fprintf(fp, "relative");
					break;
				case POSITION_ABSOLUTE:
					fprintf(fp, "absolute");
					break;
				case POSITION_FIXED:
					fprintf(fp, "fixed");
					break;
				}
				break;
			case OP_QUOTES:
				switch (value) {
				case QUOTES_STRING:
					while (value != QUOTES_NONE) {
						parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
						ADVANCE(sizeof(ptr));
						fprintf(fp, " '%.*s' ", 
							(int) ptr->len, 
							(char *) ptr->data);

						ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
						ADVANCE(sizeof(ptr));
						fprintf(fp, " '%.*s' ", 
							(int) ptr->len, 
							(char *) ptr->data);

						value = *((uint32_t *) bytecode);
						ADVANCE(sizeof(value));
					}
					break;
				case QUOTES_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_SPEAK_HEADER:
				switch (value) {
				case SPEAK_HEADER_ONCE:
					fprintf(fp, "once");
					break;
				case SPEAK_HEADER_ALWAYS:
					fprintf(fp, "always");
					break;
				}
				break;
			case OP_SPEAK_NUMERAL:
				switch (value) {
				case SPEAK_NUMERAL_DIGITS:
					fprintf(fp, "digits");
					break;
				case SPEAK_NUMERAL_CONTINUOUS:
					fprintf(fp, "continuous");
					break;
				}
				break;
			case OP_SPEAK_PUNCTUATION:
				switch (value) {
				case SPEAK_PUNCTUATION_CODE:
					fprintf(fp, "code");
					break;
				case SPEAK_PUNCTUATION_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_SPEAK:
				switch (value) {
				case SPEAK_NORMAL:
					fprintf(fp, "normal");
					break;
				case SPEAK_NONE:
					fprintf(fp, "none");
					break;
				case SPEAK_SPELL_OUT:
					fprintf(fp, "spell-out");
					break;
				}
				break;
			case OP_SPEECH_RATE:
				switch (value) {
				case SPEECH_RATE_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					dump_number(fp, val);
				}
					break;
				case SPEECH_RATE_X_SLOW:
					fprintf(fp, "x-slow");
					break;
				case SPEECH_RATE_SLOW:
					fprintf(fp, "slow");
					break;
				case SPEECH_RATE_FAST:
					fprintf(fp, "fast");
					break;
				case SPEECH_RATE_X_FAST:
					fprintf(fp, "x-fast");
					break;
				case SPEECH_RATE_FASTER:
					fprintf(fp, "faster");
					break;
				case SPEECH_RATE_SLOWER:
					fprintf(fp, "slower");
					break;
				}
				break;
			case OP_TABLE_LAYOUT:
				switch (value) {
				case TABLE_LAYOUT_AUTO:
					fprintf(fp, "auto");
					break;
				case TABLE_LAYOUT_FIXED:
					fprintf(fp, "fixed");
					break;
				}
				break;
			case OP_TEXT_ALIGN:
				switch (value) {
				case TEXT_ALIGN_LEFT:
					fprintf(fp, "left");
					break;
				case TEXT_ALIGN_RIGHT:
					fprintf(fp, "right");
					break;
				case TEXT_ALIGN_CENTER:
					fprintf(fp, "center");
					break;
				case TEXT_ALIGN_JUSTIFY:
					fprintf(fp, "justify");
					break;
				}
				break;
			case OP_TEXT_DECORATION:
				if (value == TEXT_DECORATION_NONE)
					fprintf(fp, "none");

				if (value & TEXT_DECORATION_UNDERLINE)
					fprintf(fp, " underline");
				if (value & TEXT_DECORATION_OVERLINE)
					fprintf(fp, " overline");
				if (value & TEXT_DECORATION_LINE_THROUGH)
					fprintf(fp, " line-through");
				if (value & TEXT_DECORATION_BLINK)
					fprintf(fp, " blink");
				break;
			case OP_TEXT_TRANSFORM:
				switch (value) {
				case TEXT_TRANSFORM_CAPITALIZE:
					fprintf(fp, "capitalize");
					break;
				case TEXT_TRANSFORM_UPPERCASE:
					fprintf(fp, "uppercase");
					break;
				case TEXT_TRANSFORM_LOWERCASE:
					fprintf(fp, "lowercase");
					break;
				case TEXT_TRANSFORM_NONE:
					fprintf(fp, "none");
					break;
				}
				break;
			case OP_UNICODE_BIDI:
				switch (value) {
				case UNICODE_BIDI_NORMAL:
					fprintf(fp, "normal");
					break;
				case UNICODE_BIDI_EMBED:
					fprintf(fp, "embed");
					break;
				case UNICODE_BIDI_BIDI_OVERRIDE:
					fprintf(fp, "bidi-override");
					break;
				}
				break;
			case OP_VERTICAL_ALIGN:
				switch (value) {
				case VERTICAL_ALIGN_SET:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case VERTICAL_ALIGN_BASELINE:
					fprintf(fp, "baseline");
					break;
				case VERTICAL_ALIGN_SUB:
					fprintf(fp, "sub");
					break;
				case VERTICAL_ALIGN_SUPER:
					fprintf(fp, "super");
					break;
				case VERTICAL_ALIGN_TOP:
					fprintf(fp, "top");
					break;
				case VERTICAL_ALIGN_TEXT_TOP:
					fprintf(fp, "text-top");
					break;
				case VERTICAL_ALIGN_MIDDLE:
					fprintf(fp, "middle");
					break;
				case VERTICAL_ALIGN_BOTTOM:
					fprintf(fp, "bottom");
					break;
				case VERTICAL_ALIGN_TEXT_BOTTOM:
					fprintf(fp, "text-bottom");
					break;
				}
				break;
			case OP_VISIBILITY:
				switch (value) {
				case VISIBILITY_VISIBLE:
					fprintf(fp, "visible");
					break;
				case VISIBILITY_HIDDEN:
					fprintf(fp, "hidden");
					break;
				case VISIBILITY_COLLAPSE:
					fprintf(fp, "collapse");
					break;
				}
				break;
			case OP_VOICE_FAMILY:
				while (value != VOICE_FAMILY_END) {
					switch (value) {
					case VOICE_FAMILY_STRING:
					case VOICE_FAMILY_IDENT_LIST:
					{
						parserutils_hash_entry *ptr = 
						*((parserutils_hash_entry **) 
						bytecode);
						ADVANCE(sizeof(ptr));
						fprintf(fp, "'%.*s'", 
							(int) ptr->len, 
							(char *) ptr->data);
					}
						break;
					case VOICE_FAMILY_MALE:
						fprintf(fp, "male");
						break;
					case VOICE_FAMILY_FEMALE:
						fprintf(fp, "female");
						break;
					case VOICE_FAMILY_CHILD:
						fprintf(fp, "child");
						break;
					}

					value = *((uint32_t *) bytecode);
					ADVANCE(sizeof(value));

					if (value != VOICE_FAMILY_END)
						fprintf(fp, ", ");
				}
				break;
			case OP_VOLUME:
				switch (value) {
				case VOLUME_NUMBER:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					dump_number(fp, val);
				}
					break;
				case VOLUME_DIMENSION:
				{
					fixed val = *((fixed *) bytecode);
					ADVANCE(sizeof(val));
					uint32_t unit = 
						*((uint32_t *) bytecode);
					ADVANCE(sizeof(unit));
					dump_unit(fp, val, unit);
				}
					break;
				case VOLUME_SILENT:
					fprintf(fp, "silent");
					break;
				case VOLUME_X_SOFT:
					fprintf(fp, "x-soft");
					break;
				case VOLUME_SOFT:
					fprintf(fp, "soft");
					break;
				case VOLUME_MEDIUM:
					fprintf(fp, "medium");
					break;
				case VOLUME_LOUD:
					fprintf(fp, "loud");
					break;
				case VOLUME_X_LOUD:
					fprintf(fp, "x-loud");
					break;
				}
				break;
			case OP_WHITE_SPACE:
				switch (value) {
				case WHITE_SPACE_NORMAL:
					fprintf(fp, "normal");
					break;
				case WHITE_SPACE_PRE:
					fprintf(fp, "pre");
					break;
				case WHITE_SPACE_NOWRAP:
					fprintf(fp, "nowrap");
					break;
				case WHITE_SPACE_PRE_WRAP:
					fprintf(fp, "pre-wrap");
					break;
				case WHITE_SPACE_PRE_LINE:
					fprintf(fp, "pre-line");
					break;
				}
				break;
			default:
				fprintf(fp, "Unknown opcode %x", op);
				return;
			}
		}

		if (flags & FLAG_IMPORTANT)
			fprintf(fp, " !important");

		fprintf(fp, "; ");
	}

#undef ADVANCE

}

#endif

