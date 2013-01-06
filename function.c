#include <stdlib.h>

#include "function.h"
#include "dictionnary.h"
#include "type_handler.h"

dictionnary_pointer built_ins = NULL;

void initialize_built_ins(context_pointer root_context){
	if (built_ins != NULL) //lazy mechanism
		return;
	built_ins = new_dictionnary();
	//
	function_p f = new_function(create_context_child(root_context));
	f->name = "puts";
	f->parameters = new_linked_list();
	linked_list_insert(f->parameters, "s");
	declare_typed_variable(f->inner_context,
												 "s",
												 "string");
	f->possible_return_types = new_type_list_single_from_name("string");
	dictionnary_add(built_ins,f->name,f);
}

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
		// functions hasn't been found, search in built-ins
		return dictionnary_get(built_ins, name);
	return get_function(c->parent_context, name);
}

variable_p get_parameter(function_p f, char * name){
	return get_variable(f->inner_context, name);
}
