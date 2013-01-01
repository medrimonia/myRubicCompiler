/* File name : context.h
 * Author : Medrimonia
 * created the : 2012/12/23
 * last update the : 2012/12/23
 *
 * Description :
 * This file contains the header describing a context of compilation.
 */
#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdbool.h>

#include "dictionnary.h"


typedef struct context * context_pointer;

struct context{
	// The global context will contain a father_context = NULL
	context_pointer parent_context;
	dictionnary_pointer global_variables;
	dictionnary_pointer local_variables;
	dictionnary_pointer instance_variables;
	dictionnary_pointer constants;
	dictionnary_pointer types;
	dictionnary_pointer classes;
	dictionnary_pointer functions;
};

typedef struct context * context_pointer;

context_pointer new_context();

context_pointer create_context_child(context_pointer parent);

bool is_declared_variable(context_pointer c, char * name);

void declare_variable(context_pointer c, char * name);

bool is_declared_local_variable(context_pointer c, char * name);

bool is_declared_global_variable(context_pointer c, char * name);

void declare_local_variable(context_pointer c, char * name);

void declare_global_variable(context_pointer c, char * name);

void destroy_context(context_pointer c);

#endif
