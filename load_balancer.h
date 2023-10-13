/* Copyright 2023 Razvan-Constantin Rus */
#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "utils.h"
#include "server.h"

/** init_load_balancer() - initializes the memory for a new load balancer and its fields and
 *                         returns a pointer to it.
 *
 * Return: pointer to the load balancer struct.
 */
load_balancer *init_load_balancer();

/** free_load_balancer() - frees the memory of every field that is related to the
 *                         load balancer (servers, hashring).
 *
 * @arg1: Load balancer to free
 */
void free_load_balancer(load_balancer *main);

/** load_store() - Stores the key-value pair inside the system.
 *
 * @arg1: Load balancer which distributes the work.
 * @arg2: Key represented as a string.
 * @arg3: Value represented as a string.
 * @arg4: This function will RETURN via this parameter
 *        the server ID which stores the object.
 */
void loader_store(load_balancer *main, char *key, char *value, int *server_id);

/** load_retrieve() - Gets a value associated with the key.
 *
 * @arg1: Load balancer which distributes the work.
 * @arg2: Key represented as a string.
 * @arg3: This function will RETURN the server ID
		  which stores the value via this parameter.
 */
char *loader_retrieve(load_balancer *main, char *key, int *server_id);

/** load_add_server() - Adds a new server to the system.
 *
 * @arg1: Load balancer which distributes the work.
 * @arg2: ID of the new server.
 *
 */
void loader_add_server(load_balancer *main, int server_id);

/** load_remove_server() - Removes a specific server from the system.
 *
 * @arg1: Load balancer which distributes the work.
 * @arg2: ID of the removed server.
 */
void loader_remove_server(load_balancer *main, int server_id);

/** select_server() - Gets the position of a given hash in the sistem.
 *                    If it's a server return its position.
 *					  If it's a item return the position of the server.
 *					  that should be storing it.
 *
 * @arg1: Load balancer which distributes the work.
 * @arg2: The hash in question.
 *
 * Return: Position of a server
 */
unsigned int select_server(load_balancer *main, unsigned int hash);

/** Consistent hashing functions: - Gets the hash of the value pointed by the argument.
 *
 * @arg1: Pointer to the data.
 *
 * Return: Hash.
 */
unsigned int hash_function_servers(void *a);
unsigned int hash_function_key(void *a);

#endif /* LOAD_BALANCER_H_ */
