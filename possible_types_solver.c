#include "possible_types_solver.h"
#include "type.h"
#include "type_handler.h"
#include <stdio.h>

type_solver_p new_type_solver(function_p f){
	type_solver_p new = new_doubly_linked_list();
	if (linked_list_size(f->parameters) == 0)
		return new;
	linked_list_restart(f->parameters);
	while(true){
		char * param_name = linked_list_get(f->parameters);
		//printf("param : %s\n", param_name);
		variable_p v = get_variable(f->inner_context, param_name);
		doubly_linked_list_append(new, copy_variable(v));
		if (linked_list_end(f->parameters))
			break;
		linked_list_next(f->parameters);
	}	
	//printf("new type solver with %d parameters created\n",
	//			 doubly_linked_list_size(new));
	return new;
}

void first_combination(type_solver_p ts){
	if (doubly_linked_list_size(ts) == 0)
		return;
	// return to start for each variable
	doubly_linked_list_first(ts);
	while(true){
		variable_p v = (variable_p)doubly_linked_list_get(ts);
		linked_list_restart(v->allowed_types);
		if (doubly_linked_list_is_last(ts))
			break;
		doubly_linked_list_next(ts);
	}
}

void next_combination(type_solver_p ts){
	if (doubly_linked_list_size(ts) == 0)
		return;
	// first not ended list will go to next element, others will start again
	doubly_linked_list_last(ts);
	while(true){
		variable_p v = (variable_p)doubly_linked_list_get(ts);
		if (!linked_list_end(v->allowed_types)){
			linked_list_next(v->allowed_types);
			return;
		}
		linked_list_restart(v->allowed_types);
		if (doubly_linked_list_is_first(ts))
			break;
		doubly_linked_list_previous(ts);
	}
}

bool is_last_combination(type_solver_p ts){
	if (doubly_linked_list_size(ts) == 0)
		return true;
	doubly_linked_list_last(ts);
	while(true){
		variable_p v = (variable_p)doubly_linked_list_get(ts);
		if (!linked_list_end(v->allowed_types)){
			return false;
		}
		if (doubly_linked_list_is_first(ts))
			return true;
		doubly_linked_list_previous(ts);
	}
}

//TODO destroy combination if not keeped
linked_list_pointer get_combination(type_solver_p ts){
	linked_list_pointer combination = new_linked_list();
	// names with associated type
	if (doubly_linked_list_size(ts) == 0)
		return combination;
	doubly_linked_list_last(ts);
	while(true){
		variable_p v = (variable_p)doubly_linked_list_get(ts);
		type_p type = linked_list_get(v->allowed_types);
		linked_list_insert(combination,
											 new_variable(new_type_list_single(type),
																		v->name));
		if (doubly_linked_list_is_first(ts))
			break;
		doubly_linked_list_previous(ts);
	}
	return combination;	
}

void type_solver_destroy(type_solver_p ts){
	// clearing variables (duplicated on creation)
	if(doubly_linked_list_size(ts) > 0){
		doubly_linked_list_first(ts);
		while(true){
			variable_p v = doubly_linked_list_get(ts);
			destroy_variable(v);
			if (doubly_linked_list_is_last(ts))
				break;
			doubly_linked_list_next(ts);
		}
	}
	doubly_linked_list_destroy(ts);
}
