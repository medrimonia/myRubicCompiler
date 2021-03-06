#ifndef FUNCTION_H
#define FUNCTION_H

#include "context.h"
#include "linked_list.h"
#include "tree.h"

/* TODO hash function and equals function in order to be able to use multiple
 * functions sharing the same name but with different parameters
 */
struct function{
  context_pointer inner_context;
  char * name;
  /* linked list of char * containing the parameter name */
  linked_list_pointer parameters;
  tn_pointer root;
  linked_list_pointer possible_return_types;
  /* linked list of prototypes */
  linked_list_pointer valid_prototypes;
};

typedef struct function * function_p;

struct function_call{
  function_p f_called;
  linked_list_pointer parameters;
  /* all the valid prototypes */
  linked_list_pointer valid_prototypes;
};

typedef struct function_call * function_call_p;

void initialize_built_ins(context_pointer root_context);

void declare_built_ins();

function_p new_function(context_pointer function_context);

function_call_p new_function_call(function_p f_called);

void add_function_to_context(function_p f, context_pointer c);

function_p get_function(context_pointer c, char * name);

variable_p get_parameter(function_p f, char * name);

void generate_parameters(function_p f);

void load_parameters(function_p f);

bool is_built_in(function_p f);

void function_destroy(function_p f);

void function_call_destroy(function_call_p p);

void destroy_built_ins();

#endif
