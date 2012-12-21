#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>

typedef int (*hash_function)(void *);
typedef int (*equals_function)(void *, void *);

struct hashmap{
	hash_function h_f;
	equals_function e_f;
	int nb_elements;
	int map_size;
	void ** map;
	
};


typedef struct hashmap * hashmap_pointer;

hashmap_pointer new_hash_map(hash_function h_f, equals_function e_f);

void * hash_map_get(hashmap_pointer hm, void * key);

bool hash_map_exists(hashmap_pointer hm, void * key);

void hash_map_add(hashmap_pointer hm, void * key, void * value);

int hash_map_size(hashmap_pointer hm);

#endif
