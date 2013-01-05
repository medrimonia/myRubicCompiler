#ifndef TYPE_H
#define TYPE_H

#include "linked_list.h"

//extern struct type;

typedef struct type * type_p;

extern linked_list_pointer all_types;

void initialize_types();

type_p add_new_type(const char * name);

const char * type_get_name(type_p t);

/* Restart and iterate on the all_types list */
type_p get_type_from_name(const char * c);

#endif
