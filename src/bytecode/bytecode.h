/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_bytecode_bytecode_h_
#define css_bytecode_bytecode_h_

#include <inttypes.h>
#include <stdio.h>

#include <libcss/types.h>

typedef enum opcode {
	OP_AZIMUTH			= 0x000,
	OP_BACKGROUND_ATTACHMENT	= 0x001, 
	OP_BACKGROUND_COLOR		= 0x002, 
	OP_BACKGROUND_IMAGE		= 0x003, 
	OP_BACKGROUND_POSITION		= 0x004, 
	OP_BACKGROUND_REPEAT		= 0x005, 
	OP_BORDER_COLLAPSE		= 0x006, 
	OP_BORDER_SPACING		= 0x007,
	OP_BORDER_TRBL_COLOR		= 0x008, 
	OP_BORDER_TRBL_STYLE		= 0x009, 
	OP_BORDER_TRBL_WIDTH		= 0x00a, 
	OP_BOTTOM			= 0x00b, 
	OP_CAPTION_SIDE			= 0x00c,
	OP_CLEAR			= 0x00d, 
	OP_CLIP				= 0x00e, 
	OP_COLOR			= 0x00f, 
	OP_CONTENT			= 0x010, 
	OP_COUNTER_INCREMENT		= 0x011, 
	OP_COUNTER_RESET		= 0x012, 
	OP_CUE_AFTER			= 0x013, 
	OP_CUE_BEFORE			= 0x014, 
	OP_CURSOR			= 0x015, 
	OP_DIRECTION			= 0x016, 
	OP_DISPLAY			= 0x017, 
	OP_ELEVATION			= 0x018, 
	OP_EMPTY_CELLS			= 0x019, 
	OP_FLOAT			= 0x01a, 
	OP_FONT_FAMILY			= 0x01b, 
	OP_FONT_SIZE			= 0x01c, 
	OP_FONT_STYLE			= 0x01d, 
	OP_FONT_VARIANT			= 0x01e, 
	OP_FONT_WEIGHT			= 0x01f, 
	OP_HEIGHT			= 0x020, 
	OP_LEFT				= 0x021, 
	OP_LETTER_SPACING		= 0x022, 
	OP_LINE_HEIGHT			= 0x023, 
	OP_LIST_STYLE_IMAGE		= 0x024, 
	OP_LIST_STYLE_POSITION		= 0x025, 
	OP_LIST_STYLE_TYPE		= 0x026, 
	OP_MARGIN_TRBL			= 0x027, 
	OP_MAX_HEIGHT			= 0x028, 
	OP_MAX_WIDTH			= 0x029, 
	OP_MIN_HEIGHT			= 0x02a, 
	OP_MIN_WIDTH			= 0x02b, 
	OP_ORPHANS			= 0x02c, 
	OP_OUTLINE_COLOR		= 0x02d, 
	OP_OUTLINE_STYLE		= 0x02e, 
	OP_OUTLINE_WIDTH		= 0x02f, 
	OP_OVERFLOW			= 0x030, 
	OP_PADDING_TRBL			= 0x031, 
	OP_PAGE_BREAK_AFTER		= 0x032, 
	OP_PAGE_BREAK_BEFORE		= 0x033,
	OP_PAGE_BREAK_INSIDE		= 0x034, 
	OP_PAUSE_AFTER			= 0x035, 
	OP_PAUSE_BEFORE			= 0x036, 
	OP_PITCH_RANGE			= 0x037, 
	OP_PITCH			= 0x038, 
	OP_PLAY_DURING			= 0x039, 
	OP_POSITION			= 0x03a, 
	OP_QUOTES			= 0x03b, 
	OP_RICHNESS			= 0x03c, 
	OP_RIGHT			= 0x03d, 
	OP_SPEAK_HEADER			= 0x03e, 
	OP_SPEAK_NUMERAL		= 0x03f, 
	OP_SPEAK_PUNCTUATION		= 0x040, 
	OP_SPEAK			= 0x041, 
	OP_SPEECH_RATE			= 0x042, 
	OP_STRESS			= 0x043, 
	OP_TABLE_LAYOUT			= 0x044, 
	OP_TEXT_ALIGN			= 0x045, 
	OP_TEXT_DECORATION		= 0x046, 
	OP_TEXT_INDENT			= 0x047, 
	OP_TEXT_TRANSFORM		= 0x048, 
	OP_TOP				= 0x049, 
	OP_UNICODE_BIDI			= 0x04a, 
	OP_VERTICAL_ALIGN		= 0x04b, 
	OP_VISIBILITY			= 0x04c, 
	OP_VOICE_FAMILY			= 0x04d, 
	OP_VOLUME			= 0x04e, 
	OP_WHITE_SPACE			= 0x04f, 
	OP_WIDOWS			= 0x050, 
	OP_WIDTH			= 0x051, 
	OP_WORD_SPACING			= 0x052, 
	OP_Z_INDEX			= 0x053,
} opcode;

enum flag {
	FLAG_IMPORTANT			= (1<<0),
	FLAG_INHERIT			= (1<<1),
};

typedef enum unit {
	UNIT_PX   = 0,
	UNIT_EX   = 1,
	UNIT_EM   = 2,
	UNIT_IN   = 3,
	UNIT_CM   = 4,
	UNIT_MM   = 5,
	UNIT_PT   = 6,
	UNIT_PC   = 7,

	UNIT_PCT  = (1 << 8),

	UNIT_ANGLE = (1 << 9),
	UNIT_DEG  = (1 << 9) + 0,
	UNIT_GRAD = (1 << 9) + 1,
	UNIT_RAD  = (1 << 9) + 2,

	UNIT_TIME = (1 << 10),
	UNIT_MS   = (1 << 10) + 0,
	UNIT_S    = (1 << 10) + 1,

	UNIT_FREQ = (1 << 11),
	UNIT_HZ   = (1 << 11) + 0,
	UNIT_KHZ  = (1 << 11) + 1
} unit;

typedef uint32_t colour;

typedef enum shape {
	SHAPE_RECT = 0
} shape;

static inline uint32_t buildOPV(opcode opcode, uint8_t flags, uint16_t value)
{
	return (opcode & 0x3ff) | (flags << 10) | ((value & 0x3fff) << 18);
}

static inline opcode getOpcode(uint32_t OPV)
{
	return (OPV & 0x3ff);
}

static inline uint8_t getFlags(uint32_t OPV)
{
	return ((OPV >> 10) & 0xff);
}

static inline uint16_t getValue(uint32_t OPV)
{
	return (OPV >> 18);
}

static inline bool isImportant(uint32_t OPV)
{
	return getFlags(OPV) & 0x1;
}

static inline bool isInherit(uint32_t OPV)
{
	return getFlags(OPV) & 0x2;
}

#ifndef NDEBUG
void css_bytecode_dump(void *bytecode, uint32_t length, FILE *fp);
#endif

#endif



