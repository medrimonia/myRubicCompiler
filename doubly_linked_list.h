#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stdbool.h>

struct doubly_linked_list_node{
	struct doubly_linked_list_node * previous;
	struct doubly_linked_list_node * next;
	void * data;
};

struct doubly_linked_list{
	struct doubly_linked_list_node * actual;
	struct doubly_linked_list_node * first;
	struct doubly_linked_list_node * last;
	struct doubly_linked_list_node * sentinel;
	int size;
};


typedef struct doubly_linked_list * doubly_linked_list_pointer;

/* Return a new doubly linked list */
doubly_linked_list_pointer new_doubly_linked_list();

/* Insert the specified element before the actual value */
void doubly_linked_list_insert(doubly_linked_list_pointer l, void * element);

/* Insert the specified element after the actual value */
void doubly_linked_list_insert_after(doubly_linked_list_pointer l,
																		 void * element);

/* Insert the specified element at the head of the list */
void doubly_linked_list_insert_in_head(doubly_linked_list_pointer l,
																			 void * element);

/* Insert the specified element at the queue of the list */
void doubly_linked_list_insert_in_queue(doubly_linked_list_pointer l,
																				void * element);

/* Switching to the previous element of the list */
void doubly_linked_list_previous(doubly_linked_list_pointer l);

/* Switching to the next element of the list */
void doubly_linked_list_next(doubly_linked_list_pointer l);

/* Return the element on which the iterator is pointing */
void * doubly_linked_list_get(doubly_linked_list_pointer l);

/* Return true if the iterator is pointing on the first element */
bool doubly_linked_list_is_first(doubly_linked_list_pointer l);

/* Return true if the iterator is pointing on the last element */
bool doubly_linked_list_is_last(doubly_linked_list_pointer l);

/* Return the iterator at the beginning of the list */
void doubly_linked_list_first(doubly_linked_list_pointer l);

/* Return the iterator at the beginning of the list */
void doubly_linked_list_last(doubly_linked_list_pointer l);

/* Remove the node currently pointed and optionnaly remove the data */
void doubly_linked_list_remove_opt_erase(doubly_linked_list_pointer l,
																				 bool erase_data);

/* Remove the node currently pointed and remove the data */
void doubly_linked_list_remove(doubly_linked_list_pointer l);

/* Return the number of elements contained in the list */
int doubly_linked_list_size(doubly_linked_list_pointer l);

/* Return true if the list is empty */
int doubly_linked_list_is_empty(doubly_linked_list_pointer l);

/* Destroy the specified list and optionnaly free the data contained */
void doubly_linked_list_destroy_opt_erase(doubly_linked_list_pointer l,
																					bool free_data);

/* Destroy the specified list */
void doubly_linked_list_destroy(doubly_linked_list_pointer l);

#endif
