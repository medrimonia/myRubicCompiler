#include <stdlib.h>

#include "linked_list.h"

typedef struct linked_list_node * node_pointer;

linked_list_pointer new_linked_list(){
	linked_list_pointer new = malloc(sizeof(struct linked_list));
	new->first = NULL;
	new->actual = NULL;
	new->size = 0;
}

void linked_list_next(linked_list_pointer l){
	if (l->size == 0)
		return;
	if (l->actual->next != NULL)
		l->actual = l->actual->next;
}

void linked_list_insert(linked_list_pointer l, void * element){
	node_pointer new_node =
		(node_pointer)malloc(sizeof(struct linked_list_node));
	if (l->size == 0){
		l->first = new_node;
		l->actual = new_node;
		l->actual->data = element;
		l->size++;
		return;
	}
	// the new_node is inserted between actual and next and value are swapped
	new_node->next = l->actual->next;
	l->actual->next = new_node;
	new_node->data = l->actual->data;
	l->actual->data = element;
	l->size++;		
}

void linked_list_insert_in_head(linked_list_pointer l, void * element){
	node_pointer new_node =
		(node_pointer)malloc(sizeof(struct linked_list_node));
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
	l->actual = l->first;
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
