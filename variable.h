#ifndef VARIABLE_H
#define VARIABLE_H

#include "linked_list.h"

struct variable{
	linked_list_pointer allowed_types; 
};

typedef struct variable * variable_p;

variable_p new_variable(linked_list_pointer allowed_types);

#endif
