#include <stdlib.h>

#include "variable.h"

variable_p new_variable(linked_list_pointer allowed_types, char * name){
	variable_p new = malloc(sizeof(struct variable));
	new->allowed_types = allowed_types;
	new->name = name;
	return new;
}
