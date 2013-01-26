#include <stdlib.h>
#include <stdio.h>

#include "type_updater.h"

#include "type_handler.h"
#include "function.h"
#include "function_set.h"
#include "linked_list.h"

function_p ut_current_function = NULL;
prototype_p ut_current_p = NULL;

void update_type(tn_pointer node);

void update_type_primary(tn_pointer node){
	primary_p value = (primary_p) node->content;
	if (node->allowed_types != NULL)
		linked_list_destroy_opt_erase(node->allowed_types, false);
	linked_list_pointer type_list;
	switch(value->t){
	case PRIMARY_STRING :{
		type_list =  new_type_list_single_from_name("i8 *");
		break;
	}
	case PRIMARY_DOUBLE :{
		type_list =  new_type_list_single_from_name("double");
		break;
	}
	case PRIMARY_INT :{
		type_list =  new_type_list_single_from_name("i32");
		break;
	}
	}
	node->allowed_types = type_list;
}

void update_type_affect(tn_pointer node){
	update_type(node->right_child);
	//TODO destroy_linked_list(node->allowed_types)
	linked_list_pointer types;
	types = remove_types_not_shared(node->left_child->allowed_types,
																	node->right_child->allowed_types);
	node->allowed_types = types;// list is shared with variable
}

void update_type_childs(tn_pointer node){
	update_type(node->left_child);
	update_type(node->right_child);
}

void update_type_identifier(tn_pointer node){
	variable_p v = get_variable(node->context, node->content);
	node->allowed_types = v->allowed_types;
}

void update_type_arithmetic(tn_pointer node){
	update_type_childs(node);
	if (node->allowed_types != NULL)
		linked_list_destroy_opt_erase(node->allowed_types, false);
	node->allowed_types = th_arithmetic(node->left_child->allowed_types,
																			node->right_child->allowed_types);
}

void update_type_logical(tn_pointer node){
	update_type_childs(node);
	node->allowed_types = th_logical(node->left_child->allowed_types,
																	 node->right_child->allowed_types);
}

void update_type_cmp(tn_pointer node){
	update_type_childs(node);
	node->allowed_types = th_comparison(node->left_child->allowed_types,
																			node->right_child->allowed_types);
}

void update_type_return(tn_pointer node){
	update_type(node->left_child);
	type_list_add_type_list(node->allowed_types,
													node->left_child->allowed_types);
	type_list_add_type_list(ut_current_function->possible_return_types,
													node->left_child->allowed_types);
}

void update_type_call(tn_pointer node){
	function_call_p call = (function_call_p)node->content;
	node->allowed_types = new_linked_list();
	//getting prototypes
	linked_list_pointer args = new_linked_list();
	if (linked_list_size(call->parameters) > 0){
		linked_list_restart(call->parameters);
		while(true){
			tn_pointer arg = linked_list_get(call->parameters);
			update_type(arg);
			linked_list_insert(args, arg->allowed_types);
			if (linked_list_end(call->parameters))
				break;
			linked_list_next(call->parameters);
		}
	}
	linked_list_pointer matching;
	matching = function_set_matching_functions(global_fs,
																						 call->f_called->name,
																						 args);
	bool valid_recursion = false;
	if (prototype_matches(ut_current_p,args)){
		printf(";recursion found\n");
		valid_recursion = true;
		linked_list_append(matching, ut_current_function);
	}
	if (linked_list_size(matching) == 0){
		printf(";No functions matching the specified prototype\n");
		return;
	}
	//printf(";a function matching the specified prototype was found\n");
	// adding return_types
	linked_list_pointer matching_proto;
	matching_proto = function_set_matching_prototypes(global_fs,
																										call->f_called->name,
																										args);
	if (valid_recursion){
			linked_list_append(matching_proto, ut_current_p);		
	}
	if (call->valid_prototypes != NULL)
		linked_list_destroy_opt_erase(call->valid_prototypes, false);		
	call->valid_prototypes = matching_proto;
	linked_list_restart(matching);
	while(true){
		function_p f = (function_p)linked_list_get(matching);
		type_list_add_type_list(node->allowed_types,
														f->possible_return_types);
		if (linked_list_end(matching))
			break;
		linked_list_next(matching);
	}
	linked_list_destroy_opt_erase(matching, false);
	linked_list_destroy_opt_erase(args, false);
	
}

void update_type_conditional(tn_pointer node){
	conditional_block_p cond = node->content;
	update_type(cond->condition);
	update_type(cond->true_case);
	update_type(cond->false_case);
}

void update_type_while(tn_pointer node){
	update_type_childs(node);
}

void update_type_for(tn_pointer node){
	for_block_p content = (for_block_p)node->content;
	update_type(content->from_expr);
	update_type(content->to_expr);
	update_type(content->code);
}

void update_type(tn_pointer node){
	if (node == NULL)
		return;
	switch(node->type){
	case PRIMARY :       update_type_primary(node);     break;
	case AFFECT :        update_type_affect(node);      break;
	case LIST :          update_type_childs(node);      break;
	case IDENTIFIER :    update_type_identifier(node);  break;
	case ADDITION :      update_type_arithmetic(node);  break;
	case SUBSTRACTION :  update_type_arithmetic(node);  break;
	case MULTIPLY :      update_type_arithmetic(node);  break;
	case DIVIDE :        update_type_arithmetic(node);  break;
	case OR_NODE :       update_type_logical(node);     break;
	case AND_NODE :      update_type_logical(node);     break;
		// function should be updated on it's own				  
	case FUNCTION :                                     break;
	case RETURN_NODE :   update_type_return(node);      break;
	case CALL :          update_type_call(node);        break;
	case NEQ_NODE :      update_type_cmp(node);         break;
	case EQ_NODE :       update_type_cmp(node);         break;
	case LESS_NODE :     update_type_cmp(node);         break;
	case LEQ_NODE :      update_type_cmp(node);         break;
	case GEQ_NODE :      update_type_cmp(node);         break;
	case GREATER_NODE :  update_type_cmp(node);         break;
	case IF_NODE :       update_type_conditional(node); break;
	case WHILE_NODE :    update_type_while(node);       break;
	case FOR_NODE :      update_type_for(node);         break;
	default: break;
	}
	return;	
}

void update_function(function_p f, prototype_p p){
	ut_current_function = f;
	ut_current_p = p;
	linked_list_destroy_opt_erase(f->possible_return_types, false);
	// Might be needed but implies some problems for now
	//linked_list_destroy_opt_erase(f->root->allowed_types, false);
	f->possible_return_types = new_linked_list();
	f->root->allowed_types = new_linked_list();
	update_type(f->root);
}
