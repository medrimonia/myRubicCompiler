#ifndef HASHMAP_H
#define HASHMAP_H

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

linked_list_pointer new_linked_list();

void linked_list_insert(linked_list_pointer l, void * element);

void linked_list_insert_in_head(linked_list_pointer l, void * element);

void linked_list_next(linked_list_pointer l);

void * linked_list_get(linked_list_pointer l);

bool linked_list_end(linked_list_pointer l);

void linked_list_restart(linked_list_pointer l);

/* This function shouldn't be called if linked_list_end(l) is true */
void linked_list_remove_next(linked_list_pointer l);

int linked_list_size(linked_list_pointer l);

#endif
