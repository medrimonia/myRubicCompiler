#ifndef VALIDATION_H
#define VALIDATION_H

#include "function.h"
#include "linked_list.h"
#include "possible_types_solver.h"

void apply_combination(function_p f, linked_list_pointer combination);

void validate_function(function_p f);

#endif
