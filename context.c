/* File name : context.c
 * Author : Medrimonia
 * created the : 2012/12/23
 * last update the : 2012/12/23
 *
 * Description :
 * This file contains the implementation of the context
 */
#include <stdlib.h>

#include "context.h"
#include "variable.h"
#include "type_handler.h"

variable_p get_local_variable(context_pointer c, char * name);
variable_p get_global_variable(context_pointer c, char * name);

context_pointer new_context(){
	context_pointer new = malloc(sizeof(struct context));
	new->parent_context = NULL;
 	new->global_variables = new_dictionnary();
	new->local_variables = new_dictionnary();
	new->instance_variables = new_dictionnary();
	new->constants = new_dictionnary();
	new->types = new_dictionnary();
	new->classes = new_dictionnary();
	new->functions = new_dictionnary();
	return new;
}

context_pointer create_context_child(context_pointer parent){
	context_pointer new = malloc(sizeof(struct context));
	new->parent_context = parent;
 	new->global_variables = NULL;
	new->local_variables = new_dictionnary();
	new->instance_variables = new_dictionnary();
	new->constants = new_dictionnary();
	new->types = new_dictionnary();
	new->classes = new_dictionnary();
	new->functions = new_dictionnary();
	return new;
}

void declare_parameters_to_variables(context_pointer c,
																		 linked_list_pointer l){
	if (l == NULL || linked_list_is_empty(l))
		return;
	linked_list_restart(l);
	while(true){
		declare_variable(c, linked_list_get(l));
		if (linked_list_end(l))
			return;
		linked_list_next(l);
	}
}

#define IS_GLOBAL(X) (X[0] == '@')

bool is_declared_variable(context_pointer c, char * name){
	// TODO handle constants
	if (IS_GLOBAL(name))
		return is_declared_global_variable(c, name);
	else
		return is_declared_local_variable(c, name);
}

variable_p declare_variable(context_pointer c, char * name){
	// TODO handle constants
	if (IS_GLOBAL(name))
		return declare_global_variable(c, name);
	else
		return declare_local_variable(c, name);
}

variable_p get_variable(context_pointer c, char * name){
	// TODO handle constants
	if (IS_GLOBAL(name))
		return get_global_variable(c, name);
	else
		return get_local_variable(c, name);
}

variable_p get_local_variable(context_pointer c, char * name){
	if (c->parent_context == NULL ||
			dictionnary_exists(c->local_variables, name))
		return dictionnary_get(c->local_variables, name);
	return get_local_variable(c->parent_context, name);
}

bool is_declared_local_variable(context_pointer c, char * name){
	if (c->parent_context == NULL)
		return dictionnary_exists(c->local_variables, name);
	return (dictionnary_exists(c->local_variables, name) ||
					is_declared_local_variable(c->parent_context, name));
}

variable_p declare_local_variable(context_pointer c, char * name){
	//TODO add control of variable, once parameter has changed
	variable_p v = new_variable(new_full_types_list());
	dictionnary_add(c->local_variables, name, v);
	return v;
}

variable_p get_global_variable(context_pointer c, char * name){
	if (c->parent_context != NULL)
		return get_global_variable(c->parent_context, name);
	return dictionnary_get(c->global_variables, name);
}


bool is_declared_global_variable(context_pointer c, char * name){
	if (c->parent_context != NULL)
		return is_declared_global_variable(c, name);
	return dictionnary_exists(c->global_variables, name);
}

variable_p declare_global_variable(context_pointer c, char * name){
	// if we're not on the global context, switch to a higher context
	if (c->parent_context != NULL)
		return declare_global_variable(c->parent_context, name);
	variable_p v = new_variable(new_full_types_list());
	dictionnary_add(c->global_variables, name, v);
	return v;
}

void destroy_context(context_pointer c){
	hashmap_destroy(c->global_variables, true, true);
	hashmap_destroy(c->local_variables, true, true);
	hashmap_destroy(c->instance_variables, true, true);
	hashmap_destroy(c->constants, true, true);
	hashmap_destroy(c->types, true, true);
	hashmap_destroy(c->classes, true, true);
	hashmap_destroy(c->functions, true, true);
	free(c);
}
