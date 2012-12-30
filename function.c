#include <stdlib.h>

#include "function.h"

function_p new_function(context_pointer function_context){
	function_p new = malloc(sizeof(struct function));
	new->inner_context = function_context;
	new->name = NULL;
	new->parameters = NULL;
}


void add_function_to_context(function_p f, context_pointer c){
	//TODO handle function parameters type
	dictionnary_add(c->functions, f->name,f);
	
}
