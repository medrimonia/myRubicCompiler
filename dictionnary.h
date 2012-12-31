/* File name : dictionnary.h
 * Author : Medrimonia
 * created the : 2012/12/23
 * last update the : 2012/12/23
 *
 * Description :
 * This file contains the header describing a dictionnary type based on
 * an hashmap.
 * All the keys must be \0 terminated strings. Datas might be any pointer
 * type, but the user must ensure that the type will be adapted.
 * Some functions are proposing automatic free, this is not adapted if the
 * data stored by the dictionnary are containing pointers. (Since it's
 * generic, the free is not applied recursively)
 * This file is highly susceptible to be modified soon.
 */
#ifndef DICTIONNARY_H
#define DICTIONNARY_H

#include <stdbool.h>

#include "hashmap.h"

typedef hashmap_pointer dictionnary_pointer;

/* Create a new dictionnary */
dictionnary_pointer new_dictionnary();

/* Get an element with the specified key, return NULL in two cases
 * - The key can't be found  int the hashmap
 * - The key is found but is associated with the data NULL
 */
void * dictionnary_get(dictionnary_pointer d, char * key);

/* Return true if the specified key can be found in the dictionnary,
 * false otherwise. */
bool dictionnary_exists(dictionnary_pointer d, char * key);

/* Add the specified value with the specified key in the hashmap.
 * If the key is already in the specified map, NULL is returned,
 * If everything worked properly, returns the inserted value.
 */
void * dictionnary_add(dictionnary_pointer hm, char * key, void * value);

/* Remove the entry with the specified key.
 * If no entry with the specified key can be found, nothing is done
 * If the option free_key is specified :
 * - The key of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 * If the option free_data is specified :
 * - The data of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 */
void dictionnary_remove(dictionnary_pointer d,
												void * key,
												bool free_key,
												bool free_data);

/* Return the number of elements contained in the hashmap. */
int dictionnary_size(dictionnary_pointer d);

/* Remove all the entries and destroy the hashmap.
 * If the option free_key is specified :
 * - The key of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 * If the option free_data is specified :
 * - The data of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 */
void dictionnary_destroy(dictionnary_pointer d,
												 bool free_keys,
												 bool free_data);



void dictionnary_start_iteration(dictionnary_pointer d);

void * dictionnary_get_current_key(dictionnary_pointer d);

void * dictionnary_get_current_value(dictionnary_pointer d);

void dictionnary_next_element(dictionnary_pointer d);

bool dictionnary_is_ended_iteration(dictionnary_pointer d);


#endif
