#include <stdlib.h>
#include <stdio.h>

#include "code_generator.h"
int actual_register = 0;

void generate_code_primary(tn_pointer node);
void generate_code_expr(tn_pointer node);
void generate_code_affect(tn_pointer node); 
void generate_code_list(tn_pointer node);
void generate_code_identifier(tn_pointer node);
void generate_code_addition(tn_pointer node);


int generate_code(tn_pointer node){
	switch(node->type){
	case PRIMARY : generate_code_primary(node); break;
	case EXPR : generate_code_expr(node); break;
	case AFFECT : generate_code_affect(node); break;
	case LIST : generate_code_list(node); break;
	case IDENTIFIER : generate_code_identifier(node); break;
	case ADDITION : generate_code_addition(node); break;
	default: break;
	}
	return 0;	
}

void generate_code_primary(tn_pointer node){
	primary_p p = (primary_p) node->content;
	node->reg_number = actual_register++;
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
	node->reg_number = actual_register++;
	printf("%%%s = add i32 %%%d, 0\n",
				 (char *) node->left_child->content,
				 node->right_child->reg_number);
} 

void generate_code_list(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
}

void generate_code_identifier(tn_pointer node){

}

void generate_code_addition(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = actual_register++;
	//TODO handle type
	printf("%%%d = add i32 %%%d, %%%d\n",
				 node->reg_number,
				 node->left_child->reg_number,
				 node->right_child->reg_number);
}
