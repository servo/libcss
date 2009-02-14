/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_lex_lex_h_
#define css_lex_lex_h_

#include <libcss/errors.h>
#include <libcss/functypes.h>
#include <libcss/types.h>

#include <parserutils/input/inputstream.h>
#include <parserutils/utils/hash.h>

typedef struct css_lexer css_lexer;

/**
 * Lexer option types
 */
typedef enum css_lexer_opttype {
	CSS_LEXER_EMIT_COMMENTS,
} css_lexer_opttype;

/**
 * Lexer option parameters
 */
typedef union css_lexer_optparams {
	bool emit_comments;
} css_lexer_optparams;

/**
 * Token type
 */
typedef enum css_token_type { 
	CSS_TOKEN_IDENT, CSS_TOKEN_ATKEYWORD, CSS_TOKEN_HASH,
	CSS_TOKEN_FUNCTION, 

	/* Those tokens that want lowercase strings interned appear above */
	CSS_TOKEN_LAST_INTERN_LOWER,

	CSS_TOKEN_STRING, CSS_TOKEN_INVALID_STRING, CSS_TOKEN_URI,
	CSS_TOKEN_UNICODE_RANGE, CSS_TOKEN_CHAR, CSS_TOKEN_NUMBER, 
	CSS_TOKEN_PERCENTAGE, CSS_TOKEN_DIMENSION,

	/* Those tokens that want strings interned appear above */
	CSS_TOKEN_LAST_INTERN,

 	CSS_TOKEN_CDO, CSS_TOKEN_CDC, CSS_TOKEN_S, CSS_TOKEN_COMMENT, 
	CSS_TOKEN_INCLUDES, CSS_TOKEN_DASHMATCH, CSS_TOKEN_PREFIXMATCH, 
	CSS_TOKEN_SUFFIXMATCH, CSS_TOKEN_SUBSTRINGMATCH, CSS_TOKEN_EOF 
} css_token_type;

/**
 * Token object
 */
typedef struct css_token {
	css_token_type type;

	css_string data;

	const parserutils_hash_entry *idata;
	const parserutils_hash_entry *ilower;
	
	uint32_t col;
	uint32_t line;
} css_token;

css_error css_lexer_create(parserutils_inputstream *input, 
		css_allocator_fn alloc, void *pw, css_lexer **lexer);
css_error css_lexer_destroy(css_lexer *lexer);

css_error css_lexer_setopt(css_lexer *lexer, css_lexer_opttype type, 
		css_lexer_optparams *params);

css_error css_lexer_get_token(css_lexer *lexer, css_token **token);

#endif

