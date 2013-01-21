#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <stdbool.h>

#include "linked_list.h"

struct prototype{
	const char * name;
	// params is a list of type_p, the position of each type matters
	linked_list_pointer params;
};

typedef struct prototype * prototype_p;

prototype_p new_prototype(const char * name,
													linked_list_pointer params);

// key must be a prototype_p
int prototype_hash_function(const void * key);

/* key_1 et key_2 doivent être des prototype_p */
bool prototype_equal(const void * key_1, const void * key_2);

#endif
