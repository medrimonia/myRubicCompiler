#include <stdlib.h>

#include "function.h"



function_p new_function(context_pointer function_context){
	function_p new = malloc(sizeof(struct function));
	new->inner_context = function_context;
	new->name = NULL;
	new->parameters = NULL;
	new->root = NULL;
	new->possible_return_types = new_linked_list();//empty at start
	return new;
}

function_call_p new_function_call(function_p function_called){
	function_call_p new = malloc(sizeof(struct function_call));
	new->f_called = function_called;
	new->parameters = NULL;
	return new;
}


void add_function_to_context(function_p f, context_pointer c){
	//TODO handle function parameters type
	dictionnary_add(c->functions, f->name,f);
}

function_p get_function(context_pointer c, char * name){
	if (dictionnary_exists(c->functions, name))
		return dictionnary_get(c->functions, name);
	if (c->parent_context == NULL)
		return NULL;
	return get_function(c->parent_context, name);
}

variable_p get_parameter(function_p f, char * name){
	return get_variable(f->inner_context, name);
}
