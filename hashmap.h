#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

#include "linked_list.h"

/* A hash function is needed, this function should take a pointer in
 * parameter and return an int. */
typedef int (*hash_function)(void *);

/* A function is needed in order to compare keys, this function must
 * return true if the two pointers design the same key, and false if
 * they're not identical. */
typedef bool (*equals_function)(void *, void *);

struct hashmap{
	hash_function h_f;
	equals_function e_f;
	int nb_elements;
	int map_size;
	linked_list_pointer * map;
};


typedef struct hashmap * hashmap_pointer;

hashmap_pointer new_hash_map(hash_function h_f, equals_function e_f);

void * hash_map_get(hashmap_pointer hm, void * key);

bool hash_map_exists(hashmap_pointer hm, void * key);

void hash_map_add(hashmap_pointer hm, void * key, void * value);

void hash_map_remove(hashmap_pointer hm,
										 void * key,
										 bool free_key,
										 bool free_data);

int hash_map_size(hashmap_pointer hm);

void hashmap_destroy(hashmap_pointer hm,
										 bool free_keys,
										 bool free_data);

#endif
