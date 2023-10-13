/* Copyright 2023 Razvan-Constantin Rus */
#ifndef UTILS_H_
#define UTILS_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Fixed sizes of some data structures
 */
#define REQUEST_LENGTH 1024
#define KEY_LENGTH 128
#define VALUE_LENGTH 65536
#define SERVER_H 500

/**
 * Useful macro for handling error codes
 */
#define DIE(assertion, call_description)                                       \
	do {                                                                       \
		if (assertion) {                                                       \
			fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__);                 \
			perror(call_description);                                          \
			exit(errno);                                                       \
		}                                                                      \
	} while (0)

/**
 * Key compare functions - Convert the data from void * and
 *					   	   compare there values.
 *
 * @arg1: void pointer to first data.
 * @arg2: void pointer to second data.
 */
int compare_function_ints(void *a, void *b);
int compare_function_strings(void *a, void *b);

#endif /* UTILS_H_ */
