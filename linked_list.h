/* Copyright 2023 Razvan-Constantin Rus */
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"

/** linked_list_create() - Initializes the memory for a new list struct.
 * 						   Check what is returned by malloc using DIE.
 *
 * @arg1: Size of the data contained by each node of the list.
 */
linked_list_t *ll_create(unsigned int data_size);

/** ll_add_nth_node() - ADD a new node on n-1 position from a list.
 *						First element is on position 0.
 *						The second argument need to be greater or equal the 0.
 *						If n is greater then the list size, the last element will
 *						be removed.
 *						The data of the new node will be copied from the location
 *						pointed by the 3rd argument.
 *
 * @arg1: List in which to add the nth element.
 * @arg2: Value of n.
 * @arg3: Pointer to the data to be contained by the new node.
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

/** ll_remove_nth_node() - Removes and return the node on n-1 position from a list.
 *						   First element is on position 0.
 *						   The second argument need to be greater or equal the 0.
 *						   If n is greater then the list size, the last element will
 *						   be removed.
 *
 * @arg1: List from which to remove the nth element.
 * @arg2: Value of n.
 *
 * Return: Nth node of the list.
 */
ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n);

/** ll_free() - Gets the size of the list.
 *
 * @arg1: List to get the size of.
 *
 * Return: Size of the list.
 */
unsigned int ll_get_size(linked_list_t* list);

/** ll_free() - Frees the date stored in the list using the function provied as
 *				argument and the list itself.
 *
 * @arg1: List to free.
 */
void ll_free(linked_list_t** pp_list, void (*free_value)(void *));

/** ll_prinf_int() - USES THE FUNCTION ONLY IF THE DATA STORED IN THE LIST
 *                   IS OF TYPE INT.
 *                   Print the ints stored in every node.
 *
 * @arg1: List to print.
 */
void ll_print_int(linked_list_t* list);


/** ll_prinf_string() - USES THE FUNCTION ONLY IF THE DATA STORED IN THE LIST
 *                      IS OF TYPE STRING.
 *                      Print the strings stored in every node.
 *
 * @arg1: List to print.
 */
void ll_print_string(linked_list_t* list);

/** ll_merge() - Merges the to lists provided as argument, storing everything in
 *               the one of them list.
 *
 * @arg1: First list.
 * @arg1: Second list.
 *
 * Return: A list with all elements.
 */
linked_list_t* ll_merge(linked_list_t* list1, linked_list_t* list2);

#endif /* LINKED_LIST_H_ */
