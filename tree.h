#ifndef TREE_H
#define TREE_H

typedef enum {
	PRIMARY,
	EXPR,
	AFFECT,
	LIST
} node_type;


struct tree_node{
	struct tree_node * left_child;
	struct tree_node * right_child;
	node_type type;
	void * content;
};

typedef struct tree_node * tn_pointer;

#endif
