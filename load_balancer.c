/* Copyright 2023 Razvan-Constantin Rus */

#include "load_balancer.h"

unsigned int hash_function_servers(void *a) {
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int hash_function_key(void *a) {
	unsigned char *puchar_a = (unsigned char *)a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

load_balancer *init_load_balancer() {
	unsigned int size = 9;
	unsigned int count = 0;

	// Initialize load_balancer
	load_balancer *l = (load_balancer *)malloc(sizeof(load_balancer));
	DIE(l == NULL, "Balancer allocation failed\n");

	// Initialize values
	l->servers = (server_memory **)malloc(9 * sizeof(server_memory *));
	l->size = size;
	l->count = count;
	l->rep = 100000;
	l->hash = hash_function_servers;

	// Return pointer
	return l;
}

void balance_server(load_balancer *main, unsigned int poz) {
	// Get the servers in question
	server_memory *s, *next_s;
	s = main->servers[poz];
	next_s = main->servers[(poz + 1) % main->count];

	// Get aux values
	unsigned int i;
	unsigned int hash = main->hash(&s->idx);
	unsigned int next_hash = main->hash(&next_s->idx);
	unsigned int tmp_hash;
	char *aux;

	ll_node_t *node;
	for (i = 0; i < next_s->memory->hmax; i++) {
		node = next_s->memory->buckets[i]->head;
		// Check every node it its corectly placed
		while (node) {
			tmp_hash = hash_function_key((char *)(((info *)node->data)->key));
			aux = ((info *)node->data)->key;
			if ((tmp_hash < hash || tmp_hash > next_hash) && hash < next_hash) {
				// Move the data
				server_store(s, aux, ((info *)node->data)->value);
				node = node->next;
				server_remove(next_s, aux);
			} else if (tmp_hash > next_hash && tmp_hash < hash) {
				// Move the data
				server_store(s, aux, ((info *)node->data)->value);
				node = node->next;
				server_remove(next_s, aux);

			} else {
				node = node->next;
			}
		}
	}
}

void loader_add_server(load_balancer *main, int server_id) {
	int i, j, n;
	unsigned int h;
	server_memory *s;

	// Realloc the server array if needed
	if (main->size == main->count) {
		long new_size = sizeof(server_memory *) * main->size * 2;
		server_memory **tmp = realloc(main->servers, new_size);
		DIE(tmp == NULL, "Servers reallocation failed\n");
		main->servers = tmp;
		main->size = main->size * 2;
	} else if (main->size * 2 < main->count) {
		long new_size = sizeof(server_memory *) * main->size / 2;
		server_memory **tmp = realloc(main->servers, new_size);
		DIE(tmp == NULL, "Servers reallocation failed\n");
		main->servers = tmp;
		main->size = main->size / 2;
	}

	// Add new server and its replicas on correct positions and shift the rest
	for (i = 0; i < 3; i++) {
		h = main->hash(&server_id);
		s = init_server_memory();
		s->idx = server_id;
		n = main->count - 1;
		for (j = n; j > -1 && main->hash(&main->servers[j]->idx) > h; j--)
			main->servers[j + 1] = main->servers[j];

		while(j > -1 && server_id < main->servers[j]->idx) {
			if (main->hash(&main->servers[j]->idx) != h)
				break;
			main->servers[j + 1] = main->servers[j];
			j--;
		}

		main->servers[j + 1] = s;
		server_id += main->rep;
		main->count += 1;

		// Moving data to the new server
		if (main->count > 3)
			balance_server(main, j + 1);
	}
}

unsigned int select_server(load_balancer *main, unsigned int hash)
{
	unsigned int l = 0, r = main->count;
	unsigned int mid = 1;

	if (main->hash(&main->servers[r - 1]->idx) < hash)
		return 0;
	if (main->hash(&main->servers[l]->idx) > hash)
		return 0;

	// Binary search
	while (r - l > 1) {
		mid = (l + r) / 2;

		if (main->hash(&main->servers[mid]->idx) > hash)
			r = mid;
		else if	(main->hash(&main->servers[mid]->idx) < hash)
			l = mid + 1;
		else
			break;
	}

	// Geting the right server from 3 consecutive servers
	if (main->hash(&main->servers[mid]->idx) == hash)
		r = mid;
	if (main->hash(&main->servers[l]->idx) >= hash)
		r = l;

	return r;
}

void loader_remove_server(load_balancer *main, int server_id) {
	int i;
	unsigned int j;
	unsigned int poz, poz_n;
	// Remove the last servers and its replicas
	if (main->count == 3) {
		for (j = 0; j < main->count; j++)
			free_server_memory(main->servers[j]);
		main->count -= 3;
		return;
	}

	for (i = 0; i < 3; i++) {
		// Get the server to be removed
		poz = select_server(main, main->hash(&server_id));
		poz_n = poz;

		// Get first non-replica "higher" server
		do {
			poz_n++;
			poz_n %= main->count;
		} while ((main->servers[poz_n]->idx - server_id) % main->rep == 0);

		// Move data
		ht_merge(main->servers[poz_n]->memory, main->servers[poz]->memory);
		free(main->servers[poz]);

		// Remove server from array
		server_id += main->rep;
		main->count -= 1;
		for (j = poz; j < main->count; j++) {
			main->servers[j] = main->servers[j + 1];
		}
	}
}

void loader_store(load_balancer *main, char *key, char *value, int *server_id) {
	// Get the server where to store data
	unsigned int poz = select_server(main, hash_function_key(key));

	// Add new data in the correct hashtable
	*server_id = main->servers[poz]->idx % main->rep;
	ht_put(main->servers[poz]->memory, key, KEY_LENGTH, value, VALUE_LENGTH);
}

char *loader_retrieve(load_balancer *main, char *key, int *server_id) {
	// Get the server from which to read data
    int poz = select_server(main, hash_function_key(key));

	// Return data from hashtabel
	*server_id = main->servers[poz]->idx % main->rep;
	return (char *)ht_get(main->servers[poz]->memory, key);
}

void free_load_balancer(load_balancer *main) {
	unsigned int i;

	// Free each server
	for (i = 0; i < main->count; i++)
		free_server_memory(main->servers[i]);

	// Free load_balancer
	free(main->servers);
	free(main);
}
