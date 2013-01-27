#include <stdlib.h>

#include "linked_list.h"

typedef struct linked_list_node * node_pointer;

linked_list_pointer new_linked_list(){
  linked_list_pointer new = malloc(sizeof(struct linked_list));
  new->first = NULL;
  new->actual = NULL;
  new->size = 0;
  return new;
}

void linked_list_next(linked_list_pointer l){
  if (l->size == 0)
    return;
  if (l->actual->next != NULL)
    l->actual = l->actual->next;
}

void linked_list_insert(linked_list_pointer l, void * element){
  node_pointer new_node = NULL;
  if (l->size == 0){
    new_node = (node_pointer)malloc(sizeof(struct linked_list_node));
    new_node->data = element;
    new_node->next = NULL;
    l->first = new_node;
    l->actual = new_node;
    l->size++;
    return;
  }
  new_node = (node_pointer)malloc(sizeof(struct linked_list_node));
  // the new_node is inserted between actual and next and value are swapped
  new_node->next = l->actual->next;
  l->actual->next = new_node;
  new_node->data = l->actual->data;
  l->actual->data = element;
  l->size++;    
}

//TODO merge two function with a boolean swap in order to share code
// (linked_list insert or insert_after)
void linked_list_insert_after(linked_list_pointer l, void * element){
  node_pointer new_node = NULL;
  if (l->size == 0){
    new_node = (node_pointer)malloc(sizeof(struct linked_list_node));   
    new_node->data = element;
    new_node->next = NULL;
    l->first = new_node;
    l->actual = new_node;
    l->size++;
    return;
  }
  new_node = (node_pointer)malloc(sizeof(struct linked_list_node)); 
  // values aren't swapped
  new_node->next = l->actual->next;
  l->actual->next = new_node;
  new_node->data = element;
  l->size++;    
}

void linked_list_append(linked_list_pointer l, void * element){
  linked_list_insert_after(l, element);
  if (l->size != 1)
    linked_list_next(l);
}

void linked_list_insert_in_head(linked_list_pointer l, void * element){
  node_pointer new_node = malloc(sizeof(struct linked_list_node));
  if (l->size == 0)
    l->actual = new_node;
  new_node->next = l->first;
  l->first = new_node;
  new_node->data = element;
  l->size++;
}

void * linked_list_get(linked_list_pointer l){
  if (l->actual == NULL)
    return NULL;
  return l->actual->data;
}

bool linked_list_end(linked_list_pointer l){
  return l->size == 0 || l->actual->next == NULL;
}

void linked_list_restart(linked_list_pointer l){
  if (l->size > 0)
    l->actual = l->first;
}

void linked_list_remove_opt_erase(linked_list_pointer l, bool erase_data){
  if (linked_list_size(l) == 1){
    if (erase_data)
      free(l->first->data);
    free(l->first);
    l->first = NULL;
    l->actual = NULL;
    l->size = 0;
    return;
  }
  // if next is empty, we're removing the last element of the list
  // we do need to change the element before then
  // otherwise, values will be swaped and the next node will be deleted
  node_pointer next = l->actual->next;
  if (next == NULL){
    node_pointer removed = l->actual;
    if (erase_data)
      free(l->actual->data);
    free(l->actual);
    l->size--;
    linked_list_restart(l);
    while (l->actual->next != removed)
      l->actual = l->actual->next;
    l->actual->next = NULL;
  }else{
    if (erase_data)
      free(l->actual->data);
    l->actual->next = next->next;
    l->actual->data = next->data;
    free(next);
    l->size--;
  }
}

void linked_list_remove(linked_list_pointer l){
  linked_list_remove_opt_erase(l, true);
}

void linked_list_remove_next(linked_list_pointer l){
  node_pointer to_remove = l->actual->next;
  l->actual->next = to_remove->next;
  free(to_remove->data);
  free(to_remove);
  l->size--;
}

int linked_list_size(linked_list_pointer l){
  return l->size;
}

int linked_list_is_empty(linked_list_pointer l){
  return l->size == 0;
}

void linked_list_destroy_opt_erase(linked_list_pointer l, bool free_data){
  linked_list_restart(l);
  while(l->size > 0)
    linked_list_remove_opt_erase(l, free_data);
  free(l);
}

void linked_list_destroy(linked_list_pointer l){
  linked_list_destroy_opt_erase(l, true);
}
