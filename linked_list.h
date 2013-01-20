#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

struct linked_list_node{
	struct linked_list_node * next;
	void * data;
};

struct linked_list{
	struct linked_list_node * first;
	struct linked_list_node * actual;
	int size;
};


typedef struct linked_list * linked_list_pointer;

/* Return a new linked list */
linked_list_pointer new_linked_list();

/* Insert the specified element before the actual value
 * The element must absolutely be copied before inserting it, because
 * it will be free at destroy
 */
void linked_list_insert(linked_list_pointer l, void * element);

/* Insert the specified element after the actual value
 * The element must absolutely be copied before inserting it, because
 * it will be free at destroy
 * at end, l->actual is the same as before
 */
void linked_list_insert_after(linked_list_pointer l, void * element);

/* Insert the specified element after the actual value
 * The element must absolutely be copied before inserting it, because
 * it will be free at destroy
 * at end, l->actual is on the new element
 */
void linked_list_append(linked_list_pointer l, void * element);

/* Insert the specified element at the head of the list 
 * The element must absolutely be copied before inserting it, because
 * it will be free at destroy
 */
void linked_list_insert_in_head(linked_list_pointer l, void * element);

/* Switching to the next element of the list */
void linked_list_next(linked_list_pointer l);

/* Return the element on which the iterator is pointing */
void * linked_list_get(linked_list_pointer l);

/* Return true if the iterator is pointing on the last element */
bool linked_list_end(linked_list_pointer l);

/* Return the iterator at the beginning of the list */
void linked_list_restart(linked_list_pointer l);

/* Remove the node currently pointed and optionnaly remove the data */
void linked_list_remove_opt_erase(linked_list_pointer l, bool erase_data);

/* Remove the node currently pointed and remove the data */
void linked_list_remove(linked_list_pointer l);

/* This function shouldn't be called if linked_list_end(l) is true 
 * destroy the data contained too.
 */
void linked_list_remove_next(linked_list_pointer l);

/* Return the number of elements contained in the list */
int linked_list_size(linked_list_pointer l);

/* Return the number of elements contained in the list */
int linked_list_is_empty(linked_list_pointer l);

/* Destroy the specified list and optionnaly free the data */
void linked_list_destroy_opt_erase(linked_list_pointer l, bool free_data);

/* Destroy the specified list */
void linked_list_destroy(linked_list_pointer l);

#endif
