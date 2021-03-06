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

void type_list_add(linked_list_pointer tl, type_p t){
  if (!type_list_contains(tl, t))
    linked_list_append(tl, t);
}

void type_list_add_type_list(linked_list_pointer dst,
                             linked_list_pointer src){
  if (linked_list_is_empty(src))
    return;
  linked_list_restart(src);
  while(true){
    type_list_add(dst, linked_list_get(src));
    if (linked_list_end(src))
      return;
    linked_list_next(src);
  }
}

linked_list_pointer remove_types_not_shared(linked_list_pointer src,
                                            linked_list_pointer cmp){
  if (linked_list_is_empty(src))
    return src;
  linked_list_restart(src);
  while(true){
    bool removed = false;
    bool was_on_last = linked_list_end(src);
    //type_p t = linked_list_get(src);
    //printf("Checking type %s\n", type_get_name(t));
    if (!type_list_contains(cmp, linked_list_get(src))){
      // data are not erased because all types are shared.
      //printf("\tType not shared\n");
      linked_list_remove_opt_erase(src, false);
      removed = true;
    }
    if (was_on_last)
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
    linked_list_next(l1);
  } 
}

linked_list_pointer th_arithmetic(linked_list_pointer l1,
                                  linked_list_pointer l2){
  remove_types_not_shared(l1, additionable_types);
  remove_types_not_shared(l2, additionable_types);
  type_p double_type = get_type_from_name("double");
  // If one side must be a double, both sides are double
  if (th_true_type(l1) == double_type ||
      th_true_type(l2) == double_type) 
    return new_type_list_single(double_type);
  type_p int_type = get_type_from_name("i32");
  // If one side might contain a double, return a list containing both
  if (type_list_contains(l1, double_type) ||
      type_list_contains(l2, double_type)){
    linked_list_pointer result = new_type_list_single(double_type);
    type_list_add(result, int_type);
    return result;
  }
  return new_type_list_single(int_type);
}

linked_list_pointer th_comparison(linked_list_pointer l1,
                                  linked_list_pointer l2){
  remove_types_not_shared(l1, comparable_types);
  remove_types_not_shared(l2, comparable_types);
  return new_type_list_single_from_name("i1");
}

linked_list_pointer th_logical(linked_list_pointer l1,
                               linked_list_pointer l2){
  remove_types_not_shared(l1, logical_types);
  remove_types_not_shared(l2, logical_types);
  return new_type_list_single_from_name("i1");
}

type_p th_true_type(linked_list_pointer l){
  // Special case, if the only choices are double and int, return int
  type_p double_type = get_type_from_name("double");
  type_p int_type = get_type_from_name("i32");
  if (linked_list_size(l) == 2 &&
      type_list_contains(l, double_type) &&
      type_list_contains(l, int_type))
    return int_type;
  if (linked_list_size(l) != 1)// all variables must have only one type
    return NULL;
  linked_list_restart(l);
  return linked_list_get(l);
}
