#ifndef VARIABLE_H
#define VARIABLE_H

#include "linked_list.h"

struct variable{
  linked_list_pointer allowed_types; 
  char * name;
	bool is_global;
	union {
		int i;
		float f;
	};
};

typedef struct variable * variable_p;

/* Create a new variable with the specified name and the specified
 * allowed_types.
 */
variable_p new_variable(linked_list_pointer allowed_types, char * name);

/* Copy the specified variable, the list is copied, but the name pointer is
 * keeped.
 */
variable_p copy_variable(variable_p v);

/* Destroy the specified variable, free allowed_types but not name */
void destroy_variable(variable_p v, bool free_allowed_types);

#endif
