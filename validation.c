#include <stdio.h>

#include "validation.h"

#include "function_set.h"
#include "prototype.h"
#include "type.h"
#include "type_handler.h"
#include "type_updater.h"


/* return true if the node is valid with current context, false otherwise */
bool validate_node(tn_pointer node);



bool validate_type_equality(type_p t1, type_p t2){
	if (t1 == NULL ||
			t1 != t2)
		return false;
	return true;
}

bool validate_node_childs(tn_pointer node){
	return (validate_node(node->left_child) &&
					validate_node(node->right_child));
}

bool validate_node_affect(tn_pointer node){
	validate_node(node->right_child);
	variable_p target = get_variable(node->context, node->left_child->content);
	type_p dst_type = th_true_type(target->allowed_types);
	//TODO src_type filtering maybe
	type_p src_type = th_true_type(node->right_child->allowed_types);
	return validate_type_equality(src_type, dst_type);
}

bool validate_node_list(tn_pointer node){
	return validate_node_childs(node);
}

bool validate_node_identifier(tn_pointer node){
	type_p t = th_true_type(node->allowed_types);
	if (t == NULL){
		//fprintf(stderr, "the identifier to load has an undecidable type\n");
		return false;
	}	
	return true;
}

bool validate_node_arithmetic(tn_pointer node){
	if (!validate_node_childs(node))
		return false;

	type_p t = th_true_type(node->allowed_types);
	if (t == NULL){
		//fprintf(stderr, "the result type of operation is undecidable\n");
		return false;
	}
	return true;
}

bool validate_node_logical(tn_pointer node){
	if (!validate_node_childs(node))
		return false;

	//TODO eventually pass through a tmp list with left and right child
//linked_list_pointer l = compared_type(node->left_child, node->right_child);
	type_p t1 = th_true_type(node->right_child->allowed_types);
	type_p t2 = th_true_type(node->left_child->allowed_types);
	return validate_type_equality(t1, t2);
}

bool validate_node_return(tn_pointer node){
	if (!validate_node(node->left_child))
		return false;

	type_p t = th_true_type(node->allowed_types);	
	if (t == NULL){
		//fprintf(stderr, "Return followed by an undecidable type\n");
		return false;
	}
	return true;
}

bool validate_node_call(tn_pointer node){
	// validating all parameters
	function_call_p fc = (function_call_p)node->content;
	if (linked_list_size(fc->parameters) > 0){
		linked_list_restart(fc->parameters);
		while(true){
			tn_pointer arg_node = linked_list_get(fc->parameters);
			if (!validate_node(arg_node))
				return false;
			if (linked_list_end(fc->parameters))
				break;
			linked_list_next(fc->parameters);
		}
	}
	// only one prototype should match at this point
	if (linked_list_size(fc->valid_prototypes) != 1)
		return false;

	// call should have an unique type
	type_p t = th_true_type(node->allowed_types);	
	if (t == NULL){
		//fprintf(stderr, "Return followed by an undecidable type\n");
		return false;
	}
	return true;
}

bool validate_node_icmp(tn_pointer node){
	if (!validate_node_childs(node))
		return false;

	type_p t = th_true_type(node->allowed_types);
	if (t == NULL){
		//fprintf(stderr, "the result type of icmp is undecidable\n");
		return false;
	}
	type_p t_l = th_true_type(node->left_child->allowed_types);
	type_p t_r= th_true_type(node->right_child->allowed_types);
	return validate_type_equality(t_l,t_r);
}
bool validate_node_conditional(tn_pointer node){
	conditional_block_p cond = node->content;
	return (validate_node(cond->condition) ||
					validate_node(cond->true_case) ||
					validate_node(cond->false_case));
}

bool validate_node_while(tn_pointer node){
	return validate_node_childs(node);
}

bool validate_node_for(tn_pointer node){
	for_block_p content = (for_block_p)node->content;
	return (validate_node(content->from_expr) &&
					validate_node(content->to_expr) &&
					validate_node(content->code));
}

bool validate_node(tn_pointer node){
	if (node == NULL)
		return true;
	// TODO update_allowed_types needed
	// Variables allowed_types may change, so allowed_types must be updated
	// each time
	switch(node->type){
	case PRIMARY :      return true;
	case AFFECT :       return validate_node_affect(node);
	case LIST :         return validate_node_list(node);
	case IDENTIFIER :   return validate_node_identifier(node);
	case ADDITION :     return validate_node_arithmetic(node);
	case SUBSTRACTION : return validate_node_arithmetic(node);
	case MULTIPLY :     return validate_node_arithmetic(node);
	case DIVIDE :       return validate_node_arithmetic(node);
	case OR_NODE :      return validate_node_logical(node);
	case AND_NODE :     return validate_node_logical(node);
		//function should already have been validated
	case FUNCTION :     return true;
	case RETURN_NODE :  return validate_node_return(node);
	case CALL :         return validate_node_call(node);
	case NEQ_NODE :     return validate_node_icmp(node);
	case EQ_NODE :      return validate_node_icmp(node);
	case LESS_NODE :    return validate_node_icmp(node);
	case LEQ_NODE :     return validate_node_icmp(node);
	case GEQ_NODE :     return validate_node_icmp(node);
	case GREATER_NODE : return validate_node_icmp(node);
	case IF_NODE :      return validate_node_conditional(node);
	case WHILE_NODE :   return validate_node_while(node);
	case FOR_NODE :     return validate_node_for(node);
	default: return false; // unknown type
	}
}


/* WARNING : for all parameters, type_list must be saved before calling this
 * function.
 */
void apply_combination(function_p f, linked_list_pointer combination){

	//printf("\t;function %s (%d parameters)(",f->name, linked_list_size(combination));
	// applying parameters type
	if (linked_list_size(combination)){
		linked_list_restart(combination);
		while(true){
			//Iterating over variables
			variable_p v = linked_list_get(combination);
			type_p type = th_true_type(v->allowed_types);
			variable_p v_func = get_variable(f->inner_context, v->name);
			linked_list_destroy_opt_erase(v_func->allowed_types,false);
			v_func->allowed_types = new_type_list_single(type);
			//printf("%s %s",type_get_name(type),v->name);
			if (linked_list_end(combination))
				break;
			//printf(",");
			linked_list_next(combination);
		}
	}
	//printf(")\n");
}


void validate_function(function_p f){
	//printf(";validating function\n");
	type_solver_p ts = new_type_solver(f);
	first_combination(ts);
	// if the function has no parameter, the only combination will be an empty
	// list
	while(true){
		// getting the combination
		linked_list_pointer combination = get_combination(ts);
		apply_combination(f, combination);
		//printf("\t\t;updating types\n");
		update_function(f);
		if (validate_node(f->root) &&
				th_true_type(f->possible_return_types) != NULL){
			//printf("\t\t;valid prototype\n");
			prototype_p accepted_proto = new_prototype(f->name, combination);
			function_set_add(global_fs, accepted_proto, f);
			linked_list_insert(f->valid_prototypes, accepted_proto);
		}
		if (is_last_combination(ts))
			break;
		next_combination(ts);		
	}
}
