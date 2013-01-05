/*
 * Types list will contain a simple type_p.
 * Complexity might be improved if dictionnary or hashset are used
 * instead of linked_list.
 * linked_list_pointer should be switched to something else in order to
 * have a greater liberty of changes in the implementation
 */
#ifndef TYPE_HANDLER_H
#define TYPE_HANDLER_H

#include <stdbool.h>

#include "linked_list.h"
#include "type.h"

linked_list_pointer new_full_types_list();

linked_list_pointer new_type_list_single(type_p t);

linked_list_pointer new_type_list_single_from_name(char * type_name);

/* compare pointers */
bool type_list_contains(linked_list_pointer tl, type_p t);

/* add the type specified to the list if the list didn't contain it */
void type_list_add(linked_list_pointer tl, type_p t);

/* Remove of src the types which can't be found in cmp */
linked_list_pointer remove_types_not_shared(linked_list_pointer src,
																						linked_list_pointer cmp);

/* Return a list of the types shared by l1 and l2 */
linked_list_pointer types_shared(linked_list_pointer l1,
																 linked_list_pointer l2);

/* handle an addition between two types list and return the type list
 * corresponding to the allowed types for the result
 */
linked_list_pointer th_addition(linked_list_pointer l1,
																linked_list_pointer l2);
/* Return NULL if the list doesn't contain exactly one type
 * and the only type if the list contains only one element
 */
type_p th_true_type(linked_list_pointer l);

#endif
