/* Copyright 2023 Razvan-Constantin Rus */
#include "utils.h"

/*
 * Key compare functions:
 */
int compare_function_ints(void *a, void *b)
{
	// Convert to unsigned int
	unsigned int int_a = *((unsigned int *)a);
	unsigned int int_b = *((unsigned int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int compare_function_strings(void *a, void *b)
{
	// Convert to string
	char *str_a = (char *)a;
	char *str_b = (char *)b;
	// We are using strcmp
	return strcmp(str_a, str_b);
}
