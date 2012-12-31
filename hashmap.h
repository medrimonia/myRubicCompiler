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
	int actual_indice;
};


typedef struct hashmap * hashmap_pointer;

/* Create a new hashmap with the specified hash_function and equals_function
 */
hashmap_pointer new_hashmap(hash_function h_f, equals_function e_f);

/* Get an element with the specified key, return NULL in two cases
 * - The key can't be found  int the hashmap
 * - The key is found but is associated with the data NULL
 */
void * hashmap_get(hashmap_pointer hm, void * key);

/* Return true if the specified key can be found in the map,
 * false otherwise. */
bool hashmap_exists(hashmap_pointer hm, void * key);

/* Add the specified value with the specified key in the hashmap.
 * If the key is already in the specified map, NULL is returned,
 * If everything worked properly, returns the inserted value.
 */
void * hashmap_add(hashmap_pointer hm, void * key, void * value);

/* Remove the entry with the specified key.
 * If no entry with the specified key can be found, nothing is done
 * If the option free_key is specified :
 * - The key of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 * If the option free_data is specified :
 * - The data of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 */
void hashmap_remove(hashmap_pointer hm,
										 void * key,
										 bool free_key,
										 bool free_data);

/* Return the number of elements contained in the hashmap. */
int hashmap_size(hashmap_pointer hm);

/* Remove all the entries and destroy the hashmap.
 * If the option free_key is specified :
 * - The key of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 * If the option free_data is specified :
 * - The data of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 */
void hashmap_destroy(hashmap_pointer hm,
										 bool free_keys,
										 bool free_data);


void hashmap_start_iteration(hashmap_pointer hm);

void * hashmap_get_current_key(hashmap_pointer hm);

void * hashmap_get_current_value(hashmap_pointer hm);

void hashmap_next_element(hashmap_pointer hm);

bool hashmap_is_ended_iteration(hashmap_pointer hm);

#endif
