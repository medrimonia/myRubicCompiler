#include "type_handler.h"
#include "type.h"

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
