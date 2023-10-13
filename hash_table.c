/* Copyright 2023 Razvan-Constantin Rus */

#include "hash_table.h"

/*
 * Hashing functions:
 */
unsigned int hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	unsigned int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

void key_val_free_function(void *data) {
	info *rem = (info *)data;
	free(rem->key);
	free(rem->value);
	free(rem);
}

hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*))
{
	// Hashtable allocation
	hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
    DIE(!ht, "Hashtabel allocation faied!\n");

	// Initialize values
	ht->hmax = hmax;
	ht->size = 0;
	ht->compare_function = compare_function;
	ht->hash_function = hash_function;
    ht->key_val_free_function = key_val_free_function;

	// Alloc buckets
	if (hmax) {
		ht->buckets = (linked_list_t **)calloc(hmax, sizeof(linked_list_t *));
    	DIE(!ht->buckets, "Buckets allocation faied!\n");
	}
	unsigned int i;
	for (i = 0; i < hmax; i++) {
		ht->buckets[i] = ll_create(sizeof(info));
	}
	return ht;
}

long ht_has_key(hashtable_t *ht, void *key)
{
	if (!ht)
		return -1;
	unsigned int buc = ht->hash_function(key) % ht->hmax;

	// Verify the existens of the key
	unsigned int i;
	ll_node_t *node = ht->buckets[buc]->head;
	for (i = 0; i < ht->buckets[buc]->size; i++) {
		// If exist return its position in linked list
		if(!ht->compare_function(key, ((info *)node->data)->key))
			return i;
		node = node->next;
	}

	// Else, return -1
	return -1;
}

void *ht_get(hashtable_t *ht, void *key)
{
	if (!ht)
		return NULL;

	unsigned int buc = ht->hash_function(key) % ht->hmax;

	unsigned int i;
	ll_node_t *node = ht->buckets[buc]->head;
	for (i = 0; i < ht->buckets[buc]->size; i++) {
		// If found return its value
		if(!ht->compare_function(key, ((info *)node->data)->key))
			return ((info *)node->data)->value;
		node = node->next;
	}

	// Else, return NULL
	return NULL;
}

void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if (!ht)
		return;

	// Initialize the structures for the new data
	unsigned int buc = ht->hash_function(key) % ht->hmax;
	info *data = (info *)malloc(sizeof(info));
    DIE(!data, "Bucket data allocation faied!\n");
	data->key = malloc(key_size);
	data->value = malloc(value_size);
    DIE(!data->key || !data->value, "Bucket data allocation faied!\n");
	memcpy(data->key, key, key_size);
	memcpy(data->value, value, value_size);

	long poz = ht_has_key(ht, key);
	if (poz >= 0) {
		// If key already exists, update value
		poz = (unsigned int)poz;
		ll_node_t *removed = ll_remove_nth_node(ht->buckets[buc], poz);
        ht->key_val_free_function(removed->data);
        free(removed);
        ll_add_nth_node(ht->buckets[buc], poz, data);
	} else {
		// Else, add it in the hashtable
		ll_add_nth_node(ht->buckets[buc], 0, data);
        ht->size += 1;
	}

	// Free tmp container
    free(data);
}

void ht_remove_entry(hashtable_t *ht, void *key)
{
	if (!ht)
		return;
	unsigned int buc = ht->hash_function(key) % ht->hmax;

    long poz = ht_has_key(ht, key);
    if (poz >= 0) {
		// If exist, remove the key
		poz = (unsigned int)poz;
		ll_node_t *removed = ll_remove_nth_node(ht->buckets[buc], poz);
        ht->key_val_free_function(removed->data);
        free(removed);
	}
}

void ht_free(hashtable_t *ht)
{
    if (!ht)
		return;
    unsigned int i;

	// Free the data occupaied by the hashtable with the function given at init
    for (i = 0; i < ht->hmax; i++) {
        ll_free(&(ht->buckets[i]), ht->key_val_free_function);
    }

	// Frees the rest of the structure
    free(ht->buckets);
    free(ht);
	// Update value
	ht = NULL;
}

unsigned int ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;
	// Returns the number of elements
	return ht->size;
}

unsigned int ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;
	// Returns the number of buckets
	return ht->hmax;
}

void ht_merge(hashtable_t *ht1, hashtable_t *ht2)
{
	unsigned int i;
	for (i = 0; i < ht1->hmax; i++) {
		// Merges every linked list
		ht1->buckets[i] = ll_merge(ht1->buckets[i], ht2->buckets[i]);
	}
	// Free second hashtable
	free(ht2->buckets);
	free(ht2);
}
