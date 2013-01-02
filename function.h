#ifndef FUNCTION_H
#define FUNCTION_H

#include "context.h"
#include "tree.h"
#include "linked_list.h"

/* TODO hash function and equals function in order to be able to use multiple
 * functions sharing the same name but with different parameters
 */

struct function_call{
	char * f_name;
	linked_list_pointer parameters;
};

typedef struct function_call * function_call_p;

struct function{
	context_pointer inner_context;
	char * name;
	linked_list_pointer parameters;
	tn_pointer root;
	//TODO handle return type
};

typedef struct function * function_p;

function_p new_function(context_pointer function_context);

function_call_p new_function_call(char * name);

void add_function_to_context(function_p f, context_pointer c);

#endif
