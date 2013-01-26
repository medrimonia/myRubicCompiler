#include <stdlib.h>

#include "variable.h"

variable_p new_variable(linked_list_pointer allowed_types, char * name){
	variable_p new = malloc(sizeof(struct variable));
	new->allowed_types = allowed_types;
	new->name = name;
	return new;
}

variable_p copy_variable(variable_p v){
	linked_list_pointer types_copy = new_linked_list();
	// copying types
	if (linked_list_size(v->allowed_types) != 0){
		linked_list_restart(v->allowed_types);
		while(true){
			linked_list_append(types_copy,linked_list_get(v->allowed_types));
			if (linked_list_end(v->allowed_types))
				break;
			linked_list_next(v->allowed_types);
		}
	}
	// types copy ok
	return new_variable(types_copy, v->name);	
}

void destroy_variable(variable_p v, bool free_allowed_types){
	if (free_allowed_types)
		linked_list_destroy_opt_erase(v->allowed_types, false);
	free(v);
}
