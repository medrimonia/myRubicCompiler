#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "code_generator.h"
#include "constant_string_handler.h"
#include "function.h"
#include "prototype.h"
#include "type.h"
#include "type_handler.h"
#include "type_updater.h"
#include "validation.h"
#include "variable.h"

int actual_register = 0;
int actual_label = 0;
function_p cg_actual_function = NULL;

void generate_code_primary(tn_pointer node);
void generate_code_affect(tn_pointer node); 
void generate_code_list(tn_pointer node);
void generate_code_identifier(tn_pointer node);
void generate_code_generic_operation(tn_pointer node, char * op);

void generate_code_addition(tn_pointer node);
void generate_code_substraction(tn_pointer node);
void generate_code_multiplication(tn_pointer node);
void generate_code_division(tn_pointer node);

void generate_code_cmp(tn_pointer node, const char * cmp_type);

void generate_code_conditional(tn_pointer node);

void generate_code_while(tn_pointer node);
void generate_code_for(tn_pointer node);

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
	case AFFECT : generate_code_affect(node); break;
	case LIST : generate_code_list(node); break;
	case IDENTIFIER : generate_code_identifier(node); break;
	case ADDITION : generate_code_addition(node); break;
	case SUBSTRACTION : generate_code_substraction(node); break;
	case MULTIPLY : generate_code_multiplication(node); break;
	case OR_NODE : generate_code_generic_operation(node, "or"); break;
	case AND_NODE : generate_code_generic_operation(node, "and"); break;
	case DIVIDE : generate_code_division(node); break;
	case FUNCTION : generate_code_function(node); break;
	case RETURN_NODE : generate_code_return(node); break;
	case CALL : generate_code_call(node); break;
	case NEQ_NODE : generate_code_cmp(node,"ne"); break;
	case EQ_NODE : generate_code_cmp(node,"eq"); break;
	case LESS_NODE : generate_code_cmp(node,"slt"); break;
	case LEQ_NODE : generate_code_cmp(node,"sle"); break;
	case GEQ_NODE : generate_code_cmp(node,"sge"); break;
	case GREATER_NODE : generate_code_cmp(node,"sgt"); break;
	case IF_NODE : generate_code_conditional(node); break;
	case WHILE_NODE : generate_code_while(node); break;
	case FOR_NODE : generate_code_for(node); break;
	default: break;
	}
	return 0;	
}

void generate_code_primary(tn_pointer node){
	primary_p p = (primary_p) node->content;
	node->reg_number = ++actual_register;
	switch(p->t){
	case PRIMARY_STRING:
		printf("%%%d = ", actual_register);
		printf("getelementptr inbounds [%d x i8]* @%d, i32 0, i32 0\n",
					 get_constant_size(p->s_id),
					 p->s_id);
		break;
	case PRIMARY_DOUBLE:
		printf("%%%d = fadd double %f, 0.0\n",
					 node->reg_number,
					 p->d);
		break;
	case PRIMARY_INT :
		printf("%%%d = add i32 %d, 0\n", node->reg_number, p->i);
		break;
	}
}

void generate_code_affect(tn_pointer node){
	//generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = actual_register;
	type_p t = th_true_type(node->right_child->allowed_types);
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
	
	variable_p var = get_variable(node->context, node->content);
	type_p t = th_true_type(var->allowed_types);
		//th_true_type(cg_actual_function->possible_return_types);
	if (t == NULL){
		fprintf(stderr, "the identifier to load has an undecidable type\n");
		exit(EXIT_FAILURE);
	}
	printf("%%%d = load %s * %%%s\n", 
				 actual_register,
				 type_get_name(t),
				 (char *) node->content);
	
}

/* return the register where the result can be found */
int generate_cast(char * castword,
									type_p src_type,
									type_p dst_type,
									int register_num){
	int dest = ++actual_register;
	printf("%%%d = %s %s %%%d to %s\n",
				 dest,
				 castword,
				 type_get_name(src_type),
				 register_num,
				 type_get_name(dst_type));
	return dest;
}

void generate_code_generic_operation(tn_pointer node, char * op){
	generate_code(node->left_child);
	generate_code(node->right_child);
	
	type_p t = th_true_type(node->allowed_types);
	type_p left_type = th_true_type(node->left_child->allowed_types);
	type_p right_type = th_true_type(node->right_child->allowed_types);
	int left_reg = node->left_child->reg_number;
	int right_reg = node->right_child->reg_number;
	// handling implicit cast from int to double
	type_p double_type = get_type_from_name("double");
	if (t == double_type){
		if (left_type != double_type)
			left_reg = generate_cast("sitofp", left_type, double_type, left_reg);
		if (right_type != double_type)
			right_reg = generate_cast("sitofp", right_type, double_type, right_reg);
	}
	node->reg_number = ++actual_register;
	printf("%%%d = ",node->reg_number);
	if (t == double_type)
		printf("f");//fadd, fsub, etc...
	printf("%s %s %%%d, %%%d\n",
				 op,
				 type_get_name(t),
				 left_reg,
				 right_reg);
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

void generate_code_cmp(tn_pointer node, const char * cmp_type){
	generate_code(node->left_child);
	generate_code(node->right_child);
	node->reg_number = ++actual_register;

	type_p t = th_true_type(node->right_child->allowed_types);

	char type_char = 'i';
	if (t == get_type_from_name("double"))
		type_char = 'f';
	
	printf("%%%d = %ccmp %s %s %%%d, %%%d\n",
				 node->reg_number,
				 type_char,
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

void generate_code_for(tn_pointer node){
	int id_label_start = actual_label++;
	int id_label_code = actual_label++;
	int id_label_end = actual_label++;
	for_block_p content = node->content;
	// generate from and to nodes
	generate_code(content->from_expr);
	generate_code(content->to_expr);
	// first initialize variable
	printf("%%%s = alloca i32, align 4\n", content->var_id);//alloc
	printf("%%%d = add i32 %%%d, 0\n",
				 ++actual_register,
				 content->from_expr->reg_number);
	printf("store i32 %%%d, i32 * %%%s\n",
				 actual_register,
				 content->var_id);//store value
	printf("br label %%LABEL%d\n",id_label_start);
	// START and cond
	printf("LABEL%d:\n",id_label_start);
	printf("%%%d = load i32 * %%%s\n",
				 ++actual_register,
				 content->var_id);
	printf("%%%d = icmp uge i32 %%%d, %%%d\n",// unsigned greater or equal
				 actual_register + 1,
				 actual_register,
				 content->to_expr->reg_number);
	actual_register++;
	printf("br i1 %%%d, label %%LABEL%d, label %%LABEL%d\n",
				 actual_register,
				 id_label_end,
				 id_label_code);
	// CODE
	printf("LABEL%d:\n", id_label_code);
	generate_code(content->code);
	// increasing var values
	printf("%%%d = load i32 * %%%s\n",
				 ++actual_register,
				 content->var_id);
	printf("%%%d = add i32 %%%d, 1\n",
				 actual_register + 1,
				 actual_register);
	actual_register++;
	printf("store i32 %%%d, i32 * %%%s\n",
				 actual_register,
				 content->var_id);//store value
	printf("br label %%LABEL%d\n", id_label_start);
	// END
	printf("LABEL%d:\n", id_label_end);
}

void generate_code_function(tn_pointer node){
	function_p f = (function_p) node->content;
	cg_actual_function = f;

	if (linked_list_size(f->valid_prototypes) == 0){
		fprintf(stderr,
						"No valid prototypes has been validated for %s\n",
						f->name);
		exit(EXIT_FAILURE);
	}
 
	linked_list_restart(f->valid_prototypes);
	while(true){
	actual_register = 0;
		prototype_p p = (prototype_p)linked_list_get(f->valid_prototypes);
		apply_combination(f, p->params);
		update_function(f, p);
	
		type_p t = th_true_type(f->possible_return_types);
		printf("define %s @", type_get_name(t));
		print_prototype_prefix(p);
		printf("%s(",
					 f->name);
		generate_parameters(f);
		printf("){\n");
		generate_variable_allocation(f);
		load_parameters(f);
		generate_code(f->root);
		printf("}\n");
		// When there's no more prototypes available, get out of loop
		if (linked_list_end(f->valid_prototypes))
			break;
		linked_list_next(f->valid_prototypes);
	}
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
	printf("%%%d = call %s @",
				 actual_register,
				 type_get_name(t));
	// there should be one and only one prototype matching
	linked_list_restart(fc->valid_prototypes);
	prototype_p p = (prototype_p)linked_list_get(fc->valid_prototypes);
	if (!is_built_in(fc->f_called))
		print_prototype_prefix(p);
	printf("%s(",
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
		dictionnary_next_element(d);
	}
}
