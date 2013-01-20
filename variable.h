#ifndef VARIABLE_H
#define VARIABLE_H

#include "linked_list.h"

struct variable{
	linked_list_pointer allowed_types; 
	char * name;
};

typedef struct variable * variable_p;

/* Create a new variable with the specified name and the specified
 * allowed_types.
 */
variable_p new_variable(linked_list_pointer allowed_types, char * name);

#endif
