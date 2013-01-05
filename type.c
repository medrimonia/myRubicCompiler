#include <stdlib.h>
#include <string.h>

#include "type.h"

struct type{
	const char * name;
};

linked_list_pointer all_types = NULL;
linked_list_pointer additionable_types = NULL;

void initialize_types(){
	all_types  = new_linked_list();
	additionable_types  = new_linked_list();
	add_new_type("i32", true);
	add_new_type("string", false);
}


type_p add_new_type(const char * name, bool additionable){
	type_p new = malloc(sizeof(struct type));
	new->name = name;
	linked_list_insert(all_types, new);
	if (additionable)
		linked_list_insert(additionable_types, new);
	return new;
}

const char * type_get_name(type_p t){
	return t->name;
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
