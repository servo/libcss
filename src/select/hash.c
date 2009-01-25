/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include <stdio.h>
#include <string.h>

#include "stylesheet.h"
#include "select/hash.h"

struct css_selector_hash {
#define DEFAULT_SLOTS (1<<6)
	size_t n_slots;
	size_t n_used;

	const css_selector **slots;

	css_alloc alloc;
	void *pw;
};

/* Dummy selector, used for lazy deletion */
static css_selector empty_slot;

static inline uint32_t _hash(const css_selector *sel);
static inline uint32_t _hash_name(const parserutils_hash_entry *name);
static inline void grow_slots(css_selector_hash *hash);

/**
 * Create a hash
 *
 * \param alloc  Memory (de)allocation function
 * \param pw     Pointer to client-specific private data
 * \param hash   Pointer to location to receive result
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_selector_hash_create(css_alloc alloc, void *pw, 
		css_selector_hash **hash)
{
	css_selector_hash *h;

	if (alloc == NULL || hash == NULL)
		return CSS_BADPARM;

	h = alloc(0, sizeof(css_selector_hash), pw);
	if (h == NULL)
		return CSS_NOMEM;

	h->slots = alloc(0, DEFAULT_SLOTS * sizeof(css_selector *), pw);
	if (h->slots == NULL) {
		alloc(h, 0, pw);
		return CSS_NOMEM;
	}

	memset(h->slots, 0, DEFAULT_SLOTS * sizeof(css_selector *));
	h->n_slots = DEFAULT_SLOTS;
	h->n_used = 0;

	h->alloc = alloc;
	h->pw = pw;

	*hash = h;

	return CSS_OK;
}

/**
 * Destroy a hash
 *
 * \param hash  The hash to destroy
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_selector_hash_destroy(css_selector_hash *hash)
{
	if (hash == NULL)
		return CSS_BADPARM;

	hash->alloc(hash->slots, 0, hash->pw);

	hash->alloc(hash, 0, hash->pw);

	return CSS_OK;
}

/**
 * Insert an item into a hash
 *
 * \param hash      The hash to insert into
 * \param selector  Pointer to selector
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_selector_hash_insert(css_selector_hash *hash,
		const css_selector *selector)
{
	uint32_t index, mask;
	const css_selector **entries;
	const css_selector *entry;

	if (hash == NULL || selector == NULL)
		return CSS_BADPARM;

	entries = hash->slots;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash(selector) & mask;

	/* Use linear probing to resolve collisions */
	while ((entry = entries[index]) != NULL) {
		/* If this data is already in the hash, return it */
		if (selector == entry) {
			return CSS_OK;
		}

		index = (index + 1) & mask;
	}

	/* The data isn't in the hash. Index identifies the slot to use */
	entries[index] = selector;

	/* If the table is 75% full, then increase its size */
	if (++hash->n_used >= (hash->n_slots >> 1) + (hash->n_slots >> 2))
		grow_slots(hash);

	return CSS_OK;
}

/**
 * Remove an item from a hash
 *
 * \param hash      The hash to remove from
 * \param selector  Pointer to selector
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_selector_hash_remove(css_selector_hash *hash,
		const css_selector *selector)
{
	uint32_t index, mask;
	const css_selector **entries;
	const css_selector *entry;

	if (hash == NULL || selector == NULL)
		return CSS_BADPARM;

	entries = hash->slots;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash(selector) & mask;

	/* Use linear probing to resolve collisions */
	while ((entry = entries[index]) != NULL) {
		/* If we've found the right entry, invalidate it */
		if (selector == entry) {
			entries[index] = &empty_slot;
			return CSS_OK;
		}

		index = (index + 1) & mask;
	}

	return CSS_OK;
}


/**
 * Find the first selector that matches name
 *
 * \param hash     Hash to search
 * \param name     Name to match
 * \param matched  Location to receive pointer to pointer to matched entry
 * \return CSS_OK on success, appropriate error otherwise
 *
 * If nothing matches, CSS_OK will be returned and **matched == NULL
 */
css_error css_selector_hash_find(css_selector_hash *hash,
		const parserutils_hash_entry *name,
		const css_selector ***matched)
{
	uint32_t index, mask;
	const css_selector **entries;
	const css_selector *entry;

	if (hash == NULL || name == NULL || matched == NULL)
		return CSS_BADPARM;

	entries = hash->slots;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash_name(name) & mask;

	/* Use linear probing to resolve collisions */
	while ((entry = entries[index]) != NULL) {
		/* Names match, so we're done here */
		if (entry != &empty_slot && entry->data.name == name) {
			break;
		}

		index = (index + 1) & mask;
	}

	(*matched) = &entries[index];

	return CSS_OK;
}

/**
 * Find the next selector that matches the current search
 *
 * \param hash     Hash to search
 * \param current  Current selector in search
 * \param next     Location to receive pointer to pointer to next entry
 * \return CSS_OK on success, appropriate error otherwise
 *
 * If nothing further matches, CSS_OK will be returned and **next == NULL
 */

css_error css_selector_hash_iterate(css_selector_hash *hash,
		const css_selector **current, const css_selector ***next)
{
	uint32_t index, mask;
	const css_selector **entries;
	const css_selector *entry;

	if (hash == NULL || current == NULL || next == NULL)
		return CSS_BADPARM;

	entries = hash->slots;

	/* Find index */
	mask = hash->n_slots - 1;
	index = (current - entries + 1) & mask;

	/* Use linear probing to resolve collisions */
	while ((entry = entries[index]) != NULL) {
		if (entry != &empty_slot &&
				entry->data.name == (*current)->data.name) {
			break;
		}

		index = (index + 1) & mask;
	}

	*next = &entries[index];

	return CSS_OK;
}

/******************************************************************************
 * Private functions                                                          *
 ******************************************************************************/

/**
 * Selector hash function
 *
 * \param sel  Pointer to selector
 * \return hash value
 */
uint32_t _hash(const css_selector *sel)
{
	return (uint32_t) (((uintptr_t) sel->data.name) & 0xffffffff);
}

/**
 * Name hash function
 *
 * \param name  Name to hash
 * \return hash value
 */
uint32_t _hash_name(const parserutils_hash_entry *name)
{
	return (uint32_t) (((uintptr_t) name) & 0xffffffff);
}

/**
 * Increase the size of the slot table
 *
 * \param hash  The hash to process
 */
void grow_slots(css_selector_hash *hash)
{
	const css_selector **s;
	size_t new_size;
	size_t n_used = 0;

	if (hash == NULL)
		return;

	new_size = hash->n_slots << 1;

	/* Create new slot table */
	s = hash->alloc(0, new_size * sizeof(css_selector *), hash->pw);
	if (s == NULL)
		return;

	memset(s, 0, new_size * sizeof(css_selector *));

	/* Now, populate it with the contents of the current one */
	for (uint32_t i = 0; i < hash->n_slots; i++) {
		const css_selector *e = hash->slots[i];

		/* Ignore unused slots, 
		 * and slots containing lazily-deleted items
		 */
		if (e == NULL || e == &empty_slot)
			continue;

		/* Find new index */
		uint32_t mask = new_size - 1;
		uint32_t index = _hash(e) & mask;

		/* Use linear probing to resolve collisions */
		while (s[index] != NULL)
			index = (index + 1) & mask;

		/* Insert into new slot table */
		s[index] = e;
		n_used++;
	}

	/* Destroy current table, and replace it with the new one */
	hash->alloc(hash->slots, 0, hash->pw);
	hash->slots = s;
	hash->n_slots = new_size;
	hash->n_used = n_used;

	return;
}

