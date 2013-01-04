#include <stdlib.h>

#include "type.h"

struct type{
	const char * name;
};

linked_list_pointer all_types = NULL;

void initialize_types(){
	all_types  = new_linked_list();
	add_new_type("i32");
}


type_p add_new_type(const char * name){
	type_p new= malloc(sizeof(struct type));
	new->name = name;
	linked_list_insert(all_types, new);
	return new;
}

const char * type_get_name(type_p t){
	return t->name;
}
