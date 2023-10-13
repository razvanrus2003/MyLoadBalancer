/* Copyright 2023 Razvan-Constantin Rus */
#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "hash_table.h"

/** init_server_memory() -  Initializes the memory for a new server struct.
 * 							Check what is returned by malloc using DIE.
 *
 * Return: pointer to the allocated server_memory struct.
 */
server_memory *init_server_memory();

/** free_server_memory() - Free the memory used by the server.
 *
 * @arg1: Server to free.
 */
void free_server_memory(server_memory *server);

/** server_store() - Stores a key-value pair to the server.
 *
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 * @arg3: Value represented as a string.
 */
void server_store(server_memory *server, char *key, char *value);

/** server_remove() - Removes a key-pair value from the server.
 *
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 */
void server_remove(server_memory *server, char *key);

/** server_retrieve() - Gets the value associated with the key.
 * 
 * @arg1: Server which performs the task.
 * @arg2: Key represented as a string.
 *
 * Return: String value associated with the key
 *         or NULL (in case the key does not exist).
 */
char *server_retrieve(server_memory *server, char *key);

#endif /* SERVER_H_ */
