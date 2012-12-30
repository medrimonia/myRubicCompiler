#ifndef FUNCTION_H
#define FUNCTION_H

#include "context.h"
#include "linked_list.h"

/* TODO hash function and equals function in order to be able to use multiple
 * functions sharing the same name but with different parameters
 */

struct function{
	context_pointer inner_context;
	char * name;
	linked_list_pointer parameters;
	//TODO handle return type
};

typedef struct function * function_p;

function_p new_function(context_pointer function_context);

void add_function_to_context(function_p f, context_pointer c);

#endif
