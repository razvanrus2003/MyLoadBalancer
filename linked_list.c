/* Copyright 2023 Razvan-Constantin Rus */
#include "linked_list.h"

linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t* ll;
	// Alloc the list
	ll = malloc(sizeof(*ll));
	DIE(!ll, "List allocation faied!\n");
	// Initialize values
	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;

	return ll;
}

void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	ll_node_t *prev, *curr;
	ll_node_t* new_node;

	if (!list) {
		return;
	}

	if (n > list->size) {
		n = list->size;
	}

	// Gets pointer to the nth element
	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	// Alloc the new node
	new_node = malloc(sizeof(*new_node));
	DIE(!new_node, "Node allocation faied!\n");
	new_node->data = malloc(list->data_size);
	DIE(!new_node->data, "Node allocation faied!\n");
	memcpy(new_node->data, new_data, list->data_size);

	// Places the new node in the list and redoes the links
	new_node->next = curr;
	if (prev == NULL) {
		list->head = new_node;
	} else {
		prev->next = new_node;
	}

	list->size += 1;
}

ll_node_t *ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev, *curr;

	if (!list || !list->head) {
		return NULL;
	}

	if (n > list->size - 1) {
		n = list->size - 1;
	}

	// Gets pointer to the nth element
	curr = list->head;
	prev = NULL;
	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	// Removes the node and redoes the links
	if (prev == NULL) {
		list->head = curr->next;
	} else {
		prev->next = curr->next;
	}

	list->size--;

	return curr;
}

unsigned int ll_get_size(linked_list_t* list)
{
	 if (!list) {
		return -1;
	}
	// Return size
	return list->size;
}

void ll_free(linked_list_t** pp_list, void (*free_value)(void *))
{
	ll_node_t* currNode;

	if (!pp_list || !*pp_list) {
		return;
	}
	// Removes the first node from list 'till empty
	while (ll_get_size(*pp_list) > 0) {
		currNode = ll_remove_nth_node(*pp_list, 0);
		// Frees the nodes data
		free_value(currNode->data);
		currNode->data = NULL;
		free(currNode);
		currNode = NULL;
	}

	// Free list
	free(*pp_list);
	*pp_list = NULL;
}

void ll_print_int(linked_list_t* list)
{
	ll_node_t* curr;

	if (!list) {
		return;
	}

	// Print ints
	curr = list->head;
	while (curr != NULL) {
		printf("%d ", *((unsigned int*)curr->data));
		curr = curr->next;
	}

	printf("\n");
}

void ll_print_string(linked_list_t* list)
{
	ll_node_t* curr;

	if (!list) {
		return;
	}

	// Print strings
	curr = list->head;
	while (curr != NULL) {
		printf("%s ", (char*)curr->data);
		curr = curr->next;
	}

	printf("\n");
}

linked_list_t* ll_merge(linked_list_t* list1, linked_list_t* list2)
{
	// If list one is empty, returns the second list
	if (!list1->size) {
		free(list1);
		return list2;
	}

	// If list two is empty, returns the first list
	if (!list2->size) {
		free(list2);
		return list1;
	}

	// Else, moves everything from list two to list one
	ll_node_t *aux = list1->head;
	while (aux && aux->next) {
		aux = aux->next;
	}

	// Update values
	aux->next = list2->head;
	list1->size += list2->size;

	// Frees list two and return list one with all the elements
	free(list2);
	return list1;
}
