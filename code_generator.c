#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "code_generator.h"
#include "function.h"
#include "variable.h"
#include "type.h"
#include "type_handler.h"
#include "constant_string_handler.h"

int actual_register = 0;
int actual_label = 0;
function_p cg_actual_function = NULL;

void generate_code_primary(tn_pointer node);
void generate_code_expr(tn_pointer node);
void generate_code_affect(tn_pointer node); 
void generate_code_list(tn_pointer node);
void generate_code_identifier(tn_pointer node);
void generate_code_generic_operation(tn_pointer node, char * op);

void generate_code_addition(tn_pointer node);
void generate_code_substraction(tn_pointer node);
void generate_code_multiplication(tn_pointer node);
void generate_code_division(tn_pointer node);

void generate_code_icmp(tn_pointer node, const char * cmp_type);

void generate_code_conditional(tn_pointer node);

void generate_code_while(tn_pointer node);

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
	case SUBSTRACTION : generate_code_substraction(node); break;
	case MULTIPLY : generate_code_multiplication(node); break;
	case DIVIDE : generate_code_division(node); break;
	case FUNCTION : generate_code_function(node); break;
	case RETURN_NODE : generate_code_return(node); break;
	case CALL : generate_code_call(node); break;
	case NEQ_NODE : generate_code_icmp(node,"ne"); break;
	case EQ_NODE : generate_code_icmp(node,"eq"); break;
	case LESS_NODE : generate_code_icmp(node,"slt"); break;
	case LEQ_NODE : generate_code_icmp(node,"sle"); break;
	case GEQ_NODE : generate_code_icmp(node,"sge"); break;
	case GREATER_NODE : generate_code_icmp(node,"sgt"); break;
	case IF_NODE : generate_code_conditional(node); break;
	case WHILE_NODE : generate_code_while(node); break;
	default: break;
	}
	return 0;	
}

void generate_code_primary(tn_pointer node){
	primary_p p = (primary_p) node->content;
	node->reg_number = ++actual_register;
	switch(p->t){
	case PRIMARY_STRING: //TODO : handle later
		printf("%%%d = ", actual_register);
		printf("getelementptr inbounds [%d x i8]* @%d, i32 0, i32 0\n",
					 get_constant_size(p->s_id),
					 p->s_id);
		break;
	case PRIMARY_EXPR: //TODO : handle later
		printf("PRIMARY_EXPR not handled\n");
		exit(EXIT_FAILURE);
		break;
	case PRIMARY_FLOAT: //TODO : handle later
		printf("PRIMARY_FLOAT not handled\n");
		exit(EXIT_FAILURE);
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
	//generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = actual_register;
	//TODO check types
	type_p t = th_true_type(node->right_child->allowed_types);
	if (t == NULL){
		fprintf(stderr, "the variable has an undecidable type\n");
		exit(EXIT_FAILURE);
	}
	printf("store %s %%%d, %s * %%%s\n",
				 type_get_name(t),
				 node->right_child->reg_number,
				 type_get_name(t),
				 (char *) node->left_child->content);
} 

void generate_code_list(tn_pointer node){
	generate_code(node->left_child);
	generate_code(node->right_child);
}

void generate_code_identifier(tn_pointer node){
	node->reg_number = ++actual_register;
	
	type_p t = th_true_type(cg_actual_function->possible_return_types);
	if (t == NULL){
		fprintf(stderr, "the identifier to load has an undecidable type\n");
		exit(EXIT_FAILURE);
	}
	printf("%%%d = load %s * %%%s\n", 
				 actual_register,
				 type_get_name(t),
				 (char *) node->content);
	
}

void generate_code_generic_operation(tn_pointer node, char * op){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = ++actual_register;
	
	type_p t = th_true_type(node->allowed_types);
	if (t == NULL){
		fprintf(stderr, "the result type of operation is undecidable\n");
		exit(EXIT_FAILURE);
	}
	printf("%%%d = %s %s %%%d, %%%d\n",
				 node->reg_number,
				 op,
				 type_get_name(t),
				 node->left_child->reg_number,
				 node->right_child->reg_number);
}

void generate_code_addition(tn_pointer node){
	generate_code_generic_operation(node, "add");
}

void generate_code_substraction(tn_pointer node){
	generate_code_generic_operation(node, "sub");
}

void generate_code_multiplication(tn_pointer node){
	generate_code_generic_operation(node, "mul");
}

void generate_code_division(tn_pointer node){
	generate_code_generic_operation(node, "sdiv");
}

void generate_code_icmp(tn_pointer node, const char * cmp_type){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = ++actual_register;

	//TODO eventually pass through a tmp list with left and right child
//linked_list_pointer l = compared_type(node->left_child, node->right_child);
	type_p t = th_true_type(node->right_child->allowed_types);
	if (t == NULL){
		fprintf(stderr, "the type used for comparison is undecidable\n");
		exit(EXIT_FAILURE);
	}
	printf("%%%d = icmp %s %s %%%d, %%%d\n",
				 node->reg_number,
				 cmp_type,
				 type_get_name(t),
				 node->left_child->reg_number,
				 node->right_child->reg_number);
}

void generate_code_while(tn_pointer node){
	int id_label_cond = actual_label++;
	int id_label_code = actual_label++;
	int id_label_end = actual_label++;
	/* It's forbidden to print a label if current block hasn't been terminated
	 * cf : http://zanopia.wordpress.com/2010/09/14/understanding-llvm-assembly-with-fractals-part-i/
	 * search for LOOPS 
	 */
	printf("br label %%LABEL%d\n", id_label_cond);
	printf("LABEL%d:\n",id_label_cond);
	generate_code(node->left_child);//left_child contain the code
	printf("br i1 %%%d, label %%LABEL%d, label %%LABEL%d\n",
				 node->left_child->reg_number,
				 id_label_code,
				 id_label_end);
	printf("LABEL%d:\n",id_label_code);
	//TODO variable first used in while not handled now
	generate_code(node->right_child);
	printf("br label %%LABEL%d\n", id_label_cond);
	printf("LABEL%d:\n",id_label_end);
}

void generate_code_function(tn_pointer node){
	// TODO fix in order to avoid problems when getting out of a function
	actual_register = 0;
	function_p f = (function_p) node->content;
	cg_actual_function = f;
	
	type_p t = th_true_type(f->possible_return_types);
	if (t == NULL){
		fprintf(stderr, "the function has an undecidable return type\n");
		exit(EXIT_FAILURE);
	}	
	printf("define %s @%s(",
				 type_get_name(t),
				 f->name);
	generate_parameters(f);
	printf("){\n");
	generate_variable_allocation(f);
	load_parameters(f);
	generate_code(f->root);
	printf("}\n");
}

void generate_code_return(tn_pointer node){
	if (node->left_child != NULL)
		generate_code(node->left_child);

	type_p t = th_true_type(node->allowed_types);	
	if (t == NULL){
		fprintf(stderr, "Return followed by an undecidable type\n");
		exit(EXIT_FAILURE);
	}
	printf("ret %s %%%d\n",
				 type_get_name(t),
				 actual_register);
	actual_register++;
}

//TODO parameters added in reverse order
void generate_code_call(tn_pointer node){
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
	
	type_p t = th_true_type(fc->f_called->possible_return_types);
	if (t == NULL){
		fprintf(stderr, "Calling a function with an undecidable type\n");
		exit(EXIT_FAILURE);
	}
	printf("%%%d = call %s @%s(",
				 actual_register,
				 type_get_name(t),
				 fc->f_called->name);
	node->reg_number = actual_register;
	//using parameters results
	linked_list_restart(fc->parameters);
	if (!linked_list_is_empty(fc->parameters))
		while(true){
			tn_pointer param = linked_list_get(fc->parameters);
	
			type_p t = th_true_type(fc->f_called->possible_return_types);
			if (t == NULL){
				fprintf(stderr, "A parameter of function have undecidable type\n");
				exit(EXIT_FAILURE);
			}
			printf("%s %%%d", type_get_name(t), param->reg_number);
			if (linked_list_end(fc->parameters))
				break;
			linked_list_next(fc->parameters);
			printf(", ");
	}
	printf(")\n");
	
}

//TODO label might be more explicit
void generate_code_conditional(tn_pointer node){
	conditional_block_p  cond = node->content;
	generate_code(cond->condition);
	int id_label_true = actual_label++;
	int id_label_false = -1;
	if (cond->false_case != NULL)
		id_label_false = actual_label++;
	int id_label_end = actual_label++;
	printf("br i1 %%%d, label %%LABEL%d, label %%LABEL%d\n",
				 cond->condition->reg_number,
				 id_label_true,
				 (id_label_false != -1) ? id_label_false : id_label_end);
	printf("LABEL%d:\n",id_label_true);
	generate_code(cond->true_case);
	printf("br label %%LABEL%d\n", id_label_end);
	if (cond->false_case != NULL){
		printf("LABEL%d:\n",id_label_false);
		generate_code(cond->false_case);
		//actual_register++;
		printf("br label %%LABEL%d\n", id_label_end);
	}
	printf("LABEL%d:\n", id_label_end);
	
}


void generate_variable_allocation(function_p f){
	dictionnary_pointer d = f->inner_context->local_variables;
	dictionnary_start_iteration(d);
	while(!dictionnary_is_ended_iteration(d)){
		variable_p v = dictionnary_get_current_value(d);
		if (th_true_type(v->allowed_types) == NULL){
			fprintf(stderr, "Impossible type choice for variable %s\n",
							(char *) dictionnary_get_current_key(d));
			exit(EXIT_FAILURE);
		}
		printf("%%%s = alloca %s, align 4\n",
					 (char *) dictionnary_get_current_key(d),
					 type_get_name(th_true_type(v->allowed_types)));
		/*variable_p v = dictionnary_get_current_value(d);
		int nb_types = linked_list_size(v->possible_types);
		printf("\tnb_types = %d\n",nb_types);
		linked_list_restart(v->possible_types);
		if (!linked_list_is_empty(v->possible_types)){
			while(true){
				type_p t = linked_list_get(v->possible_types);
				printf("\t\ttype : '%s'\n",type_get_name(t));
				if (linked_list_end(v->possible_types))
					break;
				linked_list_next(v->possible_types);
			}
			}*/
		dictionnary_next_element(d);
	}
	// ALLOCATION for parameters
	/*if (f->parameters == NULL)
		return;
	linked_list_restart(f->parameters);
	while (true){
		// TODO handle type
		variable_p v = get_variable(f->inner_context,
																linked_list_get(f->parameters));
		printf("%%%s = alloca %s, align 4\n",
					 (char *) linked_list_get(f->parameters),
					 type_get_name(th_true_type(v->allowed_types)));
		//th_true_type();
		if (linked_list_end(f->parameters))
			break;
		linked_list_next(f->parameters);
		}*/
}

