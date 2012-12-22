#include <stdlib.h>
#include <stdio.h>

#include "hashmap.h"

#define INITIAL_SIZE 100

struct hashmap_element{
	void * key;
	void * data;
};

typedef struct hashmap_element * e_p;

void fill_map(linked_list_pointer * map, int size){
	for (int i = 0; i < size; i++)
		map[i] = new_linked_list();
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
	hm->map = malloc(INITIAL_SIZE * sizeof(linked_list_pointer));
	fill_map(hm->map, INITIAL_SIZE);
	return hm;
}

void * hash_map_get(hashmap_pointer hm, void * key){
	// The key is in this list if it's in the map
	linked_list_pointer l = hm->map[get_index(hm, key)];
	if (linked_list_is_empty(l))
		return NULL;
	linked_list_restart(l);
	while (!linked_list_end(l)){
		e_p element = linked_list_get(l);
		if ((*hm->e_f)(element->key, key))
			return element->data;
		linked_list_next(l);
	}
	e_p element = linked_list_get(l);
	if ((*hm->e_f)(element->key, key))
		return element->data;
	return NULL;	
}

bool hash_map_exists(hashmap_pointer hm, void * key){
	return hash_map_get(hm, key) != NULL;
}

void hash_map_add(hashmap_pointer hm, void * key, void * value){
	//TODO increase size if nb_elements > map_size
	if (hash_map_exists(hm, key))
		return;
	int index = get_index(hm, key);
	e_p element = malloc(sizeof(struct hashmap_element));
	element->key = key;
	element->data = value;
	linked_list_insert_in_head(hm->map[index], element);
	hm->nb_elements++;
}

void hash_map_remove(hashmap_pointer hm, void * key){
	//TODO
}

int hash_map_size(hashmap_pointer hm){
	return hm->nb_elements;
}
