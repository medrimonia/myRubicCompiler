#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "tree.h"

int generate_code(tn_pointer node);

void generate_global_allocation(context_pointer global_context);

#endif
