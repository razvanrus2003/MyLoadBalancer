/* Copyright 2023 Razvan-Constantin Rus */
#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "linked_list.h"
#include "utils.h"

/** Hashing functions: - Gets the hash of the value pointed by the argument.
 *
 * @arg1: Pointer to the data.
 *
 * Return: Hash.
 */
unsigned int hash_function_int(void *a);
unsigned int hash_function_string(void *a);

/** key_val_free_function() - Frees the space ocupaied by a info struct.
 *
 * @arg1: Pointer to a info strunct.
 */
void key_val_free_function(void *data);

/** ht_create() - Initialize a hashtable.
 *
 * @arg1: Number of buckets.
 * @arg2: Hashing function.
 * @arg3: Compare function.
 * @arg4: Function to free data.
 *
 * Return: Pointer to a hashtable.
 */
hashtable_t *ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*),
		void (*key_val_free_function)(void*));

/** ht_has_key() - Returns the position of the key in the list.
 *				   Returns -1 if the key doesn't exist.
 *				   Returns a long so it can be -1 and a valid
 *				   unsigned int posiztion at the same time.
 *
 * @arg1: Hashtabel.
 * @arg2: Pointer to the key in question.
 *
 * Return: Posiztion of the or error code.
 */
long ht_has_key(hashtable_t *ht, void *key);

/** ht_get() - Returns the data of the correspondent key.
 *
 * @arg1: Hashtabel.
 * @arg2: Pointer to the key in question.
 *
 * Return: Pointer to the data.
 */
void *ht_get(hashtable_t *ht, void *key);

/** ht_put() - Add a new value in the hashtable.
 *
 * @arg1: Hashtable.
 * @arg2: Pointer to key.
 * @arg3: Size of the key in bytes.
 * @arg4: Pointer to value.
 * @arg5: Size of the value in bytes.
 */
void ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

/** ht_removes_entry() - Removes the entry of the correspondent key from
 *				   		 hashtabe.
 *
 * @arg1: Hashtabel.
 * @arg2: Pointer to the key in question.
 */
void ht_remove_entry(hashtable_t *ht, void *key);

/** ht_free() - Frees the hashtable.
 *
 * @arg1: Hashtabel.
 */
void ht_free(hashtable_t *ht);

/** ht_get_size() - Gets the number of elements.
 *
 * @arg1: Hashtabel.
 *
 * Return: Number of elements.
 */
unsigned int ht_get_size(hashtable_t *ht);

/** ht_get_hmax() - Gets the number of buckets.
 *
 * @arg1: Hashtabel.
 *
 * Return: Number of buckets.
 */
unsigned int ht_get_hmax(hashtable_t *ht);

/** ht_merge() - Merges two hashtables with the same
 *				 height and hash function.
 *
 * @arg1: Hashtabel one.
 * @arg2: hashtabel two.
 *
 * Return: Pointer to a hast_table.
 */
void ht_merge(hashtable_t *ht1, hashtable_t *ht2);

#endif /* HASH_TABLE_H_ */
