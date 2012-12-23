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

bool is_declared_local_variable(context_pointer c, char * name){
	return (dictionnary_exists(c->local_variables, name) ||
					is_declared_local_variable(c->parent_context, name));
}

bool is_declared_global_variable(context_pointer c, char * name){
	if (c->parent_context != NULL)
		return is_declared_global_variable(c, name);
	return dictionnary_exists(c->global_variables, name);
}

void declare_global_variable(context_pointer c, char * name){
	// if we're not on the global context, switch to a higher context
	if (c->parent_context != NULL){
		declare_global_variable(c->parent_context, name);
	}
	else{
		//TODO add control of variable, once parameter has changed
		dictionnary_add(c->global_variables, name, name);
	}
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
