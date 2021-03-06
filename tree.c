#include <stdlib.h>

#include "tree.h"

#include "function.h"
#include "type_handler.h"


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

void destroy_call(tn_pointer node){
  // destroying parameters exprs
  function_call_p fc = node->content;
  if (linked_list_size(fc->parameters) > 0){
    linked_list_restart(fc->parameters);
    while(true){
      destroy_tree(linked_list_get(fc->parameters));
      if (linked_list_end(fc->parameters))
        break;
      linked_list_next(fc->parameters);
    }
  }
  function_call_destroy(fc);
}

void destroy_primary(tn_pointer node){
  primary_p p = node->content;
  free(p);
}

void destroy_conditional(tn_pointer node){
  conditional_block_p c = node->content;
  destroy_tree(c->condition);
  destroy_tree(c->true_case); 
  destroy_tree(c->false_case);
  free(c);
}

void destroy_for(tn_pointer node){
  for_block_p content = node->content;
  destroy_tree(content->from_expr);
  destroy_tree(content->to_expr);
  destroy_tree(content->code);
  free(content);
}

void destroy_tree(tn_pointer node){
  if (node == NULL)
    return;
  if (node->allowed_types != NULL &&
      node->type != IDENTIFIER &&
      node->type != AFFECT)
    linked_list_destroy_opt_erase(node->allowed_types, false);
  destroy_tree(node->left_child);
  destroy_tree(node->right_child);
  switch(node->type){
  case CALL:              destroy_call(node);            break;
  case FUNCTION:          destroy_function(node);        break;
  case IDENTIFIER:                                       break;
  case PRIMARY:           destroy_primary(node);         break;
  case IF_NODE:           destroy_conditional(node);     break;
  case FOR_NODE:          destroy_for(node);             break;
  default: 
    if(node->content != NULL)
      free(node->content);
  }
  free(node);
}
