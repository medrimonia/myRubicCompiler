#include <stdlib.h>

#include "type_handler.h"

linked_list_pointer new_full_types_list(){
	linked_list_pointer new = new_linked_list();
	linked_list_restart(all_types);
	while(true){
		linked_list_insert(new,linked_list_get(all_types));
		if (linked_list_end(all_types))
			break;
		linked_list_next(all_types);
	}
	return new;
}

linked_list_pointer new_type_list_single(type_p t){
	linked_list_pointer new = new_linked_list();
	linked_list_insert(new, t);
	return new;
}

linked_list_pointer new_type_list_single_from_name(char * type_name){
	return new_type_list_single(get_type_from_name(type_name));
}

bool type_list_contains(linked_list_pointer tl, type_p t){
	if (linked_list_is_empty(tl))
		return false;
	linked_list_restart(tl);
	while(true){
		if (linked_list_get(tl) == t)// pointer shared
			return true;
		if (linked_list_end(tl))
			return false;
		linked_list_next(tl);
	}
}

linked_list_pointer remove_types_not_shared(linked_list_pointer src,
																						linked_list_pointer cmp){
	if (linked_list_is_empty(src))
		return src;
	linked_list_restart(src);
	while(true){
		bool removed = false;
		//type_p t = linked_list_get(src);
		//printf("Checking type %s\n", type_get_name(t));
		if (!type_list_contains(cmp, linked_list_get(src))){
			// data are not erased because all types are shared.
			//printf("\tType not shared\n");
			linked_list_remove_opt_erase(src, false);
			removed = true;
		}
		if (linked_list_end(src))
			return src;
		if (!removed)
			linked_list_next(src);
	}
}

linked_list_pointer types_shared(linked_list_pointer l1,
																 linked_list_pointer l2){
	linked_list_pointer new = new_linked_list();
	if (linked_list_is_empty(l1) || linked_list_is_empty(l2))
		return new;
	linked_list_restart(l1);
	while(true){
		type_p t = linked_list_get(l1);
		if (type_list_contains(l2, t))
			linked_list_insert(new, t);
		if (linked_list_end(l1))
			return new;
	}	
}

linked_list_pointer th_addition(linked_list_pointer l1,
																linked_list_pointer l2){
	//TODO handle better (restriction on l1 and l2 etc)
	return types_shared(l1,l2);
}

type_p th_true_type(linked_list_pointer l){
	if (linked_list_size(l) != 1)// all variables must have only one type
		return NULL;
	linked_list_restart(l);
	return linked_list_get(l);
}
