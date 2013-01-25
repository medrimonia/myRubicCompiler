#include <stdlib.h>

#include "doubly_linked_list.h"

typedef struct doubly_linked_list_node * node_pointer;

node_pointer create_sentinel(){
	node_pointer sentinel =
		(node_pointer)malloc(sizeof(struct doubly_linked_list_node));
	sentinel->previous = sentinel;
	sentinel->next = sentinel;
	sentinel->data = NULL;
	return sentinel;
}

doubly_linked_list_pointer new_doubly_linked_list(){
	doubly_linked_list_pointer new = malloc(sizeof(struct doubly_linked_list));
	new->sentinel = create_sentinel();
	new->first = new->sentinel;
	new->last = new->sentinel;
	new->actual = new->sentinel;
	new->size = 0;
	return new;
}

/* Generic function inserting before/after the specified node */
void insert_with_place(doubly_linked_list_pointer l,
											 node_pointer dest,
											 void * element,
											 bool before){
	node_pointer new_node =
		(node_pointer)malloc(sizeof(struct doubly_linked_list_node));
	// if the insert is done after and dest is not sentinel, values are swapped
	if (before || dest == l->sentinel){
		new_node->data = element;
	}
	else{
		new_node->data = dest->data;
		dest->data = element;
	}
	// Updating first and last
	if (dest == l->sentinel)
		l->last = new_node;
	if (dest->previous == l->sentinel)
		l->first = new_node;
	// Updating links
	new_node->previous = dest->previous;
	new_node->next = dest;
	if (dest != l->sentinel)
		dest->previous = new_node;
	if (new_node->previous != l->sentinel)
		new_node->previous->next = new_node;
	// Updating first and last pointers
	// TODO issues here
	if (l->size == 0){
		l->actual = new_node;
	}
	l->size++;
}

void doubly_linked_list_append(doubly_linked_list_pointer l,
															 void * element){
	insert_with_place(l, l->actual, element, false);//insert after
	if (l->size > 1)
		doubly_linked_list_next(l);
}

void doubly_linked_list_insert(doubly_linked_list_pointer l,
															 void * element){
	insert_with_place(l, l->actual, element, true);
}

void doubly_linked_list_insert_after(doubly_linked_list_pointer l,
																		 void * element){
	insert_with_place(l, l->actual, element, false);
}

void doubly_linked_list_insert_in_head(doubly_linked_list_pointer l,
																			 void * element){
	insert_with_place(l, l->first, element, true);
}

void doubly_linked_list_insert_in_queue(doubly_linked_list_pointer l,
																				void * element){
	insert_with_place(l, l->last, element, false);
}

void doubly_linked_list_previous(doubly_linked_list_pointer l){
	if (l->actual->previous == l->sentinel)
		return;
	l->actual = l->actual->previous;
}

void doubly_linked_list_next(doubly_linked_list_pointer l){
	if (l->actual->next == l->sentinel)
		return;
	l->actual = l->actual->next;
}

void * doubly_linked_list_get(doubly_linked_list_pointer l){
	return l->actual->data;
}

bool doubly_linked_list_is_first(doubly_linked_list_pointer l){
	return l->actual->previous == l->sentinel;
}

bool doubly_linked_list_is_last(doubly_linked_list_pointer l){
	return l->actual->next == l->sentinel;
}

void doubly_linked_list_first(doubly_linked_list_pointer l){
	l->actual = l->first;
}

void doubly_linked_list_last(doubly_linked_list_pointer l){
	l->actual = l->last;
}

void doubly_linked_list_remove_opt_erase(doubly_linked_list_pointer l,
																				 bool erase_data){
	if (l->actual == l->sentinel)
		return;// sentinel should never be removed
	// editing links
	if (l->actual->previous != l->sentinel)
		l->actual->previous->next = l->actual->next;
	if (l->actual->next != l->sentinel)
		l->actual->next->previous = l->actual->previous;
	// keeping pointer
	node_pointer tmp = l->actual;
	// updating l->actual
	if (l->actual->next == l->sentinel)
		l->actual = l->actual->previous;
	else
		l->actual = l->actual->next;
	if (erase_data)
		free(tmp->data);
	free(tmp);
	l->size--;
}

void doubly_linked_list_remove(doubly_linked_list_pointer l){
	doubly_linked_list_remove_opt_erase(l, true);
}

int doubly_linked_list_size(doubly_linked_list_pointer l){
	return l->size;
}

int doubly_linked_list_is_empty(doubly_linked_list_pointer l){
	return l->size == 0;
}

void doubly_linked_list_destroy_opt_erase(doubly_linked_list_pointer l,
																					bool free_data){
	doubly_linked_list_first(l);
	while(l->size > 1)
		doubly_linked_list_remove_opt_erase(l, free_data);
	free(l->sentinel);
	free(l);
}

void doubly_linked_list_destroy(doubly_linked_list_pointer l){
	doubly_linked_list_destroy_opt_erase(l, true);
}
