#include <stdlib.h>

#include "tree.h"
#include "type_handler.h"
#include "function.h"


tn_pointer new_tree_node(node_type t){
	tn_pointer new = (tn_pointer)malloc(sizeof(struct tree_node));
	new->left_child = NULL;
	new->right_child = NULL;
	new->reg_number = -1;
	new->content = NULL;
	new->type = t;
	new->allowed_types = NULL;
	return new;
}

tn_pointer new_logical_node(node_type t,
														tn_pointer left_child,
														tn_pointer right_child){
	tn_pointer new = new_tree_node(t);
	new->left_child = left_child;
	new->right_child = right_child;
	new->allowed_types = th_logical(left_child->allowed_types,
																	right_child->allowed_types);
	return new;
}

tn_pointer new_icmp_node(node_type t,
												 tn_pointer left_child,
												 tn_pointer right_child){
	tn_pointer new = new_tree_node(t);
	new->left_child = left_child;
	new->right_child = right_child;
	new->allowed_types = th_comparison(left_child->allowed_types,
																		 right_child->allowed_types);
	return new;	
}

conditional_block_p new_conditional_block(tn_pointer cond,
																					tn_pointer true_case,
																					tn_pointer false_case){
	conditional_block_p new = malloc(sizeof(struct conditional_block));
	new->condition = cond;
	new->true_case = true_case;
	new->false_case = false_case;
	return new;
}

for_block_p new_for_block(char * var_id,
													tn_pointer from_expr,
													tn_pointer to_expr,
													tn_pointer code){
	for_block_p new = malloc(sizeof(struct for_block));
	new->var_id = var_id;
	new->from_expr = from_expr;
	new->to_expr = to_expr;
	new->code = code;
	return new;
}

void destroy_function(tn_pointer node){
	function_p f = (function_p) node->content;
	destroy_tree(f->root);
	function_destroy(f);
}

void destroy_tree(tn_pointer node){
	if (node == NULL)
		return;
	destroy_tree(node->left_child);
	destroy_tree(node->right_child);
	switch(node->type){
	case FUNCTION:          destroy_function(node);        break;
	default: 
		if(node->content != NULL)
			free(node->content);
	}
	free(node);
}
