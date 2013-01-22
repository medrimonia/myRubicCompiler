#ifndef FUNCTION_SET_H
#define FUNCTION_SET_H

#include "dictionnary.h"
#include "function.h"
#include "prototype.h"

/* A function set is : dictionnary<name,hashmap<prototype,function>> */
typedef dictionnary_pointer function_set_p;

extern function_set_p global_fs;

void initialize_global_function_set();

function_set_p new_function_set();

/* Get an element with the specified key, return NULL in two cases
 * - The key can't be found  int the hashmap
 * - The key is found but is associated with the data NULL
 */
function_p function_set_get(function_set_p fs, prototype_p p);

/* Return true if the specified prototype can be found in the function set,
 * false otherwise. */
bool function_set_exists(function_set_p fs, prototype_p p);

/* Add the specified function with the specified prototype in the function
 * set.
 * If the prototype is already in the specified function set, NULL is
 * returned, if everything worked properly, returns the inserted function.
 */
function_p function_set_add(function_set_p fs, prototype_p p, function_p f);

/* Remove the entry with the specified prototype.
 * If no entry with the specified key can be found, nothing is done
 * If the option free_prototype is specified :
 * - The prototype of the entry found will be erased.
 * If the option free_function is specified :
 * - The function of the entry found will be erased.
 */
void function_set_remove(function_set_p fs,
												 prototype_p key,
												 bool free_prototype,
												 bool free_function);

/* Return the number of function names contained in the function_set.
 * Different prototypes are counted as only one function if they share the
 * name.
 */
int function_set_size(function_set_p fs);

/* Remove all the entries and destroy the function_set.
 * If the option free_key is specified :
 * - The key of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 * If the option free_data is specified :
 * - The data of the object found will be erased.
 *   (If it contains pointers of pointers, better use your own free)
 */
void function_set_destroy(function_set_p fs,
										 bool free_keys,
										 bool free_data);


void function_set_start_iteration(function_set_p fs);

prototype_p function_set_get_current_key(function_set_p fs);

function_p function_set_get_current_value(function_set_p fs);

void function_set_next_element(function_set_p fs);

bool function_set_is_ended_iteration(function_set_p fs);

/* l is a list of type_list corresponding to the parameters */
linked_list_pointer function_set_matching_functions(function_set_p fs,
																										const char * name,
																										linked_list_pointer l);
#endif
