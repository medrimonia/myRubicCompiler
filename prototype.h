#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <stdbool.h>

#include "linked_list.h"
#include "type.h"

struct prototype{
	const char * name;
	// params is a list of variables, the position of each type matters
	linked_list_pointer params;
	type_p return_type;
};

typedef struct prototype * prototype_p;

prototype_p new_prototype(const char * name,
													linked_list_pointer params);

// key must be a prototype_p
int prototype_hash_function(const void * key);

/* key_1 et key_2 doivent être des prototype_p */
bool prototype_equal(const void * key_1, const void * key_2);

/* Return true if actual might fit the target prototype.
 */
bool prototype_matches(prototype_p target, linked_list_pointer args);

/* print the list of var true types, as : i32_double_i32 */
void print_prototype_prefix(prototype_p p);

/* print the list of true types associated, as : i32_double_i32 */
//void print_type_list_prefix(linked_list_pointer l);

void destroy_prototype(prototype_p p);




#endif
