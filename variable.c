#include <stdlib.h>

#include "variable.h"

variable_p new_variable(linked_list_pointer possible_types){
	variable_p new = malloc(sizeof(struct variable));
	new->possible_types = possible_types;
	return new;
}
