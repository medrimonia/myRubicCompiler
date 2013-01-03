#include <stdlib.h>
#include <stdio.h>

#include "code_generator.h"
#include "function.h"

int actual_register = 0;

void generate_code_primary(tn_pointer node);
void generate_code_expr(tn_pointer node);
void generate_code_affect(tn_pointer node); 
void generate_code_list(tn_pointer node);
void generate_code_identifier(tn_pointer node);
void generate_code_addition(tn_pointer node);
void generate_code_function(tn_pointer node);
void generate_code_return(tn_pointer node);
void generate_code_call(tn_pointer node);

void generate_variable_allocation(function_p f);
void generate_parameters(function_p f);
void load_parameters(function_p f);


int generate_code(tn_pointer node){
	if (node == NULL)
		return 1;
	switch(node->type){
	case PRIMARY : generate_code_primary(node); break;
	case EXPR : generate_code_expr(node); break;
	case AFFECT : generate_code_affect(node); break;
	case LIST : generate_code_list(node); break;
	case IDENTIFIER : generate_code_identifier(node); break;
	case ADDITION : generate_code_addition(node); break;
	case FUNCTION : generate_code_function(node); break;
	case RETURN_NODE : generate_code_return(node); break;
	case CALL : generate_code_call(node); break;
	default: break;
	}
	return 0;	
}

void generate_code_primary(tn_pointer node){
	primary_p p = (primary_p) node->content;
	node->reg_number = ++actual_register;
	switch(p->t){
	case PRIMARY_STRING: //TODO : handle later
		break;
	case PRIMARY_EXPR: //TODO : handle later
		break;
	case PRIMARY_FLOAT: //TODO : handle later
		break;
	case PRIMARY_INT :
		printf("%%%d = add i32 %d, 0\n", node->reg_number, p->i);
		break;
	}
}
void generate_code_expr(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
}

void generate_code_affect(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = actual_register;
	//TODO handle type
	printf("store i32 %%%d, i32 * %%%s\n",
				 node->right_child->reg_number,
				 (char *) node->left_child->content);
} 

void generate_code_list(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
}

void generate_code_identifier(tn_pointer node){
	//TODO hack
	node->reg_number = ++actual_register;
	printf("%%%d = load i32 * %%%s\n", 
				 actual_register,
				 (char *) node->content);
	
}

void generate_code_addition(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = ++actual_register;
 
	//TODO handle type
	printf("%%%d = add i32 %%%d, %%%d\n",
				 node->reg_number,
				 node->left_child->reg_number,
				 node->right_child->reg_number);
}

void generate_code_function(tn_pointer node){
	// TODO fix in order to avoid problems when getting out of a function
	actual_register = 0;
	function_p f = (function_p) node->content;
	// TODO handle type and parameters
	printf("define i32 @%s(",f->name);
	generate_parameters(f);
	printf("){\n");
	generate_variable_allocation(f);
	load_parameters(f);
	generate_code(f->root);
	printf("}\n");
}

void generate_code_return(tn_pointer node){
	// TODO handle type
	if (node->left_child != NULL)
		generate_code(node->left_child);
	printf("ret i32 %%%d\n", actual_register);
}

void generate_code_call(tn_pointer node){
	// TODO handle types
	function_call_p fc = node->content;
	//calculating parameters values
	linked_list_restart(fc->parameters);
	if (!linked_list_is_empty(fc->parameters))
		while(true){
			generate_code(linked_list_get(fc->parameters));
			if (linked_list_end(fc->parameters))
				break;
			linked_list_next(fc->parameters);
	}
	actual_register++;
	printf("%%%d = call i32 @%s(",
				 actual_register,
				 fc->f_name);
	//using parameters results
	linked_list_restart(fc->parameters);
	if (!linked_list_is_empty(fc->parameters))
		while(true){
			tn_pointer param = linked_list_get(fc->parameters);
			printf("i32 %%%d",param->reg_number);
			if (linked_list_end(fc->parameters))
				break;
			linked_list_next(fc->parameters);
			printf(", ");
	}
	printf(")\n");
	
}

// TODO variable not added to dictionnary
void generate_variable_allocation(function_p f){
	//TODO handle type
	dictionnary_pointer d = f->inner_context->local_variables;
	dictionnary_start_iteration(d);
	while(!dictionnary_is_ended_iteration(d)){
		printf("%%%s = alloca i32, align 4\n",
					 (char *) dictionnary_get_current_key(d));
		dictionnary_next_element(d);
	}
	// ALLOCATION for parameters
	if (f->parameters == NULL)
		return;
	linked_list_restart(f->parameters);
	while (true){
		// TODO handle type
		printf("%%%s = alloca i32, align 4\n",
					 (char *) linked_list_get(f->parameters));
		if (linked_list_end(f->parameters))
			break;
		linked_list_next(f->parameters);
	}
}

void generate_parameters(function_p f){
	if (f->parameters == NULL)
		return;
	linked_list_restart(f->parameters);
	while (true){
		// TODO handle type
		printf("i32 %%_%s",(char *) linked_list_get(f->parameters));
		if (linked_list_end(f->parameters))
			break;
		printf(", ");
		linked_list_next(f->parameters);
	}
}

void load_parameters(function_p f){
	if (f->parameters == NULL)
		return;
	linked_list_restart(f->parameters);
	while (true){
		// TODO handle type
		printf("store i32 %%_%s, i32 * %%%s\n",
					 (char *) linked_list_get(f->parameters),
					 (char *) linked_list_get(f->parameters));
		if (linked_list_end(f->parameters))
			break;
		linked_list_next(f->parameters);
	}
}

