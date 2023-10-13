/* Copyright 2023 Razvan-Constantin Rus */
#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
	// Stored data
	void* data;
	// Next element
	struct ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
	// Fires element
	ll_node_t* head;
	// Size of the data in bytes
	unsigned int data_size;
	// number of nodes
	unsigned int size;
};

typedef struct info info;
struct info {
	void *key;
	void *value;
};

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	// Linked list array
	linked_list_t **buckets;
	// Number of entries
	unsigned int size;
	// Linked list array size
	unsigned int hmax;
	// Hashing function
	unsigned int (*hash_function)(void*);
	// Compare function
	int (*compare_function)(void*, void*);
	// Free function for data
	void (*key_val_free_function)(void*);
};

typedef struct server_memory server_memory;
struct server_memory {
	// Hashtable
	hashtable_t *memory;
	// Server_id
	int idx;
};

typedef struct load_balancer load_balancer;
struct load_balancer {
	// Servers array
	server_memory **servers;
	// Size of the servers array
	unsigned int size;
	// Number of servers
	unsigned int count;
	// Value 'till next replica (100000)
	unsigned int rep;
	// Hashing function for servers
	unsigned int (*hash)(void *);
};

#endif  // STRUCTS_H_
