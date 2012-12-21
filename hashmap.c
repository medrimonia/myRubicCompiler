#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#define INITIAL_SIZE 100

void fill_map(void ** map, int size){
	int i;
	for (i = 0; i < size; i++)
		map[i] = NULL;
}

int get_index(hashmap_pointer hm, void * key){
	return (*hm->h_f)(key) % hm->map_size;
}

hashmap_pointer new_hash_map(hash_function h_f, equals_function e_f){
	struct hashmap * hm = malloc(sizeof(struct hashmap));
	hm->h_f = h_f;
	hm->e_f = e_f;
	hm->map_size = INITIAL_SIZE;
	hm->nb_elements = 0;
	hm->map = malloc(INITIAL_SIZE * sizeof(void *));
	return hm;
}

void * hash_map_get(hashmap_pointer hm, void * key){
	return hm->map[get_index(hm, key)];
	
}

void hash_map_add(hashmap_pointer hm, void * key, void * value){
	int index = get_index(hm, key);
	if (hm->map[index] != NULL){
		fprintf(stderr,"An object is already at specified location\n");
		return;
	}
	hm->map[index] = value;
}
