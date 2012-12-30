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
	printf("%%%d = load i32 * %%test\n", ++actual_register);
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
	function_p f = (function_p) node->content;
	// TODO handle type and parameters
	printf("define i32 @%s(){\n",f->name);
	printf("%%test = alloca i32, align 4\n");//TODO Hack
	generate_code(f->root);
	printf("}\n");
}

void generate_code_return(tn_pointer node){
	// TODO handle type
	printf("ret i32 %%%d\n", actual_register);
	
}

void generate_variable_allocation(function_p f){
	
}
