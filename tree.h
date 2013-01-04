#ifndef TREE_H
#define TREE_H

typedef enum {
	NOT_DECIDED,
	PRIMARY,
	EXPR,
	AFFECT,
	LIST,
	IDENTIFIER,
	ADDITION,
	DIVIDE,
	MULTIPLY,
	FUNCTION,
	RETURN_NODE,
	CALL
} node_type;

typedef enum{
	PRIMARY_STRING,
	PRIMARY_FLOAT,
	PRIMARY_INT,
	PRIMARY_EXPR
} primary_type;

struct tree_node{
	struct tree_node * left_child;
	struct tree_node * right_child;
	node_type type;
	int reg_number;
	void * content;
};

typedef struct tree_node * tn_pointer;

struct primary{
	primary_type t;
	union{
		char * s;         // for strings
		int i;            // for ints
		float f;          // for floats
		tn_pointer child; // for expr
	};	
};

typedef struct primary * primary_p;

tn_pointer new_tree_node(node_type t);

#endif
