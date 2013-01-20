/* Author : HOFER Ludovic
 * This type is supposed to handle the case where a function is defined with
 * ambiguous types in the parameters, the part described here allows the user
 * to test every possible combination
 */
#ifndef POSSIBLE_TYPES_SOLVER
#define POSSIBLE_TYPES_SOLVER

#include <stdbool.h>

#include "function.h"
#include "doubly_linked_list.h"

typedef doubly_linked_list_pointer type_solver_p;

/* Create a new type solver according to the specified parameters */
type_solver_p new_type_solver(function_p f);

/* Ensure that the actual combination is the first possible
 * (for every variable, the first type is chosen).
 */
void first_combination(type_solver_p ts);

/* Jump to the next possible combination of the type_solver */
void next_combination(type_solver_p ts);

/* return true if the actual combination is the last possible
 * (for every variable, the last type is chosen).
 */
bool is_last_combination(type_solver_p ts);

/* return a list of variable containing the actual types for the variable */
linked_list_pointer get_combination(type_solver_p ts);

#endif
