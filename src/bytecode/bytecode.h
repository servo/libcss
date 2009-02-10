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
	OP_BORDER_TOP_COLOR		= 0x008, 
	OP_BORDER_RIGHT_COLOR		= 0x009,
	OP_BORDER_BOTTOM_COLOR		= 0x00a,
	OP_BORDER_LEFT_COLOR		= 0x00b,
	OP_BORDER_TOP_STYLE		= 0x00c, 
	OP_BORDER_RIGHT_STYLE		= 0x00d,
	OP_BORDER_BOTTOM_STYLE		= 0x00e,
	OP_BORDER_LEFT_STYLE		= 0x00f,
	OP_BORDER_TOP_WIDTH		= 0x010, 
	OP_BORDER_RIGHT_WIDTH		= 0x011,
	OP_BORDER_BOTTOM_WIDTH		= 0x012,
	OP_BORDER_LEFT_WIDTH		= 0x013,
	OP_BOTTOM			= 0x014, 
	OP_CAPTION_SIDE			= 0x015,
	OP_CLEAR			= 0x016, 
	OP_CLIP				= 0x017, 
	OP_COLOR			= 0x018, 
	OP_CONTENT			= 0x019, 
	OP_COUNTER_INCREMENT		= 0x01a, 
	OP_COUNTER_RESET		= 0x01b, 
	OP_CUE_AFTER			= 0x01c, 
	OP_CUE_BEFORE			= 0x01d, 
	OP_CURSOR			= 0x01e, 
	OP_DIRECTION			= 0x01f, 
	OP_DISPLAY			= 0x020, 
	OP_ELEVATION			= 0x021, 
	OP_EMPTY_CELLS			= 0x022, 
	OP_FLOAT			= 0x023, 
	OP_FONT_FAMILY			= 0x024, 
	OP_FONT_SIZE			= 0x025, 
	OP_FONT_STYLE			= 0x026, 
	OP_FONT_VARIANT			= 0x027, 
	OP_FONT_WEIGHT			= 0x028, 
	OP_HEIGHT			= 0x029, 
	OP_LEFT				= 0x02a, 
	OP_LETTER_SPACING		= 0x02b, 
	OP_LINE_HEIGHT			= 0x02c, 
	OP_LIST_STYLE_IMAGE		= 0x02d, 
	OP_LIST_STYLE_POSITION		= 0x02e, 
	OP_LIST_STYLE_TYPE		= 0x02f, 
	OP_MARGIN_TOP			= 0x030, 
	OP_MARGIN_RIGHT			= 0x031,
	OP_MARGIN_BOTTOM		= 0x032,
	OP_MARGIN_LEFT			= 0x033,
	OP_MAX_HEIGHT			= 0x034, 
	OP_MAX_WIDTH			= 0x035, 
	OP_MIN_HEIGHT			= 0x036, 
	OP_MIN_WIDTH			= 0x037, 
	OP_ORPHANS			= 0x038, 
	OP_OUTLINE_COLOR		= 0x039, 
	OP_OUTLINE_STYLE		= 0x03a, 
	OP_OUTLINE_WIDTH		= 0x03b, 
	OP_OVERFLOW			= 0x03c, 
	OP_PADDING_TOP			= 0x03d, 
	OP_PADDING_RIGHT		= 0x03e,
	OP_PADDING_BOTTOM		= 0x03f,
	OP_PADDING_LEFT			= 0x040,
	OP_PAGE_BREAK_AFTER		= 0x041, 
	OP_PAGE_BREAK_BEFORE		= 0x042,
	OP_PAGE_BREAK_INSIDE		= 0x043, 
	OP_PAUSE_AFTER			= 0x044, 
	OP_PAUSE_BEFORE			= 0x045, 
	OP_PITCH_RANGE			= 0x046, 
	OP_PITCH			= 0x047, 
	OP_PLAY_DURING			= 0x048, 
	OP_POSITION			= 0x049, 
	OP_QUOTES			= 0x04a, 
	OP_RICHNESS			= 0x04b, 
	OP_RIGHT			= 0x04c, 
	OP_SPEAK_HEADER			= 0x04d, 
	OP_SPEAK_NUMERAL		= 0x04e, 
	OP_SPEAK_PUNCTUATION		= 0x04f, 
	OP_SPEAK			= 0x050, 
	OP_SPEECH_RATE			= 0x051, 
	OP_STRESS			= 0x052, 
	OP_TABLE_LAYOUT			= 0x053, 
	OP_TEXT_ALIGN			= 0x054, 
	OP_TEXT_DECORATION		= 0x055, 
	OP_TEXT_INDENT			= 0x056, 
	OP_TEXT_TRANSFORM		= 0x057, 
	OP_TOP				= 0x058, 
	OP_UNICODE_BIDI			= 0x059, 
	OP_VERTICAL_ALIGN		= 0x05a, 
	OP_VISIBILITY			= 0x05b, 
	OP_VOICE_FAMILY			= 0x05c, 
	OP_VOLUME			= 0x05d, 
	OP_WHITE_SPACE			= 0x05e, 
	OP_WIDOWS			= 0x05f, 
	OP_WIDTH			= 0x060, 
	OP_WORD_SPACING			= 0x061, 
	OP_Z_INDEX			= 0x062,

	N_OPCODES			= OP_Z_INDEX + 1
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

#endif



