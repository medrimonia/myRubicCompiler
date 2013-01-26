#include <stdlib.h>
#include <stdio.h>

#include "code_generator.h"
#include "dictionnary.h"
#include "function.h"
#include "function_set.h"
#include "prototype.h"
#include "type_handler.h"

dictionnary_pointer built_ins = NULL;
dictionnary_pointer used_built_ins = NULL;

void generate_function_declaration(function_p f);

void initialize_built_ins(context_pointer root_context){
	if (built_ins != NULL) //lazy mechanism
		return;
	built_ins = new_dictionnary();
	used_built_ins = new_dictionnary();
	function_p f;
	prototype_p p;
	linked_list_pointer vars;
	//Adding puts
	f = new_function(create_context_child(root_context));
	f->name = "puts";
	f->parameters = new_linked_list();
	linked_list_insert(f->parameters, "s");
	declare_typed_variable(f->inner_context,
												 "s",
												 "i8 *");
	// The empty list created must be free before
	linked_list_destroy_opt_erase(f->possible_return_types, false);
	f->possible_return_types = new_type_list_single_from_name("i8 *");
	vars = new_linked_list();
	// variable should be copied in order to work like user functions
	linked_list_append(vars,
										 copy_variable(get_variable(f->inner_context, "s")));
	p = new_prototype(f->name, vars);
	dictionnary_add(built_ins,f->name,f);
	function_set_add(global_fs, p, f);
	linked_list_insert(f->valid_prototypes, p);
}

void declare_built_ins(){
	if (dictionnary_size(used_built_ins) == 0)
		return;
	dictionnary_start_iteration(used_built_ins);
	while(!dictionnary_is_ended_iteration(used_built_ins)) {
		function_p f = dictionnary_get_current_value(used_built_ins);
		generate_function_declaration(f);
		dictionnary_next_element(used_built_ins);
	}
}

void generate_function_declaration(function_p f){

	type_p t = th_true_type(f->possible_return_types);
	if (t == NULL){
		fprintf(stderr, "The function has an undecidable return type\n");
		exit(EXIT_FAILURE);
	}	
	printf("declare %s @%s(",
				 type_get_name(t),
				 f->name);
	generate_parameters(f);
	printf(")\n");
	
}

function_p new_function(context_pointer function_context){
	function_p new = malloc(sizeof(struct function));
	new->inner_context = function_context;
	new->name = NULL;
	new->parameters = NULL;
	new->root = NULL;
	new->possible_return_types = new_linked_list();//empty at start
	new->valid_prototypes = new_linked_list();
	return new;
}

function_call_p new_function_call(function_p function_called){
	function_call_p new = malloc(sizeof(struct function_call));
	new->f_called = function_called;
	new->parameters = NULL;
	new->valid_prototypes = new_linked_list();
	return new;
}


void add_function_to_context(function_p f, context_pointer c){
	dictionnary_add(c->functions, f->name,f);
}

function_p get_function(context_pointer c, char * name){
	if (dictionnary_exists(c->functions, name))
		return dictionnary_get(c->functions, name);
	if (c->parent_context == NULL){
		// functions hasn't been found, search in built-ins
		function_p f = dictionnary_get(built_ins, name);
		if (f != NULL)
			dictionnary_add(used_built_ins, f->name, f);
		return f;
	}
	return get_function(c->parent_context, name);
}

variable_p get_parameter(function_p f, char * name){
	return get_variable(f->inner_context, name);
}


void generate_parameters(function_p f){
	if (linked_list_size(f->parameters) == 0)
		return;
	linked_list_restart(f->parameters);
	while (true){
		char * param_name = linked_list_get(f->parameters);
		variable_p v = get_parameter(f, param_name);
		type_p t = th_true_type(v->allowed_types);
		if (t == NULL){
			fprintf(stderr, "A parameter has an undecidable type\n");
			exit(EXIT_FAILURE);
		}
		printf("%s %%_%s",
					 type_get_name(t),
					 (char *)  linked_list_get(f->parameters));
		if (linked_list_end(f->parameters))
			break;
		printf(", ");
		linked_list_next(f->parameters);
	}
}

void load_parameters(function_p f){
	if (linked_list_size(f->parameters) == 0)
		return;
	linked_list_restart(f->parameters);
	while (true){
		char * param_name = linked_list_get(f->parameters);
		variable_p v = get_parameter(f, param_name);
		type_p t = th_true_type(v->allowed_types);
		if (t == NULL){
			fprintf(stderr, "A parameter has an undecidable type\n");
			exit(EXIT_FAILURE);
		}
		printf("store %s %%_%s, %s * %%%s\n",
					 type_get_name(t),
					 (char *) linked_list_get(f->parameters),
					 type_get_name(t),
					 (char *) linked_list_get(f->parameters));
		if (linked_list_end(f->parameters))
			break;
		linked_list_next(f->parameters);
	}
}

bool is_built_in(function_p f){
	return dictionnary_exists(built_ins, f->name);
}

void function_destroy(function_p f){
	linked_list_destroy_opt_erase(f->valid_prototypes, false);
	linked_list_destroy_opt_erase(f->parameters, false);
	linked_list_destroy_opt_erase(f->possible_return_types, false);
	free(f);
}

void function_call_destroy(function_call_p fc){
	linked_list_destroy_opt_erase(fc->parameters, false);
	linked_list_destroy_opt_erase(fc->valid_prototypes, false);
	free(fc);
}


void destroy_built_ins(){
	dictionnary_destroy(used_built_ins, false, false);
	if (dictionnary_size(built_ins) > 0){
		dictionnary_start_iteration(built_ins);
		while(!dictionnary_is_ended_iteration(built_ins)){
			function_p f = dictionnary_get_current_value(built_ins);
			function_destroy(f);
			dictionnary_next_element(built_ins);
		}
	}
	dictionnary_destroy(built_ins, false, false);
}
