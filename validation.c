#include <stdio.h>

#include "validation.h"

#include "type.h"
#include "type_handler.h"

void validate_function(function_p f){
	type_solver_p ts = new_type_solver(f);
	first_combination(ts);
	while(true){
		// print combination :
		linked_list_pointer combination = get_combination(ts);
		//printf("function %s (%d parameters)(",f->name, linked_list_size(combination));
		if (linked_list_size(combination)){
			linked_list_restart(combination);
			while(true){
				//Iterating over varia
				variable_p v = linked_list_get(combination);
				type_p type = th_true_type(v->allowed_types);
				//printf("%s %s",type_get_name(type),v->name);
				if (linked_list_end(combination))
					break;
				//printf(",");
				linked_list_next(combination);
			}
		}
		//printf(")\n");
		if (is_last_combination(ts))
			break;
		next_combination(ts);		
	}
}
