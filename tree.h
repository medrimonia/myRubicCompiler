#ifndef TREE_H
#define TREE_H

#include "linked_list.h"
#include "context.h"

typedef enum {
	NOT_DECIDED,
	PRIMARY,
	AFFECT,
	LIST,
	IDENTIFIER,
	ADDITION,
	SUBSTRACTION,
	DIVIDE,
	MULTIPLY,
	FUNCTION,
	RETURN_NODE,
	CALL,
	NEQ_NODE,
	EQ_NODE,
	LESS_NODE,
	GREATER_NODE,
	LEQ_NODE,
	GEQ_NODE,
	IF_NODE,
	WHILE_NODE,
	FOR_NODE,
	OR_NODE,
	AND_NODE
} node_type;

typedef enum{
	PRIMARY_STRING,
	PRIMARY_FLOAT,
	PRIMARY_INT
} primary_type;

struct tree_node{
	struct tree_node * left_child;
	struct tree_node * right_child;
	node_type type;
	int reg_number;
	void * content;
	linked_list_pointer allowed_types;
	context_pointer context;
};

typedef struct tree_node * tn_pointer;

struct primary{
	primary_type t;
	union{
		int s_id;         // for strings
		int i;            // for ints
		float f;          // for floats
		tn_pointer child; // for expr
	};	
};

typedef struct primary * primary_p;

struct conditional_block{
	tn_pointer condition;
	tn_pointer true_case;
	tn_pointer false_case;
};

typedef struct conditional_block * conditional_block_p;

struct for_block{
	char * var_id;
	tn_pointer from_expr;
	tn_pointer to_expr;
	tn_pointer code;
};

typedef struct for_block * for_block_p;

tn_pointer new_tree_node(node_type t);

tn_pointer new_logical_node(node_type t,
														tn_pointer left_child,
														tn_pointer right_child);

tn_pointer new_icmp_node(node_type t,
												 tn_pointer left_child,
												 tn_pointer right_child);

conditional_block_p new_conditional_block(tn_pointer cond,
																					tn_pointer true_case,
																					tn_pointer false_case);

for_block_p new_for_block(char * var_id,
													tn_pointer from_node,
													tn_pointer to_node,
													tn_pointer code);

#endif
