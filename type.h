#ifndef TYPE_H
#define TYPE_H

#include "linked_list.h"

//extern struct type;

typedef struct type * type_p;

extern linked_list_pointer all_types;
extern linked_list_pointer additionable_types;
extern linked_list_pointer comparable_types;
extern linked_list_pointer logical_types;

void initialize_types();

type_p add_new_type(const char * name,
										bool additionable,
										bool comparable,
										bool logical);

const char * type_get_name(type_p t);

/* Restart and iterate on the all_types list to get the type needed 
 * return NULL if no type with the given name can be found
 */
type_p get_type_from_name(const char * c);

#endif
