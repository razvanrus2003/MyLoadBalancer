/* Copyright 2023 Razvan-Constantin Rus */

#include "server.h"

server_memory *init_server_memory()
{
	// Server allocation
	server_memory *s = (server_memory *)malloc(sizeof(server_memory));
	DIE(s == NULL, "Server allocation failed\n");
	// Server hashtable allocation
	s->memory = ht_create(SERVER_H, hash_function_string,
		compare_function_strings,
		key_val_free_function);
	DIE(s->memory == NULL, "Server memory allocation failed\n");
	return s;
}

void server_store(server_memory *server, char *key, char *value) {
	// In order to store the data we are using hashtable a function
	if (server)
		ht_put(server->memory, key, KEY_LENGTH, value, VALUE_LENGTH);
}

char *server_retrieve(server_memory *server, char *key) {
	// In order to store the data we are using hashtable a function
	if (server)
		return ht_get(server->memory, key);
	return NULL;
}

void server_remove(server_memory *server, char *key) {
	// In order to obtain the data we are using hashtable a function
	if (server)
		ht_remove_entry(server->memory, key);
}

void free_server_memory(server_memory *server) {
	if (server) {
		// Frees the hashtable
		ht_free(server->memory);
		// Frees the server
		free(server);
	}
	// Update value
	server = NULL;
}
