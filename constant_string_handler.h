#ifndef CONSTANT_STRING_HANDLER_H
#define CONSTANT_STRING_HANDLER_H

/* Return the id of the constant */
int add_constant(const char * s);

/* Get the constant content of the id specified */
const char * get_constant(int id);

/* Get the total length of the constant with the specified id. */
int get_constant_size(int id);

/* Print the initialization of all the constants */
void print_constants();

#endif
