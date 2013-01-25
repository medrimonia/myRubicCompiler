#include <stdlib.h>
#include <string.h>

#include "type.h"

int next_type = 1;

struct type{
	const char * name;
	int associated_int;
};

linked_list_pointer all_types = NULL;
linked_list_pointer additionable_types = NULL;
linked_list_pointer comparable_types = NULL;
linked_list_pointer logical_types = NULL;

void initialize_types(){
	all_types  = new_linked_list();
	additionable_types  = new_linked_list();
	comparable_types = new_linked_list();
	logical_types = new_linked_list();
	add_new_type("i32"   , true  , true  , false );// int
	add_new_type("double", true  , true  , false );// double
	add_new_type("i8 *"  , false , false , false );// string
	add_new_type("i1"    , false , true  , true  );// bool
}


type_p add_new_type(const char * name,
										bool additionable,
										bool comparable,
										bool logical){
	type_p new = malloc(sizeof(struct type));
	new->name = name;
	new->associated_int = next_type++;
	linked_list_insert(all_types, new);
	if (additionable)
		linked_list_insert(additionable_types, new);
	if (comparable)
		linked_list_insert(comparable_types, new);
	if (logical)
		linked_list_insert(logical_types, new);
	return new;
}

const char * type_get_name(type_p t){
	return t->name;
}

int type_associated_int(type_p t){
	return t->associated_int;
}

type_p get_type_from_name(const char * name){
	if (linked_list_is_empty(all_types))
		return NULL;
	linked_list_restart(all_types);
	while(true){
		type_p t = linked_list_get(all_types);
		if (strcmp(t->name, name) == 0)
			return t;
		if (linked_list_end(all_types))
			break;
		linked_list_next(all_types);
	}
	return NULL;
}
