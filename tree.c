#include <stdlib.h>

#include "tree.h"


tn_pointer new_tree_node(node_type t){
	tn_pointer new = (tn_pointer)malloc(sizeof(struct tree_node));
	new->left_child = NULL;
	new->right_child = NULL;
	new->reg = NULL;
	new->content = NULL;
	new->type = t;
}
