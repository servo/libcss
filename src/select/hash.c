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

typedef struct hash_entry {
	const css_selector *sel;
	struct hash_entry *next;
} hash_entry;

struct css_selector_hash {
#define DEFAULT_SLOTS (1<<6)
	size_t n_slots;

	hash_entry *slots;

	css_allocator_fn alloc;
	void *pw;
};

static hash_entry empty_slot;

static inline uint32_t _hash(const css_selector *sel);
static inline uint32_t _hash_name(const parserutils_hash_entry *name);

/**
 * Create a hash
 *
 * \param alloc  Memory (de)allocation function
 * \param pw     Pointer to client-specific private data
 * \param hash   Pointer to location to receive result
 * \return CSS_OK on success, appropriate error otherwise
 */
css_error css_selector_hash_create(css_allocator_fn alloc, void *pw, 
		css_selector_hash **hash)
{
	css_selector_hash *h;

	if (alloc == NULL || hash == NULL)
		return CSS_BADPARM;

	h = alloc(0, sizeof(css_selector_hash), pw);
	if (h == NULL)
		return CSS_NOMEM;

	h->slots = alloc(0, DEFAULT_SLOTS * sizeof(hash_entry), pw);
	if (h->slots == NULL) {
		alloc(h, 0, pw);
		return CSS_NOMEM;
	}

	memset(h->slots, 0, DEFAULT_SLOTS * sizeof(hash_entry));
	h->n_slots = DEFAULT_SLOTS;

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
	uint32_t i;

	if (hash == NULL)
		return CSS_BADPARM;

	for (i = 0; i < hash->n_slots; i++) {
		hash_entry *d, *e;

		for (d = hash->slots[i].next; d != NULL; d = e) {
			e = d->next;

			hash->alloc(d, 0, hash->pw);
		}
	}

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
	hash_entry *head;

	if (hash == NULL || selector == NULL)
		return CSS_BADPARM;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash(selector) & mask;

	head = &hash->slots[index];

	if (head->sel == NULL) {
		head->sel = selector;
		head->next = NULL;
	} else {
		hash_entry *prev = NULL;
		hash_entry *entry = 
				hash->alloc(NULL, sizeof(hash_entry), hash->pw);
		if (entry == NULL)
			return CSS_NOMEM;

		/* Find place to insert entry */
		do {
			/* Sort by ascending specificity */
			if (head->sel->specificity > selector->specificity)
				break;

			/* Sort by ascending rule index */
			if (head->sel->specificity == selector->specificity && 
				head->sel->rule->index > selector->rule->index)
				break;

			prev = head;
			head = head->next;
		} while (head != NULL);

		if (prev == NULL) {
			entry->sel = hash->slots[index].sel;
			entry->next = hash->slots[index].next;
			hash->slots[index].sel = selector;
			hash->slots[index].next = entry;
		} else {
			entry->sel = selector;
			entry->next = prev->next;
			prev->next = entry;
		}
	}

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
	hash_entry *head, *prev = NULL;

	if (hash == NULL || selector == NULL)
		return CSS_BADPARM;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash(selector) & mask;

	head = &hash->slots[index];

	if (head->sel == NULL)
		return CSS_INVALID;

	do {
		if (head->sel == selector)
			break;

		prev = head;
		head = head->next;
	} while (head != NULL);

	if (head == NULL)
		return CSS_INVALID;

	if (prev == NULL) {
		if (head->next != NULL) {
			hash->slots[index].sel = head->next->sel;
			hash->slots[index].next = head->next->next;
		} else {
			hash->slots[index].sel = NULL;
			hash->slots[index].next = NULL;
		}
	} else {
		prev->next = head->next;

		hash->alloc(head, 0, hash->pw);
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
	hash_entry *head;

	if (hash == NULL || name == NULL || matched == NULL)
		return CSS_BADPARM;

	/* Find index */
	mask = hash->n_slots - 1;
	index = _hash_name(name) & mask;

	head = &hash->slots[index];

	if (head->sel != NULL) {
		do {
			if (head->sel->data.name == name)
				break;

			head = head->next;
		} while (head != NULL);

		if (head == NULL)
			head = &empty_slot;
	}

	(*matched) = (const css_selector **) head;

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
	hash_entry *head = (hash_entry *) current;

	if (hash == NULL || current == NULL || next == NULL)
		return CSS_BADPARM;

	for (head = head->next; head != NULL; head = head->next) {
		if (head->sel->data.name == (*current)->data.name)
			break;
	}

	if (head == NULL)
		head = &empty_slot;

	(*next) = (const css_selector **) head;

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

